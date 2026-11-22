// CEngine::ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const -- constant-return override: returns false
// Leaf: ?ConvertScreenPosToWorldPos@CEngine@@UBE_NABVC2DVector@@AAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23CF0, 5 bytes (`xor al,al ; ret 8`); slot 11 of CEngine vtable 0x012A0F3C (its only reference).
// Names: Ego_r.pdb + FableWin.pdb ?ConvertScreenPosToWorldPos@CEngine@@UBE_NABVC2DVector@@AAVC3DVector@@@Z; engine/CEngine.h OVERRIDES slot 11.
// NF251 (q12701): was the stand-in `?ConvertScreenPosToWorldPos@CEngine_Methods@@QAE_NHH@Z` on a local `struct ..._Methods` derived class (owner rule 2026-11-05 stand-in);
//   now the PDB member on the shared header, the spelling the CEngine vtable emitters 0x00B260B0 / 0x00B261D0 / 0x00B2AB10 bind.
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CENGINE
#define FABLE_RETAIL_CENGINE_OVERRIDES
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CEngine.h"

bool CEngine::ConvertScreenPosToWorldPos(const C2DVector& screenPos, C3DVector& worldPos) const
{
    return false;
}
