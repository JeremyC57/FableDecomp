#include <cstdio>
#include <cstring>

static int g_assigns = 0;

struct CCharString {                         // 4 bytes: the string-data pointer
    void* PStringData;
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; ++g_assigns; return *this; }
};
struct SStringPairWord0C {                       // 12 bytes; implicit memberwise operator=
    CCharString m_first;                    // +0x00
    CCharString m_second;                    // +0x04
    long m_value;                           // +0x08
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SStringPairWord0C* Copy_SStringPairWord0C_00a40040(const SStringPairWord0C* first, const SStringPairWord0C* last, SStringPairWord0C* result, const random_access_iterator_tag& category, int* distance)
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
    static SStringPairWord0C src[3];
    static SStringPairWord0C dst[4];
    for (int i = 0; i < 3; ++i) { src[i].m_first.PStringData = &src[i]; src[i].m_second.PStringData = &src[i]; src[i].m_value = 100 + i; }
    random_access_iterator_tag tag;
    SStringPairWord0C* r = Copy_SStringPairWord0C_00a40040(src, src + 3, dst, tag, 0);
    bool ok = (r == dst + 3) && (g_assigns == 3 * 2);
    for (int i = 0; i < 3; ++i) if (dst[i].m_first.PStringData != &src[i] || dst[i].m_second.PStringData != &src[i] || dst[i].m_value != 100 + i) ok = false;
    if (dst[3].m_first.PStringData != 0) ok = false;
    if (Copy_SStringPairWord0C_00a40040(src, src, dst + 1, tag, 0) != dst + 1 || g_assigns != 3 * 2) ok = false;   // empty range: result unchanged
    if (!ok) { std::printf("00a40040_TEST FAIL\n"); return 1; }
    std::printf("00a40040_TEST PASS\n");
    return 0;
}