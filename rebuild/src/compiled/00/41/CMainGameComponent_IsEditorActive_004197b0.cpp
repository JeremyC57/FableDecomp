// CMainGameComponent::IsEditorActive() const -- the game component is never the editor: returns false
// Leaf: ?IsEditorActive@CMainGameComponent@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004197B0, 3 bytes: `xor al,al ; ret`; slot 9 of vtable 0x0122F180 (CThingTrackNode::ConstructFromLevelScript
//   0x00837EED calls [eax+24h]).
// Name (NF278, 2026-11-22): FableWin.pdb 0x018A1210 ?IsEditorActive@CMainGameComponent@@UBE_NXZ (public, const, virtual); both landed vtable
//   emitters (0x00418DCA ctor, 0x00418B7A dtor) bind slot 9 under this spelling. Was the stand-in struct T ?IsEditorActive@T@@QAE_NXZ. On
//   the shared header with the landed declaration opt-in FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR.
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "engine/CMainGameComponent.h"

bool CMainGameComponent::IsEditorActive() const
{
    return false;
}
