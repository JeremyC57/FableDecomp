// Behaviour test for 008f3020: src/008f3020.cpp copied verbatim; with a target, the owner's ability def is fetched and its
// shot fired at the target with the owner's accuracy; the ability and melee defs are released afterwards.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_DragonFlight::PerformAction() -- with a target, fetch the owner's ability def and fire its shot at the target (accuracy from the owner's shot-accuracy percentage)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3020, length 212 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CDefClassBase::GetSubDef<CAbilityDef>(CDefPointer*) 0x0042B1A6 (on the
// owner's def-class base at +0x70), CThingCreatureBase::GetShotAccuracyPercentage 0x00663B10, CreateShot(long, CThing*, CThing*,
// CThing*, float, float, float, CDefPointer<CMeleeCombatAbilityDef>*, C3DVector*, CRightHandedSet*) 0x007C0AD0 (`__fastcall`),
// the defs' virtual slot 1 on the last release.
// Shape: the owner argument goes through the intelligent pointer's inline conversion (fetched before the stack arguments);
// the melee def pointer releases through the def's inline ReleaseTemporary (ref-count base at +4).
// GetSubDef gets a same-TU `auto_inline(off)` stand-in body that writes only the out slot: VC7.1 then knows the ability
//   holder does not escape and keeps the def in ebx from the call to the release (8-byte frame, the slot reused); the
//   def-class base is a named local so GetPItem is called before the out pointer is pushed, as retail.
struct CThing;
struct C3DVector;
struct CRightHandedSet;

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                      // slot 1
};
struct CAbilityDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0xa0 - 0x08];
    long ShotDefIndex;                           // +0xa0
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointerAbility {                      // CDefPointer<const CAbilityDef>
    CAbilityDef* m_p;
    CDefPointerAbility() : m_p(0) { }
    ~CDefPointerAbility() { if (m_p && --m_p->m_count == 0) m_p->Destroy(); }
    CAbilityDef* operator->() const { return m_p; }
};
struct CDefPointerMelee {                        // CDefPointer<const CMeleeCombatAbilityDef>
    CMeleeCombatAbilityDef* m_p;
    CDefPointerMelee() : m_p(0) { }
    ~CDefPointerMelee() { if (m_p) m_p->ReleaseTemporary(); }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointerAbility* out);     // 0x0042B1A6 (GetSubDef<CAbilityDef>)
};
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerAbility* out)
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->m_p = (CAbilityDef*)def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PhysicsTC;                    // +0x70 (the def-class base queried for the sub def)
    long GetShotAccuracyPercentage();            // 0x00663B10 (CThingCreatureBase)
};
CThing* __fastcall CreateShot(long shotDef, CThing* owner, CThing* parent, CThing* target, float accuracy, float a, float b,
                              CDefPointerMelee* meleeDef, C3DVector* offset, CRightHandedSet* hands);   // 0x007C0AD0

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                             // +0x04
    CThing* GetPItem() const;                    // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};

class CCombatActionBase {
public:
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;           // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;             // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
};

class CCreatureAction_DragonFlight : public CCombatActionBase {
public:
    bool  FaceTarget;                            // +0x114 (PDB +0x12c)
    float TurnRate;                              // +0x118 (PDB +0x130)
    virtual void PerformAction();
};

void CCreatureAction_DragonFlight::PerformAction()
{
    if (PTarget.GetPItem()) {
        CDefPointerAbility ability;
        CDefClassBase* defClass = PCreature.GetPItem()->PhysicsTC;
        if (defClass->GetSubDef(&ability)) {
            long shotDef = ability->ShotDefIndex;
            long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
            CDefPointerMelee melee;
            CreateShot(shotDef, PCreature, 0, PTarget.GetPItem(), (float)accuracy, 1.0f, 1.0f, &melee, 0, 0);
        }
    }
}

// ---- harness ----
static int g_destroyed = 0, g_shots = 0, g_subDefCalls = 0;
static void* g_destroyedLast = 0;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() { ++g_destroyed; g_destroyedLast = this; }
void CCombatActionBase::v00() { }
static CAbilityDef g_ability;
static CMeleeCombatAbilityDef g_melee;
static bool g_haveAbility = true;
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase*) { ++g_subDefCalls; if (!g_haveAbility) return 0; ++g_ability.m_count; return &g_ability; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
long CThing::GetShotAccuracyPercentage() { return 75; }
static long g_shotDef; static CThing *g_owner, *g_parent, *g_target; static float g_acc, g_a, g_b;
static void *g_offset, *g_hands;
CThing* __fastcall CreateShot(long shotDef, CThing* owner, CThing* parent, CThing* target, float accuracy, float a, float b,
                              CDefPointerMelee* meleeDef, C3DVector* offset, CRightHandedSet* hands)
{
    ++g_shots; g_shotDef = shotDef; g_owner = owner; g_parent = parent; g_target = target; g_acc = accuracy; g_a = a; g_b = b;
    g_offset = offset; g_hands = hands;
    if (meleeDef->m_p == 0) { ++g_melee.m_count; meleeDef->m_p = &g_melee; }
    return 0;
}
int main()
{
    CDefClassBase defClass;
    CThing owner, target;
    owner.PhysicsTC = &defClass;
    g_ability.ShotDefIndex = 0x1234;
    g_ability.m_count = 1; g_melee.m_count = 0;
    CCreatureAction_DragonFlight a;
    a.PCreature.m_pItem = &owner;
    a.PTarget.m_pItem = 0;
    a.PerformAction();                                 // no target: nothing
    CHECK(g_subDefCalls == 0 && g_shots == 0);
    a.PTarget.m_pItem = &target;
    a.PerformAction();
    CHECK(g_subDefCalls == 1 && g_shots == 1);
    CHECK(g_shotDef == 0x1234 && g_owner == &owner && g_parent == 0 && g_target == &target);
    CHECK(g_acc == 75.0f && g_a == 1.0f && g_b == 1.0f && g_offset == 0 && g_hands == 0);
    CHECK(g_ability.m_count == 1);                     // ability released back to the def system's reference
    CHECK(g_melee.m_count == 0 && g_destroyed == 1 && g_destroyedLast == (CDefVirtual*)&g_melee);   // temporary melee def destroyed
    g_haveAbility = false;
    a.PerformAction();                                 // no ability: no shot
    CHECK(g_subDefCalls == 2 && g_shots == 1);
    g_haveAbility = true; g_ability.m_count = 0;       // the holder owns the last reference
    a.PerformAction();
    CHECK(g_shots == 2 && g_ability.m_count == 0 && g_destroyed == 3);
    if (fails == 0) printf("008f3020_TEST PASS\n");
    return fails;
}