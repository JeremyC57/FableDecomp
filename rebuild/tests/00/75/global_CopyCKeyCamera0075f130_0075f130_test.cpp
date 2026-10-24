#include <cstdio>
#include <cstring>

static int g_assigns = 0;

struct CKeyCamera {                              // 56 bytes (0x38); its own assignment copies the body
    unsigned char _body[0x38];
    CKeyCamera& operator=(const CKeyCamera& other) { std::memcpy(_body, other._body, sizeof(_body)); ++g_assigns; return *this; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

static CKeyCamera* Copy_CKeyCamera_0075f130(const CKeyCamera* first, const CKeyCamera* last, CKeyCamera* result, const random_access_iterator_tag& category, int* distance)
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
    static CKeyCamera src[3];
    static CKeyCamera dst[4];
    for (int i = 0; i < 3; ++i) { for (unsigned j = 0; j < sizeof(src[i]._body); ++j) src[i]._body[j] = (unsigned char)(i * 37 + j + 1); }
    random_access_iterator_tag tag;
    CKeyCamera* r = Copy_CKeyCamera_0075f130(src, src + 3, dst, tag, 0);
    bool ok = (r == dst + 3) && (g_assigns == 3 * 1);
    for (int i = 0; i < 3; ++i) if (std::memcmp(dst[i]._body, src[i]._body, sizeof(src[i]._body)) != 0) ok = false;
    if (dst[3]._body[0] != 0) ok = false;
    if (Copy_CKeyCamera_0075f130(src, src, dst + 1, tag, 0) != dst + 1 || g_assigns != 3 * 1) ok = false;   // empty range: result unchanged
    if (!ok) { std::printf("0075f130_TEST FAIL\n"); return 1; }
    std::printf("0075f130_TEST PASS\n");
    return 0;
}