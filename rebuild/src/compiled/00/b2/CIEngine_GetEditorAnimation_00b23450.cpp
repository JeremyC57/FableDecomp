// CIEngine::GetEditorAnimation(const CWideString&) const -- default implementation: no editor animation (returns 0)
// Leaf: ?GetEditorAnimation@CIEngine@@UBEPBVC3DAnimationInfo@@ABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B23450, 5 bytes (`xor eax,eax ; ret 4`); slot 35 of CIEngine vtable 0x012A0D24 (its only reference).
// Names: FableWin.pdb + Ego_d.pdb ?GetEditorAnimation@CIEngine@@UBEPBVC3DAnimationInfo@@ABVCWideString@@@Z; engine/CIEngine.h slot 35.
// NF251 (q12701 follow-up): was the stand-in `?GetEditorAnimation@CIEngine_Methods@@QAEHH@Z` on a local `struct CIEngine_Methods` derived class (owner rule
//   2026-11-05 stand-in); now the PDB member on the shared header, the spelling the ??_7CIEngine@@6B@ emitters bind (0x00B23750 and the
//   CEngine ctor / dtor rows 0x00B260B0 / 0x00B261D0 / 0x00B2AB10).
#define FABLE_RETAIL_CINITBASECLASS_VALIDATE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_INHERITED_VALIDATE
#define FABLE_RETAIL_CIENGINE_RENDER
#include "fable_string.h"
#include "engine/CInitBaseClass.h"
#include "engine/CIEngine.h"

const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString&) const
{
    return 0;
}
