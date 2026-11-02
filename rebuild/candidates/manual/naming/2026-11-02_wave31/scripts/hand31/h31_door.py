# h31_door.py: naming wave 31, family door: engine/CTCDoor.h gains a retail opt-in branch (FABLE_RETAIL_CTCDOOR: CTCDoor : CTCBase on the
#  family-tcb retail CTCBase, Ego_r.pdb members, PDef a CDefPointer<const CDoorDef>); the flattened default is unchanged. Then
#  CTCDoor::Copy 0x007305A0 (c702 queue: Ego_r.pdb ?Copy@CTCDoor@@UAEXABVCTCBase@@@Z, slot 12 of 0x01266DE4; was the invented non-virtual
#  CTCDoor_Methods::Copy under the bare Leaf "Copy") and the implicit ~CTCDoor 0x00731330 (queue row 25, a ~CTCBase binder: FableWin.pdb
#  ??1CTCDoor@@UAE@XZ; the slot-0 ??_G 0x00731310 of 0x01266DE4 calls it; was CSharedOwner_00731330 over CBase_00686830).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REL = 'rebuild/include/engine/CTCDoor.h'
dst = os.path.join(W, os.environ.get('HE_OV', 'ov_hand'), REL)
new = open(os.path.join(W, 'hdr', 'CTCDoor.h'), encoding='utf-8', newline='').read()
if os.path.exists(dst) and open(dst, encoding='utf-8', newline='').read() == new: print('already', REL)
else:
    os.makedirs(os.path.dirname(dst), exist_ok=True); open(dst, 'w', encoding='utf-8', newline='').write(new); print('ok', REL)
a = '007305a0'
he.edit(a, 'src', '// Leaf: Copy\n', '// Leaf: ?Copy@CTCDoor@@UAEXABVCTCBase@@@Z\n')
he.edit(a, 'src', '''#include "engine/CTCDoor.h"

class CTCBase;                                             // the flat engine/CTCDoor.h model does not derive from it

struct CTCDoor_Methods : CTCDoor {
    void Copy(const CTCBase& other);
};

void CTCDoor_Methods::Copy(const CTCBase& other)
{
    const CTCDoor& door = reinterpret_cast<const CTCDoor&>(other);''', '''#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCDOOR
#include "engine/CTCDoor.h"                              // retail branch: CTCDoor : CTCBase, vtable 0x01266DE4

void CTCDoor::Copy(const CTCBase& other)
{
    const CTCDoor& door = static_cast<const CTCDoor&>(other);''')
note(a, 'door', 'the own row is the retail virtual ?Copy@CTCDoor@@UAEXABVCTCBase@@@Z (Ego_r.pdb; slot 12 = CTCBase::Copy) on engine/CTCDoor.h\'s\n'
     '//   retail branch (was the invented non-virtual CTCDoor_Methods::Copy under the bare Leaf "Copy")')
a = '00731330'
he.put(a, 'src', '''// CTCDoor::~CTCDoor() -- the compiler-generated destructor: release the PDef def pointer (+0x20), then ~CTCBase (tail jump)
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
}''')
