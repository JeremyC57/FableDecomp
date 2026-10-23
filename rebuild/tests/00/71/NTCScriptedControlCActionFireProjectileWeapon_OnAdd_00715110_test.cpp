// Behaviour test for 0x00715110 (RST): model plus the body verbatim from final.cpp, callees stubbed.
// Checks that a creature holding an unloaded missile weapon queues Reload, Aim and Fire (Fire aimed at the target),
// a loaded one Aim and Fire only, a dead target / no inventory / non-missile weapon queue nothing, and every queued
// counted-pointer temporary is released (each action destroyed once).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0;                  // live CScriptThing copies (every exit must release the watched thing)
static int g_iatt = 0;                  // IsActiveThreadTerminating call count
static int g_termAt = 1 << 30;          // ... it returns true from this call number on
static int g_termFrame = 1 << 30;       // ... or once this many script frames have run
static int g_frames = 0;                // NewScriptFrame count
static bool g_badName = false;          // a callee saw an unexpected name / argument
static bool Term() { ++g_iatt; return g_iatt >= g_termAt || g_frames >= g_termFrame; }
static void Reset() { g_live = 0; g_iatt = 0; g_termAt = 1 << 30; g_termFrame = 1 << 30; g_frames = 0; g_badName = false; }

struct CTCBase;
struct CThing;
static int g_destroyed = 0;
struct CCreatureAction { int Kind; CThing* Target; virtual ~CCreatureAction() { ++g_destroyed; } };
struct CCreatureAction_ReloadMissileWeapon : CCreatureAction { CCreatureAction_ReloadMissileWeapon(CThing*) { Kind = 1; Target = 0; } };
struct CCreatureAction_AimMissileWeapon : CCreatureAction { CCreatureAction_AimMissileWeapon(CThing*) { Kind = 2; Target = 0; } };
struct CCreatureAction_FireMissileWeapon : CCreatureAction { CCreatureAction_FireMissileWeapon(CThing*, CThing* target) { Kind = 3; Target = target; } };
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
static void __fastcall DestroyAction(void* p) { delete (CCreatureAction*)p; }
struct CCountedPointer {
    CCreatureAction* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(CCreatureAction* object) : m_pObject(object)
    {
        m_pCount = new SCountBlock; m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyAction; m_pCount->m_pObject = object;
    }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};
static int g_kinds[8]; static CThing* g_fireTargets[8]; static int g_added = 0;
struct CCreatureActionQueue { void Add(const CCountedPointer& action) { if (g_added < 8) { g_kinds[g_added] = action.m_pObject->Kind; g_fireTargets[g_added] = action.m_pObject->Target; } ++g_added; } };
static bool g_missile = true, g_loaded = false;
struct CTCInventoryAbilities { CThing* Weapon; CThing* GetWeaponInHand() { return Weapon; } };
struct CTCWeapon { bool IsMissileWeapon() { return g_missile; } bool IsLoaded() { return g_loaded; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    void*               __vftable;
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];
    CInterfaceMap       Components;
    unsigned char       _pad4c[0x91 - 0x4c];
    unsigned char       Flags91;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
static bool g_targetAlive = true; static CThing g_targetThing;
struct CScriptThing {
    CThing* GetThing() const { return &g_targetThing; }
    bool IsAlive() const { return g_targetAlive; }
};
struct CCreaturePointer { void* __vftable; CThing* PThing; };
class CActionFireProjectileWeapon {
public:
    CCreaturePointer* PControl;
    CScriptThing PTargetThing;
    CCreatureActionQueue Actions;
    void OnAdd();
};

void CActionFireProjectileWeapon::OnAdd()
{
    CThing* creature = PControl->PThing;
    bool hasMissileWeapon = false;
    bool needsReload = false;
    CTCInventoryAbilities* inventory;
    if (creature->GetInterface(&inventory, 0x46)) {
        CThing* weapon = inventory->GetWeaponInHand();
        CTCWeapon* tcWeapon;
        if (weapon && !(weapon->Flags91 & 1) && weapon->GetInterface(&tcWeapon, 0x48) && tcWeapon->IsMissileWeapon()) {
            hasMissileWeapon = true;
            needsReload = !tcWeapon->IsLoaded();
        }
    }
    if (PTargetThing.IsAlive() && hasMissileWeapon) {
#pragma inline_depth(0)
        if (needsReload)
            Actions.Add(CCountedPointer(new CCreatureAction_ReloadMissileWeapon(creature)));
#pragma inline_depth()
        Actions.Add(CCountedPointer(new CCreatureAction_AimMissileWeapon(creature)));
        Actions.Add(CCountedPointer(new CCreatureAction_FireMissileWeapon(creature, PTargetThing.GetThing())));
    }
}

static CThing g_creature, g_weapon; static CTCInventoryAbilities g_inv; static CTCWeapon g_tcw;
static CInterfaceMap::SEntry g_cEntries[1], g_wEntries[1];
static void Setup(bool hasInventory, bool missile, bool loaded, bool targetAlive)
{
    memset(&g_creature, 0, sizeof(g_creature)); memset(&g_weapon, 0, sizeof(g_weapon));
    g_cEntries[0].type = 0x46; g_cEntries[0].component = (CTCBase*)&g_inv;
    g_wEntries[0].type = 0x48; g_wEntries[0].component = (CTCBase*)&g_tcw;
    g_creature.Components.first = g_cEntries; g_creature.Components.last = g_cEntries + 1;
    g_weapon.Components.first = g_wEntries; g_weapon.Components.last = g_wEntries + 1;
    if (hasInventory) g_creature.UsingComponents[0x46 >> 5] |= 1UL << (0x46 & 31);
    g_weapon.UsingComponents[0x48 >> 5] |= 1UL << (0x48 & 31);
    g_inv.Weapon = &g_weapon; g_missile = missile; g_loaded = loaded; g_targetAlive = targetAlive;
    g_added = 0; g_destroyed = 0;
}
int main()
{
    CCreaturePointer control; control.__vftable = 0; control.PThing = &g_creature;
    CActionFireProjectileWeapon action; action.PControl = &control;
    Setup(true, true, false, true); action.OnAdd();
    CHECK(g_added == 3 && g_kinds[0] == 1 && g_kinds[1] == 2 && g_kinds[2] == 3 && g_fireTargets[2] == &g_targetThing && g_destroyed == 3);
    Setup(true, true, true, true); action.OnAdd();
    CHECK(g_added == 2 && g_kinds[0] == 2 && g_kinds[1] == 3 && g_destroyed == 2);
    Setup(true, true, false, false); action.OnAdd(); CHECK(g_added == 0 && g_destroyed == 0);
    Setup(false, true, false, true); action.OnAdd(); CHECK(g_added == 0);
    Setup(true, false, false, true); action.OnAdd(); CHECK(g_added == 0);
    Setup(true, true, false, true); g_weapon.Flags91 = 1; action.OnAdd(); CHECK(g_added == 0);
    Setup(true, true, false, true); g_inv.Weapon = 0; action.OnAdd(); CHECK(g_added == 0);
    char line[64];
    _snprintf(line, sizeof(line), "00715110_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}