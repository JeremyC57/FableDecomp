#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_baseCtors = 0;
class CBaseObject { public: int m_id; };
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
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    virtual int Kind() const { return 1; }
};
struct CThingGraphicArgPasser { CIntelligentPointer<CThing> thing; bool bypassSet; CThingGraphicArgPasser() : bypassSet(false) { } };
static CBaseObject g_objects[4];
typedef CThingGraphicArgPasser TElem;
static void Fill(TElem& e, int i)
{
    e.thing.SetPItem(&g_objects[i]); e.bypassSet = i != 1;
}
static bool Check(const TElem& e, int i)
{
    return e.thing.GetPItem() == &g_objects[i] && e.thing.Kind() == 1 && e.bypassSet == (i != 1);
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
    if (!ok) { std::printf("006a3130_TEST FAIL\n"); return 1; }
    std::printf("006a3130_TEST PASS\n");
    return 0;
}