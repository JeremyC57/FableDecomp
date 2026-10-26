// Behaviour test for 005c3180: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. NInventory::CTCInventoryWeapons::GetActiveProjectileWeaponDamage()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#else
#define snprintf _snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCInventoryWeapons::GetActiveProjectileWeaponDamage() -- the damage of the projectile fired by the hero's active ranged weapon (its weapon def's projectile def), or 0 when no living ranged weapon is active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C3180, length 186 bytes (float in st0). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: IS_THING_ALIVE<CIntelligentPointer<const CThingPhysical>> @ 0x004CC340 (`__fastcall`, by reference), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<T>*) @ 0x0042B1A6 (catalog GetSubDef<CAbilityDef>, a byte twin), CTCBase::
// GetDefinitions() @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex(long) @ 0x009AD9E0 (a counted def pointer by value through the
// hidden pointer), the defs' virtual slot 1 on release.
// Layout: retail CTCInventoryWeapons PActiveRangedWeapon +0x141 (PDB +0x159; an 8-byte intelligent pointer at an odd offset, hence pack(1));
// CThing::PDef +0x70; the weapon def's projectile def index at +0xA0 and the projectile def's damage at +0x54 (retail; the PDB CWeaponDef has
// MissileLoadCarrySlot at +0xA0 -- the retail def is the projectile-weapon variant).
// Shape: the result lives in [ebp-8] (zeroed with `fldz ; fstp`, read back for the return); the weapon-def out slot ([ebp-4]) is the landed
// OnCreate dance (owner addref / GetSubDef / owner release by hand, all null-tested) with the same-TU GetSubDef stand-in (the out word does
// not escape, so the manager's by-value temporary shares it once the weapon def is in edi); the projectile def is a CDefPointerCopy local
// copy-initialised from that temporary (add-ref through the returned pointer, the temporary released with the long form and zeroed) and
// released by its destructor through the `DecRefToZero(long&)` helper (`mov eax,[esi+54h] ; dec [esi+4] ; mov [ebp-8],eax ; jne`: the
// damage load is scheduled before the decrement only with the by-reference helper), then the weapon def by hand.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRefCounted {                                 // the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()                          // inlined into ~CDefPointerTemp
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointerTemp {                             // the manager's by-value def reference
    CDefBase* m_p;
    __forceinline ~CDefPointerTemp()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
template <class T> class CDefPointer { public:              // the out slot the definition lookups fill (released by hand, as the landed OnCreate twins)
    T* Object;
    T* operator->() const { return Object; }
};
__forceinline bool DecRefToZero(long& c) { return --c == 0; }
template <class T> struct CDefPointerCopy {          // a def copied (add-ref) out of the manager's by-value temporary, released by its destructor
    T* Object;
    CDefPointerCopy(const CDefPointerTemp& temp) : Object((T*)temp.m_p) { if (Object) ++Object->m_count; }
    ~CDefPointerCopy() { if (DecRefToZero(Object->m_count)) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CProjectileDef : CDefBase {
    unsigned char _pad08[0x54 - 0x08];
    float Damage;                                    // +0x54
};
class CWeaponDef : public CDefBase { public:
    unsigned char _pad08[0xa0 - 0x08];
    long ProjectileDefIndex;                         // +0xa0
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefPointer<CWeaponDef>* out);    // 0x0042B1A6
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CWeaponDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CWeaponDef*)def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    CDefPointerTemp GetPDefFromGlobalIndex(long index);   // 0x009AD9E0 (hidden result pointer)
};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
};
struct CIntelligentPointer {                         // CIntelligentPointer<CThing> (8 bytes)
    void*   __vftable;
    CThing* PItem;
    CThing* GetPItem() const;                        // 0x00A01B50
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x004CC340 for CIntelligentPointer

namespace NInventory {
#pragma pack(push, 1)
struct CTCInventoryWeapons {
    unsigned char       _pad000[0x141];
    CIntelligentPointer PActiveRangedWeapon;         // +0x141
    CDefinitionManager* GetDefinitions();            // 0x006869D0 (CTCBase)
    float GetActiveProjectileWeaponDamage();
};
#pragma pack(pop)
float CTCInventoryWeapons::GetActiveProjectileWeaponDamage()
{
    float damage = 0.0f;
    if (IS_THING_ALIVE(PActiveRangedWeapon)) {
        CDefPointer<CWeaponDef> def;
        def.Object = 0;
        CDefClassBase* thingDef = PActiveRangedWeapon.GetPItem()->PDef.Object;
        if (thingDef)
            ++thingDef->m_count;
        thingDef->GetSubDef(&def);
        if (thingDef && --thingDef->m_count == 0)
            thingDef->Destroy();
        CWeaponDef* weaponDef = def.Object;
        {
            CDefPointerCopy<CProjectileDef> projectileDef = GetDefinitions()->GetPDefFromGlobalIndex(weaponDef->ProjectileDefIndex);
            damage = projectileDef->Damage;
        }
        if (--weaponDef->m_count == 0)
            weaponDef->Destroy();
    }
    return damage;
}
}

// ---- stubs ----
static char g_log[2048];
static void logf(const char* fmt, long a = 0, long b = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b); strcat(g_log, t); strcat(g_log, ";"); }
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { logf("Destroy(%ld)", (long)(size_t)this); }
struct CTestWeaponDef : CWeaponDef { };
struct CTestProjectileDef : CProjectileDef { };
struct CTestDefClass : CDefClassBase { };
static CTestWeaponDef g_weapon; static CTestProjectileDef g_projectile; static CTestDefClass g_thingDef;
static CThing g_thing; static CDefinitionManager g_manager;
static bool g_alive; static CWeaponDef* g_subDef; static CDefBase* g_projectileFound;
void* Find_CDefClassBase_GetSubDef(const void* owner) { logf("GetSubDef(owner %ld)", (long)(owner == &g_thingDef)); return g_subDef; }
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { logf("IS_THING_ALIVE(%ld)", (long)(thing.PItem == &g_thing)); return g_alive; }
CThing* CIntelligentPointer::GetPItem() const { logf("GetPItem"); return PItem; }
// VC7.1 has no NRVO: the local below is copied out bitwise and then destroyed (one release), so it takes one extra
// reference first; the returned temporary then carries no reference of its own, as the cases below expect.
CDefPointerTemp CDefinitionManager::GetPDefFromGlobalIndex(long index) { logf("GetPDefFromGlobalIndex(%ld)", index); CDefPointerTemp t; t.m_p = g_projectileFound; if (t.m_p) ++t.m_p->m_count; return t; }
CDefinitionManager* NInventory::CTCInventoryWeapons::GetDefinitions() { logf("GetDefinitions"); return &g_manager; }

int main()
{
    using namespace NInventory;
    CTCInventoryWeapons* inv = (CTCInventoryWeapons*)calloc(1, sizeof(CTCInventoryWeapons));
    inv->PActiveRangedWeapon.PItem = &g_thing;
    g_thing.PDef.Object = &g_thingDef;
    g_weapon.ProjectileDefIndex = 42; g_projectile.Damage = 12.5f;

    // 1. dead / no active ranged weapon: 0, nothing looked up
    g_alive = false; g_log[0] = 0;
    CHECK(inv->GetActiveProjectileWeaponDamage() == 0.0f);
    CHECK(strcmp(g_log, "IS_THING_ALIVE(1);") == 0);

    // 2. live weapon: owner add-ref'd around GetSubDef, projectile def add-ref'd/released, damage returned, refcounts restored
    g_alive = true; g_subDef = &g_weapon; g_projectileFound = &g_projectile;
    g_thingDef.m_count = 1; g_weapon.m_count = 1; g_projectile.m_count = 1; g_log[0] = 0;
    float d = inv->GetActiveProjectileWeaponDamage();
    CHECK(d == 12.5f);
    CHECK(strncmp(g_log, "IS_THING_ALIVE(1);GetPItem;GetSubDef(owner 1);GetDefinitions;GetPDefFromGlobalIndex(42);Destroy(", 86) == 0);
    CHECK(g_thingDef.m_count == 1 && g_weapon.m_count == 0 && g_projectile.m_count == 0);
    // (the single references held by the lookups were consumed: both defs destroyed, in projectile-then-weapon order)
    g_log[0] = 0;

    // 3. extra references: the releases do not destroy
    g_thingDef.m_count = 2; g_weapon.m_count = 2; g_projectile.m_count = 2; g_log[0] = 0;
    d = inv->GetActiveProjectileWeaponDamage();
    CHECK(d == 12.5f);
    CHECK(strstr(g_log, "Destroy") == 0);
    CHECK(g_thingDef.m_count == 2 && g_weapon.m_count == 1 && g_projectile.m_count == 1);

    // 4. destruction order: projectile def released before the weapon def
    g_weapon.m_count = 1; g_projectile.m_count = 1; g_log[0] = 0;
    inv->GetActiveProjectileWeaponDamage();
    char expect[256]; snprintf(expect, sizeof expect, "Destroy(%ld);Destroy(%ld);", (long)(size_t)(CDefVirtual*)&g_projectile, (long)(size_t)(CDefVirtual*)&g_weapon);
    CHECK(strstr(g_log, expect) != 0);

    if (fails == 0) printf("005c3180_TEST PASS\n"); else printf("005c3180_TEST FAIL\n");
    return fails != 0;
}