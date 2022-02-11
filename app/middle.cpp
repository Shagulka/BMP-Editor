#include <assert.h>
#include <bits/stdc++.h>
#include <node_api.h>

#include <string>

#include "editorHeaders/ImageBMP.h"
std::string x = "image.bmp";
BMP::ImageBMP a(x);
static napi_value generate(napi_env env, napi_callback_info info) {
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

  std::string col = "FFFF00";
  BMP::ImageBMP gen(x, value1, value0, col);
  gen.save();
  a = gen;
  a.save();
  napi_value world;
  status = napi_create_string_utf8(env, "world", 5, &world);
  assert(status == napi_ok);
  return world;
}

static napi_value prepare(napi_env env, napi_callback_info info) {
  size_t argc = 1;

  napi_value args[1];

  napi_get_cb_info(env, info, &argc, args, NULL, NULL);
  char *client_name;
  size_t str_size;
  napi_get_value_string_utf8(env, args[0], NULL, 0, &str_size);
  str_size += 1;

  client_name = (char *)calloc(str_size + 1, sizeof(char));
  size_t str_size_read;
  napi_get_value_string_utf8(env, args[0], client_name, str_size,
                             &str_size_read);

  // Print the final string
  printf("%s", "ddd");
  std::string filename = std::string(client_name);
  BMP::ImageBMP tmp(filename);
  a = tmp;
  a.save(x);
  
 napi_value result;
  return result;
}

static napi_value toGray(napi_env env, napi_callback_info info) {
  napi_status status;
  a.toGreyScale();
  a.save();
  napi_value world;
  return world;
}

static napi_value turnLeft(napi_env env, napi_callback_info info) {
  napi_status status;
  a.turnLeft();
  a.save();
  napi_value world;
  return world;
}

static napi_value turnRight(napi_env env, napi_callback_info info) {
  napi_status status;
  a.turnRight();
  a.save();
  napi_value world;
  return world;
}

static napi_value flipHorizontal(napi_env env, napi_callback_info info) {
  napi_status status;
  a.flipHorizontal();
  a.save();
  napi_value world;
  return world;
}

static napi_value flipVertical(napi_env env, napi_callback_info info) {
  napi_status status;
  a.flipVertical();
  a.save();
  napi_value world;
  return world;
}

#define DECLARE_NAPI_METHOD(name, func) \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc1 = DECLARE_NAPI_METHOD("generate", generate);
  status = napi_define_properties(env, exports, 1, &desc1);
  napi_property_descriptor desc2 = DECLARE_NAPI_METHOD("toGray", toGray);
  status = napi_define_properties(env, exports, 1, &desc2);
  napi_property_descriptor desc3 = DECLARE_NAPI_METHOD("turnLeft", turnLeft);
  status = napi_define_properties(env, exports, 1, &desc3);
  napi_property_descriptor desc4 = DECLARE_NAPI_METHOD("turnRight", turnRight);
  status = napi_define_properties(env, exports, 1, &desc4);
  napi_property_descriptor desc5 =
      DECLARE_NAPI_METHOD("flipVertical", flipVertical);
  status = napi_define_properties(env, exports, 1, &desc5);
  napi_property_descriptor desc6 =
      DECLARE_NAPI_METHOD("flipHorizontal", flipHorizontal);
  status = napi_define_properties(env, exports, 1, &desc6);
  napi_property_descriptor desc7 = DECLARE_NAPI_METHOD("prepare", prepare);
  status = napi_define_properties(env, exports, 1, &desc7);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(testaddon, Init)
