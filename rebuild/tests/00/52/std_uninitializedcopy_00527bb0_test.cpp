#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_stringCopies = 0;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringCopies; }
};
struct SNamedRecord0C { long m_word0; long m_word4; CCharString m_name; SNamedRecord0C() : m_word0(0), m_word4(0) { } };
static const char* g_texts[4] = { "zero", "one", "two", "three" };
typedef SNamedRecord0C TElem;
static void Fill(TElem& e, int i)
{
    e.m_word0 = i; e.m_word4 = 2 * i + 1; e.m_name.m_pData = g_texts[i];
}
static bool Check(const TElem& e, int i)
{
    return e.m_word0 == i && e.m_word4 == 2 * i + 1 && e.m_name.m_pData == g_texts[i];
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
    if (g_stringCopies != 3) ok = false;
    if (!ok) { std::printf("00527bb0_TEST FAIL\n"); return 1; }
    std::printf("00527bb0_TEST PASS\n");
    return 0;
}