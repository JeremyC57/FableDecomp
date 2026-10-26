// Behaviour test for 007f6df0: src/007f6df0.cpp copied verbatim; the filter is true only when the awareness check (if
// enabled) and the five member filters pass and the thing's creature def's ghost flag matches IAmAGhostExclaim; the base
// def is add-ref'd/released around GetSubDef and the creature def is released on every exit past the filters.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CThingFilter_IsValidAttackTargetForCreature::operator()(CThing* thing) const -- a valid attack target is perceived (when awareness is checked), an enemy, attackable by a willing creature, not excluded, not on the ignore list, engageable, and a ghost exactly when the attacker is the ghost-exclaiming kind
// Leaf: ??RCThingFilter_IsValidAttackTargetForCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F6DF0, length 237 bytes, `ret 4`, bool in al.
// Callees: the aware-of test @ 0x00864600 (`__fastcall` creature, thing, false; `ret 4`; the manifest label is propagated,
//   neighbour of CanCreatureSeeCreature 0x008645E0), the member filters' operator()(CThing*): CThingFilter_EnemyCreature
//   @ 0x006A7550, CThingFilter_IsAICreatureWillingToAttack @ 0x006A7570, CThingFilter_IsNot @ 0x006A76A0,
//   CThingFilter_IsNotList @ 0x006A7EB0, CThingFilter_IsThingAbleToBeEngagedInCombat @ 0x006A7580 (the three 0x006A75xx
//   manifest labels are propagated _Cons_val names), CDefClassBase::GetSubDef<CCreatureDef> @ 0x00485815, the defs'
//   virtual Destroy (slot 1).
// Layout: CThingFilter_IsValidAttackTargetForCreature (Ego_r.pdb, 32 bytes, matches retail): FilterWilling +0, FilterAware
//   +4, FilterNot +8, FilterAbleToBeEngaged +0xc, FilterEnemy +0x10, FilterNotInList +0x14, CheckAwareness +0x1c,
//   IAmAGhostExclaim +0x1d; retail CThing PBaseDef +0x70; retail CCreatureDef AGhostIsWhatIBe +0xe4 (PDB +0xf8).
// Shape: the awareness filter is inline (its creature loaded into ecx); the sub-def is read through the by-value base-def
//   temporary of the landed CTCAnimationComplex::OnCreate (add-ref, GetSubDef, release in one expression) into a
//   CDefPointer out pointer homed in the dead argument slot, copied to a local and released by hand on each exit
//   (null-tested only on the success exit); the ghost rule is two separate mismatch tests. GetSubDef has a same-TU
//   out-slot-only stand-in body; the six filter tests are one `||` condition, so every early exit shares the first
//   ghost exit's `pop edi ; xor al,al ; pop esi ; ret 4` tail.
class CThing;
class CThingAICreature;
bool __fastcall IsAICreatureAwareOfMoreGenerous_00864600(CThingAICreature* creature, CThing* thing, bool flag);   // 0x00864600

class CDefPointeeBase { public:
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CCountedDefPointer {  // the by-value CDefPointer<T> copy: add-ref on copy, release on destruction
    T* Object;
    CCountedDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CCountedDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xe4 - 0x08];
    bool AGhostIsWhatIBe;                       // +0xe4
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CCreatureDef>* out) const;     // 0x00485815 GetSubDef<CCreatureDef>
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CCreatureDef*)def;
    return true;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70 CDefPointer<CThingBaseDef const>::Object
    CCountedDefPointer<CDefClassBase> GetBaseDef() const { return CCountedDefPointer<CDefClassBase>(PBaseDef); }
};

struct CThingFilter_IsAICreatureWillingToAttack { CThingAICreature* Creature;
    bool operator()(CThing* thing) const; };                    // 0x006A7570
struct CThingFilter_IsAICreatureAwareOfMoreGenerous { CThingAICreature* Creature;
    bool operator()(CThing* thing) const { return IsAICreatureAwareOfMoreGenerous_00864600(Creature, thing, false); } };
class CThingFilter_IsNot { public: const CThing* Thing;
    bool operator()(const CThing* thing) const; };              // 0x006A76A0
class CThingFilter_IsThingAbleToBeEngagedInCombat { public: CThingAICreature* Creature;
    bool operator()(CThing* thing) const; };                    // 0x006A7580
class CThingFilter_EnemyCreature { public: CThingAICreature* Creature;
    bool operator()(CThing* thing) const; };                    // 0x006A7550
class CThingFilter_IsNotList { public: void* Things[2];
    bool operator()(CThing* thing) const; };                    // 0x006A7EB0

struct CThingFilter_IsValidAttackTargetForCreature {
    CThingFilter_IsAICreatureWillingToAttack FilterWilling;             // +0x00
    CThingFilter_IsAICreatureAwareOfMoreGenerous FilterAware;           // +0x04
    CThingFilter_IsNot FilterNot;                                       // +0x08
    CThingFilter_IsThingAbleToBeEngagedInCombat FilterAbleToBeEngaged;  // +0x0c
    CThingFilter_EnemyCreature FilterEnemy;                             // +0x10
    CThingFilter_IsNotList FilterNotInList;                             // +0x14
    bool CheckAwareness;                                                // +0x1c
    bool IAmAGhostExclaim;                                              // +0x1d
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsValidAttackTargetForCreature::operator()(CThing* thing) const
{
    if ((CheckAwareness && !FilterAware(thing)) || !FilterEnemy(thing) || !FilterWilling(thing) || !FilterNot(thing)
        || !FilterNotInList(thing) || !FilterAbleToBeEngaged(thing))
        return false;
    CDefPointer<CCreatureDef> creatureDef;
    thing->GetBaseDef()->GetSubDef(&creatureDef);
    CCreatureDef* def = creatureDef.Object;
    if (IAmAGhostExclaim && !def->AGhostIsWhatIBe) {
        def->Release();
        return false;
    }
    if (!IAmAGhostExclaim && def->AGhostIsWhatIBe) {
        def->Release();
        return false;
    }
    if (def)
        def->Release();
    return true;
}

// ---- harness ----
static bool g_aware = true, g_enemy = true, g_willing = true, g_not = true, g_notList = true, g_engage = true;
static int g_calls = 0, g_destroyed = 0; static void* g_sub = 0; static const void* g_owner = 0;
static CThing* g_lastThing = 0; static CThingAICreature* g_awareCreature = 0; static bool g_awareFlag = true;
bool __fastcall IsAICreatureAwareOfMoreGenerous_00864600(CThingAICreature* creature, CThing* thing, bool flag)
{ ++g_calls; g_awareCreature = creature; g_lastThing = thing; g_awareFlag = flag; return g_aware; }
bool CThingFilter_IsAICreatureWillingToAttack::operator()(CThing* thing) const { ++g_calls; CHECK(thing == g_lastThing); return g_willing; }
bool CThingFilter_IsNot::operator()(const CThing* thing) const { ++g_calls; return g_not; }
bool CThingFilter_IsThingAbleToBeEngagedInCombat::operator()(CThing* thing) const { ++g_calls; return g_engage; }
bool CThingFilter_EnemyCreature::operator()(CThing* thing) const { ++g_calls; g_lastThing = thing; return g_enemy; }
bool CThingFilter_IsNotList::operator()(CThing* thing) const { ++g_calls; return g_notList; }
void CDefPointeeBase::Destroy0() { }
void CDefPointeeBase::Destroy() { ++g_destroyed; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_owner = owner; CHECK(((const CDefClassBase*)owner)->RefCount == 2); if (g_sub) ((CDefPointeeBase*)g_sub)->AddRef(); return g_sub; }
static void reset() { g_aware = g_enemy = g_willing = g_not = g_notList = g_engage = true; g_calls = 0; g_owner = 0; g_lastThing = 0; }
int main()
{
    CDefClassBase base; base.RefCount = 1;
    CCreatureDef cdef; cdef.RefCount = 1; cdef.AGhostIsWhatIBe = false;
    CThing thing; memset(&thing, 0, sizeof thing); thing.PBaseDef = &base;
    CThingFilter_IsValidAttackTargetForCreature f; memset(&f, 0, sizeof f);
    f.FilterAware.Creature = (CThingAICreature*)0x1234; f.CheckAwareness = true; f.IAmAGhostExclaim = false;
    g_sub = &cdef;

    reset();
    CHECK(f(&thing) && g_calls == 6 && g_awareCreature == (CThingAICreature*)0x1234 && !g_awareFlag && g_owner == &base);
    CHECK(base.RefCount == 1 && cdef.RefCount == 1 && g_destroyed == 0);

    // each filter failing short-circuits and returns false without touching the defs
    reset(); g_aware = false; CHECK(!f(&thing) && g_calls == 1 && g_owner == 0);
    reset(); g_enemy = false; CHECK(!f(&thing) && g_calls == 2 && g_owner == 0);
    reset(); g_willing = false; CHECK(!f(&thing) && g_calls == 3);
    reset(); g_not = false; CHECK(!f(&thing) && g_calls == 4);
    reset(); g_notList = false; CHECK(!f(&thing) && g_calls == 5);
    reset(); g_engage = false; CHECK(!f(&thing) && g_calls == 6 && g_owner == 0);
    // awareness not checked
    reset(); f.CheckAwareness = false; g_aware = false; CHECK(f(&thing) && g_calls == 5); f.CheckAwareness = true;

    // ghost mismatch both ways
    reset(); cdef.AGhostIsWhatIBe = true; CHECK(!f(&thing) && cdef.RefCount == 1 && base.RefCount == 1);
    reset(); f.IAmAGhostExclaim = true; CHECK(f(&thing) && cdef.RefCount == 1);
    reset(); cdef.AGhostIsWhatIBe = false; CHECK(!f(&thing) && cdef.RefCount == 1);
    f.IAmAGhostExclaim = false;

    // last reference to the creature def: destroyed
    reset(); cdef.RefCount = 0; CHECK(f(&thing) && cdef.RefCount == 0 && g_destroyed == 1);
    reset(); cdef.RefCount = 0; cdef.AGhostIsWhatIBe = true; CHECK(!f(&thing) && g_destroyed == 2);

    if (fails == 0) printf("007f6df0_TEST PASS\n");
    return fails;
}