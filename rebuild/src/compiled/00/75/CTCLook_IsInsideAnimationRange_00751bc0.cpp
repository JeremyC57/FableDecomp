// CTCLook::IsInsideAnimationRange() -- whether the creature is close enough to the camera (in the XY plane) for its look animation to matter, and is either on screen or in a movie sequence
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751BC0, length 176 bytes (+ int3 padding); speed TU (no frame pointer, esp-relative frame).
// Callees: CThing::GetPos @ 0x004C73D0, CTCBase::GetWorld @ 0x00686A70 (twice), CWorld virtual slot 8 (+0x20, GetViewPosition --
// a C3DVector through the hidden pointer), Distance2D(const C2DVector&, const C2DVector&) @ 0x009E2460 (`__fastcall`, float in st0),
// CTCGraphicAppearance::WasRenderedLastFrame @ 0x004BD200 on CThing::PTCGraphicAppearance (+0x64), CGameScriptInterface::
// IsInMovieSequence() const (slot 395, +0x62C) through CWorld::PGameScriptInterface (+0x38). CLookDef::CutOffNoLooking +0x50 (PDB).
// Shape: the thing's position is copied into a C3DVector local (member-wise copy ctor, so the dead Z store drops and the
// 12-byte slot keeps retail's 4-byte hole above X/Y) and the view position into a second 3D local; both reach Distance2D as 2D
// conversion temporaries in the frame. The range test is `!(distance <= cutoff)` (`fcomp ; test ah,41h ; jp` on the false edge);
// the `||` of the two queries is materialised as the 32-bit 0/1 tails. The script interface comes through an inline CWorld getter
// over the scoped_ptr (`mov ecx,[eax+38h]`); the scoped_ptr's operator-> or get() at the call site gives `add eax,38h` / `mov ecx,eax`.
// Layout: CThing::PTCGraphicAppearance at +0x64 (PDB +0x74; retail loads [eax+64h]); CWorld is modelled locally because the shared
// header exposes the scoped_ptr as a raw pointer and has no inline script-interface getter (the getter is what fixes the codegen).
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { } };
class C2DVector { public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};
float __fastcall Distance2D(const C2DVector& a, const C2DVector& b);   // 0x009E2460
struct CTCGraphicAppearance {
    bool WasRenderedLastFrame() const;               // 0x004BD200
};
class CThing { public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64 (PDB +0x74)
    const C3DVector& GetPos() const;            // 0x004C73D0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    T* get() const { return ptr; }
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual C3DVector GetViewPosition() const;       // slot 8 (+0x20)
    unsigned char _pad04[0x38 - 0x04];
    scoped_ptr<CGameScriptInterface> PGameScriptInterface;   // +0x38
    CGameScriptInterface* GetGameScriptInterface() const { return PGameScriptInterface.get(); }
};
class CLookDef { public:
    unsigned char _pad00[0x50];
    float CutOffNoLooking;                           // +0x50
};
template <class T> class CDefPointer { public: T* Object; T* operator->() const { return Object; } };

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCLook : public CTCBase { public:
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x0c];
    CDefPointer<CLookDef> PDef;                      // +0x14
    bool IsInsideAnimationRange();
};
bool CTCLook::IsInsideAnimationRange()
{
    C3DVector position = ParentThing->GetPos();
    C3DVector view = GetWorld().GetViewPosition();
    CLookDef* def = PDef.Object;
    if (Distance2D(C2DVector(position), C2DVector(view)) <= def->CutOffNoLooking)
        return ParentThing->PTCGraphicAppearance->WasRenderedLastFrame() || GetWorld().GetGameScriptInterface()->IsInMovieSequence();
    return false;
}