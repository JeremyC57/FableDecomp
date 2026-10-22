// Behaviour test for 0x006A62E0: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
// CThingPhysical::InternalModifyHealth(float delta, bool allowDeath) -- apply a health delta: invulnerability cheats, boss phase floor, def minimum, death floor, observer notifications
// Leaf: InternalModifyHealth
// Retail: ?InternalModifyHealth@CThingPhysical@@AAEXM_N@Z (private). Callees: CTCBoss::GetCurrentPhase 0x7AED10, GetMinHealthForPhase 0x7AEE40,
//   TC4 0x57A753 (catalog label AddSuccessfulBlockParry is wrong: hero health observer), TC9 0x7EA270 (label AddExclusiveObserver is wrong: flag setter),
//   SetAsFrameUpdated 0x4C7C40; LowerBound non-const 0x40F020 (TC 4/9), const 0x4365B0 (TC 0xad).
// Layout: retail CThing is 0x10 smaller and CThingPhysical a further 0x8 smaller than the PDB view: MaxHealth/Health/flag bits sit at
//   +0xb0/+0xb4/+0xbc (fld [esi+0xb4], test byte [esi+0xbc]) where the PDB has +0xc8/+0xcc/+0xd4; the shared header's +0xb4 "Scale" does not fit retail.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
    SEntry* LowerBound(const long& type) const;             // 0x004365B0
};
struct CTCBoss {                                            // TC 0xad
    unsigned char _pad00[0x0c];
    void* PPhases;                                          // +0x0c
    unsigned char _pad10[0x11];
    bool Active;                                            // +0x21
    long GetCurrentPhase() const;                           // 0x007AED10
    float GetMinHealthForPhase(long phase) const;           // 0x007AEE40
};
struct CTCHeroHealthObserver {                              // TC 4
    void OnHealthChanged();                                 // 0x0057A753
};
struct CTCObservable {                                      // TC 9
    unsigned char _pad00[0x0c];
    unsigned long Flags;                                    // +0x0c
    void SetFlags(unsigned long flags);                     // 0x007EA270
};
struct CThingPhysicalDef {
    unsigned char _pad00[0x54];
    float MinimumHealth;                                    // +0x54
};
extern bool g_bPlayerInvulnerable;                          // 0x013B86C7
extern bool g_bCreaturesInvulnerable;                       // 0x013B86C8
struct CThingPhysical {
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                               // +0x10 (1 creature, 2 player)
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad50[0x70 - 0x50];
    const CThingPhysicalDef* PBaseDef;                      // +0x70
    unsigned char _pad74[0xb0 - 0x74];
    float MaxHealth;                                        // +0xb0
    float Health;                                           // +0xb4
    unsigned char _padb8[4];
    unsigned char Damageable : 1;                           // +0xbc
    unsigned char Killable : 1;
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void SetAsFrameUpdated();                               // 0x004C7C40
    void InternalModifyHealth(float delta, bool allowDeath);
};

void CThingPhysical::InternalModifyHealth(float delta, bool allowDeath)
{
    if (delta < 0.0f) {
        if (ThingClass == 2 && g_bPlayerInvulnerable)
            return;
        if (ThingClass == 1 && g_bCreaturesInvulnerable)
            return;
        if (!Damageable)
            return;
    }
    Health += delta;
    if (HasTC(0xad)) {
        const CTCBoss* boss = (const CTCBoss*)static_cast<const CThingPhysical*>(this)->GetTC(0xad);
        if (boss->Active && boss->PPhases) {
            float minimum = boss->GetMinHealthForPhase(boss->GetCurrentPhase());
            if (minimum > Health)
                Health = minimum - 1.0f;
        }
    }
    if (Health < PBaseDef->MinimumHealth)
        Health = PBaseDef->MinimumHealth;
    float floor = (allowDeath && Killable && !HasTC(0x84)) ? 0.0f : 1.0f;
    if (floor > Health)
        Health = floor;
    else if (Health > MaxHealth)
        Health = MaxHealth;
    if (ThingClass == 2) {
        if (HasTC(4))
            ((CTCHeroHealthObserver*)GetTC(4))->OnHealthChanged();
    } else if (ThingClass == 1) {
        if (Health <= 0.0001f && HasTC(9)) {
            CTCObservable* observable = (CTCObservable*)GetTC(9);
            if (observable->Flags & 2)
                observable->SetFlags(observable->Flags & ~2);
        }
    }
    SetAsFrameUpdated();
}

bool g_bPlayerInvulnerable = false;
bool g_bCreaturesInvulnerable = false;
struct CTCBase { int dummy; };
static CInterfaceMap::SEntry* Lower(CInterfaceMap::SEntry* b, CInterfaceMap::SEntry* e, long type)
{
    while (b != e && b->type < type) ++b;
    return b;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type) { return Lower(first, last, type); }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long& type) const { return Lower(first, last, type); }
static long g_phase = 0;
static float g_phaseMin = 0.0f;
static long g_phaseAsked = -1;
long CTCBoss::GetCurrentPhase() const { return g_phase; }
float CTCBoss::GetMinHealthForPhase(long phase) const { g_phaseAsked = phase; return g_phaseMin; }
static int g_heroNotes = 0;
void CTCHeroHealthObserver::OnHealthChanged() { ++g_heroNotes; }
static unsigned long g_setFlags = 0xffffffff;
void CTCObservable::SetFlags(unsigned long flags) { g_setFlags = flags; Flags = flags; }
static int g_frameUpdates = 0;
void CThingPhysical::SetAsFrameUpdated() { ++g_frameUpdates; }
static int g_fail = 0;
static void Check(bool c, const char* what) { if (!c) { printf("FAIL %s\n", what); g_fail = 1; } }
static void SetTC(CThingPhysical& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }
int main()
{
    CThingPhysicalDef def; memset(&def, 0, sizeof(def)); def.MinimumHealth = -50.0f;
    CTCBoss boss; memset(&boss, 0, sizeof(boss));
    CTCObservable obs; memset(&obs, 0, sizeof(obs));
    CTCHeroHealthObserver hero;
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 4; entries[0].component = (CTCBase*)&hero;
    entries[1].type = 9; entries[1].component = (CTCBase*)&obs;
    entries[2].type = 0xad; entries[2].component = (CTCBase*)&boss;

    CThingPhysical t; memset(&t, 0, sizeof(t));
    t.PBaseDef = &def; t.MaxHealth = 100.0f; t.Health = 50.0f; t.Damageable = 1; t.Killable = 1;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 3; t.m_interfaces.end = entries + 3;

    t.ThingClass = 3;
    t.InternalModifyHealth(-10.0f, true);
    Check(t.Health == 40.0f, "plain damage");
    Check(g_frameUpdates == 1, "frame updated");
    t.InternalModifyHealth(100.0f, true);
    Check(t.Health == 100.0f, "clamped to max");
    t.InternalModifyHealth(-200.0f, true);
    Check(t.Health == 0.0f, "killable dies at zero");
    t.Health = 50.0f;
    t.InternalModifyHealth(-200.0f, false);
    Check(t.Health == 1.0f, "no death without allowDeath");
    t.Health = 50.0f; SetTC(t, 0x84);
    t.InternalModifyHealth(-200.0f, true);
    Check(t.Health == 1.0f, "TC 0x84 prevents death");
    t.m_presentBits[0x84 >> 5] = 0;
    t.Health = 50.0f; t.Damageable = 0;
    t.InternalModifyHealth(-10.0f, true);
    Check(t.Health == 50.0f && g_frameUpdates == 5, "undamageable ignores damage early");
    t.InternalModifyHealth(10.0f, true);
    Check(t.Health == 60.0f, "undamageable still heals");
    t.Damageable = 1;

    t.ThingClass = 2; g_bPlayerInvulnerable = true;
    t.InternalModifyHealth(-10.0f, true);
    Check(t.Health == 60.0f, "player invulnerable");
    g_bPlayerInvulnerable = false;
    t.InternalModifyHealth(-10.0f, true);
    Check(t.Health == 50.0f && g_heroNotes == 0, "player damaged, no observer");
    SetTC(t, 4);
    t.InternalModifyHealth(-10.0f, true);
    Check(g_heroNotes == 1, "hero observer notified");

    t.ThingClass = 1; g_bCreaturesInvulnerable = true;
    t.InternalModifyHealth(-10.0f, true);
    Check(t.Health == 40.0f, "creature invulnerable");
    g_bCreaturesInvulnerable = false;
    SetTC(t, 9); obs.Flags = 6;
    t.InternalModifyHealth(-10.0f, true);
    Check(t.Health == 30.0f && g_setFlags == 0xffffffff, "alive creature leaves observable");
    t.InternalModifyHealth(-100.0f, true);
    Check(t.Health == 0.0f && g_setFlags == 4, "dead creature clears flag 2");
    g_setFlags = 0xffffffff;
    t.InternalModifyHealth(-1.0f, true);
    Check(g_setFlags == 0xffffffff, "flag already clear: no call");

    t.ThingClass = 3; t.Health = 50.0f; SetTC(t, 0xad);
    boss.Active = true; boss.PPhases = &boss; g_phase = 2; g_phaseMin = 30.0f;
    t.InternalModifyHealth(-30.0f, true);
    Check(g_phaseAsked == 2 && t.Health == 29.0f, "boss phase floor");
    t.InternalModifyHealth(10.0f, true);
    Check(t.Health == 39.0f, "boss above floor untouched");
    boss.Active = false; t.Health = 50.0f;
    t.InternalModifyHealth(-40.0f, true);
    Check(t.Health == 10.0f, "inactive boss no floor");
    def.MinimumHealth = 20.0f; t.Health = 50.0f;
    t.InternalModifyHealth(-40.0f, true);
    Check(t.Health == 20.0f, "def minimum health");

    if (!g_fail) printf("006a62e0_TEST PASS\n");
    return g_fail;
}
