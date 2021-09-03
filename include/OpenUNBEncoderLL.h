#include <vector>
#include <stdint.h>

std::vector<uint8_t> stdpolar_encode_systematic_noperm(const std::vector<uint8_t>& _iwd, const std::vector<uint8_t>& _frozen_indicator);
std::vector<uint8_t> solve_recursively_prep(std::vector<int>& _inf_idx, std::vector<uint8_t>& _u, std::vector<uint8_t>& _x, int last);
void solve_recursively(std::vector<int>* _inf_idx, uint8_t* _u, int uSize, uint8_t* _x, int xSize, int last);
std::vector<uint8_t> add_crc(const uint32_t& polynom, const std::vector<uint8_t>& a);
