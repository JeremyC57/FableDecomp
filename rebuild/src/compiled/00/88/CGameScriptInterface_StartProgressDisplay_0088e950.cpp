// Leaf: ?StartProgressDisplay@CGameScriptInterface@@UBEXXZ
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NProgressDisplay { void __fastcall StartProgress(bool, bool, bool, bool, bool, bool); }   // 0x009E9EF0   // 0x009E9EF0
void CGameScriptInterface::StartProgressDisplay() const
{ NProgressDisplay::StartProgress((char)0, (char)0, 0, 1, 0, 0); }
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_StartProgressDisplay -> CGameScriptInterface::StartProgressDisplay, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
