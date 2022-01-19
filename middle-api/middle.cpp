#include <assert.h>
#include <bits/stdc++.h>
#include <node_api.h>

#include "../ImageBMP.h"

static napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  assert(status == napi_ok);

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  int value0;
  status = napi_get_value_int32(env, args[0], &value0);
  assert(status == napi_ok);

  int value1;
  status = napi_get_value_int32(env, args[1], &value1);
  assert(status == napi_ok);

  std::string x = "image.bmp";
  std::string col = "FFFF00";

  BMP::ImageBMP a(x, value1, value0, col);
  a.save();
  napi_value world;
  status = napi_create_string_utf8(env, "world", 5, &world);
  assert(status == napi_ok);
  return world;
}

static napi_value toGray(napi_env env, napi_callback_info info) {
  napi_status status;
  std::string x = "image.bmp";
  BMP::ImageBMP a(x);
  a.toGreyScale();
  a.save();
  napi_value world;
  return world;
}

#define DECLARE_NAPI_METHOD(name, func) \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc1 = DECLARE_NAPI_METHOD("hello", Method);
  status = napi_define_properties(env, exports, 1, &desc1);
  napi_property_descriptor desc2 = DECLARE_NAPI_METHOD("toGray", toGray);
  status = napi_define_properties(env, exports, 1, &desc2);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(testaddon, Init)
