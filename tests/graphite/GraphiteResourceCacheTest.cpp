/*
 * Copyright 2022 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "tests/Test.h"

#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkSurface.h"
#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/Recorder.h"
#include "include/gpu/graphite/Recording.h"
#include "src/core/SkCanvasPriv.h"
#include "src/gpu/graphite/Device.h"
#include "src/gpu/graphite/RecorderPriv.h"
#include "src/gpu/graphite/Resource.h"
#include "src/gpu/graphite/ResourceCache.h"
#include "src/gpu/graphite/ResourceProvider.h"
#include "src/gpu/graphite/SharedContext.h"
#include "src/gpu/graphite/Texture.h"
#include "src/gpu/graphite/TextureProxyView.h"
#include "src/image/SkImage_Base.h"

namespace skgpu::graphite {

class TestResource : public Resource {
public:
    static sk_sp<TestResource> Make(const SharedContext* sharedContext,
                                    Ownership owned,
                                    skgpu::Budgeted budgeted,
                                    Shareable shareable,
                                    size_t gpuMemorySize = 1) {
        auto resource = sk_sp<TestResource>(new TestResource(sharedContext,
                                                             owned,
                                                             budgeted,
                                                             gpuMemorySize));
        if (!resource) {
            return nullptr;
        }

        GraphiteResourceKey key;
        CreateKey(&key, shareable);

        resource->setKey(key);
        return resource;
    }

    static void CreateKey(GraphiteResourceKey* key, Shareable shareable) {
        // Internally we assert that we don't make the same key twice where the only difference is
        // shareable vs non-shareable. That allows us to now have Shareable be part of the Key's
        // key. So here we make two different resource types so the keys will be different.
        static const ResourceType kType = GraphiteResourceKey::GenerateResourceType();
        static const ResourceType kShareableType = GraphiteResourceKey::GenerateResourceType();
        ResourceType type = shareable == Shareable::kNo ? kType : kShareableType;
        GraphiteResourceKey::Builder(key, type, 0, shareable);
    }

private:
    TestResource(const SharedContext* sharedContext,
                 Ownership owned,
                 skgpu::Budgeted budgeted,
                 size_t gpuMemorySize)
            : Resource(sharedContext, owned, budgeted, gpuMemorySize) {}

    void freeGpuData() override {}
};

static sk_sp<SkData> create_image_data(const SkImageInfo& info) {
    const size_t rowBytes = info.minRowBytes();
    sk_sp<SkData> data(SkData::MakeUninitialized(rowBytes * info.height()));
    {
        SkBitmap bm;
        bm.installPixels(info, data->writable_data(), rowBytes);
        SkCanvas canvas(bm);
        canvas.clear(SK_ColorRED);
    }
    return data;
}

DEF_GRAPHITE_TEST_FOR_ALL_CONTEXTS(GraphiteBudgetedResourcesTest, reporter, context) {
    std::unique_ptr<Recorder> recorder = context->makeRecorder();
    ResourceProvider* resourceProvider = recorder->priv().resourceProvider();
    ResourceCache* resourceCache = resourceProvider->resourceCache();
    const SharedContext* sharedContext = resourceProvider->sharedContext();

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 0);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 0);

    // Test making a non budgeted, non shareable resource.
    auto resource = TestResource::Make(
            sharedContext, Ownership::kOwned, skgpu::Budgeted::kNo, Shareable::kNo);
    if (!resource) {
        ERRORF(reporter, "Failed to make TestResource");
        return;
    }
    Resource* resourcePtr = resource.get();

    REPORTER_ASSERT(reporter, resource->budgeted() == skgpu::Budgeted::kNo);
    resourceCache->insertResource(resourcePtr);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    // Resource is not shareable and we have a ref on it. Thus it shouldn't ben findable in the
    // cache.
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 0);

    // When we reset our TestResource it should go back into the cache since it can be used as a
    // scratch texture (since it is not shareable). At that point the budget should be changed to
    // skgpu::Budgeted::kYes.
    resource.reset();
    resourceCache->forceProcessReturnedResources();
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 1);
    // Even though we reset our ref on the resource we still have the ptr to it and should be the
    // resource in the cache. So in general this is dangerous it should be safe for this test to
    // directly access the texture.
    REPORTER_ASSERT(reporter, resourcePtr->budgeted() == skgpu::Budgeted::kYes);

    GraphiteResourceKey key;
    TestResource::CreateKey(&key, Shareable::kNo);
    Resource* resourcePtr2 = resourceCache->findAndRefResource(key, skgpu::Budgeted::kNo);
    REPORTER_ASSERT(reporter, resourcePtr == resourcePtr2);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 0);
    REPORTER_ASSERT(reporter, resourcePtr2->budgeted() == skgpu::Budgeted::kNo);
    resourcePtr2->unref();
    resourceCache->forceProcessReturnedResources();

    // Test making a non budgeted, non shareable resource.
    resource = TestResource::Make(
            sharedContext, Ownership::kOwned, skgpu::Budgeted::kYes, Shareable::kYes);
    if (!resource) {
        ERRORF(reporter, "Failed to make TestResource");
        return;
    }
    resourcePtr = resource.get();
    REPORTER_ASSERT(reporter, resource->budgeted() == skgpu::Budgeted::kYes);
    resourceCache->insertResource(resourcePtr);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 2);

    resource.reset();
    resourceCache->forceProcessReturnedResources();
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 2);
    REPORTER_ASSERT(reporter, resourcePtr->budgeted() == skgpu::Budgeted::kYes);

    TestResource::CreateKey(&key, Shareable::kYes);
    resourcePtr2 = resourceCache->findAndRefResource(key, skgpu::Budgeted::kYes);
    REPORTER_ASSERT(reporter, resourcePtr == resourcePtr2);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 2);
    REPORTER_ASSERT(reporter, resourcePtr2->budgeted() == skgpu::Budgeted::kYes);
    resourcePtr2->unref();

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Test that SkImage's and SkSurface's underlying Resource's follow the expected budgeted
    // system.
    auto info = SkImageInfo::Make(10, 10, kRGBA_8888_SkColorType, kPremul_SkAlphaType);

    // First test SkImages. Since we can't directly create a Graphite SkImage we first have to make
    // a raster SkImage than convert that to a Graphite SkImage via makeTextureImage.
    sk_sp<SkData> data(create_image_data(info));
    sk_sp<SkImage> image = SkImages::RasterFromData(info, std::move(data), info.minRowBytes());
    REPORTER_ASSERT(reporter, image);

    sk_sp<SkImage> imageGpu = image->makeTextureImage(recorder.get());
    REPORTER_ASSERT(reporter, imageGpu);

    TextureProxy* imageProxy = nullptr;
    {
        // We don't want the view holding a ref to the Proxy or else we can't send things back to
        // the cache.
        auto [view, colorType] = as_IB(imageGpu.get())->asView(recorder.get(), Mipmapped::kNo);
        REPORTER_ASSERT(reporter, view);
        imageProxy = view.proxy();
    }
    // Make sure the proxy is instantiated
    if (!imageProxy->instantiate(resourceProvider)) {
        ERRORF(reporter, "Failed to instantiate Proxy");
        return;
    }
    const Resource* imageResourcePtr = imageProxy->texture();
    REPORTER_ASSERT(reporter, imageResourcePtr);
    // There is an extra resource for the buffer that is uploading the data to the texture
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 4);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 2);
    REPORTER_ASSERT(reporter, imageResourcePtr->budgeted() == skgpu::Budgeted::kNo);

    // Submit all upload work so we can drop refs to the image and get it returned to the cache.
    std::unique_ptr<Recording> recording = recorder->snap();
    if (!recording) {
        ERRORF(reporter, "Failed to make recording");
        return;
    }
    InsertRecordingInfo insertInfo;
    insertInfo.fRecording = recording.get();
    context->insertRecording(insertInfo);
    context->submit(SyncToCpu::kYes);
    recording.reset();
    imageGpu.reset();
    resourceCache->forceProcessReturnedResources();

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 4);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 4);
    REPORTER_ASSERT(reporter, imageResourcePtr->budgeted() == skgpu::Budgeted::kYes);

    // Now try an SkSurface. This is simpler since we can directly create Graphite SkSurface's.
    sk_sp<SkSurface> surface = SkSurface::MakeGraphite(recorder.get(), info);
    if (!surface) {
        ERRORF(reporter, "Failed to make surface");
        return;
    }

    TextureProxy* surfaceProxy = SkCanvasPriv::TopDeviceGraphiteTargetProxy(surface->getCanvas());
    if (!surfaceProxy) {
        ERRORF(reporter, "Failed to get surface proxy");
        return;
    }

    // Make sure the proxy is instantiated
    if (!surfaceProxy->instantiate(resourceProvider)) {
        ERRORF(reporter, "Failed to instantiate surface proxy");
        return;
    }
    const Resource* surfaceResourcePtr = surfaceProxy->texture();

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 5);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 4);
    REPORTER_ASSERT(reporter, surfaceResourcePtr->budgeted() == skgpu::Budgeted::kNo);

    // The creation of the surface may have added an initial clear to it. Thus if we just reset the
    // surface it will flush the clean on the device and we don't be dropping all our refs to the
    // surface. So we force all the work to happen first.
    recording = recorder->snap();
    insertInfo.fRecording = recording.get();
    context->insertRecording(insertInfo);
    context->submit(SyncToCpu::kYes);
    recording.reset();

    surface.reset();
    resourceCache->forceProcessReturnedResources();
    REPORTER_ASSERT(reporter, surfaceResourcePtr->budgeted() == skgpu::Budgeted::kYes);
}

namespace {
sk_sp<Resource> add_new_resource(skiatest::Reporter* reporter,
                                 const SharedContext* sharedContext,
                                 ResourceCache* resourceCache,
                                 size_t gpuMemorySize) {
    auto resource = TestResource::Make(sharedContext,
                                       Ownership::kOwned,
                                       skgpu::Budgeted::kYes,
                                       Shareable::kNo,
                                       gpuMemorySize);
    if (!resource) {
        ERRORF(reporter, "Failed to make TestResource");
        return nullptr;
    }
    resourceCache->insertResource(resource.get());
    return std::move(resource);
}

Resource* add_new_purgeable_resource(skiatest::Reporter* reporter,
                                     const SharedContext* sharedContext,
                                     ResourceCache* resourceCache,
                                     size_t gpuMemorySize) {
    auto resource = add_new_resource(reporter, sharedContext, resourceCache, gpuMemorySize);
    if (!resource) {
        return nullptr;
    }

    Resource* ptr = resource.get();
    resource.reset();
    resourceCache->forceProcessReturnedResources();
    return ptr;
}
} // namespace

DEF_GRAPHITE_TEST_FOR_ALL_CONTEXTS(GraphitePurgeAsNeededResourcesTest, reporter, context) {
    std::unique_ptr<Recorder> recorder = context->makeRecorder();
    ResourceProvider* resourceProvider = recorder->priv().resourceProvider();
    ResourceCache* resourceCache = resourceProvider->resourceCache();
    const SharedContext* sharedContext = resourceProvider->sharedContext();

    resourceCache->setMaxBudget(10);

    auto resourceSize10 = add_new_resource(reporter,
                                           sharedContext,
                                           resourceCache,
                                           /*gpuMemorySize=*/10);

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 10);

    auto resourceSize1 = add_new_resource(reporter,
                                          sharedContext,
                                          resourceCache,
                                          /*gpuMemorySize=*/1);

    // We should now be over budget, but nothing should be purged since neither resource is
    // purgeable.
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 11);

    // Dropping the ref to the size 1 resource should cause it to get purged when we add a new
    // resource to the cache.
    resourceSize1.reset();

    auto resourceSize2 = add_new_resource(reporter,
                                          sharedContext,
                                          resourceCache,
                                          /*gpuMemorySize=*/2);

    // The purging should have happened when we return the resource above so we also shouldn't
    // see anything in the purgeable queue.
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 12);

    // Reset the cache back to no resources by setting budget to 0.
    resourceSize10.reset();
    resourceSize2.reset();
    resourceCache->forceProcessReturnedResources();
    resourceCache->setMaxBudget(0);

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 0);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 0);

    // Add a bunch of purgeable resources that keeps us under budget. Nothing should ever get purged.
    resourceCache->setMaxBudget(10);
    auto resourceSize1Ptr = add_new_purgeable_resource(reporter,
                                                       sharedContext,
                                                       resourceCache,
                                                       /*gpuMemorySize=*/1);
    /*auto resourceSize2Ptr=*/ add_new_purgeable_resource(reporter,
                                                          sharedContext,
                                                          resourceCache,
                                                          /*gpuMemorySize=*/2);
    auto resourceSize3Ptr = add_new_purgeable_resource(reporter,
                                                       sharedContext,
                                                       resourceCache,
                                                       /*gpuMemorySize=*/3);
    auto resourceSize4Ptr = add_new_purgeable_resource(reporter,
                                                       sharedContext,
                                                       resourceCache,
                                                       /*gpuMemorySize=*/4);

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 4);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == resourceSize1Ptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 10);

    // Now add some resources that should cause things to get purged.
    // Add a size 2 resource should purge the original size 1 and size 2
    add_new_purgeable_resource(reporter,
                               sharedContext,
                               resourceCache,
                               /*gpuMemorySize=*/2);

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 3);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == resourceSize3Ptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 9);

    // Adding a non-purgeable resource should also trigger resources to be purged from purgeable
    // queue.
    resourceSize10 = add_new_resource(reporter,
                                      sharedContext,
                                      resourceCache,
                                      /*gpuMemorySize=*/10);

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 10);

    // Adding a resources that is purgeable back to the cache shouldn't trigger the previous
    // non-purgeable resource or itself to be purged yet (since processing our return mailbox
    // doesn't trigger the purgeAsNeeded call)
    resourceSize4Ptr = add_new_purgeable_resource(reporter,
                                                  sharedContext,
                                                  resourceCache,
                                                  /*gpuMemorySize=*/4);

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == resourceSize4Ptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 14);

    // Resetting the budget to 0 should trigger purging the size 4 purgeable resource but should
    // leave the non purgeable size 10 alone.
    resourceCache->setMaxBudget(0);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 10);

    resourceSize10.reset();
    resourceCache->forceProcessReturnedResources();
    resourceCache->forcePurgeAsNeeded();

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 0);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);
    REPORTER_ASSERT(reporter, resourceCache->currentBudgetedBytes() == 0);
}

DEF_GRAPHITE_TEST_FOR_ALL_CONTEXTS(GraphiteZeroSizedResourcesTest, reporter, context) {
    std::unique_ptr<Recorder> recorder = context->makeRecorder();
    ResourceProvider* resourceProvider = recorder->priv().resourceProvider();
    ResourceCache* resourceCache = resourceProvider->resourceCache();
    const SharedContext* sharedContext = resourceProvider->sharedContext();

    // First make a normal resource that has a non zero size
    Resource* resourcePtr = add_new_purgeable_resource(reporter,
                                                       sharedContext,
                                                       resourceCache,
                                                       /*gpuMemorySize=*/1);
    if (!resourcePtr) {
        return;
    }

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 1);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == resourcePtr);

    // First confirm if we set the max budget to zero, this sized resource is removed.
    resourceCache->setMaxBudget(0);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 0);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 0);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == nullptr);

    // Set the budget back to something higher
    resourceCache->setMaxBudget(100);

    // Now create a zero sized resource and add it to the cache.
    resourcePtr = add_new_purgeable_resource(reporter,
                                             sharedContext,
                                             resourceCache,
                                             /*gpuMemorySize=*/0);
    if (!resourcePtr) {
        return;
    }

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 1);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == resourcePtr);

    // Setting the budget down to 0 should not cause the zero sized resource to be purged
    resourceCache->setMaxBudget(0);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 1);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 1);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == resourcePtr);

    // Now add a sized resource to cache. Set budget higher again so that it fits
    resourceCache->setMaxBudget(100);

    Resource* sizedResourcePtr = add_new_purgeable_resource(reporter,
                                                            sharedContext,
                                                            resourceCache,
                                                            /*gpuMemorySize=*/1);
    if (!resourcePtr) {
        return;
    }

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 2);
    // Even though the zero sized resource was added to the cache first, the top of the purgeable
    // stack should be the sized resource.
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == sizedResourcePtr);

    // Add another zero sized resource
    resourcePtr = add_new_purgeable_resource(reporter,
                                             sharedContext,
                                             resourceCache,
                                             /*gpuMemorySize=*/0);
    if (!resourcePtr) {
        return;
    }

    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 3);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 3);
    // Again the sized resource should still be the top of the purgeable queue
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue() == sizedResourcePtr);

    // If we set the cache budget to 0, it should clear out the sized resource but leave the two
    // zero-sized resources.
    resourceCache->setMaxBudget(0);
    REPORTER_ASSERT(reporter, resourceCache->getResourceCount() == 2);
    REPORTER_ASSERT(reporter, resourceCache->numFindableResources() == 2);
    REPORTER_ASSERT(reporter, resourceCache->topOfPurgeableQueue()->gpuMemorySize() == 0);
}

}  // namespace skgpu::graphite
