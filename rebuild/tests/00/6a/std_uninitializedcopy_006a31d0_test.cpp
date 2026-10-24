#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_baseCtors = 0;
struct CBaseObject { int m_id; };
class CThing;
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_baseCtors; }
    virtual ~CBaseIntelligentPointer() { }
    virtual int Kind() const { return 0; }
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    virtual int Kind() const { return 1; }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, CIntelligentPointer<CThing> > SThingEntry;
static CBaseObject g_objects[4];
typedef SThingEntry TElem;
static void Fill(TElem& e, int i)
{
    e.first = 50 + i; e.second.SetPItem(&g_objects[i]);
}
static bool Check(const TElem& e, int i)
{
    return e.first == 50 + i && e.second.GetPItem() == &g_objects[i] && e.second.Kind() == 1;
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
    if (g_baseCtors != 6) ok = false;
    if (!ok) { std::printf("006a31d0_TEST FAIL\n"); return 1; }
    std::printf("006a31d0_TEST PASS\n");
    return 0;
}