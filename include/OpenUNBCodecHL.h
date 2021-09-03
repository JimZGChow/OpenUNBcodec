#include "OpenUNBDecoderLL.h"
#include "OpenUNBEncoderLL.h"


void initOpenUNBCodec();

std::vector<uint8_t> encode_64(const std::vector<uint8_t>& data);
std::vector<uint8_t> encode_96(const std::vector<uint8_t>& data);
std::vector<uint8_t> decode_64(const std::vector<float>& data);
std::vector<uint8_t> decode_96(const std::vector<float>& data);

std::vector<uint8_t> getVectorFromArray(uint8_t* array, int size);
std::vector<uint8_t> getVectorFromStringHex(std::string s);
std::vector<uint8_t> getVectorFromStringBin(std::string s);
std::vector<uint8_t> getRandomVector(int size);
std::string getStringHexFromVector(std::vector<uint8_t>);
std::string getStringBinFromVector(std::vector<uint8_t>);
