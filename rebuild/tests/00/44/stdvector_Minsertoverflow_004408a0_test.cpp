// 004408a0 std::vector<float>::_M_insert_overflow
// Leaf: ?_M_insert_overflow@?$vector@MV?$allocator@M@std@@@std@@IAEXPAMABMABU__true_type@2@I_N@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include "stlport/fable_containers.h"

namespace std {
template void vector<float>::_M_insert_overflow(float*, const float&, const __true_type&, unsigned int, bool);
}

#include <cstdio>

class OverflowFixture : public std::vector<float> {
public:
    void Insert(unsigned int pos, const float& value, unsigned int count, bool at_end)
    {
        iterator where = pos ? begin() + pos : begin();
        _M_insert_overflow(where, value, std::__true_type(), count, at_end);
    }
};

int main()
{
    for (unsigned int initial = 0; initial <= 4; ++initial) {
        for (unsigned int pos = 0; pos <= initial; ++pos) {
            for (unsigned int count = 0; count <= 6; ++count) {
                for (unsigned int append = 0; append <= (pos == initial ? 1U : 0U); ++append) {
                    OverflowFixture v;
                    for (unsigned int i = 0; i < initial; ++i)
                        v.push_back(2.0f + i);
                    const float value = -3.5f;
                    v.Insert(pos, value, count, append != 0);
                    if (v.size() != initial + count ||
                        v.capacity() != initial + (initial < count ? count : initial)) return 1;
                    for (unsigned int j = 0; j < v.size(); ++j) {
                        const float expected = j < pos ? 2.0f + j :
                            j < pos + count ? value : 2.0f + j - count;
                        if (v[j] != expected) return 2;
                    }
                }
            }
        }
    }
    OverflowFixture v;
    v.push_back(1.25f);
    v.push_back(3.5f);
    v.Insert(0, v[1], 3, false);
    if (v.size() != 5 || v[0] != 3.5f || v[1] != 3.5f || v[2] != 3.5f || v[3] != 1.25f || v[4] != 3.5f) return 3;
    std::puts("004408a0_TEST PASS");
    return 0;
}
