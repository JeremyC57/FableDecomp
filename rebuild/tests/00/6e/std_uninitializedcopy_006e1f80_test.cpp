#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
struct CBase { virtual ~CBase() { } virtual int Kind() const { return 0; } };
template <class T1, class T2>
struct CParticleUpdateParamDesc : CBase {
    void* m_pName; void* m_pParam;
    CParticleUpdateParamDesc() : m_pName(0), m_pParam(0) { }
    virtual int Kind() const { return 2; }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > SParamDescEntry;
static char g_names[4];
static char g_params[4];
typedef SParamDescEntry TElem;
static void Fill(TElem& e, int i)
{
    e.first = 0x40u + i; e.second.m_pName = &g_names[i]; e.second.m_pParam = &g_params[i];
}
static bool Check(const TElem& e, int i)
{
    return e.first == 0x40u + i && e.second.m_pName == &g_names[i] && e.second.m_pParam == &g_params[i] && e.second.Kind() == 2;
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
    
    if (!ok) { std::printf("006e1f80_TEST FAIL\n"); return 1; }
    std::printf("006e1f80_TEST PASS\n");
    return 0;
}