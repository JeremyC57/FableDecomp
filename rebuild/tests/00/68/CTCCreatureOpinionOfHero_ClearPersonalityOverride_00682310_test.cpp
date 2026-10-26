// Behaviour test for CTCCreatureOpinionOfHero::ClearPersonalityOverride (0x00682310): married wife -> WIFE_NORMAL, divorced
// wife -> WIFE_DIVORCED (both forced), anyone else -> the def's default personality index (forced).
#include <cstdio>
#include <cstring>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const { const SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long length) : PStringData(text) { }
    ~CCharString() { }
};
class CTCWife { public:
    bool DivorcedHero; bool Married;
    bool IsMarriedToThePlayer() { return Married; }
};
class COpinionOfHeroDef { public: long DefaultPersonality; };
class CTCCreatureOpinionOfHero { public:
    virtual void v00() { }
    const CThing* ParentThing;
    COpinionOfHeroDef* PDef;
    const char* LastName; long LastIndex; bool LastForce; int Calls;
    void SetPersonalityOverride(long personality, bool force) { LastName = 0; LastIndex = personality; LastForce = force; ++Calls; }
    void SetPersonalityOverride(const CCharString& personality, bool force) { LastName = personality.PStringData; LastIndex = -1; LastForce = force; ++Calls; }
    void ClearPersonalityOverride();
};

void CTCCreatureOpinionOfHero::ClearPersonalityOverride()
{
    if (ParentThing->HasTC(0xd7)) {
        CTCWife* wife = (CTCWife*)ParentThing->GetTC(0xd7);
        if (wife->IsMarriedToThePlayer()) {
            CCharString name("OPINION_PERSONALITY_WIFE_NORMAL", -1);
            SetPersonalityOverride(name, true);
            return;
        }
        if (wife->DivorcedHero) {
            CCharString name("OPINION_PERSONALITY_WIFE_DIVORCED", -1);
            SetPersonalityOverride(name, true);
            return;
        }
    }
    SetPersonalityOverride(PDef->DefaultPersonality, true);
}

int main()
{
    int bad = 0;
    CTCWife wife; wife.Married = true; wife.DivorcedHero = false;
    CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    CInterfaceMap::SEntry e[1] = { { 0xd7, (CTCBase*)&wife } };
    thing.m_interfaces.first = e; thing.m_interfaces.last = e + 1; thing.m_presentBits[6] = 1UL << 23;
    COpinionOfHeroDef def; def.DefaultPersonality = 42;
    CTCCreatureOpinionOfHero o; o.ParentThing = &thing; o.PDef = &def; o.Calls = 0;
    o.ClearPersonalityOverride();
    if (o.Calls != 1 || !o.LastName || std::strcmp(o.LastName, "OPINION_PERSONALITY_WIFE_NORMAL") || !o.LastForce) bad |= 1;
    wife.Married = false; wife.DivorcedHero = true;
    o.ClearPersonalityOverride();
    if (o.Calls != 2 || !o.LastName || std::strcmp(o.LastName, "OPINION_PERSONALITY_WIFE_DIVORCED")) bad |= 2;
    wife.DivorcedHero = false;
    o.ClearPersonalityOverride();                    // a wife that is neither: the default
    if (o.Calls != 3 || o.LastName || o.LastIndex != 42 || !o.LastForce) bad |= 4;
    thing.m_presentBits[6] = 0; wife.Married = true;
    o.ClearPersonalityOverride();                    // not a wife
    if (o.Calls != 4 || o.LastName || o.LastIndex != 42) bad |= 8;
    if (bad) { std::printf("00682310_TEST FAIL\n"); return 1; }
    std::printf("00682310_TEST PASS\n");
    return 0;
}