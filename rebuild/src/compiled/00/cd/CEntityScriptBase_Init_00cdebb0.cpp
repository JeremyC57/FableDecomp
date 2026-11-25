// CEntityScriptBase::Init() -- the entity-script base's default: does nothing (`ret`)
// Leaf: ?Init@CEntityScriptBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEBB0, `ret`.
// Name (NF434, two sources): slot 2 of the CEntityScriptBase vftable 0x012C3224 (retail; the vftable every entity-script Alloc stores
//   first, e.g. 0x00DC3F05, before the derived class's) and FableWin.pdb ??_7CEntityScriptBase@@6B@ slot 2 = ?Init@CEntityScriptBase@@UAEXXZ
//   (slots 1 and 3 are __purecall: Main / GetParentScript); access and order as engine/CEntityScriptBase.h declares them. The body matches.
// Was: the shared gapscan stand-in ?Hook@T@@QAEXXZ (struct T), which every NScript entity-script vftable slot 2 pointed at.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CEntityScriptBase.h"
void CEntityScriptBase::Init()
{
}
