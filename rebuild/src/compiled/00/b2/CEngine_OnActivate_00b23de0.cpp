// CEngine::OnActivate() -- marks the engine active (CIEngine::Active, +0x08)
// Leaf: ?OnActivate@CEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23DE0, 5 bytes (`mov byte ptr [ecx+8],1 ; ret`); slot 49 of CEngine vtable 0x012A0F3C (its only reference).
// Names: Ego_r.pdb + FableWin.pdb ?OnActivate@CEngine@@UAEXXZ; engine/CEngine.h OVERRIDES slot 49; the member is CIEngine::Active (+0x08, CIEngine.h).
// NF251 (q12701): was the stand-in `?OnActivate@CEngine_Methods@@QAEXXZ` on a local `struct ..._Methods` derived class (owner rule 2026-11-05 stand-in);
//   now the PDB member on the shared header, the spelling the CEngine vtable emitters 0x00B260B0 / 0x00B261D0 / 0x00B2AB10 bind.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CEngine.h"

void CEngine::OnActivate()
{
    Active = true;
}
