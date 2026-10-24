#include <cstdio>
#include <cstring>

static int g_assigns = 0;

struct SArray0C_0045bc09 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_0045bc09& operator=(const SArray0C_0045bc09& other) { for (int i = 0; i < 3; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};
struct CRetailTree_0045c811 {                    // 12 bytes, assigned out of line
    void* m_words[3];
    CRetailTree_0045c811& operator=(const CRetailTree_0045c811& other) { for (int i = 0; i < 3; ++i) m_words[i] = other.m_words[i]; ++g_assigns; return *this; }
};
struct SArrayTreePair18_00adc120 {               // 24 bytes; implicit memberwise operator=
    SArray0C_0045bc09 m_values;        // +0x00
    CRetailTree_0045c811 m_tree;        // +0x0c
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

static SArrayTreePair18_00adc120* Copy_SArrayTreePair18_00adc120(const SArrayTreePair18_00adc120* first, const SArrayTreePair18_00adc120* last, SArrayTreePair18_00adc120* result, const random_access_iterator_tag& category, int* distance)
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
    static SArrayTreePair18_00adc120 src[3];
    static SArrayTreePair18_00adc120 dst[4];
    for (int i = 0; i < 3; ++i) { for (int w = 0; w < 3; ++w) src[i].m_values.m_words[w] = &src[i].m_values.m_words[w]; for (int w = 0; w < 3; ++w) src[i].m_tree.m_words[w] = &src[i].m_tree.m_words[w]; }
    random_access_iterator_tag tag;
    SArrayTreePair18_00adc120* r = Copy_SArrayTreePair18_00adc120(src, src + 3, dst, tag, 0);
    bool ok = (r == dst + 3) && (g_assigns == 3 * 2);
    for (int i = 0; i < 3; ++i) if (dst[i].m_values.m_words[2] != &src[i].m_values.m_words[2] || dst[i].m_tree.m_words[2] != &src[i].m_tree.m_words[2]) ok = false;
    if (dst[3].m_values.m_words[0] != 0) ok = false;
    if (Copy_SArrayTreePair18_00adc120(src, src, dst + 1, tag, 0) != dst + 1 || g_assigns != 3 * 2) ok = false;   // empty range: result unchanged
    if (!ok) { std::printf("00adc120_TEST FAIL\n"); return 1; }
    std::printf("00adc120_TEST PASS\n");
    return 0;
}