// CIEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const -- default implementation: returns false
// Leaf: ?IsStaticMapNeedingUpdate@CIEngine@@UBE_NJABVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23490, 5 bytes (`xor al,al ; ret 0Ch`); slot 39 of CIEngine vtable 0x012A0D24 (its only reference).
// Names: Ego_r.pdb + FableWin.pdb ?IsStaticMapNeedingUpdate@CIEngine@@UBE_NJABVCCharString@@K@Z; engine/CIEngine.h slot 39.
// NF251 (q12701 follow-up): was the stand-in `?IsStaticMapNeedingUpdate@CIEngine_Methods@@QAE_NHHH@Z` on a local `struct CIEngine_Methods` derived class (owner rule
//   2026-11-05 stand-in); now the PDB member on the shared header, the spelling the ??_7CIEngine@@6B@ emitters bind (0x00B23750 and the
//   CEngine ctor / dtor rows 0x00B260B0 / 0x00B261D0 / 0x00B2AB10).
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

bool CIEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const
{
    return false;
}
