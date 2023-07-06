struct FSIn {
  @builtin(front_facing) sk_Clockwise: bool,
};
struct FSOut {
  @location(0) sk_FragColor: vec4<f32>,
};
struct _GlobalUniforms {
  src: vec4<f32>,
  dst: vec4<f32>,
};
@binding(0) @group(0) var<uniform> _globalUniforms: _GlobalUniforms;
fn blend_overlay_component_Qhh2h2(_skParam0: vec2<f32>, _skParam1: vec2<f32>) -> f32 {
  let s = _skParam0;
  let d = _skParam1;
  {
    return select(s.y * d.y - (2.0 * (d.y - d.x)) * (s.y - s.x), (2.0 * s.x) * d.x, 2.0 * d.x <= d.y);
  }
}
fn blend_overlay_h4h4h4(_skParam0: vec4<f32>, _skParam1: vec4<f32>) -> vec4<f32> {
  let src = _skParam0;
  let dst = _skParam1;
  {
    let _skTemp0 = blend_overlay_component_Qhh2h2(src.xw, dst.xw);
    let _skTemp1 = blend_overlay_component_Qhh2h2(src.yw, dst.yw);
    let _skTemp2 = blend_overlay_component_Qhh2h2(src.zw, dst.zw);
    var result: vec4<f32> = vec4<f32>(_skTemp0, _skTemp1, _skTemp2, src.w + (1.0 - src.w) * dst.w);
    result = vec4<f32>((result.xyz + (dst.xyz * (1.0 - src.w) + src.xyz * (1.0 - dst.w))), result.w).xyzw;
    return result;
  }
}
fn main(_stageOut: ptr<function, FSOut>) {
  {
    let _skTemp3 = blend_overlay_h4h4h4(_globalUniforms.dst, _globalUniforms.src);
    (*_stageOut).sk_FragColor = _skTemp3;
  }
}
@fragment fn fragmentMain(_stageIn: FSIn) -> FSOut {
  var _stageOut: FSOut;
  main(&_stageOut);
  return _stageOut;
}
