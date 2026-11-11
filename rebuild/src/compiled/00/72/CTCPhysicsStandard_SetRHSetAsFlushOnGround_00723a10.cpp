// CTCPhysicsStandard::SetRHSetAsFlushOnGround() -- lay the right-handed set flush on the ground: SetRHSetAsFlushOnNormal with the world map's ground face normal at Position
// Leaf: ?SetRHSetAsFlushOnGround@CTCPhysicsStandard@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723A10, 49 bytes.
// Name (agentNF23, name-fix queue land1011cy; was the free stand-in CTCPhysicsStandard_SetRHSetAsFlushOnGround(self) over
//   ComputeFlushGround / Helper_use): Ego_r.pdb 0x0073A6D0 / FableWin.pdb 0x02527020 ?SetRHSetAsFlushOnGround@CTCPhysicsStandard@@UAEXXZ
//   (the catalog name). Retail 0x00723A16..0x00723A28: `lea eax,[esi+0Ch] ; push eax ; lea ecx,[esp+8] ; push ecx ; mov ecx,esi ;
//   call 0x00686D20 ; mov ecx,eax ; call 0x0063AB10`: 0x00686D20 is the arg-less CTCBase::PeekWorldMap (Ego_r / FableWin
//   ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ), so the two pushes are the hidden result and Position (+0x0C) of
//   CWorldMap::GetGroundFaceNormalAt(const C3DVector&) const 0x0063AB10 (Ego_r 0x006E7420 / FableWin 0x024A4450; `ret 8`); the
//   result goes to the virtual slot 77 (+0x134), CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector&), the slot before
//   SetRHSetAsFlushOnGround in Ego_r's CTCPhysicsBase.
// Headers: engine/CTCPhysicsStandard.h (FABLE_RETAIL_CTCPHYSICSSTANDARD + agentNF23 opt-in _FLUSHONGROUND), engine/CWorldMap.h
//   (FABLE_RETAIL_CWORLDMAP + _GROUNDFACENORMAL).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLUSHONGROUND
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CWorldMap.h"
void CTCPhysicsStandard::SetRHSetAsFlushOnGround()
{
    C3DVector normal = PeekWorldMap().GetGroundFaceNormalAt(Position);
    SetRHSetAsFlushOnNormal(normal);
}
