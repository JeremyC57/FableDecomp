// CTCPhysicsStandard::ResetInterpolation() -- snap the interpolation: the old right-handed set becomes the current one, stamped with
// the world frame, then the base resets the old position
// Leaf: ?ResetInterpolation@CTCPhysicsStandard@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007236E0, 69 bytes, ends `jmp 0x006AF790` (the base call in tail position).
// Identity (agentNF14 2026-11-11; the catalog name, landed as a free __fastcall stub with invented helper / tail callees): vtable slot 80
// of CTCPhysicsStandard (RTTI vtable 0x01265F84), the slot whose CTCPhysicsBase entry (vtable 0x0125D2C4) is 0x006AF790
// CTCPhysicsBase::ResetInterpolation; Ego_r.pdb ?ResetInterpolation@CTCPhysicsStandard@@UAEXXZ. The body copies RHSet +0x50 to
// OldRHSet +0x68 (24 bytes) and stores the frame to RHSetLastFrameSet +0x80 (Ego_r.pdb layout).
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (result unused), CWorld::GetFrame() @ 0x0049D870 (static __fastcall),
// CTCPhysicsBase::ResetInterpolation() @ 0x006AF790 (non-virtual base call).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CWorld.h"

void CTCPhysicsStandard::ResetInterpolation()
{
    OldRHSet = RHSet;
    RHSetLastFrameSet = GetWorld().GetFrame();
    CTCPhysicsBase::ResetInterpolation();
}
