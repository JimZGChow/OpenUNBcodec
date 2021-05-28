#include "OpenUNBCodecHL.h"

uint8_t info_bit_pattern_64[] = { 0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
const int num_of_nonzero_bits_64 = 74;
const int short_64 = 0;
uint8_t info_bit_pattern_96[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
const int num_of_nonzero_bits_96 = 170;
const int short_96 = 64;

int* short_mask;
uint8_t* frozen_indicator_64;
uint8_t* frozen_indicator_96;
uint16_t* crc_mask_64;
uint16_t* crc_mask_96;

std::vector<uint8_t> decode_96(const std::vector<float>& data) {
    std::vector<std::vector<float>> prob;
    std::vector<std::vector<uint8_t>> dec = pcscl_prep(8, 16, data, &prob, info_bit_pattern_96);

    dec = polar_transform_noperm(dec);

    dec = extract_with_filter(dec, crc_mask_96, num_of_nonzero_bits_96 - short_96);

    std::vector<uint8_t> crc_err;

    crc_err = crc_ok_array(0x327, dec);

    //std::cout << " crc_ok: " << getStringBinFromVector(crc_err) << std::endl;

    float max = -1000000000000;
    int index = -1;

    for (int i = 0; i < crc_err.size(); i++) {
        if (crc_err[i]) {
            if (prob[i][prob[i].size() - 1] > max) {
                max = prob[i][prob[i].size() - 1];
                index = i;
            }
        }
    }

    if (index == -1)
        return std::vector<uint8_t>(0);
    else
        return dec[index];
}

std::vector<uint8_t> decode_64(const std::vector<float>& data) {
    std::vector<std::vector<float>> prob;
    std::vector<std::vector<uint8_t>> dec = pcscl_prep(7, 16, data, &prob, info_bit_pattern_64);

    dec = polar_transform_noperm(dec);

    dec = extract_with_filter(dec, crc_mask_64, num_of_nonzero_bits_64 - short_64);

    std::vector<uint8_t> crc_err;

    crc_err = crc_ok_array(0x327, dec);

    float max = -1000000000000;
    int index = -1;

    for (int i = 0; i < crc_err.size(); i++) {
        if (crc_err[i]) {
            if (prob[i][prob[i].size() - 1] > max) {
                max = prob[i][prob[i].size() - 1];
                index = i;
            }
        }
    }

    if (index == -1)
        return std::vector<uint8_t>(0);
    else
        return dec[index];
}

std::vector<uint8_t> getRandomVector(int size) {
    std::vector<uint8_t> ret(size);

    for (int i = 0; i < size; i++) {
        ret[i] = rand() % 2;
    }

    return ret;
}

std::string getStringHexFromVector(std::vector<uint8_t> data) {
    std::string ret = "";
    for (int i = data.size() / 4 - 1; i >= 0; i--) {
        uint8_t hex = 0;
        for (int j = 0; j < 4; j++) {
            hex = (hex << 1) | data[i * 4 + 3 - j];
        }

        if (hex < 10)
            ret.push_back(hex + '0');
        else
            ret.push_back(hex + 'A' - 10);
    }

    return ret;
}

std::string getStringBinFromVector(std::vector<uint8_t> data) {
    std::string ret = "";
    for (int i = 0; i < data.size(); i++) {
        //for (int i = data.size() - 1; i >= 0; i--) {
        ret.push_back(data[i] + '0');
        //ret.push_back(',');
    }

    return ret;
}


std::vector<uint8_t> getVectorFromStringHex(std::string s) {
    std::vector<uint8_t> ret;

    //for (int i = 0; i < s.length(); i++) {
    for (int i = s.length() - 1; i >= 0; i--) {
        uint8_t halfByte;

        if (s[i] >= 'a' && s[i] <= 'f') {
            halfByte = 10 + s[i] - 'a';
        }
        else
            if (s[i] >= 'A' && s[i] <= 'F') {
                halfByte = 10 + s[i] - 'A';
            }
            else
                if (s[i] >= '0' && s[i] <= '9') {
                    halfByte = s[i] - '0';
                }
                else
                    continue;

        for (int j = 0; j < 4; j++) {
            ret.push_back((halfByte >> j) & 1);
        }
    }

    return ret;
}

std::vector<uint8_t> getVectorFromStringBin(std::string s) {
    std::vector<uint8_t> ret;

    //for (int i = 0; i < s.length(); i++) {
    for (int i = s.length() - 1; i >= 0; i--) {
        if (s[i] == '0')
            ret.push_back(0);
        else
            if (s[i] == '1')
                ret.push_back(1);
    }

    return ret;
}

std::vector<uint8_t> getVectorFromArray(uint8_t* array, int size) {
    std::vector<uint8_t> ret(size);

    for (int i = 0; i < size; i++) {
        ret[i] = array[i];
    }

    return ret;
}

std::vector<uint8_t> encode_64(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> frozen_indicator = getVectorFromArray(frozen_indicator_64, sizeof(info_bit_pattern_64));
    return stdpolar_encode_systematic_noperm(data, frozen_indicator);
}

std::vector<uint8_t> encode_96(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> frozen_indicator = getVectorFromArray(frozen_indicator_96, sizeof(info_bit_pattern_96));
    //std::vector<uint8_t> frozen_indicator = getVectorFromStringHex("1011F013F7FFF011717FF17FFFFFF0001077F177F7FFF177FFFFFFFFFFFFF");
    std::vector<uint8_t> dataS = data;
    for (int i = 0; i < short_96; i++)
        dataS.push_back(0);

    return stdpolar_encode_systematic_noperm(dataS, frozen_indicator);
}

void initOpenUNBCodec() {
    frozen_indicator_64 = new uint8_t[sizeof(info_bit_pattern_64)];
    frozen_indicator_96 = new uint8_t[sizeof(info_bit_pattern_96)];

    for (int i = 0; i < sizeof(info_bit_pattern_64); i++) {
        frozen_indicator_64[i] = !info_bit_pattern_64[i];
    }

    for (int i = 0; i < sizeof(info_bit_pattern_96); i++) {
        frozen_indicator_96[i] = !info_bit_pattern_96[i];
    }

    short_mask = new int[64];

    for (int i = 0, j = sizeof(info_bit_pattern_96) - 1; i < 64; j--) {
        if (info_bit_pattern_96[j] == 1) {
            short_mask[i++] = j + 1;
        }
    }

    for (int i = 0; i < 32; i++) {
        int tmp = short_mask[i];
        short_mask[i] = short_mask[63 - i];
        short_mask[63 - i] = tmp;
    }

    crc_mask_64 = new uint16_t[num_of_nonzero_bits_64 - short_64];
    crc_mask_96 = new uint16_t[num_of_nonzero_bits_96 - short_96];

    for (int i = 0, j = 0; j < (num_of_nonzero_bits_64 - short_64); i++) {
        if (info_bit_pattern_64[i]) {
            crc_mask_64[j++] = i;
        }
    }

    for (int i = 0, j = 0; j < (num_of_nonzero_bits_96 - short_96); i++) {
        if (info_bit_pattern_96[i]) {
            crc_mask_96[j++] = i;
        }
    }

}
