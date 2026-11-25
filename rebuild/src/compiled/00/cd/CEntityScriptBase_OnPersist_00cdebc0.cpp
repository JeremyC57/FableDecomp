// CEntityScriptBase::OnPersist(CPersistContext&) -- the entity-script base's default: does nothing (`ret 4`)
// Leaf: ?OnPersist@CEntityScriptBase@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEBC0, `ret 4`.
// Name (NF434, two sources): slot 4 of the CEntityScriptBase vftable 0x012C3224 (retail; the vftable every entity-script Alloc stores
//   first, e.g. 0x00DC3F05, before the derived class's) and FableWin.pdb ??_7CEntityScriptBase@@6B@ slot 4 = ?OnPersist@CEntityScriptBase@@UAEXAAVCPersistContext@@@Z
//   (slots 1 and 3 are __purecall: Main / GetParentScript); access and order as engine/CEntityScriptBase.h declares them. The body matches.
// Was: the shared gapscan stand-in ?Hook@T@@QAEXH@Z (struct T), which every NScript entity-script vftable slot 4 pointed at.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CEntityScriptBase.h"
void CEntityScriptBase::OnPersist(CPersistContext& context)
{
}
