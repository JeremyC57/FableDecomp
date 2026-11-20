// CNavigatorManager::FrameUpdate() -- reset the per-frame A* node-examination budget (NumberOfNodeExaminationsLeftThisFrame = 800)
// Leaf: ?FrameUpdate@CNavigatorManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A14E20, length 8 bytes: `mov dword ptr [ecx+20h],320h ; ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?FrameUpdate@CNavigatorManager@@QAEXXZ; CWorld::UpdateFrame 0x004A5A40 calls it on the
//   navigator manager (`mov ecx,[world+48h] ; call 0x00A14E20`). The catalog's CGameEventPackage::Clear label was propagated.
// NF150: re-landed on engine/CNavigatorManager.h (FABLE_RETAIL_CNAVIGATORMANAGER declares FrameUpdate; the layout is unchanged);
//   was a local `struct C` with `C::Clear`.
#define FABLE_RETAIL_CNAVIGATORMANAGER
#include "engine/CNavigatorManager.h"

void CNavigatorManager::FrameUpdate()
{
    NumberOfNodeExaminationsLeftThisFrame = 800;
}
