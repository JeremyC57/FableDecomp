// CThingFilter_IsValidAttackTargetForCreature::operator()(CThing* thing) const -- a valid attack target is perceived (when awareness is checked), an enemy, attackable by a willing creature, not excluded, not on the ignore list, engageable, and a ghost exactly when the attacker is the ghost-exclaiming kind
// Leaf: ??RCThingFilter_IsValidAttackTargetForCreature
// Grade: DIFFER(289v239) -- the creature-def releases: retail keeps the def in ecx and releases with `dec [ecx+4] ; jne`, ours re-forms `mov esi,[eax+4] ; add eax,4 ; dec ; test ; mov [eax],..` on every exit (CDefPointer dtor, Release() and direct-decrement forms; the inline release lane gap)
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
//   (null-tested only on the success exit); the ghost rule is two separate mismatch tests.
class CThing;
class CThingAICreature;
bool __fastcall IsAICreatureAwareOfMoreGenerous_00864600(CThingAICreature* creature, CThing* thing, bool flag);   // 0x00864600

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CCountedDefPointer {  // the by-value CDefPointer<T> copy: add-ref on copy, release on destruction
    T* Object;
    CCountedDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CCountedDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CCreatureDef : CDefPointeeBase {
    unsigned char _pad08[0xe4 - 0x08];
    bool AGhostIsWhatIBe;                       // +0xe4
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureDef>* out) const;     // 0x00485815 GetSubDef<CCreatureDef>
};

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
struct CThingFilter_IsNot { const CThing* Thing;
    bool operator()(const CThing* thing) const; };              // 0x006A76A0
struct CThingFilter_IsThingAbleToBeEngagedInCombat { CThingAICreature* Creature;
    bool operator()(CThing* thing) const; };                    // 0x006A7580
struct CThingFilter_EnemyCreature { CThingAICreature* Creature;
    bool operator()(CThing* thing) const; };                    // 0x006A7550
struct CThingFilter_IsNotList { void* Things[2];
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
    if (CheckAwareness && !FilterAware(thing))
        return false;
    if (!FilterEnemy(thing))
        return false;
    if (!FilterWilling(thing))
        return false;
    if (!FilterNot(thing))
        return false;
    if (!FilterNotInList(thing))
        return false;
    if (!FilterAbleToBeEngaged(thing))
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
