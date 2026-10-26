// NUISystem::CZoomingComponent::SetZoomCenter(C2DVector& zoomCenter) -- virtual setter: copy the 2D vector into ZoomCenter
// Leaf: ?SetZoomCenter@CZoomingComponent@NUISystem@@UAEXAAVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00546BC0, 22 bytes (+ int3 padding to 32), thiscall `ret 4`.
// Identity (proven): NUISystem::CZoomingComponent::SetZoomCenter; evidence: it is slot 145 of vtable 0x01247DC4 (RTTI .?AVCZoomingComponent@NUISystem@@); ZoomCenter / MinZoom / MaxZoom are consecutive C2DVectors at Ego_r +0x190 / +0x198 / +0x1A0 and the three setters write retail +0x160 / +0x168 / +0x170; the PDB name sets hold ?SetZoomCenter@CZoomingComponent@NUISystem@@UAEXAAVC2DVector@@@Z, a virtual taking `C2DVector&`
// Label: the manifest's "NUISystem::CZoomingComponent::vfunc_145" is a slot placeholder (family H078: six C2DVector member setters of the NUISystem components;
//   retail has no identical-code folding, so each address is its own function).
// Layout: ZoomCenter at retail +0x160 (Ego_r.pdb +0x190: the debug CComponent base is 0x30 bytes larger -- its 16-byte
//   vectors and 12-byte lists are 12 and 4 bytes in retail); local model, no engine header for NUISystem::CZoomingComponent (Review at landing).
// Shape: the implicit C2DVector copy -- two dword moves through edx / eax.
class C2DVector {                                            // 8 bytes
public:
    float X;                                                 // +0x0
    float Y;                                                 // +0x4
};

namespace NUISystem {

class CZoomingComponent {                                              // vtable 0x01247DC4 (RTTI .?AVCZoomingComponent@NUISystem@@)
public:
    virtual void SetZoomCenter(C2DVector& zoomCenter);             // slot 145
    unsigned char _before[0x160 - 4];                    // the CComponent base and earlier members
    C2DVector     ZoomCenter;                                // +0x160
};

void CZoomingComponent::SetZoomCenter(C2DVector& zoomCenter)
{
    ZoomCenter = zoomCenter;
}

}   // namespace NUISystem