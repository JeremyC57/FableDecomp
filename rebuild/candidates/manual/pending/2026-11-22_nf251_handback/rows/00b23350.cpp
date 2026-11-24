// CIEngine::CheckPrimitiveVisibility(unsigned __int64) -- default implementation: every primitive is visible (returns true)
// Leaf: ?CheckPrimitiveVisibility@CIEngine@@UAE_N_K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23350, 5 bytes (`mov al,1 ; ret 8`); slot 18 of CIEngine vtable 0x012A0D24 and of CEngine vtable 0x012A0F3C.
// Names: Ego_r.pdb + FableWin.pdb ?CheckPrimitiveVisibility@CIEngine@@UAE_N_K@Z; engine/CIEngine.h slot 18.
// NF251 (q12701): was the stand-in `?CheckPrimitiveVisibility@CIEngine_Methods@@QAE_NHH@Z` on a local `struct ..._Methods` derived class (owner rule 2026-11-05 stand-in);
//   now the PDB member on the shared header, the spelling the CEngine vtable emitters 0x00B260B0 / 0x00B261D0 / 0x00B2AB10 bind.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

bool CIEngine::CheckPrimitiveVisibility(unsigned __int64)
{
    return true;
}
