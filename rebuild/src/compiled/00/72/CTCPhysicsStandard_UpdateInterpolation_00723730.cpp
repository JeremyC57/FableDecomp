// CTCPhysicsStandard::UpdateInterpolation() -- snapshot the right-handed set as the old one (virtual UpdateOldRHSet), then the base
// class's interpolation update
// Leaf: ?UpdateInterpolation@CTCPhysicsStandard@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723730, oracle length 19 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi] ; call [eax+17Ch] ; mov ecx,esi ; pop esi ; jmp 0x006AF780
// Identity (NF154, PRIORITY q3906 / q5088; was the stand-in ?Initialise@CCreatureAction_TrollWhackGroundBase_Methods@@QAEXXZ calling
//   ?InitialiseBase@CCreatureAction_TrollWhackGroundBase_Methods@@QAEXXZ): (1) the only reference is slot 94 of the CTCPhysicsStandard
//   vtable 0x01265F84 (RTTI .?AVCTCPhysicsStandard@@ : CTCPhysicsBase); slot 94 of the CTCPhysicsBase vtable 0x0125D2C4 is 0x006AF780
//   (the tail-jump target) and slot 95 is UpdateOldRHSet (0x00723930 here, 0x006B13C0 in the base: the `call [eax+17Ch]`);
//   (2) Ego_r.pdb 0x0073A570 / FableWin.pdb 0x02526850 ?UpdateInterpolation@CTCPhysicsStandard@@MAEXXZ (protected virtual) over
//   ?UpdateInterpolation@CTCPhysicsBase@@MAEXXZ (Ego_r 0x006EC3C0 / FableWin 0x0246F980), the same body shape as the landed
//   CTCPhysicsRigidBody::UpdateInterpolation 0x00759970. The catalog name already reads CTCPhysicsStandard::UpdateInterpolation.
// Callees: CTCPhysicsStandard::UpdateOldRHSet (virtual slot 95), CTCPhysicsBase::UpdateInterpolation() 0x006AF780 (qualified base
//   call = direct tail jump).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_UPDATEINTERPOLATION
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"

void CTCPhysicsStandard::UpdateInterpolation()
{
    UpdateOldRHSet();
    CTCPhysicsBase::UpdateInterpolation();
}
