// Behaviour test for 0x0072BEC0 CTCSpecialAbilities::ResumeAbilitiesForCutscene (copy of the source logic over spy components).
#include <cstdio>
#include <cstring>
class CTCBase { public: virtual ~CTCBase() {} };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CTCPhysicalShieldSpell : public CTCBase { public: int resumed; CTCPhysicalShieldSpell() : resumed(0) {} void ResumeForCutscene() { ++resumed; } };
class CTCSummonSpell : public CTCBase { public: int resumed; CTCSummonSpell() : resumed(0) {} void ResumeForCutscene() { ++resumed; } };
class CTCGhostSword : public CTCBase { public: int resumed; CTCGhostSword() : resumed(0) {} void ResumeForCutscene() { ++resumed; } };
class CTCMultiArrow : public CTCBase { public: int resumed; CTCMultiArrow() : resumed(0) {} void ResumeForCutscene() { ++resumed; } };
class CTCSpecialAbilities { public:
    CThing* ParentThing;
    void ResumeAbilitiesForCutscene();
};

void CTCSpecialAbilities::ResumeAbilitiesForCutscene()
{
    if (ParentThing->HasTC(0x93))
        ParentThing->GetTC<CTCPhysicalShieldSpell>(0x93)->ResumeForCutscene();
    if (ParentThing->HasTC(0x91))
        ParentThing->GetTC<CTCSummonSpell>(0x91)->ResumeForCutscene();
    if (ParentThing->HasTC(0x9a))
        ParentThing->GetTC<CTCGhostSword>(0x9a)->ResumeForCutscene();
    if (ParentThing->HasTC(0x88))
        ParentThing->GetTC<CTCMultiArrow>(0x88)->ResumeForCutscene();
}

int main()
{
    CTCPhysicalShieldSpell shield; CTCSummonSpell summon; CTCGhostSword sword; CTCMultiArrow arrow;
    CInterfaceMap::SEntry e[4] = { { 0x88, &arrow }, { 0x91, &summon }, { 0x93, &shield }, { 0x9a, &sword } };
    CThing hero; std::memset(hero.m_presentBits, 0, sizeof hero.m_presentBits);
    hero.m_interfaces.first = e; hero.m_interfaces.last = e + 4;
    hero.m_presentBits[0x93 >> 5] |= 1UL << (0x93 & 31);
    hero.m_presentBits[0x88 >> 5] |= 1UL << (0x88 & 31);
    CTCSpecialAbilities sa; sa.ParentThing = &hero;
    sa.ResumeAbilitiesForCutscene();
    if (shield.resumed != 1 || arrow.resumed != 1 || summon.resumed != 0 || sword.resumed != 0) { std::printf("0072bec0_TEST FAIL\n"); return 1; }
    hero.m_presentBits[0x91 >> 5] |= 1UL << (0x91 & 31);
    hero.m_presentBits[0x9a >> 5] |= 1UL << (0x9a & 31);
    sa.ResumeAbilitiesForCutscene();
    if (shield.resumed != 2 || arrow.resumed != 2 || summon.resumed != 1 || sword.resumed != 1) { std::printf("0072bec0_TEST FAIL\n"); return 1; }
    std::printf("0072bec0_TEST PASS\n");
    return 0;
}