#include <cstdio>
#include <cstring>

static int g_assigns = 0;

struct SArray0C_00515320 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_00515320& operator=(const SArray0C_00515320& other) { for (int i = 0; i < 3; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};
struct SArray0C_0051bcd0 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_0051bcd0& operator=(const SArray0C_0051bcd0& other) { for (int i = 0; i < 3; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};
struct SArrayPair18_0051cae0 {                   // 24 bytes; implicit memberwise operator=
    SArray0C_00515320 m_first;        // +0x00
    SArray0C_0051bcd0 m_second;        // +0x0c
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SArrayPair18_0051cae0* Copy_SArrayPair18_0051cae0(const SArrayPair18_0051cae0* first, const SArrayPair18_0051cae0* last, SArrayPair18_0051cae0* result, const random_access_iterator_tag& category, int* distance)
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
    static SArrayPair18_0051cae0 src[3];
    static SArrayPair18_0051cae0 dst[4];
    for (int i = 0; i < 3; ++i) { for (int w = 0; w < 3; ++w) src[i].m_first.m_words[w] = &src[i].m_first.m_words[w]; for (int w = 0; w < 3; ++w) src[i].m_second.m_words[w] = &src[i].m_second.m_words[w]; }
    random_access_iterator_tag tag;
    SArrayPair18_0051cae0* r = Copy_SArrayPair18_0051cae0(src, src + 3, dst, tag, 0);
    bool ok = (r == dst + 3) && (g_assigns == 3 * 2);
    for (int i = 0; i < 3; ++i) if (dst[i].m_first.m_words[2] != &src[i].m_first.m_words[2] || dst[i].m_second.m_words[2] != &src[i].m_second.m_words[2]) ok = false;
    if (dst[3].m_first.m_words[0] != 0) ok = false;
    if (Copy_SArrayPair18_0051cae0(src, src, dst + 1, tag, 0) != dst + 1 || g_assigns != 3 * 2) ok = false;   // empty range: result unchanged
    if (!ok) { std::printf("0051cae0_TEST FAIL\n"); return 1; }
    std::printf("0051cae0_TEST PASS\n");
    return 0;
}