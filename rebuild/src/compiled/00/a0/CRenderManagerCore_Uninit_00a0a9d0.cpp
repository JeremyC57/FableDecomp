// CRenderManagerCore::Uninit() -- tear the system font down if it was initialised (tail jump to CSystemFont::Uninitialise)
// Leaf: ?Uninit@CRenderManagerCore@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A9D0, oracle length 28 bytes.
// Retail bytes:  push esi ; lea esi,[ecx+3A4Ch] ; mov ecx,esi ; call <IsValid> ; test al,al ; je R ; mov ecx,esi ; pop esi ; jmp <Uninitialise> ; R: pop esi ; ret
// Identity: Ego_r.pdb ?Uninit@CRenderManagerCore@@QAEXXZ (0x00B73F80) and engine/CRenderManagerCore.h; its landed caller 0x009BF500
//   binds ?Uninit@CRenderManagerCore@@QAEXXZ. SystemFont (+0x3A4C) is the CSystemFont of engine/CSystemFont.h (overlay
//   FABLE_RETAIL_CRENDERMANAGERCORE_SYSTEMFONT, as the landed ~CRenderManagerCore 0x00A0B060): IsValid 0x0099A330 is the
//   CInitBaseClass query, Uninitialise 0x00A6C120 is ?Uninitialise@CSystemFont@@QAEXXZ.
// NF132 (q4019): moved off the local CInitBaseClass / CInitBaseClassCalls / CRenderManagerCore_Methods stand-ins onto the shared headers.
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_SYSTEMFONT
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::Uninit()
{
    if (SystemFont.IsValid())
        SystemFont.Uninitialise();
}
