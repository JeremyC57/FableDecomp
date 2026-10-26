// behaviour test for 0x0074CD10 std::__uninitialized_fill_n over CIntelligentPointer<CThingCreatureBase >: n destinations are
// copy-constructed from x (base ctor, then SetPItem(x.GetPItem())), the result is first + n, n == 0 constructs nothing
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
static CIntelligentPointer* ufill(CIntelligentPointer* first, unsigned int n, const CIntelligentPointer& x)
{
    CIntelligentPointer* cur = first;
    for (; n > 0; --n, ++cur) new (cur) CIntelligentPointer(x);
    return cur;
}
int main()
{
    int fails = 0;
    CBaseObject obj = { 7 };
    CIntelligentPointer x; x.SetPItem(&obj);
    g_ctors = 0;
    static double raw[4 * sizeof(CIntelligentPointer) / sizeof(double) + 1];
    CIntelligentPointer* dst = (CIntelligentPointer*)raw;
    CIntelligentPointer* end = ufill(dst, 4, x);
    if (end != dst + 4 || g_ctors != 4) fails |= 1;
    for (int i = 0; i < 4; ++i) if (dst[i].GetPItem() != &obj) fails |= 2;
    if (ufill(dst, 0, x) != dst || g_ctors != 4) fails |= 4;
    for (int i = 0; i < 4; ++i) dst[i].~CIntelligentPointer();
    if (fails) { std::printf("0074cd10_TEST FAIL %d\n", fails); return 1; }
    std::printf("0074cd10_TEST PASS\n");
    return 0;
}
