// CScriptBase::OnPersist(CPersistContext&) -- the script base's default: does nothing (`ret 4`)
// Leaf: ?OnPersist@CScriptBase@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD4E0, `ret 4`.
// Name (NF432, two sources): slot 4 of the CScriptBase vftable 0x012C1648 (retail; slot 0 0x00CBD4F0 is the deleting destructor) and
//   FableWin.pdb ??_7CScriptBase@@6B@ slot 4 = ?OnPersist@CScriptBase@@UAEXAAVCPersistContext@@@Z (Ego_r.pdb and FableWin.pdb name sets; access as engine/CScriptBase.h declares it).
//   The body matches: the one-argument slot (`ret 4`).
// Was: the shared gapscan stand-in ?Hook@T@@QAEXH@Z (struct T), which every NScript script vftable slot 4 pointed at.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
void CScriptBase::OnPersist(CPersistContext& context)
{
}
