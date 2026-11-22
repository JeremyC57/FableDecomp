// CEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const -- constant-return override: returns false
// Leaf: ?IsStaticMapNeedingUpdate@CEngine@@UBE_NJABVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23D90, 5 bytes (`xor al,al ; ret 0Ch`); slot 39 of CEngine vtable 0x012A0F3C (its only reference).
// Names: Ego_r.pdb + FableWin.pdb ?IsStaticMapNeedingUpdate@CEngine@@UBE_NJABVCCharString@@K@Z; engine/CEngine.h OVERRIDES slot 39.
// NF251 (q12701): was the stand-in `?IsStaticMapNeedingUpdate@CEngine_Methods@@QAE_NHHH@Z` on a local `struct ..._Methods` derived class (owner rule 2026-11-05 stand-in);
//   now the PDB member on the shared header, the spelling the CEngine vtable emitters 0x00B260B0 / 0x00B261D0 / 0x00B2AB10 bind.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CEngine.h"

bool CEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const
{
    return false;
}
