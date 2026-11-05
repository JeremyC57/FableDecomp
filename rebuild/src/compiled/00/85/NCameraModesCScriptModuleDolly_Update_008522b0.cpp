// NCameraModes::CScriptModuleDolly::Update() -- put the owning camera at PosAround + DollyVector looking at PosAround, then turn DollyVector about Z by one frame's share of a full turn
// Leaf: Update
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008522B0, length 160 bytes (153 + int3 padding).
// Callees: C3DVector::Rotate(const C3DVector& axis, float angle) @ 0x00A13C60.
// Globals: the frames-per-second count @ 0x01375550 (a long, 15 at rest).
// Shape: `PosAround + DollyVector` through the inline member operator+ of the landed GoToLookInDirection
// model, the look-at a plain copy; the frame rate read into a long local FIRST (the owner stores could
// alias the global, so the value is kept in the frame and `fild`ed from there), the Z axis a `C3DVector(0,0,1)` temporary, the angle `1.0f / (fps * TimeFor360Turn)`.
// Layout: PDB NCameraModes::CScriptModuleDolly (0x68 bytes): PosAround +0x4c, DollyVector +0x58,
// TimeFor360Turn +0x64, over the 0x4c-byte CScriptModuleBase (POwner +0x38).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

#include "engine/C3DVector.h"

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

struct CScriptModuleDolly {
    virtual void v00();
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    C3DVector     PosAround;                         // +0x4c
    C3DVector     DollyVector;                       // +0x58
    float         TimeFor360Turn;                    // +0x64
    void Update();
};

void CScriptModuleDolly::Update()
{
    long fps = CMainGameComponent::GetConstantFPS();
    POwner->Position = PosAround + DollyVector;
    POwner->LookAt = PosAround;
    DollyVector.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 1.0f / (fps * TimeFor360Turn));
}

}