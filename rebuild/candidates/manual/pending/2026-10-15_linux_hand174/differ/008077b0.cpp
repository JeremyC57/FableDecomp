// CThingFilter_ViableProjectileAimingTarget::operator()(CThing* thing) -- rejects things of class 0x0D and things whose creature sub-def has the +0xe4 flag set (the ghost flag), accepts everything else
// Grade: DIFFER(132v114) -- the sub-def release re-reads the escaped out slot after `dec [eax+4]` on both arms where retail loads it once into ecx (`dec [ecx+4] ; jne`); a register copy in the dtor, a Release() member or a copied holder all give the `mov esi,[eax+4] ; add eax,4 ; dec esi` form (same gap as 0x007F6D70)
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
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CCreatureSubDef : CDefPointeeBase {
    unsigned char _pad08[0xe4 - 0x08];
    bool IsGhost;                               // +0xe4
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CCreatureSubDef>* out);   // 0x00485815
};
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
    CDefPointer<CCreatureSubDef> def;
    if (thingDef)
        ++thingDef->m_refCount;
    bool got = thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    if (got && def->IsGhost)
        return false;
    return true;
}
