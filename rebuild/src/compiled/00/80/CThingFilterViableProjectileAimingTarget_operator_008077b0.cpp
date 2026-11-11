// CThingFilter_ViableProjectileAimingTarget::operator()(CThing* thing) -- rejects things of class 0x0D and things whose creature sub-def has the +0xe4 flag set (the ghost flag), accepts everything else
// Leaf: ??RCThingFilter_ViableProjectileAimingTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008077B0, length 114 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x00485815 (manifest names a propagated CAbilityDef instantiation),
// the defs' slot 1 (Destroy) on release.
// Layout: retail CThing class byte at +0x10 (0x0D rejected), PDef at +0x70; the creature sub-def's flag byte at +0xe4 (the
// same flag CThingFilter_IsValidAttackTargetForCreature 0x007F6D70 reads as IAmAGhostExclaim; local model).
// Shape: the thing def is a raw local with hand-written null-tested add-ref/release around GetSubDef (the bool result
// kept in bl); the out pointer is a CDefPointer local whose dtor null-tests, so the arm that dereferenced it releases
// without the test; it lives in the dead thing argument slot.
class CThing;
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
};
class CCreatureDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xe4 - 0x08];
    bool IsGhost;                               // +0xe4
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00485815 GetSubDef<CCreatureDef> (Ego_r.pdb member template)
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingBase {
    unsigned char _pad00[0x10];
    unsigned char m_class;                      // +0x10
    unsigned char _pad11[0x70 - 0x11];
    CDefClassBase* PDef;                        // +0x70 (CDefPointer object)
};
struct CThingFilter_ViableProjectileAimingTarget {
    bool operator()(CThing* thing) const;
};

bool CThingFilter_ViableProjectileAimingTarget::operator()(CThing* pthing) const
{
    CThingBase* thing = (CThingBase*)pthing;
    if (thing->m_class == 0x0d)
        return false;
    CDefClassBase* thingDef = thing->PDef;
    CDefPointer<const CCreatureDef> def;
    if (thingDef)
        ++thingDef->m_refCount;
    bool got = thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    if (got && def->IsGhost)
        return false;
    return true;
}