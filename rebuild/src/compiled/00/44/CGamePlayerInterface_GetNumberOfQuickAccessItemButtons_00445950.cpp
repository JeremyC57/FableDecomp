// CGamePlayerInterface::GetNumberOfQuickAccessItemButtons() const -- number of entries in the QuickAccessItemButtons button list (8-byte CCountedPointer<CInputTypeBase const> elements)
// Leaf: ?GetNumberOfQuickAccessItemButtons@CGamePlayerInterface@@QBEJXZ
// Retail: Fable.exe @ 0x00445950, 16 bytes. Catalog label CShaderPreParser::GetConstantCount was wrong (shared by 0x00445940 and
// 0x00445950). Called by CTCInventory 0x00590E4F, NPlayerGui::CDrawQuickAccessIconsPC 0x006575FE and 0x00581A01 / 0x00581FE3.
// Ego_r.pdb CGamePlayerInterface: the three STLport (12-byte) vectors AssignableSpecialMoveButtons +0x7b8, QuickAccessItemButtons +0x7c4,
// ContextSensitiveItemButtons +0x7d0 (PDB +0x7bc/+0x7cc/+0x7dc with 16-byte vectors).
// Shape: a signed pointer difference (`sar eax,3`), not the unsigned size().
// Name fix (agentNF94, q11291 / q11328): Ego_r.pdb 0x00443080 + FableWin.pdb ?GetNumberOfQuickAccessItemButtons@CGamePlayerInterface@@QBEJXZ
//   (directly after GetNumberOfAssignableSpecialMoveButtons, as retail 0x00445950 after 0x00445940; the landed 0x00657580 binds it so). The
//   old row defined GetNumQuickAccessItemButtons on a local CGamePlayerInterface_Retail view; it is now on engine/CGamePlayerInterface.h
//   (QUICKACCESSBUTTONS declares it; overlay opt-in QUICKACCESSITEMBUTTONS_TYPED types the vector).
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESSBUTTONS
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESSITEMBUTTONS_TYPED
#include "engine/CGamePlayerInterface.h"
long CGamePlayerInterface::GetNumberOfQuickAccessItemButtons() const
{
    return QuickAccessItemButtons.end() - QuickAccessItemButtons.begin();
}
