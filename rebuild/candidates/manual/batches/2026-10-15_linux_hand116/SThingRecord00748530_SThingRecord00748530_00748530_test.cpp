#include <cstdio>
#include <cstring>
struct CThing { int id; };
static int g_sets, g_live;
struct CBaseIntelligentPointer {
    virtual long Kind() { return 0; }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; ++g_sets; }
    ~CBaseIntelligentPointer() { --g_live; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual long Kind() { return 1; }
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct SThingRef { CThing* m_pThing; long m_a; long m_b; };
struct SThingRecord {
    CIntelligentPointer m_thing; long m_a; long m_b;
    SThingRecord(const SThingRef& ref) : m_thing(ref.m_pThing), m_a(ref.m_a), m_b(ref.m_b) {}
};
int main()
{
    int ok = 1;
    CThing thing; SThingRef ref; ref.m_pThing = &thing; ref.m_a = 11; ref.m_b = -4;
    SThingRecord r(ref);
    if (r.m_thing.GetPItem() != &thing || r.m_a != 11 || r.m_b != -4 || r.m_thing.Kind() != 1) ok = 0;
    if (!ok) { std::printf("00748530_TEST FAIL\n"); return 1; }
    std::printf("00748530_TEST PASS\n");
    return 0;
}
