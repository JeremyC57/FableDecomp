// CGameScriptThing::GetFocalPos() const -- the focal position of the live thing's graphic appearance, or the stored position when the thing is gone or dead
// Leaf: ?GetFocalPos@CGameScriptThing@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D03E0, 92 bytes, thiscall, `ret 4` (C3DVector returned through the hidden pointer).
// Identity (proven): slot 5 of the CGameScriptThing vtable 0x012852CC (RTTI .?AVCGameScriptThing@@); Ego_r.pdb
//   ?GetFocalPos@CGameScriptThing@@UBE?AVC3DVector@@XZ (the catalog row's own mangled name).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (three times, on PThing +0x0C),
//   NGraphicAppearance::CTCGraphicAppearance::GetFocalPos(float) const 0x004C1060 (Ego_r.pdb spelling, also in reloc_bindings; the
//   other binding there, GetHelperPointOrCentre, is not a PDB name) with 1.0f.
// Layout: Ego_r.pdb CGameScriptThing: PThing (CIntelligentPointer<CThing>) +0x0C, Pos (C3DVector) +0x28; retail CThing
//   PTCGraphicAppearance +0x64 (Ego_r +0x74 less the STLport CVectorMap/list savings), Dead +0x91 bit 0.
// Shape: nested ifs (the `&&` form tests `!Dead` as a value: `mov al ; not al ; test al,1`), the thing read into a named local
//   before the call (so the third GetPItem runs before the pushes), and C3DVector's inline user copy constructor (FableWin.pdb)
//   for the member-wise copy of Pos on the fallback arm.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }   // FableWin.pdb user copy constructor (inline)
};
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    C3DVector GetFocalPos(float scale) const;                // 0x004C1060
};
}
class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Dead : 1;                                  // +0x91 bit 0
    bool IsDead() const { return Dead; }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                              // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    operator T*() const { return (T*)GetPItem(); }
};
class CGameScriptThing {
public:
    virtual ~CGameScriptThing();
    virtual C3DVector GetFocalPos() const;
    unsigned char             _pad04[8];
    CIntelligentPointer<CThing> PThing;                      // +0x0C
    unsigned char             _pad14[0x14];
    C3DVector                 Pos;                           // +0x28
};

C3DVector CGameScriptThing::GetFocalPos() const
{
    if (PThing) {
        if (!PThing->Dead) {
            CThing* thing = PThing;
            return thing->PTCGraphicAppearance->GetFocalPos(1.0f);
        }
    }
    return Pos;
}