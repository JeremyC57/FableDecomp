#include <cstdio>
#include <new>
static int g_ctor = 0;
class CThing { public: int id; };
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
struct CDangerStatus { long m_level; long m_time; };
struct CThingDangerPair {
    CIntelligentPointer first;
    CDangerStatus second;
};
void Construct_PairIntelligentPointerThingDangerStatus_00748a00(CThingDangerPair* p, const CThingDangerPair& value)
{
    if (p) new (p) CThingDangerPair(value);   // VC null-tests the placement result itself
}
int main()
{
    CThing thing; thing.id = 3;
    CThingDangerPair src;
    src.first.SetPItem(&thing);
    src.second.m_level = 4; src.second.m_time = 1234;
    static double storage[8];
    CThingDangerPair* dst = (CThingDangerPair*)(void*)storage;
    int before = g_ctor;
    Construct_PairIntelligentPointerThingDangerStatus_00748a00(dst, src);
    if (g_ctor != before + 1 || dst->first.GetPItem() != &thing || dst->second.m_level != 4 || dst->second.m_time != 1234) { std::printf("00748a00_TEST FAIL\n"); return 1; }
    Construct_PairIntelligentPointerThingDangerStatus_00748a00(0, src);
    if (g_ctor != before + 1) { std::printf("00748a00_TEST FAIL\n"); return 1; }
    std::printf("00748a00_TEST PASS\n");
    return 0;
}