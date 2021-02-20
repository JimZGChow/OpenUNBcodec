#include <iostream>
#include <vector>
#include <ctime>
#include <OpenUNBCodecHL.h>

void init();
bool test64();
bool test96();

int main() {
    srand(time(0));
    init();
    test64();
}

bool test96() {
    std::vector<uint8_t> data_in = getRandomVector(96);

    std::cout << "data: size = " << data_in.size() << " bit" << std::endl;
    std::cout << getStringBinFromVector(data_in) << std::endl;
    std::cout << getStringHexFromVector(data_in) << std::endl;
    std::cout << std::endl;

    std::vector<uint8_t> data_in_crc = add_crc(0x327, data_in);

    std::cout << "data_with_crc: size = " << data_in_crc.size() << " bit" << std::endl;
    std::cout << getStringBinFromVector(data_in_crc) << std::endl;
    std::cout << std::endl;

    std::vector<uint8_t> data_enc = encode_96(data_in_crc);

    std::cout << "data_encoded: size = " << data_enc.size() << " bit" << std::endl;
    std::cout << getStringBinFromVector(data_enc) << std::endl;
    std::cout << getStringHexFromVector(data_enc) << std::endl;
    std::cout << std::endl;

    std::vector<float> data_enc_corrupted(data_enc.size());
    for (int i = 0; i < data_enc.size(); i++)
        data_enc_corrupted[i] = data_enc[i] ? -1.0 : 1.0;

    std::vector<uint8_t> data_dec = decode_96(data_enc_corrupted);
    if (data_dec.size() > 0) {
        std::cout << "decoded!" << std::endl;

        std::cout << "data_dec == data_in_crc test: " << (data_dec == data_in_crc ? "true" : "false") << std::endl;
        return data_dec == data_in_crc;
    }
    else {
        std::cout << "decod error!!" << std::endl;
        return false;
    }
}

bool test64() {
    std::vector<uint8_t> data_in = getRandomVector(64);

    std::cout << "data: size = " << data_in.size() << " bit" << std::endl;
    std::cout << getStringBinFromVector(data_in) << std::endl;
    std::cout << getStringHexFromVector(data_in) << std::endl;
    std::cout << std::endl;

    std::vector<uint8_t> data_in_crc = add_crc(0x327, data_in);

    std::cout << "data_with_crc: size = " << data_in_crc.size() << " bit" << std::endl;
    std::cout << getStringBinFromVector(data_in_crc) << std::endl;
    std::cout << std::endl;

    std::vector<uint8_t> data_enc = encode_64(data_in_crc);

    std::cout << "data_encoded: size = " << data_enc.size() << " bit" << std::endl;
    std::cout << getStringBinFromVector(data_enc) << std::endl;
    std::cout << getStringHexFromVector(data_enc) << std::endl;
    std::cout << std::endl;

    std::vector<float> data_enc_corrupted(data_enc.size());
    for (int i = 0; i < data_enc.size(); i++)
        data_enc_corrupted[i] = data_enc[i] ? -1.0 : 1.0;

    std::vector<uint8_t> data_dec = decode_64(data_enc_corrupted);
    if (data_dec.size() > 0) {
        std::cout << "decoded!" << std::endl;
        std::cout << getStringBinFromVector(data_dec) << std::endl;

        std::cout << "data_dec == data_in_crc test: " << (data_dec == data_in_crc ? "true" : "false") << std::endl;
        return data_dec == data_in_crc;
    }
    else {
        std::cout << "decod error!!" << std::endl;
        return false;
    }
}
