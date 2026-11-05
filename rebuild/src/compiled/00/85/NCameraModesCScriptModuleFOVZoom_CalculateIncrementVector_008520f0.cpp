// NCameraModes::CScriptModuleFOVZoom::CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames) -- Increment = (to - from) / frames
// Leaf: CalculateIncrementVector
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Grade: RELOCATION_MATCH -- C3DVector operators built through the (x, y, z) ctor keep diff.X/diff.Y on the FPU; the reciprocal passed inline as operator*'s float argument is homed in the dead `to` slot as retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008520F0, length 112 bytes (106 + int3 padding). No callees.
// Shape: the difference through an inline member `operator-` (named local), then `diff * (1.0f / frames)`
// through an inline member `operator*(float)`: the reciprocal (`fild ; fdivr [1.0f]`) is stored in the
// dead `to` argument slot, the product lands in a frame temporary and goes to Increment (+0x68)
// through integer registers.
// Layout: PDB NCameraModes::CScriptModuleFOVZoom (0x84 bytes): LookAtPos +0x4c, EndFrame +0x58,
// WorldFrames +0x5c, FOVIncrement +0x60, FOVFraction +0x64, Increment +0x68, StartPos +0x74,
// ShortenOnOcclusion +0x80, ResetCamera +0x81, over the 0x4c-byte CScriptModuleBase.
#include "engine/C3DVector.h"

namespace NCameraModes {

struct CScriptModuleFOVZoom {
    virtual void v00();
    unsigned char _pad04[0x48];
    C3DVector     LookAtPos;                         // +0x4c
    long          EndFrame;                          // +0x58
    long          WorldFrames;                       // +0x5c
    float         FOVIncrement;                      // +0x60
    float         FOVFraction;                       // +0x64
    C3DVector     Increment;                         // +0x68
    C3DVector     StartPos;                          // +0x74
    bool          ShortenOnOcclusion;                // +0x80
    bool          ResetCamera;                       // +0x81
    void CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames);
};

void CScriptModuleFOVZoom::CalculateIncrementVector(const C3DVector& from, const C3DVector& to, long frames)
{
    C3DVector diff = to - from;
    Increment = diff * (1.0f / frames);
}

}