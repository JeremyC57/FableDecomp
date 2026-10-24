// Behaviour test for 0x008002F0 UninitCopy_PairC2DVectorByte_008002f0: placement copy of 9-byte pair<C2DVector, unsigned char>.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
#pragma pack(push, 1)
struct C2DVector { float X, Y; };
#pragma pack(pop)
struct SPair {
    C2DVector     first;
    unsigned char second;
    SPair() { first.X = 0.0f; first.Y = 0.0f; second = 0; }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};

static SPair* UninitCopy(const SPair* first, const SPair* last, SPair* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SPair(*first);
    return result;
}

int main()
{
    SPair src[3];
    for (int i = 0; i < 3; ++i) { src[i].first.X = 1.5f * i; src[i].first.Y = -2.0f * i; src[i].second = (unsigned char)(200 + i); }
    SPair dst[3];
    __false_type tag;
    SPair* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && sizeof(SPair) == 9;
    for (int k = 0; k < 3; ++k)
        if (dst[k].first.X != 1.5f * k || dst[k].first.Y != -2.0f * k || dst[k].second != (unsigned char)(200 + k)) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("008002f0_TEST FAIL\n"); return 1; }
    std::printf("008002f0_TEST PASS\n");
    return 0;
}