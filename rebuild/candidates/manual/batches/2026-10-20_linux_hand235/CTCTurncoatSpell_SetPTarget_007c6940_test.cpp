// behaviour test for 007c6940 CTCTurncoatSpell::SetPTarget: stores the target; a live, turncoat-able target gets the def's
// TurncoatTime entry for the caster's turncoat ability level (ability 5); nothing when the caster has no abilities TC
#include <cstdio>
class CThing;
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
    const SEntry* LowerBound(const long* type) const { const SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags;
    bool IsDead() const { return (m_flags & 1) != 0; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CThingCreatureBase;
static int g_live = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    void* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_live; }
    ~CBaseIntelligentPointer() { --g_live; }
    CThing* GetPItem() const { return (CThing*)PData; }
    void SetPItem(CThing* item) { PData = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CTCTurncoat { float timeLast; void SetTurncoatTimeLast(float time) { timeLast = time; } };
struct CTCInventoryAbilities { long level; long GetAbilityLevel(long ability) const { return ability == 5 ? level : -1; } };
struct CFloatVector {
    float* _M_start;
    float* _M_finish;
    float* _M_end_of_storage;
    const float& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CSpecialAbilitiesTurncoatSpellDef { CFloatVector TurncoatTime; };
struct CTCTurncoatSpell {
    void*                     __vftable;
    CThing*                   ParentThing;
    CSpecialAbilitiesTurncoatSpellDef* PDef;
    CIntelligentPointer<CThingCreatureBase> PTarget;
    CTCTurncoatSpell() : PTarget(0) {}
    void SetPTarget(CThingCreatureBase* target);
};
void CTCTurncoatSpell::SetPTarget(CThingCreatureBase* target)
{
    PTarget = CIntelligentPointer<CThingCreatureBase>(target);
    if (IS_THING_ALIVE(PTarget)) {
        CThing* targetThing = PTarget.GetPItem();
        if (targetThing->HasTC(0x9f)) {
            CTCTurncoat* turncoat = (CTCTurncoat*)targetThing->GetTC(0x9f);
            const CThing* caster = ParentThing;
            if (caster->HasTC(0x6f)) {
                const CTCInventoryAbilities* abilities = (const CTCInventoryAbilities*)caster->GetTC(0x6f);
                CSpecialAbilitiesTurncoatSpellDef* def = PDef;
                turncoat->SetTurncoatTimeLast(def->TurncoatTime[abilities->GetAbilityLevel(5)]);
            }
        }
    }
}
static void Clear(CThing& t) { for (int i = 0; i < 9; ++i) t.m_presentBits[i] = 0; t.m_flags = 0; }
int main()
{
    CTCTurncoat turncoat; turncoat.timeLast = -1.0f;
    CTCInventoryAbilities abilities; abilities.level = 2;
    CInterfaceMap::SEntry targetEntries[1] = { { 0x9f, (CTCBase*)&turncoat } };
    CInterfaceMap::SEntry casterEntries[1] = { { 0x6f, (CTCBase*)&abilities } };
    CThing target; Clear(target); target.m_presentBits[4] = 0x80000000UL; target.m_interfaces.first = targetEntries; target.m_interfaces.last = targetEntries + 1;
    CThing caster; Clear(caster); caster.m_presentBits[3] = 1UL << 15; caster.m_interfaces.first = casterEntries; caster.m_interfaces.last = casterEntries + 1;
    float times[3] = { 1.0f, 2.0f, 4.5f };
    CSpecialAbilitiesTurncoatSpellDef def; def.TurncoatTime._M_start = times; def.TurncoatTime._M_finish = times + 3;
    CTCTurncoatSpell spell; spell.ParentThing = &caster; spell.PDef = &def;
    spell.SetPTarget((CThingCreatureBase*)&target);
    bool ok = spell.PTarget.GetPItem() == &target && turncoat.timeLast == 4.5f && g_live == 1;
    turncoat.timeLast = -1.0f; target.m_flags = 1;   // dead target: stored, no time set
    spell.SetPTarget((CThingCreatureBase*)&target);
    ok = ok && turncoat.timeLast == -1.0f;
    target.m_flags = 0; caster.m_presentBits[3] = 0;
    spell.SetPTarget((CThingCreatureBase*)&target);
    ok = ok && turncoat.timeLast == -1.0f && g_live == 1;
    if (!ok) { std::printf("007c6940_TEST FAIL\n"); return 1; }
    std::printf("007c6940_TEST PASS\n");
    return 0;
}
