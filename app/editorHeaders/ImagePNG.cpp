#include <bits/stdc++.h>
#include "classes/ImageBMP.h"
#include "classes/lodepng.h"

enum CHANNELS {
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    ALPHA = 3
};

/* иногда удобнее хранить пиксель просто как 4 последовательных байта в массиве
 * иногда удобнее в виде структуры
 * однако, для save() нужен именно первый вариант
 */

struct Pixel{
    unsigned char R, G, B, A;
    Pixel() = default;
    Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a){
        R = r;
        G = g;
        B = b;
        A = a;
    }
};

class ImagePNG {
private:
    unsigned width, height;
    std::vector<unsigned char> rawPixels;
    const char *fileName;
public:

    ImagePNG() = default;

    explicit ImagePNG(const char *filename) {
        fileName = filename;
        rawPixels = {};
        width = 0;
        height = 0;
        unsigned error = lodepng::decode(rawPixels, width, height, fileName);
        //width, heigth and rawPixels are all passed by link and will be changed in lodepng::decode
        if (error) {
            std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << "\n";
        }
    }

    ~ImagePNG() {
        rawPixels = {};
        fileName = "";
    }

    [[nodiscard]] unsigned getWidth() const {
        return width;
    }

    [[nodiscard]] unsigned getHeight() const {
        return height;
    }

    void save() {
        std::vector<unsigned char> png;
        unsigned error = lodepng::encode(png, rawPixels, width, height);
        if (!error) {
            lodepng::save_file(png, fileName);
        } else {
            std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << "\n";
        }
    }

    void save(const char *newName) {
        fileName = newName;
        save();
    }

    void changeTransparency(unsigned char newAlpha) {
        for (int i = 0; i < width * height * 4; ++i) {
            if (i % 4 == CHANNELS::ALPHA) {
                rawPixels[i] = newAlpha;
            }
        }
    }

    void changeColor(CHANNELS color, unsigned char value) {
        for (int i = 0; i < width * height * 4; ++i) {
            if (i % 4 == color) {
                rawPixels[i] = value;
            }
        }
    }

    //RGBA size is 4, this function makes monotonous png
    ImagePNG(const char *filename, unsigned _width, unsigned _height, std::vector<unsigned char> &RGBA) {
        fileName = filename;
        width = _width;
        height = _height;
        rawPixels.resize(4 * width * height);
        for (int i = 0; i < 4 * width * height - (int) RGBA.size(); i += (int) RGBA.size()) {
            for (int j = 0; j < (int) RGBA.size(); ++j) {
                rawPixels[i + j] = RGBA[j];
            }
        }
        save();
    }

    void toGreyScale() {
        unsigned int tempSum = 0;
        for (int i = 0; i < (int) rawPixels.size(); ++i) {
            if (i % 4 == CHANNELS::ALPHA) {
                rawPixels[i - 3] = (tempSum / 3);
                rawPixels[i - 2] = (tempSum / 3);
                rawPixels[i - 1] = (tempSum / 3);
                tempSum = 0;
            } else {
                tempSum += rawPixels[i];
            }
        }
    }

    void turnLeft() { //TODO fix
        std::vector<std::vector<unsigned char>> temp(height, std::vector<unsigned char>(4 * width));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < 4 * width; ++j) {
                temp[i][j] = rawPixels[i * (4 * width) + j];
            }
        }
        int m = (int) temp.size(), n = (int) temp[0].size();
        std::vector<std::vector<unsigned char>> tempTemp(n,
                                                         std::vector<unsigned char>(m));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                tempTemp[j][m - 1 - i] = temp[i][j];
            }
        }
        for (int i = 0; i < (int) tempTemp[i].size(); ++i) {
            for (int j = 0; j < (int) tempTemp.size(); ++j) {
                rawPixels[i * 4 * width + j] = tempTemp[j][i];
            }
        }
        std::swap(width, height);
    }

    void flipVertical(){ //TODO fix
        std::vector <Pixel> temp(width * height);
        for (int i = 0; i < (int)rawPixels.size() / 4; i += 4){
            temp[i] = {rawPixels[i], rawPixels[i + 1], rawPixels[i + 2], rawPixels[i + 3]};
        }
        std::vector<Pixel> ans(width * height);
        for (int i = (int)temp.size(); i >= 0; --i){
            ans[temp.size() - i - 1] = temp[i];
        }
        rawPixels.clear();
        for (int i = 0; i < width * height; ++i){
            rawPixels.push_back(ans[i].R);
            rawPixels.push_back(ans[i].G);
            rawPixels.push_back(ans[i].B);
            rawPixels.push_back(ans[i].A);
        }
    }

};

using namespace BMP;

int main() {
    std::vector<unsigned char> RGBA = {190, 70, 150, 254};
    ImagePNG img("461.png");
    img.flipVertical();
    img.save();
    return 0;
}
