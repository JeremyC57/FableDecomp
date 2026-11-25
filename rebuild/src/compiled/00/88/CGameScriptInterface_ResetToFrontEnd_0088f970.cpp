// CGameScriptInterface::ResetToFrontEnd() const -- hands the reset back to the world: World (+0x04)->ResetToFrontEnd()
// Leaf: ?ResetToFrontEnd@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F970, a tail jump to CWorld::ResetToFrontEnd (0x0049DCC0) on World.
// Identity (NF431, 2026-11-25): the retail RTTI vftable placement of CGameScriptInterface (0x01260F0C) names
//   ?ResetToFrontEnd@CGameScriptInterface@@UBEXXZ at this address; Ego_r.pdb and FableWin.pdb both list that symbol. Was the
//   non-virtual ?ResetToFrontEnd@CGameScriptInterface@@QBEXXZ on a local model; now the virtual member of engine/CGameScriptInterface.h.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"

void CGameScriptInterface::ResetToFrontEnd() const
{
    World->ResetToFrontEnd();
}
