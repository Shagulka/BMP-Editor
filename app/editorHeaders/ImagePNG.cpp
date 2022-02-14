#include <bits/stdc++.h>
#include "lodepng.h"

enum CHANNELS {
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    ALPHA = 3
};

const unsigned int CHANNELS_COUNT = 4;

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

    void turnRight() {
        std::vector <unsigned char> output(rawPixels.size());
        for (int X = 0; X < width; ++X){
            for (int Y = 0;  Y < height; ++Y) {
                for (int chan = 0; chan < CHANNELS_COUNT; ++chan){
                    output[(X * height + Y) * CHANNELS_COUNT + chan] = rawPixels[((height - 1 - Y) * width + X) * CHANNELS_COUNT + chan];
                }
            }
        }
        rawPixels = output;
        std::swap(width, height);
    }

    void turnLeft(){
        turnRight();
        turnRight();
        turnRight();
    }

    void flipVertical() {
        std::vector<unsigned char> output(rawPixels.size());
        unsigned short Colors = 4;
        for (unsigned int X = 0; X < width; X++) {
            for (unsigned int Y = 0; Y < height; Y++) {
                for (unsigned int P = 0; P < Colors; P++) {
                    output[(X + Y * width) * Colors + P] = rawPixels[(X + (height - 1 - Y) * width) * Colors + P];
                }
            }
        }
        rawPixels = output;
    }

    void flipHorizontal() {
        turnRight();
        turnRight();
        flipVertical();
    }

};

int main() {
    std::vector<unsigned char> RGBA = {190, 70, 150, 254};
    ImagePNG img("461.png");
    img.flipVertical();
    img.save();
    return 0;
}
