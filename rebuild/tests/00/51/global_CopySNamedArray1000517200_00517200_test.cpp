#include <cstdio>
#include <cstring>

static int g_assigns = 0;

class CCharString { public:                         // 4 bytes: the string-data pointer
    void* PStringData;
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
};
struct SArray0C_00454886 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_00454886& operator=(const SArray0C_00454886& other) { for (int i = 0; i < 3; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};
struct SNamedArray10 {                           // 16 bytes; implicit memberwise operator=
    CCharString m_name;                    // +0x00
    SArray0C_00454886 m_values;        // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SNamedArray10* Copy_SNamedArray10_00517200(const SNamedArray10* first, const SNamedArray10* last, SNamedArray10* result, const random_access_iterator_tag& category, int* distance)
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
    static SNamedArray10 src[3];
    static SNamedArray10 dst[4];
    for (int i = 0; i < 3; ++i) { src[i].m_name.PStringData = &src[i]; for (int w = 0; w < 3; ++w) src[i].m_values.m_words[w] = &src[i].m_values.m_words[w]; }
    random_access_iterator_tag tag;
    SNamedArray10* r = Copy_SNamedArray10_00517200(src, src + 3, dst, tag, 0);
    bool ok = (r == dst + 3) && (g_assigns == 3 * 2);
    for (int i = 0; i < 3; ++i) if (dst[i].m_name.PStringData != &src[i] || dst[i].m_values.m_words[2] != &src[i].m_values.m_words[2]) ok = false;
    if (dst[3].m_name.PStringData != 0) ok = false;
    if (Copy_SNamedArray10_00517200(src, src, dst + 1, tag, 0) != dst + 1 || g_assigns != 3 * 2) ok = false;   // empty range: result unchanged
    if (!ok) { std::printf("00517200_TEST FAIL\n"); return 1; }
    std::printf("00517200_TEST PASS\n");
    return 0;
}