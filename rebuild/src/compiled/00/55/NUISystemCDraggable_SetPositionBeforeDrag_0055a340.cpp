// NUISystem::CDraggable::SetPositionBeforeDrag(C2DVector& position) -- virtual setter: copy the 2D vector into PositionBeforeDrag
// Leaf: ?SetPositionBeforeDrag@CDraggable@NUISystem@@UAEXAAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055A340, 22 bytes (+ int3 padding to 32), thiscall `ret 4`.
// Identity (proven): NUISystem::CDraggable::SetPositionBeforeDrag; evidence: it is slot 151 of vtable 0x0124BA94 (RTTI .?AVCDraggable@NUISystem@@) and slot 151 of vtable 0x0124B7E4 (RTTI .?AVCScrollBar@NUISystem@@); PositionBeforeDrag is CDraggable's only C2DVector member (Ego_r +0x1C4), written at retail +0x194; the PDB name sets hold ?SetPositionBeforeDrag@CDraggable@NUISystem@@UAEXAAVC2DVector@@@Z, a virtual taking `C2DVector&`; CScrollBar (a CDraggable) inherits it in the same slot
// Label: the manifest's "NUISystem::CScrollBar::vfunc_151" is a slot placeholder (family H078: six C2DVector member setters of the NUISystem components;
//   retail has no identical-code folding, so each address is its own function).
// Layout: PositionBeforeDrag at retail +0x194 (Ego_r.pdb +0x1C4: the debug CComponent base is 0x30 bytes larger -- its 16-byte
//   vectors and 12-byte lists are 12 and 4 bytes in retail); local model, no engine header for NUISystem::CDraggable (Review at landing).
// Shape: the implicit C2DVector copy -- two dword moves through edx / eax.
class C2DVector {                                            // 8 bytes
public:
    float X;                                                 // +0x0
    float Y;                                                 // +0x4
};

namespace NUISystem {

class CDraggable {                                              // vtable 0x0124BA94 (RTTI .?AVCDraggable@NUISystem@@)
public:
    virtual void SetPositionBeforeDrag(C2DVector& position);             // slot 151
    unsigned char _before[0x194 - 4];                    // the CComponent base and earlier members
    C2DVector     PositionBeforeDrag;                                // +0x194
};

void CDraggable::SetPositionBeforeDrag(C2DVector& position)
{
    PositionBeforeDrag = position;
}

}   // namespace NUISystem