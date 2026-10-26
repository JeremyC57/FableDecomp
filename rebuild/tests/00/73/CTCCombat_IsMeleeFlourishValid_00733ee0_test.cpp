// CTCCombat::IsMeleeFlourishValid() const -- a flourish is possible when the creature lacks interface 0x84, its interface-8 component has a live target that is vulnerable to the creature's melee strikes, an ability is selected and the usable-ability list is not empty
//
// Retail 0x00733EE0, 129 bytes (plain `ret`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0, the interface-8 component's virtual slot 33 (+0x84, returns the target CThing*),
// IsThingVulnerableToMeleeStrikesFromThing(CThing*, CThing*) @ 0x007F59F0 (`__fastcall`).
// Layout: retail CTCCombat with 12-byte STLport vectors (engine/CTCCombat.h is the PDB's 16-byte
// layout, so a local model is used): AbilityList +0x2c, UsableAbilityList {first +0x38, last +0x3c},
// CurrentAbilityIndex +0x44 (PDB +0x4c); ParentThing +0x04.
// Shape: one `if (... && ...) return true; return false;` chain (separate `mov al,1` exit, shared
// `xor al,al`); interface 0x84 is a byte test of bitset word 4 (`test byte [eax+30h],10h`), the
// interface-8 component comes from the out-parameter GetInterface with an uninitialised out pointer
// (whole-word bit test `mov ecx,[eax+20h] ; test ch,1`); the target passes the inline IsThingAlive.
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;                   // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                             // +0x91 (bit 0 = dead)
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(CThing* target, CThing* attacker);   // 0x007F59F0

struct CTCTargeting {                                       // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget();                            // slot 33 (+0x84)
};

class CTCCombat { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[0x2c - 0x08];
    void*         AbilityList[3];                           // +0x2c vector<CDefPointer<const CMeleeCombatAbilityDef>>
    void**        UsableAbilityList_first;                  // +0x38 vector<CDefPointer<const CMeleeCombatAbilityDef>>
    void**        UsableAbilityList_last;                   // +0x3c
    void**        UsableAbilityList_end;                    // +0x40
    long          CurrentAbilityIndex;                      // +0x44
    bool IsMeleeFlourishValid() const;
};

bool CTCCombat::IsMeleeFlourishValid() const
{
    if (!ParentThing->HasInterface(0x84))
    {
        CTCTargeting* targeting;
        ParentThing->GetInterface(&targeting, 8);
        CThing* target = targeting->GetTarget();
        if (IsThingAlive(target) && IsThingVulnerableToMeleeStrikesFromThing(target, ParentThing)
            && CurrentAbilityIndex >= 0 && UsableAbilityList_first != UsableAbilityList_last)
            return true;
    }
    return false;
}

// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
void CTCTargeting::v00() { }
void CTCTargeting::v01() { }
void CTCTargeting::v02() { }
void CTCTargeting::v03() { }
void CTCTargeting::v04() { }
void CTCTargeting::v05() { }
void CTCTargeting::v06() { }
void CTCTargeting::v07() { }
void CTCTargeting::v08() { }
void CTCTargeting::v09() { }
void CTCTargeting::v10() { }
void CTCTargeting::v11() { }
void CTCTargeting::v12() { }
void CTCTargeting::v13() { }
void CTCTargeting::v14() { }
void CTCTargeting::v15() { }
void CTCTargeting::v16() { }
void CTCTargeting::v17() { }
void CTCTargeting::v18() { }
void CTCTargeting::v19() { }
void CTCTargeting::v20() { }
void CTCTargeting::v21() { }
void CTCTargeting::v22() { }
void CTCTargeting::v23() { }
void CTCTargeting::v24() { }
void CTCTargeting::v25() { }
void CTCTargeting::v26() { }
void CTCTargeting::v27() { }
void CTCTargeting::v28() { }
void CTCTargeting::v29() { }
void CTCTargeting::v30() { }
void CTCTargeting::v31() { }
void CTCTargeting::v32() { }
static CThing* g_target = 0;
static bool g_vulnerable = true;
static int g_vulnChecks = 0;
static CThing* g_lastAttacker = 0;
CThing* CTCTargeting::GetTarget() { return g_target; }
bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(CThing* target, CThing* attacker)
{
    ++g_vulnChecks; g_lastAttacker = attacker;
    CHECK(target == g_target);
    return g_vulnerable;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
void CTCCombat::v00() { }
static CThing g_self, g_enemy;
static CTCTargeting g_targeting;
static CInterfaceMap::SEntry g_entries[2];
static void* g_abilities[2];
int main()
{
    g_entries[0].type = 8; g_entries[0].component = (CTCBase*)&g_targeting;
    g_entries[1].type = 0x84; g_entries[1].component = 0;
    g_self.m_interfaces.first = g_entries; g_self.m_interfaces.last = g_entries + 2;
    g_self.m_presentBits[0] = 1UL << 8;
    CTCCombat combat;
    memset(&combat, 0, sizeof combat);
    combat.ParentThing = &g_self;
    combat.CurrentAbilityIndex = 0;
    combat.UsableAbilityList_first = g_abilities; combat.UsableAbilityList_last = g_abilities + 1;
    CHECK(!combat.IsMeleeFlourishValid() && g_vulnChecks == 0);      // no target
    g_target = &g_enemy;
    CHECK(combat.IsMeleeFlourishValid() && g_vulnChecks == 1 && g_lastAttacker == &g_self);
    g_enemy.m_stateFlags = 1;                                         // dead target
    CHECK(!combat.IsMeleeFlourishValid() && g_vulnChecks == 1);
    g_enemy.m_stateFlags = 0; g_vulnerable = false;                   // not vulnerable
    CHECK(!combat.IsMeleeFlourishValid() && g_vulnChecks == 2);
    g_vulnerable = true; combat.CurrentAbilityIndex = -1;             // no current ability
    CHECK(!combat.IsMeleeFlourishValid());
    combat.CurrentAbilityIndex = 2; combat.UsableAbilityList_last = g_abilities;   // no usable abilities
    CHECK(!combat.IsMeleeFlourishValid());
    combat.UsableAbilityList_last = g_abilities + 2;
    CHECK(combat.IsMeleeFlourishValid());
    g_self.m_presentBits[0x84 >> 5] |= 1UL << (0x84 & 31);            // interface 0x84 present: never
    int checks = g_vulnChecks;
    CHECK(!combat.IsMeleeFlourishValid() && g_vulnChecks == checks);
    if (fails == 0) printf("00733ee0_TEST PASS\n");
    return fails;
}