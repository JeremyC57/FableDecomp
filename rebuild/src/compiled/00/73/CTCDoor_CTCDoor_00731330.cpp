// CTCDoor::~CTCDoor() -- the compiler-generated destructor: release the PDef def pointer (+0x20), then ~CTCBase (tail jump)
// Leaf: ??1CTCDoor@@UAE@XZ
// Naming wave 31 (family door): FableWin.pdb ??1CTCDoor@@UAE@XZ; the slot-0 ??_G 0x00731310 of vtable 0x01266DE4 (RTTI .?AVCTCDoor@@ : CTCBase)
//   calls it. Was "_unattributed::~CSharedOwner_00731330" over the invented non-virtual base CBase_00686830 (??1CBase_00686830@@QAE@XZ bound
//   to 0x00686830): the class is engine/CTCDoor.h's retail branch on engine/CTCBase.h's, so the tail is ??1CTCBase@@UAE@XZ 0x00686830.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00731330, 35 bytes, thiscall, tail `jmp ~CTCBase`.
// Layout: Ego_r.pdb CTCDoor: PDef CDefPointer<const CDoorDef> +0x20; the other members are trivially destructible.
// Shape: no vptr store -- the class declares no destructor (the implicit ??1, as the landed ~CTCLightningOrb 0x004DCEDD); the def pointer
//   is engine/CDefPointer.h's (forced-inline release). DestroyOne (a destruction by name, ungraded) makes VC7.1 emit the out-of-line
//   implicit ??1 (chunk 656/657; wave 30 0x00A9D3A0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCDOOR
#include "engine/CTCDoor.h"                              // retail branch: CTCDoor : CTCBase, vtable 0x01266DE4

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit ??1.
void DestroyOne(CTCDoor* object)
{
    object->CTCDoor::~CTCDoor();
}