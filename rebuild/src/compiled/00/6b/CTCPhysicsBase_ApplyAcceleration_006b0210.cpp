// CTCPhysicsBase::ApplyAcceleration(const C3DVector& acceleration) -- Velocity += acceleration, flag the parent thing as frame-updated and mark the body Moving
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B0210, 49 bytes.
// Callees: CThing::SetAsFrameUpdated @ 0x004C7C40 (on the parent thing at +0x04).
// Layout: engine/CTCPhysicsBase.h (Velocity +0x28, the parent thing +0x04); the PDB bool:1 Moving (bit 0 of +0x3c, the header's
//   opaque OldMoving[4] bytes) through a bit-field view.
// Shape: the inline C3DVector += (X member-first, Y/Z argument-first: the compiler's operand choice); `or byte ptr [esi+3Ch],1`.
// Leaf: ApplyAcceleration
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/CTCPhysicsBase.h"

#include "engine/C3DVector.h"

struct SPhysicsBaseFlags {                  // CTCPhysicsBase +0x3c bool:1 group
    bool Moving : 1;                        // bit 0
    bool OldMoving : 1;                     // bit 1
};

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#include "engine/CThing.h"

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    CThing* GetThing() { return reinterpret_cast<CThing*>(sub); }
    C3DVector& VelocityVector() { return *reinterpret_cast<C3DVector*>(&Velocity_X); }
    SPhysicsBaseFlags& Flags()  { return *reinterpret_cast<SPhysicsBaseFlags*>(OldMoving); }
    void ApplyAcceleration(const C3DVector& acceleration);
};

void CTCPhysicsBase_Methods::ApplyAcceleration(const C3DVector& acceleration)
{
    VelocityVector() += acceleration;
    GetThing()->SetAsFrameUpdated();
    Flags().Moving = true;
}