#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
struct SValue04 { unsigned long m_value; SValue04() : m_value(0) { } };
typedef SValue04 TElem;
static void Fill(TElem& e, int i)
{
    e.m_value = 0x2000u * i + 3;
}
static bool Check(const TElem& e, int i)
{
    return e.m_value == 0x2000u * i + 3;
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
    
    if (!ok) { std::printf("005f79bd_TEST FAIL\n"); return 1; }
    std::printf("005f79bd_TEST PASS\n");
    return 0;
}