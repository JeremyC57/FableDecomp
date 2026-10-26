#include <cstdio>
#include <new>
class CThing { public: int id; };
class CTCBase { public: int id; };
static int g_ctor = 0;
struct CBaseIntelligentPointer {
    virtual int Kind() const { return 1; }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
    virtual ~CBaseIntelligentPointer() { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual int Kind() const { return 2; }
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingTCPair {
    CIntelligentPointer first;
    CTCBase* second;
};
void Construct_PairIntelligentPointerThingTCBase_004cc3c0(CThingTCPair* p, const CThingTCPair& value)
{
    if (p) new (p) CThingTCPair(value);   // VC7.1 null-tests the placement result; g++ does not, so the test spells it
}
int main()
{
    CThing thing; thing.id = 7;
    CTCBase tc; tc.id = 9;
    CThingTCPair src;
    src.first.SetPItem(&thing);
    src.second = &tc;
    static unsigned char storage[sizeof(CThingTCPair) + 16];
    CThingTCPair* dst = (CThingTCPair*)(void*)storage;
    int before = g_ctor;
    Construct_PairIntelligentPointerThingTCBase_004cc3c0(dst, src);
    if (g_ctor != before + 1 || dst->first.GetPItem() != &thing || dst->second != &tc || dst->first.Kind() != 2) {
        std::printf("004cc3c0_TEST FAIL\n"); return 1;
    }
    Construct_PairIntelligentPointerThingTCBase_004cc3c0(0, src);
    if (g_ctor != before + 1) { std::printf("004cc3c0_TEST FAIL\n"); return 1; }
    std::printf("004cc3c0_TEST PASS\n");
    return 0;
}