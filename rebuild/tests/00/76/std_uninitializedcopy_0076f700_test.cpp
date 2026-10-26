#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<CCharString, CCharString> SStringPair;
static const char* g_texts[4] = { "k0", "k1", "k2", "k3" };
static const char* g_other[4] = { "v0", "v1", "v2", "v3" };
typedef SStringPair TElem;
static void Fill(TElem& e, int i)
{
    e.first.m_pData = g_texts[i]; e.second.m_pData = g_other[i];
}
static bool Check(const TElem& e, int i)
{
    return e.first.m_pData == g_texts[i] && e.second.m_pData == g_other[i];
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    if (g_stringCopies != 6) ok = false;
    if (!ok) { std::printf("0076f700_TEST FAIL\n"); return 1; }
    std::printf("0076f700_TEST PASS\n");
    return 0;
}