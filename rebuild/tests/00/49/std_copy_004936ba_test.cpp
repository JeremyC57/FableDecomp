// Behaviour test for 0x004936BA std::__copy<SRecord16> (model copy).
#include <cstdio>

struct SRecord16 { unsigned char _body[16]; };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SRecord16* copy_model(const SRecord16* first, const SRecord16* last, SRecord16* result,
                             const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    SRecord16 src[3], dst[4];
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 16; ++j) src[i]._body[j] = (unsigned char)(i * 16 + j);
    for (int k = 0; k < 4; ++k) for (int j = 0; j < 16; ++j) dst[k]._body[j] = 0xEE;
    SRecord16* end = copy_model(src, src + 3, dst, random_access_iterator_tag(), 0);
    int ok = end == dst + 3 && dst[0]._body[0] == 0 && dst[2]._body[15] == 47 && dst[3]._body[0] == 0xEE;
    SRecord16* same = copy_model(src, src, dst + 1, random_access_iterator_tag(), 0);
    if (same != dst + 1) ok = 0;
    if (!ok) { std::printf("004936ba_TEST FAIL\n"); return 1; }
    std::printf("004936ba_TEST PASS\n");
    return 0;
}