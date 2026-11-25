// CScriptBase::RegisterMain() -- the script base's default: does nothing (`ret`)
// Leaf: ?RegisterMain@CScriptBase@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD4B0, `ret`.
// Name (NF432, two sources): slot 1 of the CScriptBase vftable 0x012C1648 (retail; slot 0 0x00CBD4F0 is the deleting destructor) and
//   FableWin.pdb ??_7CScriptBase@@6B@ slot 1 = ?RegisterMain@CScriptBase@@MAEXXZ (Ego_r.pdb and FableWin.pdb name sets; access as engine/CScriptBase.h declares it).
//   The body matches: an empty void member.
//   0x012C1648 is CScriptBase's vftable: the landed ctor 0x00CB8110 (Leaf ??0CScriptBase@@QAE@XZ, Ego_r / FableWin) stores it (`mov [esi],12C1648h`).
// Was: the shared gapscan stand-in ?Hook@T@@QAEXXZ (struct T), which every NScript script vftable slot 1 pointed at.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
void CScriptBase::RegisterMain()
{
}
