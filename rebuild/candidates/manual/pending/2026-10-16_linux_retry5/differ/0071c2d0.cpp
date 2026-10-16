// CTCHeroMorph::HasAPermanentTattooCoveringArea() const -- the body area covered by the first permanent tattoo among the hero's tattoo defs, or 0
// Grade: DIFFER(158v140) -- the out-pointer release: retail keeps the object in ecx and the count in eax (hoisted above the Permanent branch) and tests the decremented register; ours swaps eax/ecx and re-reads the out pointer from the frame for the zero test | next: dtor through a local copy `CDefPointeeBase* o = Object; if (--o->m_refCount == 0) o->Delete();` | retry batch 5: the CSubDefPointer dtor through a local copy still re-reads the out pointer and swaps eax/ecx (158v140); parked
// Leaf: HasAPermanentTattooCoveringArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C2D0, length 140 bytes (+4 int3 padding).
// Ego_r.pdb: ?HasAPermanentTattooCoveringArea@CTCHeroMorph@@QBEJXZ
// Callees: CDefClassBase::GetSubDef<CTattooDef>(CDefPointer*) @ 0x005B1EFC (manifest "GetSubDef<CAbilityDef>", propagated;
// its bool result is ignored), the defs' slot 1 on release.
// Shape: each Tattoos element is copied into a CDefPointer local (null-tested AddRef / release); the sub-def out pointer starts
// null in the `push ecx` frame word and is released by hand with no null test (`mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`),
// the permanent flag and its reference count both loaded before the branch; the found arm reads the area before both releases.
// Layout: retail Tattoos +0x1c (12-byte vector of CDefPointer<const CThingBaseDef>); the tattoo sub-def: permanent flag +0x3d,
// covered area +0x40 (descriptive names).
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                      // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return (T*)Object; }
};
template <class T> struct CSubDefPointer {      // GetSubDef's out pointer (released by hand)
    CDefPointeeBase* Object;
    CSubDefPointer() : Object(0) {}
    ~CSubDefPointer() { if (--Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return (T*)Object; }
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
};
struct CTattooDef : CDefPointeeBase {
    unsigned char _pad08[0x3d - 0x08];
    bool  Permanent;                            // +0x3d
    unsigned char _pad3e[2];
    long  CoveredArea;                          // +0x40
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CSubDefPointer<const CTattooDef>* out) const;  // 0x005B1EFC
};
class CThing;
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroMorph : public CTCBase {
public:
    void* PMeshMorphSet[2];                     // +0x0c CCountedPointer<CMeshMorphSet>
    void* ParticleEmitters;                     // +0x14
    void* IdleParticleEmitters;                 // +0x18
    CRetailVector<CDefPointer<const CThingBaseDef> > Tattoos;   // +0x1c
    long HasAPermanentTattooCoveringArea() const;
};

long CTCHeroMorph::HasAPermanentTattooCoveringArea() const
{
    for (const CDefPointer<const CThingBaseDef>* it = Tattoos.begin(); it != Tattoos.end(); ++it) {
        CDefPointer<const CThingBaseDef> def = *it;
        CSubDefPointer<const CTattooDef> out;
        def->GetSubDef(&out);
        if (out->Permanent)
            return out->CoveredArea;
    }
    return 0;
}
