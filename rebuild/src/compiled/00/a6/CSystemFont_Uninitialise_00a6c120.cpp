// CSystemFont::Uninitialise() -- chains to the protected CInitBaseClass::Uninitialise (VC7.1 tail-calls it: `jmp 0x0099A360`)
// Leaf: ?Uninitialise@CSystemFont@@QAEXXZ
// Identity: Ego_r.pdb ?Uninitialise@CSystemFont@@QAEXXZ (0x00C060A0, then GetCharacter 0x00C060B0 / Initialise 0x00C060F0) and
//   FableWin.pdb; engine/CSystemFont.h (RTTI .?AVCSystemFont@@ : CInitBaseClass) declares it at retail 0x00A6C120, right before
//   GetCharacter 0x00A6C130 / Initialise 0x00A6C170 as in the donor. Its landed caller ~CRenderManagerCore 0x00A0B060 binds
//   ?Uninitialise@CSystemFont@@QAEXXZ. NF132 (q4019): was the free extern "C" forwarder `_Forward` -> `_ForwardTarget_0099a360`.
#define FABLE_RETAIL_CINITBASECLASS_UNINITIALISE
#include "engine/CSystemFont.h"

void CSystemFont::Uninitialise()
{
    CInitBaseClass::Uninitialise();
}
