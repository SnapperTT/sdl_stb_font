static const uint8_t vs_textured_passthrough_bin_glsl[] = {
  0x56, 0x53, 0x48, 0x09, 0x00, 0x00, 0x00, 0x00, 0x6f, 0x1e, 0x3e, 0x3c, 0x01, 0x00, 0x0f, 0x75,  // VSH.....o.><...u
  0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01,  // _modelViewProj..
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0e, 0x01, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62,  // ..........attrib
  0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74,  // ute vec3 a_posit
  0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76,  // ion;.attribute v
  0x65, 0x63, 0x32, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b,  // ec2 a_texcoord0;
  0x0a, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6e, 0x67, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x76, 0x5f,  // .varying vec2 v_
  0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f, 0x72, 0x64, 0x30, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f,  // texcoord0;.unifo
  0x72, 0x6d, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56,  // rm mat4 u_modelV
  0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61,  // iewProj;.void ma
  0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74,  // in ().{.  vec4 t
  0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61,  // mpvar_1;.  tmpva
  0x72, 0x5f, 0x31, 0x2e, 0x77, 0x20, 0x3d, 0x20, 0x31, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74,  // r_1.w = 1.0;.  t
  0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x79, 0x7a, 0x20, 0x3d, 0x20, 0x61, 0x5f,  // mpvar_1.xyz = a_
  0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x50,  // position;.  gl_P
  0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x28, 0x75, 0x5f, 0x6d, 0x6f, 0x64,  // osition = (u_mod
  0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70,  // elViewProj * tmp
  0x76, 0x61, 0x72, 0x5f, 0x31, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x76, 0x5f, 0x74, 0x65, 0x78, 0x63,  // var_1);.  v_texc
  0x6f, 0x6f, 0x72, 0x64, 0x30, 0x20, 0x3d, 0x20, 0x61, 0x5f, 0x74, 0x65, 0x78, 0x63, 0x6f, 0x6f,  // oord0 = a_texcoo
  0x72, 0x64, 0x30, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00 };                                          // _texcoord0;.}...
unsigned int vs_textured_passthrough_bin_glsl_size = 313;

BX_UNUSED(vs_textured_passthrough_bin_glsl)
BX_UNUSED(vs_textured_passthrough_bin_glsl_size)
static const uint8_t * vs_textured_passthrough_bin_dx9 = NULL;
unsigned int vs_textured_passthrough_bin_dx9_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_dx9)
BX_UNUSED(vs_textured_passthrough_bin_dx9_size)
static const uint8_t * vs_textured_passthrough_bin_dx11 = NULL;
unsigned int vs_textured_passthrough_bin_dx11_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_dx11)
BX_UNUSED(vs_textured_passthrough_bin_dx11_size)
static const uint8_t * vs_textured_passthrough_bin_metal = NULL;
unsigned int vs_textured_passthrough_bin_metal_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_metal)
BX_UNUSED(vs_textured_passthrough_bin_metal_size)
static const uint8_t * vs_textured_passthrough_bin_spirv = NULL;
unsigned int vs_textured_passthrough_bin_spirv_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_spirv)
BX_UNUSED(vs_textured_passthrough_bin_spirv_size)
static const uint8_t * vs_textured_passthrough_bin_esslandroid = NULL;
unsigned int vs_textured_passthrough_bin_esslandroid_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_esslandroid)
BX_UNUSED(vs_textured_passthrough_bin_esslandroid_size)
static const uint8_t * vs_textured_passthrough_bin_pssl = NULL;
unsigned int vs_textured_passthrough_bin_pssl_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_pssl)
BX_UNUSED(vs_textured_passthrough_bin_pssl_size)
static const uint8_t * vs_textured_passthrough_bin_esslnacl = NULL;
unsigned int vs_textured_passthrough_bin_esslnacl_size = 0;

BX_UNUSED(vs_textured_passthrough_bin_esslnacl)
BX_UNUSED(vs_textured_passthrough_bin_esslnacl_size)