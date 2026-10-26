// behaviour test for 0x0074CCC0 std::__uninitialized_copy over CIntelligentPointer<CThingCreatureBase >: every destination is
// copy-constructed (base ctor, then SetPItem(source.GetPItem())), the result is the end of the constructed range,
// an empty range constructs nothing
#include <cstdio>
#include <new>
static int g_ctors = 0;
struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    CBaseObject* item;
    CBaseIntelligentPointer() : item(0) { ++g_ctors; }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return item; }
    void SetPItem(CBaseObject* p) { item = p; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
typedef CIntelligentPointer* InIt;
static CIntelligentPointer* ucopy(InIt first, InIt last, CIntelligentPointer* result)
{
    CIntelligentPointer* cur = result;
    for (; first != last; ++first, ++cur) new (cur) CIntelligentPointer(*first);
    return cur;
}
int main()
{
    int fails = 0;
    CBaseObject objs[3] = { { 1 }, { 2 }, { 3 } };
    CIntelligentPointer src[3];
    for (int i = 0; i < 3; ++i) src[i].SetPItem(&objs[i]);
    g_ctors = 0;
    static double raw[3 * sizeof(CIntelligentPointer) / sizeof(double) + 1];
    CIntelligentPointer* dst = (CIntelligentPointer*)raw;
    CIntelligentPointer* end = ucopy(src, src + 3, dst);
    if (end != dst + 3 || g_ctors != 3) fails |= 1;
    for (int i = 0; i < 3; ++i) if (dst[i].GetPItem() != &objs[i]) fails |= 2;
    if (ucopy(src + 1, src + 1, dst) != dst || g_ctors != 3) fails |= 4;
    for (int i = 0; i < 3; ++i) dst[i].~CIntelligentPointer();
    if (fails) { std::printf("0074ccc0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0074ccc0_TEST PASS\n");
    return 0;
}