// CFrontendGameComponent::ChangeTextureColourDepth(long) -- re-run the front end's graphics initialisation (InitGraphics 0x00494840)
// Leaf: ?ChangeTextureColourDepth@CFrontendGameComponent@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00495610, 8 bytes (`call 0x00494840 ; ret 4`). Referenced only from CFrontendGameComponent's
//   vtable 0x01238774 (.rdata 0x01238774).
// Names (NF227, 2026-11-22, q12581): Ego_r.pdb / FableWin.pdb ?ChangeTextureColourDepth@CFrontendGameComponent@@UAEXJ@Z (public virtual;
//   engine/CFrontendGameComponent.h declares it virtual) and the callee ?InitGraphics@CFrontendGameComponent@@AAE_NXZ (private; both
//   PDBs; self row 0x00494840 and the ctor 0x00496070 spell it so). Was a row-local stand-in class CFrontendGameComponent_00495610
//   (leaf ?ChangeTextureColourDepth@CFrontendGameComponent_00495610@@QAEXJ@Z, callee ?InitGraphics@CFrontendGameComponent_00495610@@QAE_NXZ).
// Port: onto engine/CFrontendGameComponent.h with FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS (as 0x00494840); the depth
//   argument is unused.
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS
#include "engine/CFrontendGameComponent.h"
void CFrontendGameComponent::ChangeTextureColourDepth(long)
{
    InitGraphics();
}
