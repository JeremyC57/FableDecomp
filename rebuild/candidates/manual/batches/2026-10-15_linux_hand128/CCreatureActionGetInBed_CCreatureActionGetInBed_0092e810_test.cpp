// Behaviour test for 0x0092E810 CCreatureAction_GetInBed copy constructor: base copied, bed re-pointed, both strings
// copy-constructed and both animation ids copied.
#include <cstdio>
#include <cstring>

struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem; int m_sets;
    CBaseIntelligentPointer() : m_pItem(0), m_sets(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* p) { m_pItem = p; ++m_sets; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CCreatureActionBase {
    int m_baseValue; int m_copies;
    CCreatureActionBase() : m_baseValue(0), m_copies(0) { }
    CCreatureActionBase(const CCreatureActionBase& o) : m_baseValue(o.m_baseValue), m_copies(o.m_copies + 1) { }
};
struct CThing; struct CThingAICreature; struct CThingBuilding;
struct CCharString {
    char m_text[16];
    CCharString() { m_text[0] = 0; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); }
};
struct CCreatureAction_GetInBed : CCreatureActionBase {
    CIntelligentPointer<CThing> PBed; CCharString AnimGroup, SleepAnimName; long BedGetInAnim, BedSleepAnim;
    CCreatureAction_GetInBed() : BedGetInAnim(0), BedSleepAnim(0) { }
    CCreatureAction_GetInBed(const CCreatureAction_GetInBed& o)
        : CCreatureActionBase(o), PBed(o.PBed), AnimGroup(o.AnimGroup), SleepAnimName(o.SleepAnimName),
          BedGetInAnim(o.BedGetInAnim), BedSleepAnim(o.BedSleepAnim) { }
};
int main()
{
    CBaseObject bed = { 1 };
    CCreatureAction_GetInBed src; src.PBed.SetPItem(&bed); std::strcpy(src.AnimGroup.m_text, "BED");
    std::strcpy(src.SleepAnimName.m_text, "SLEEP"); src.BedGetInAnim = 11; src.BedSleepAnim = 12;
    CCreatureAction_GetInBed c(src);
    if (c.m_copies != 1 || c.PBed.m_pItem != &bed || std::strcmp(c.AnimGroup.m_text, "BED") != 0 ||
        std::strcmp(c.SleepAnimName.m_text, "SLEEP") != 0 || c.BedGetInAnim != 11 || c.BedSleepAnim != 12) {
        std::printf("0092e810_TEST FAIL\n"); return 1;
    }
    std::printf("0092e810_TEST PASS\n");
    return 0;
}
