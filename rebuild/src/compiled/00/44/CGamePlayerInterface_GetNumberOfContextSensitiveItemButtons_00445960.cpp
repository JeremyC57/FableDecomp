// CGamePlayerInterface::GetNumberOfContextSensitiveItemButtons() const -- number of entries in the ContextSensitiveItemButtons button list (8-byte CCountedPointer<CInputTypeBase const> elements)
// Leaf: ?GetNumberOfContextSensitiveItemButtons@CGamePlayerInterface@@QBEJXZ
// Retail: Fable.exe @ 0x00445960, 16 bytes: mov eax,[ecx+0x7d4] ; sub eax,[ecx+0x7d0] ; sar eax,3 ; ret.
// Name fix (NF96, q11465): Ego_r.pdb 0x004431A0 + FableWin.pdb 0x01A9BAB0 ?GetNumberOfContextSensitiveItemButtons@CGamePlayerInterface@@QBEJXZ,
//   the third of the button-count trio (GetNumberOfAssignableSpecialMoveButtons 0x00445940, GetNumberOfQuickAccessItemButtons 0x00445950,
//   this one, in Ego_r / FableWin order); the landed callers 0x00590D32 and 0x0062ABA0 bind it so. The old row defined a free
//   CShaderPreParser_GetConstantCount over a local CShaderPreParser view (catalog CShaderPreParser::GetConstantCount, a propagated label).
// Layout: Ego_r.pdb CGamePlayerInterface: ContextSensitiveItemButtons +0x7d0 (the 12-byte STLport vector), from engine/CGamePlayerInterface.h
//   (CONTEXTSENSITIVEBUTTONS declares the member function; overlay opt-in CONTEXTSENSITIVEITEMBUTTONS_TYPED types the vector).
// Shape: a signed pointer difference (`sar eax,3`), not the unsigned size(), as in 0x00445950.
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_CONTEXTSENSITIVEBUTTONS
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_CONTEXTSENSITIVEITEMBUTTONS_TYPED
#include "engine/CGamePlayerInterface.h"
long CGamePlayerInterface::GetNumberOfContextSensitiveItemButtons() const
{
    return ContextSensitiveItemButtons.end() - ContextSensitiveItemButtons.begin();
}
