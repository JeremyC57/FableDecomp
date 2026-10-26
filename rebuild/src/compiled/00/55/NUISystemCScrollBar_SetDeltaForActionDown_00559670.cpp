// NUISystem::CScrollBar::SetDeltaForActionDown(C2DVector& delta) -- virtual setter: copy the 2D vector into DeltaForActionDown
// Leaf: ?SetDeltaForActionDown@CScrollBar@NUISystem@@UAEXAAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00559670, 22 bytes (+ int3 padding to 32), thiscall `ret 4`.
// Identity (proven): NUISystem::CScrollBar::SetDeltaForActionDown; evidence: it is slot 155 of vtable 0x0124B7E4 (RTTI .?AVCScrollBar@NUISystem@@); DeltaForActionUp / DeltaForActionDown are consecutive C2DVectors at Ego_r +0x1EC / +0x1F4 and the two setters write retail +0x1BC / +0x1C4; the PDB name sets hold ?SetDeltaForActionDown@CScrollBar@NUISystem@@UAEXAAVC2DVector@@@Z, a virtual taking `C2DVector&`
// Label: the manifest's "NUISystem::CScrollBar::vfunc_155" is a slot placeholder (family H078: six C2DVector member setters of the NUISystem components;
//   retail has no identical-code folding, so each address is its own function).
// Layout: DeltaForActionDown at retail +0x1C4 (Ego_r.pdb +0x1F4: the debug CComponent base is 0x30 bytes larger -- its 16-byte
//   vectors and 12-byte lists are 12 and 4 bytes in retail); local model, no engine header for NUISystem::CScrollBar (Review at landing).
// Shape: the implicit C2DVector copy -- two dword moves through edx / eax.
class C2DVector {                                            // 8 bytes
public:
    float X;                                                 // +0x0
    float Y;                                                 // +0x4
};

namespace NUISystem {

class CScrollBar {                                              // vtable 0x0124B7E4 (RTTI .?AVCScrollBar@NUISystem@@)
public:
    virtual void SetDeltaForActionDown(C2DVector& delta);             // slot 155
    unsigned char _before[0x1C4 - 4];                    // the CComponent base and earlier members
    C2DVector     DeltaForActionDown;                                // +0x1C4
};

void CScrollBar::SetDeltaForActionDown(C2DVector& delta)
{
    DeltaForActionDown = delta;
}

}   // namespace NUISystem