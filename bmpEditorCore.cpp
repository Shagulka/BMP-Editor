#include <bits/stdc++.h>

std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());

unsigned char transformToDecimal(const std::string &hex) {
    unsigned char ans = 0;
    for (int i = (int) hex.size() - 1; i >= 0; --i) {
        ans += (hex[i] - '0') * (int) pow(16, (int) hex.size() - i);
    }
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
    std::pair<unsigned int, unsigned int> widhtAndHeight;

    struct Pixel {
        unsigned char red, green, blue;

        Pixel() = default;

        Pixel(int _red, int _green, int _blue) : red(_red), green(_green), blue(_blue) {}
    };

public:
    std::vector<std::vector<Pixel>> rawPixelData;

    ImageBMP() = default;

    ImageBMP(std::string &_fileName) {
        fileName = _fileName;
        std::ifstream inputFile(fileName, std::ios_base::binary);
        inputFile.seekg(18);
        unsigned int curr;
        inputFile.read((char *) &curr, 4);
        widhtAndHeight.first = curr;
        inputFile.read((char *) &curr, 4);
        widhtAndHeight.second = curr;
        inputFile.seekg(10);
        unsigned int PixelDataOffset;
        inputFile.read((char *) &PixelDataOffset, 4);
        inputFile.seekg(PixelDataOffset);
        rawPixelData.resize(widhtAndHeight.second);
        for (int i = 0; i < widhtAndHeight.second; ++i) {
            for (int j = 0; j < widhtAndHeight.first; ++j) {
                rawPixelData[i].push_back({});
                inputFile.read((char *) &curr, 1);
                rawPixelData[i].back().red = curr;
                inputFile.read((char *) &curr, 1);
                rawPixelData[i].back().green = curr;
                inputFile.read((char *) &curr, 1);
                rawPixelData[i].back().blue = curr;
            }
            for (int j = 0; j < (4 - widhtAndHeight.first * 3 % 4) % 4; ++j){
                char byte;
                inputFile.read(&byte, 1);
            }
        }
    }

    [[nodiscard]] std::string getFileName() const {
        return fileName;
    }

    [[nodiscard]] unsigned int getHeight() const {
        return widhtAndHeight.second;
    }

    [[nodiscard]] unsigned int getWidth() const {
        return widhtAndHeight.first;
    }

    void changePixel(int i, int j, Pixel newPixel) {
        if (i > rawPixelData.size() || rawPixelData.size() == 0 || rawPixelData[0].size() <= j) {
            std::cerr << "YOU ARE DUMB\n";
            throw SIGSEGV;
        }
        rawPixelData[i][j].red = newPixel.red;
        rawPixelData[i][j].blue = newPixel.blue;
        rawPixelData[i][j].green = newPixel.green;
    }

    Pixel makeGrey(Pixel thisPixel){
        unsigned int R = thisPixel.red, B = thisPixel.blue, G = thisPixel.green;
        thisPixel.red = (R + B + G) / 3;
        thisPixel.blue = (R + B + G) / 3;
        thisPixel.green = (R + B + G) / 3;
        return thisPixel;
    }

    void save() {
        std::ofstream outputFile(fileName, std::ios_base::binary);
        createHeader(outputFile, widhtAndHeight.first, widhtAndHeight.second);
        if (rawPixelData.size() < widhtAndHeight.second) {
            rawPixelData.resize(widhtAndHeight.second);
        }
        outputFile.seekp(54);
        for (int i = 0; i < widhtAndHeight.second; ++i) {
            for (int j = 0; j < widhtAndHeight.first; ++j) {
                outputFile.write((char *) &rawPixelData[i][j].red, 1);
                outputFile.write((char *) &rawPixelData[i][j].green, 1);
                outputFile.write((char *) &rawPixelData[i][j].blue, 1);
            }
            for (int j = 0; j < (4 - widhtAndHeight.first * 3 % 4) % 4; ++j){
                char byte = 0;
                outputFile.write(&byte, 1);
            }
        }
    }

    void createHeader(std::ostream &outputFile, unsigned int width, unsigned int height) {
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
        createHeader(outputFile, widhtAndHeight.first, widhtAndHeight.second);
        outputFile.seekp(54);
        for (int i = 0; i < widhtAndHeight.second; ++i) {
            for (int j = 0; j < widhtAndHeight.first; ++j) {
                outputFile.write((char *) &rawPixelData[i][j].red, 1);
                outputFile.write((char *) &rawPixelData[i][j].green, 1);
                outputFile.write((char *) &rawPixelData[i][j].blue, 1);
            }
            for (int j = 0; j < (4 - widhtAndHeight.first * 3 % 4) % 4; ++j){
                char byte = 1;
                outputFile.write(&byte, 1);
            }
        }
    }

    ImageBMP(std::string &_fileName, unsigned int width, unsigned int height, std::string &backGroundColor) {
        fileName = _fileName;
        widhtAndHeight.first = width;
        widhtAndHeight.second = height;
        std::ofstream outputFile(_fileName, std::ios_base::binary);
        rawPixelData.resize(height);
        createHeader(outputFile, width, height);
        unsigned char R = transformToDecimal(std::string({backGroundColor[0], backGroundColor[1]}));
        unsigned char G = transformToDecimal(std::string({backGroundColor[2], backGroundColor[3]}));
        unsigned char B = transformToDecimal(std::string({backGroundColor[4], backGroundColor[5]}));
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
            for (int j = 0; j < (4 - width * 3 % 4) % 4; ++j){
                char byte = 1;
                outputFile.write(&byte, 1);
            }
        }
    }

    void disableChannel(RGB channel) {
        for (int i = 0; i < widhtAndHeight.second; ++i){
            for (int j = 0; j < widhtAndHeight.first; ++j){
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

    void toGreyScale(){
        for (int i = 0; i < widhtAndHeight.second; ++i){
            for (int j = 0; j < widhtAndHeight.first; ++j){
                rawPixelData[i][j] = makeGrey(rawPixelData[i][j]);
            }
        }
    }
    void flipHorizontal(){
        for (int i = 0; i < widhtAndHeight.second; ++i){
            for (int j = 0; j < widhtAndHeight.first / 2; ++j){
                std::swap(rawPixelData[i][j], rawPixelData[i][widhtAndHeight.first - j - 1]);
            }
        }
    }
    void flipVertical(){
        for (int i = 0; i < widhtAndHeight.second / 2; ++i){
            for (int j = 0; j < widhtAndHeight.first; ++j){
                std::swap(rawPixelData[i][j], rawPixelData[widhtAndHeight.second - i - 1][j]);
            }
        }
    }
    void turnRight(){
        std::vector <std::vector<Pixel>> ans(widhtAndHeight.first, std::vector<Pixel> (widhtAndHeight.second, Pixel(0, 0, 0)));
        for (int i = 0; i < widhtAndHeight.second; i++) {
            for (int j = i; j < widhtAndHeight.first; j++) {
                ans[j][widhtAndHeight.second - 1 - i] = rawPixelData[i][j];
            }
        }
        rawPixelData = ans;
    }
};

int main() {
    // ignore this
    std::string s = "matinf_photo.bmp", s1 = "amoga.bmp", s2 = "1.bmp";
    ImageBMP amoga(s);
    amoga.save(s1);
    amoga.turnRight();
    amoga.save();
    return 0;
}
