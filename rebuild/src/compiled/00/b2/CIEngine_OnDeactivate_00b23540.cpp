// CIEngine::OnDeactivate() -- marks the engine inactive (Active, +0x08)
// Leaf: ?OnDeactivate@CIEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23540, 5 bytes (`mov byte ptr [ecx+8],0 ; ret`); slot 50 of CIEngine vtable 0x012A0D24 and of CEngine vtable 0x012A0F3C.
// Names: Ego_r.pdb + FableWin.pdb ?OnDeactivate@CIEngine@@UAEXXZ; engine/CIEngine.h slot 50.
// NF251 (q12701): was the stand-in `?OnDeactivate@CIEngine_Methods@@QAEXXZ` on a local `struct ..._Methods` derived class (owner rule 2026-11-05 stand-in);
//   now the PDB member on the shared header, the spelling the CEngine vtable emitters 0x00B260B0 / 0x00B261D0 / 0x00B2AB10 bind.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

void CIEngine::OnDeactivate()
{
    Active = false;
}
