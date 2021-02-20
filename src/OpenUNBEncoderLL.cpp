#include "OpenUNBEncoderLL.h"

std::vector<uint8_t> stdpolar_encode_systematic_noperm(const std::vector<uint8_t>& _iwd, const std::vector<uint8_t>& _frozen_indicator) {
    int n_polar = _frozen_indicator.size();
    std::vector<uint8_t> iwd_s(n_polar);
    std::vector<int> inf_idx;
    std::vector<uint8_t> u(n_polar);

    for (int i = 0, j = 0; i < iwd_s.size(); i++) {
        if (_frozen_indicator[i] == 0) {
            iwd_s[i] = _iwd[j++];
            inf_idx.push_back(i + 1);
        }
        else {
            iwd_s[i] = 0;
        }
        u[i] = 0;
    }

    return solve_recursively_prep(inf_idx, u, iwd_s, n_polar);
}

std::vector<uint8_t> solve_recursively_prep(std::vector<int>& _inf_idx, std::vector<uint8_t>& _u, std::vector<uint8_t>& _x, int last) {
    solve_recursively(&_inf_idx, _u.data(), _u.size(), _x.data(), _x.size(), last);
    return _x;
}

void solve_recursively(std::vector<int>* _inf_idx, uint8_t* _u, int uSize, uint8_t* _x, int xSize, int last) {
    int n = xSize;

    if (n == 1) {
        uint8_t f = false;
        for (int i = 0; i < _inf_idx->size() && !f; i++) {
            if (_inf_idx->at(i) == last) {
                f = true;
            }
        }

        if (f) {
            *_u = *_x;
        }
        else {
            *_x = *_u;
        }
    }
    else {
        int n0 = n / 2;

        solve_recursively(_inf_idx, _u + n0, uSize - n0, _x + n0, xSize - n0, last);

        uint8_t* x_first = new uint8_t[n0];

        for (int i = 0; i < n0; i++) {
            x_first[i] = _x[i] ^ _x[i + n0];
        }

        solve_recursively(_inf_idx, _u, n0, x_first, n0, last - n0);

        for (int i = 0; i < n0; i++) {
            _x[i] = _x[n0 + i] ^ x_first[i];
        }

        delete[] x_first;

    }
}


std::vector<uint8_t> add_crc(const uint32_t& polynom, const std::vector<uint8_t>& a) {
    uint32_t reg = 0;
    for (auto b : a) {
        reg ^= b;
        if (reg & 1) reg = (reg >> 1) ^ polynom;
        else reg >>= 1;
    }
    reg = reg & 0x3FF;

    std::vector<uint8_t> ret(a);

    for (int i = 0; i < 10; i++)
        ret.push_back((reg >> i) & 1);
    return ret;
}
