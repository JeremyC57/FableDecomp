// NCameraModes::CScriptModuleBase::GetTransformedPosition(const C3DVector& offset) -- the offset placed relative to the module's origin: the live origin thing's position when there is one, else CoordOrigin
// Leaf: GetTransformedPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00853290, length 144 bytes (131 + int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, re-fetched per test),
// CThing::GetPos @ 0x004C73D0 (out of line here).
// Shape: `C3DVector origin = CoordOrigin;` (frame local through the memberwise user copy ctor: direct
// member loads, no `lea`), overwritten from the thing's
// position under the nested null / dead-bit (+0x91) tests, then `return C3DVector(origin.X + offset.X, ...)`
// through the inline three-float ctor: its arguments evaluate right to left, so Z is summed first and the
// three results pop into the hidden result in X, Y, Z order. `ret 8` (result pointer + offset).
// Layout: PDB NCameraModes::CScriptModuleBase (0x4c bytes): PThingOrigin +0xc, CoordOrigin +0x14.
class CBaseObject;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                        // +0x91 bit 0
    unsigned char m_91_1 : 7;
    const C3DVector& GetPos() const;            // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {        // vtable 0x01238C6C
    virtual void v00();
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

namespace NCameraModes {

class CScriptModuleBase { public:                           // vtable 0x01255F74
    virtual void v00();
    CIntelligentPointer_CThing      PCamPoint;       // +0x04
    CIntelligentPointer_CThingConst PThingOrigin;    // +0x0c
    C3DVector                       CoordOrigin;     // +0x14
    C3DVector GetTransformedPosition(const C3DVector& offset);
};

C3DVector CScriptModuleBase::GetTransformedPosition(const C3DVector& offset)
{
    C3DVector origin = CoordOrigin;
    if (PThingOrigin.GetPItem()) {
        if (!PThingOrigin->m_dead)
            origin = PThingOrigin->GetPos();
    }
    return C3DVector(origin.X + offset.X, origin.Y + offset.Y, origin.Z + offset.Z);
}

}