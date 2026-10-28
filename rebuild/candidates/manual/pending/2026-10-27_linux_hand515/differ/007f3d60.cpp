// (anonymous namespace)::CThingFilter_IsElectrocutable::operator()(const CThingObject& thing) const -- the object has a definition and that definition marks it electrocutable
// Grade: DIFFER(90v90) -- the second counted copy's reference count goes through edx where retail uses ecx (`mov ecx,[esi+4] ; inc ecx ; mov eax,ecx ; dec eax`); the vtable then in edx; everything else byte-identical (3 forms: inline PeekDefinition, early return, named local)
// Leaf: ??RCThingFilter_IsElectrocutable@?A0x
// Retail: Fable.exe @ 0x007F3D60, 90 bytes, thiscall `ret 4` (the stateless filter's `this` is never read).
// Identity (proven): FableWin.pdb ??RCThingFilter_IsElectrocutable@?A0xc5a89a9e@@QBE_NABVCThingObject@@@Z; the caller
//   0x007F3E30 is CFilter_Bind3<CThingFilter_In3DRange, unary_negate<CThingFilter_HasTC>, CThingFilter_IsElectrocutable>
//   ::operator()(const CThingObject&) const (Ego_r.pdb F1 +0 / F2 +4 / F3 +8; it calls the landed In3DRange 0x006A7780 on
//   F1, the landed HasTC 0x006A80A0 on F2 negated, then this on F3). The flag is Ego_r.pdb CThingObjectDef::Electrocutable
//   (bit 4 of the bool:1 byte, Ego_r +0x9C, retail +0x98). The catalog label "CCombatActionBase::GetDamageMultiplier" is
//   propagated; the old park's CAnimatedAction / CAnimationEntry classes were invented.
// Body: CThing::GetPBaseDef() const (by value; FableWin.pdb ?GetPBaseDef@CThing@@QBE?AV?$CDefPointer@$$CBVCThingBaseDef@@@@XZ)
//   is tested and released, then CThingObject::PeekDefinition() const (protected, PDB ?PeekDefinition@CThingObject@@IBE...)
//   takes the raw def out of a second counted copy, released before the flag read. Both inline.
class CDefPointeeBase {
public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1 (+0x04)
    mutable long RefCount;                                  // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* Get() const { return Object; }
    bool IsValid() const { return Object != 0; }
};
class CThingBaseDef : public CDefPointeeBase { };
class CThingObjectDef : public CThingBaseDef {
public:
    unsigned char _pad08[0x98 - 0x08];
    bool IsFragile : 1;                                     // +0x98 bit 0
    bool IsClothing : 1;                                    // bit 1
    bool IsSelfIlluminating : 1;                            // bit 2
    bool UseHighDetailQuadTree : 1;                         // bit 3
    bool Electrocutable : 1;                                // bit 4
};
namespace { class CThingFilter_IsElectrocutable; }
class CThing {
public:
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;              // +0x70
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingObject : public CThingGameObject {
    friend class CThingFilter_IsElectrocutable;
protected:
    const CThingObjectDef& PeekDefinition() const { return *(const CThingObjectDef*)GetPBaseDef().Get(); }
};
namespace {
class CThingFilter_IsElectrocutable {
public:
    bool operator()(const CThingObject& thing) const;
};
bool CThingFilter_IsElectrocutable::operator()(const CThingObject& thing) const
{
    if (thing.GetPBaseDef().IsValid())
        return thing.PeekDefinition().Electrocutable;
    return false;
}
}
// Keeps the anonymous-namespace operator() emitted (the TU's Bind3 user).
bool CallIsElectrocutable_007f3d60(const CThingFilter_IsElectrocutable& f, const CThingObject& t) { return f(t); }
