#include <assert.h>
#include <node_api.h>
#include <bits/stdc++.h>

namespace BMP {

    unsigned char transformToDecimal(const char *hex) {
        unsigned char ans = 0;
        sscanf(hex, "%02hhx", &ans);
        return ans;
    }

    enum class RGB {
        RED,
        GREEN,
        BLUE
    };

    class ImageBMP {
    private:
        std::string fileName;
        std::pair<unsigned int, unsigned int> widthAndHeight;

        struct Pixel {
            unsigned char red, green, blue;

            Pixel() = default;

            Pixel(int _red, int _green, int _blue) : red(_red), green(_green), blue(_blue) {}
        };

    public:
        std::vector<std::vector<Pixel>> rawPixelData;

        ImageBMP() = default;

        explicit ImageBMP(std::string &_fileName) {
            fileName = _fileName;
            std::ifstream inputFile(fileName, std::ios_base::binary);
            inputFile.seekg(18);
            unsigned int curr;
            inputFile.read((char *) &curr, 4);
            widthAndHeight.first = curr;
            inputFile.read((char *) &curr, 4);
            widthAndHeight.second = curr;
            inputFile.seekg(10);
            unsigned int PixelDataOffset;
            inputFile.read((char *) &PixelDataOffset, 4);
            inputFile.seekg(PixelDataOffset);
            rawPixelData.resize(widthAndHeight.second);
            for (int i = 0; i < widthAndHeight.second; ++i) {
                for (int j = 0; j < widthAndHeight.first; ++j) {
                    rawPixelData[i].push_back({});
                    inputFile.read((char *) &curr, 1);
                    rawPixelData[i].back().red = curr;
                    inputFile.read((char *) &curr, 1);
                    rawPixelData[i].back().green = curr;
                    inputFile.read((char *) &curr, 1);
                    rawPixelData[i].back().blue = curr;
                }
                for (int j = 0; j < (4 - widthAndHeight.first * 3 % 4) % 4; ++j) {
                    char byte;
                    inputFile.read(&byte, 1);
                }
            }
        }

        [[nodiscard]] std::string getFileName() const {
            return fileName;
        }

        [[nodiscard]] unsigned int getHeight() const {
            return widthAndHeight.second;
        }

        [[nodiscard]] unsigned int getWidth() const {
            return widthAndHeight.first;
        }

        void changePixel(int i, int j, Pixel newPixel) {
            if (i > rawPixelData.size() || rawPixelData.empty() || rawPixelData[0].size() <= j) {
                std::cerr << "YOU ARE DUMB\n";
                exit(SIGSEGV);
            }
            rawPixelData[i][j].red = newPixel.red;
            rawPixelData[i][j].blue = newPixel.blue;
            rawPixelData[i][j].green = newPixel.green;
        }

        static Pixel makeGrey(Pixel thisPixel) {
            unsigned int R = thisPixel.red, B = thisPixel.blue, G = thisPixel.green;
            thisPixel.red = (R + B + G) / 3;
            thisPixel.blue = (R + B + G) / 3;
            thisPixel.green = (R + B + G) / 3;
            return thisPixel;
        }

        void save() {
            std::ofstream outputFile(fileName, std::ios_base::binary);
            createHeader(outputFile, widthAndHeight.first, widthAndHeight.second);
            if (rawPixelData.size() < widthAndHeight.second) {
                rawPixelData.resize(widthAndHeight.second);
            }
            outputFile.seekp(54);
            for (int i = 0; i < widthAndHeight.second; ++i) {
                for (int j = 0; j < widthAndHeight.first; ++j) {
                    outputFile.write((char *) &rawPixelData[i][j].red, 1);
                    outputFile.write((char *) &rawPixelData[i][j].green, 1);
                    outputFile.write((char *) &rawPixelData[i][j].blue, 1);
                }
                for (int j = 0; j < (4 - widthAndHeight.first * 3 % 4) % 4; ++j) {
                    char byte = 0;
                    outputFile.write(&byte, 1);
                }
            }
        }

        static void createHeader(std::ostream &outputFile, unsigned int width, unsigned int height) {
            outputFile.write("BM", 2);
            unsigned int k = (width * 3) + ((4 - width * 3 % 4) % 4) * height + 54;
            outputFile.write((char *) &k, 4);
            unsigned int curr = 0;
            outputFile.write((char *) &curr, 4);
            curr = 54;
            outputFile.write((char *) &curr, 4);
            curr = 40;
            outputFile.write((char *) &curr, 4);
            outputFile.write((char *) &width, 4);
            outputFile.write((char *) &height, 4);
            curr = 1;
            outputFile.write((char *) &curr, 2);
            curr = 24;
            outputFile.write((char *) &curr, 2);
            curr = 0;
            outputFile.write((char *) &curr, 24);
        }

        void save(std::string FileName) {
            fileName = FileName;
            std::ofstream outputFile(FileName, std::ios_base::binary);
            createHeader(outputFile, widthAndHeight.first, widthAndHeight.second);
            outputFile.seekp(54);
            for (int i = 0; i < widthAndHeight.second; ++i) {
                for (int j = 0; j < widthAndHeight.first; ++j) {
                    outputFile.write((char *) &rawPixelData[i][j].red, 1);
                    outputFile.write((char *) &rawPixelData[i][j].green, 1);
                    outputFile.write((char *) &rawPixelData[i][j].blue, 1);
                }
                for (int j = 0; j < (4 - widthAndHeight.first * 3 % 4) % 4; ++j) {
                    char byte = 1;
                    outputFile.write(&byte, 1);
                }
            }
        }

        ImageBMP(std::string &_fileName, unsigned int width, unsigned int height, std::string &backGroundColor) {
            fileName = _fileName;
            widthAndHeight.first = width;
            widthAndHeight.second = height;
            std::ofstream outputFile(_fileName, std::ios_base::binary);
            rawPixelData.resize(height);
            createHeader(outputFile, width, height);
            std::string a = std::string({backGroundColor[0], backGroundColor[1]});
            const char* cod = a.c_str();
            unsigned char R = transformToDecimal(cod);
            a = std::string({backGroundColor[2], backGroundColor[3]});
            cod = a.c_str();
            unsigned char G = transformToDecimal(cod);
            a = std::string({backGroundColor[4], backGroundColor[5]});
            cod = a.c_str();
            unsigned char B = transformToDecimal(cod);
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    rawPixelData[i].push_back({});
                    outputFile.write((char *) &R, 1);
                    outputFile.write((char *) &G, 1);
                    outputFile.write((char *) &B, 1);
                    rawPixelData[i].back().red = R;
                    rawPixelData[i].back().green = G;
                    rawPixelData[i].back().blue = B;
                }
                for (int j = 0; j < (4 - width * 3 % 4) % 4; ++j) {
                    char byte = 1;
                    outputFile.write(&byte, 1);
                }
            }
        }

        void disableChannel(RGB channel) {
            for (int i = 0; i < widthAndHeight.second; ++i) {
                for (int j = 0; j < widthAndHeight.first; ++j) {
                    switch (channel) {
                        case RGB::RED:
                            rawPixelData[i][j].blue = 0;
                            break;
                        case RGB::GREEN:
                            rawPixelData[i][j].green = 0;
                            break;
                        case RGB::BLUE:
                            rawPixelData[i][j].red = 0;
                            break;
                    }
                }
            }
        }

        void toGreyScale() {
            for (int i = 0; i < widthAndHeight.second; ++i) {
                for (int j = 0; j < widthAndHeight.first; ++j) {
                    rawPixelData[i][j] = makeGrey(rawPixelData[i][j]);
                }
            }
        }

        void flipHorizontal() {
            for (int i = 0; i < widthAndHeight.second; ++i) {
                for (int j = 0; j < widthAndHeight.first / 2; ++j) {
                    std::swap(rawPixelData[i][j], rawPixelData[i][widthAndHeight.first - j - 1]);
                }
            }
        }

        void flipVertical() {
            for (int i = 0; i < widthAndHeight.second / 2; ++i) {
                for (int j = 0; j < widthAndHeight.first; ++j) {
                    std::swap(rawPixelData[i][j], rawPixelData[widthAndHeight.second - i - 1][j]);
                }
            }
        }

        void turnLeft() {
            int m = (int)rawPixelData.size(), n = (int)rawPixelData[0].size();
            std::vector<std::vector<Pixel>> ans(n,
                                                std::vector<Pixel>(m, Pixel(0, 0, 0)));
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    ans[j][m - 1 - i] = rawPixelData[i][j];
                }
            }
            rawPixelData = ans;
            std::swap(widthAndHeight.first, widthAndHeight.second);
            save();
        }
        void turnRight(){
            turnLeft();
            turnLeft();
            turnLeft();
        }
    };
} // namespace BMP

static napi_value Method(napi_env env, napi_callback_info info) {




    napi_status status;

  size_t argc = 3;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, NULL, NULL, NULL);
  assert(status == napi_ok);

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  napi_valuetype valuetype2;
  status = napi_typeof(env, args[2], &valuetype2);
  assert(status == napi_ok);

  int value0;
  status = napi_get_value_int32(env, args[0], &value0);
  assert(status == napi_ok);

  int value1;
  status = napi_get_value_int32(env, args[1], &value1);
  assert(status == napi_ok);







    std::string x = "123.bmp";
    std::string col = "FFFF00";

  BMP::ImageBMP a(x, value1, value0, col);
  a.save();
  napi_value world;
  status = napi_create_string_utf8(env, "world", 5, &world);
  assert(status == napi_ok);
  return world;
}

#define DECLARE_NAPI_METHOD(name, func) \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

static napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("hello", Method);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(testaddon, Init)
