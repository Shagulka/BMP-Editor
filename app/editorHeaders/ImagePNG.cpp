#include <bits/stdc++.h>
#include "classes/ImageBMP.h"
#include "classes/lodepng.h"

enum CHANNELS {
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    ALPHA = 3
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

    [[nodiscard]] unsigned getWidth() const{
        return width;
    }

    [[nodiscard]] unsigned getHeight() const{
        return height;
    }

    void save(){
        std::vector<unsigned char> png;
        unsigned error = lodepng::encode(png, rawPixels, width, height);
        if (!error){
            lodepng::save_file(png, fileName);
        }
        else {
            std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << "\n";
        }
    }

    void save(const char* newName){
        fileName = newName;
        save();
    }

    void changeTransparency(unsigned char newAlpha){
        for (int i = 0;  i < width * height * 4; ++i){
            if (i % 4 == CHANNELS::ALPHA){
                rawPixels[i] = newAlpha;
            }
        }
    }

    void changeColor(CHANNELS color, unsigned char value){
        for (int i = 0; i < width * height * 4; ++i){
            if (i % 4 == color){
                rawPixels[i] = value;
            }
        }
    }
    

};

using namespace BMP;

int main() {
    ImagePNG img("456.png");
    img.changeColor(CHANNELS::GREEN, 0);
    img.save("457.png");
    return 0;
}
