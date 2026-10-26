// CThingFilter_IsValidAttackTargetForCreature::CThingFilter_IsValidAttackTargetForCreature(CThingAICreature* creature, vector<const CThing*>* notList, bool checkAwareness) -- builds the composite attack-target filter and records whether the creature's def flags it as a ghost
// Leaf: ??0CThingFilter_IsValidAttackTargetForCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F6D70, length 125 bytes (`ret 0Ch`).
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x00485815 (manifest names a propagated CAbilityDef instantiation),
// the defs' slot 1 (Destroy) on release.
// Layout: PDB CThingFilter_IsValidAttackTargetForCreature (32 bytes): FilterWilling +0 (SourceCreature), FilterAware +4 (PCreature),
// FilterNot +8 (Thing), FilterAbleToBeEngaged +0xc (empty), FilterEnemy +0x10 (SourceThing), FilterNotInList +0x14
// {PNotList, PNotList2}, CheckAwareness +0x1c, IAmAGhostExclaim +0x1d. CThing PDef at +0x70; the creature sub-def's ghost
// flag byte at +0xe4 (retail offset, local model).
// Shape: init list in member order (the empty filter stores nothing); the thing def is a raw local with hand-written
// null-tested add-ref/release around GetSubDef, the out pointer zeroed between them (it lands in the dead
// checkAwareness argument slot); the out pointer is a CDefPointer-like local whose dtor releases it without a null
// test (`dec [ecx+4] ; jne`, one register for the read and the release).
class CThing;
class CThingAICreature;
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
};
template <class T> struct CSubDefPointer {      // the GetSubDef out pointer: zeroed, released at scope end (no null test)
    T* Object;
    CSubDefPointer() : Object(0) {}
    ~CSubDefPointer() { if (--Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CCreatureSubDef : CDefPointeeBase {
    unsigned char _pad08[0xe4 - 0x08];
    bool IsGhost;                               // +0xe4
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CSubDefPointer<CCreatureSubDef>* out);   // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CSubDefPointer<CCreatureSubDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingBase {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
};
template <class T> struct vector;
struct CThingFilter_IsAICreatureWillingToAttack {
    CThingAICreature* SourceCreature;
    CThingFilter_IsAICreatureWillingToAttack(CThingAICreature* c) : SourceCreature(c) {}
};
struct CThingFilter_IsAICreatureAwareOfMoreGenerous {
    CThingAICreature* PCreature;
    CThingFilter_IsAICreatureAwareOfMoreGenerous(CThingAICreature* c) : PCreature(c) {}
};
class CThingFilter_IsNot { public:
    CThing* Thing;
    CThingFilter_IsNot(CThing* t) : Thing(t) {}
};
class CThingFilter_IsThingAbleToBeEngagedInCombat { public:
};
class CThingFilter_EnemyCreature { public:
    CThing* SourceThing;
    CThingFilter_EnemyCreature(CThing* t) : SourceThing(t) {}
};
class CThingFilter_IsNotList { public:
    vector<const CThing*>* PNotList;
    void*                  PNotList2;
    CThingFilter_IsNotList(vector<const CThing*>* l) : PNotList(l), PNotList2(0) {}
};
struct CThingFilter_IsValidAttackTargetForCreature {
    CThingFilter_IsAICreatureWillingToAttack     FilterWilling;          // +0x00
    CThingFilter_IsAICreatureAwareOfMoreGenerous FilterAware;            // +0x04
    CThingFilter_IsNot                           FilterNot;              // +0x08
    CThingFilter_IsThingAbleToBeEngagedInCombat  FilterAbleToBeEngaged;  // +0x0c
    CThingFilter_EnemyCreature                   FilterEnemy;            // +0x10
    CThingFilter_IsNotList                       FilterNotInList;        // +0x14
    bool                                         CheckAwareness;         // +0x1c
    bool                                         IAmAGhostExclaim;       // +0x1d
    CThingFilter_IsValidAttackTargetForCreature(CThingAICreature* creature, vector<const CThing*>* notList, bool checkAwareness);
};

CThingFilter_IsValidAttackTargetForCreature::CThingFilter_IsValidAttackTargetForCreature(CThingAICreature* creature, vector<const CThing*>* notList, bool checkAwareness)
    : FilterWilling(creature), FilterAware(creature), FilterNot((CThing*)creature), FilterEnemy((CThing*)creature),
      FilterNotInList(notList), CheckAwareness(checkAwareness), IAmAGhostExclaim(false)
{
    CDefClassBase* thingDef = ((CThingBase*)creature)->PDef.Object;
    CSubDefPointer<CCreatureSubDef> def;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    if (def->IsGhost)
        IAmAGhostExclaim = true;
}