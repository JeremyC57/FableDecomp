// CTCPhysicsBase::ResetInterpolation() -- OldPosition = Position, stamp OldPositionLastFrameSet with the current frame, OldMoving = Moving
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF790, 59 bytes, CTCPhysicsBase vtable slot 80.
// Callees: 0x00686A70 (this-ignoring member returning g_pMainGameComponent->+0x24, the world; manifest "CViewBase::GetDisplayEngine",
//   propagated) and 0x0049D870 (static: returns the global frame counter 0x013B89BC; manifest "GFGetPreMainMemoryUsed", propagated).
// Layout: engine/CTCPhysicsBase.h; the PDB bool:1 pair Moving (bit 0) / OldMoving (bit 1) at +0x3c, which the header carries as
//   the opaque OldMoving[4] bytes, is reached through a bit-field view.
// Shape: `GetWorld()->GetFrame()` -- a STATIC member called through an object expression: the world call is made and its result
//   dropped; the bit copy is the shl/xor/and/xor insert through ONE reference to the flag byte (two view calls re-read it).
// Leaf: ResetInterpolation
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#define CTCPhysicsBase CTCPhysicsBaseLayout
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
class CWorld;

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCPhysicsBase : public CTCBase, public CTCPhysicsBaseLayout
{
};

#include "engine/C3DVector.h"

struct SPhysicsBaseFlags {                  // CTCPhysicsBase +0x3c bool:1 group
    bool Moving : 1;                        // bit 0
    bool OldMoving : 1;                     // bit 1
};

class CWorld { public:
    static long __fastcall GetFrame();                 // 0x0049D870
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    C3DVector& PositionVector()    { return *reinterpret_cast<C3DVector*>(&Position_X); }
    C3DVector& OldPositionVector() { return *reinterpret_cast<C3DVector*>(&OldPosition_X); }
    SPhysicsBaseFlags& Flags()     { return *reinterpret_cast<SPhysicsBaseFlags*>(OldMoving); }
    void ResetInterpolation();
};

void CTCPhysicsBase_Methods::ResetInterpolation()
{
    OldPositionVector() = PositionVector();
    OldPositionLastFrameSet = GetWorld().GetFrame();
    SPhysicsBaseFlags& flags = Flags();
    flags.OldMoving = flags.Moving;
}