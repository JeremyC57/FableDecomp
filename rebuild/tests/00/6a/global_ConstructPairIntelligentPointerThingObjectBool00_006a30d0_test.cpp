#include <cstdio>
#include <new>
static int g_ctor = 0;
struct CThing { int id; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
    virtual ~CBaseIntelligentPointer() { }
    virtual int Kind() const { return 1; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual int Kind() const { return 2; }
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingObjectBoolPair {
    CIntelligentPointer first;
    bool second;
};
void Construct_PairIntelligentPointerThingObjectBool_006a30d0(CThingObjectBoolPair* p, const CThingObjectBoolPair& value)
{
    if (p) new (p) CThingObjectBoolPair(value);   // VC null-tests the placement result itself
}
int main()
{
    CThing thing; thing.id = 3;
    CThingObjectBoolPair src;
    src.first.SetPItem(&thing);
    src.second = true;
    static double storage[8];
    CThingObjectBoolPair* dst = (CThingObjectBoolPair*)(void*)storage;
    int before = g_ctor;
    Construct_PairIntelligentPointerThingObjectBool_006a30d0(dst, src);
    if (g_ctor != before + 1 || dst->first.GetPItem() != &thing || dst->second != true || dst->first.Kind() != 2) { std::printf("006a30d0_TEST FAIL\n"); return 1; }
    Construct_PairIntelligentPointerThingObjectBool_006a30d0(0, src);
    if (g_ctor != before + 1) { std::printf("006a30d0_TEST FAIL\n"); return 1; }
    std::printf("006a30d0_TEST PASS\n");
    return 0;
}