// CTCWobble::OnInitialActivate() -- set the wobble angle, remember the parent's right-handed set, apply the initial half-angle rotation of Up about Forward, then reset the direction and the frame count (half the frame rate, rounded up)
// Leaf: ?OnInitialActivate@CTCWobble@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E2DF0, 257 bytes, `ret`. CTCWobble vtable 0x0123DF34.
// Callees: CTCPhysicsBase::GetRHSet() const (vtable +0x120) and SetRHSet (+0x12C, slot 75: CTCPhysicsBase vtable 0x0125D2C4 slot 75 = 0x006B12C0 SetRHSet; slot 74 0x006B12B0 is SetFacingAnglesFromRHSet), C3DVector::Rotate(const C3DVector&,
//   float) 0x00A13C60. Globals: CMainGameComponent::ConstantFPS 0x01375550, 0.5f 0x0122F59C. AngleOfWobble = 0.004f (0x3B83126F).
// Ego_d locals: new_rhset (CRightHandedSet), up (C3DVector).
// Matched (hand2c 2026-11-19): ported onto engine/CTCWobble.h (PASS22 layout) + CTCPhysicsBase / CRightHandedSet / C3DVector with
//   Ego_d's locals; the old draft's residue (the ParentRHSet copy through edi/edx) closed once ParentRHSet is assigned from the real
//   `const CRightHandedSet&` GetRHSet(); the second virtual is SetRHSet (slot 75), not SetFacingAnglesFromRHSet (slot 74).
// Retail-asm: 0x007E2ECE `fistp dword ptr [ebp-4]` (block `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp ; fild ; fsub [ebp-8] ; fstp ; mov eax ;
//   cmp eax ; jne ; dec dword ptr [ebp-4]`) -- the engine's inline-assembly ceiling GFFloatToLongCeil (shared engine/GFFloatToLongCeil.h;
//   out-of-line twin 0x004FA9A0); VC7.1 emits no bare dword fistp from C++ (`(long)x` = __ftol2, /QIfist = `fistp qword`; per-site
//   attempt table rebuild/candidates/manual/pending/2026-11-04_linux_hand747/ATTEMPTS_TABLE.md row 0x007E2DF0). This row carries no
//   asm of its own.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCWOBBLE_PASS22
#define FABLE_RETAIL_CTCWOBBLE_ONINITIALACTIVATE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCWobble.h"

void CTCWobble::OnInitialActivate()
{
    AngleOfWobble = 0.004f;
    ParentRHSet = ParentThing->GetPhysicsTC()->GetRHSet();
    CRightHandedSet new_rhset = ParentRHSet;
    C3DVector up = new_rhset.GetUp();
    up.Rotate(new_rhset.GetForward(), AngleOfWobble * 0.5f);
    new_rhset.SetUp(up);
    ParentThing->GetPhysicsTC()->SetRHSet(new_rhset);
    DirectionLastRotated = 1.0f;
    FramesOfWobbleRemaining = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * 0.5f);
}