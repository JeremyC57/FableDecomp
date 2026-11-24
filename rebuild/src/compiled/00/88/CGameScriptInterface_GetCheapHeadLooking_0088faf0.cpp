// Leaf: ?GetCheapHeadLooking@CGameScriptInterface@@UAE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_GetCheapHeadLooking@@YI_NPBUCGameScriptInterface@@@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
extern unsigned char g_CheapHeadLooking;

class CGameScriptInterface;

bool CGameScriptInterface::GetCheapHeadLooking()
{
    bool r;
    *(unsigned char*)&r = g_CheapHeadLooking;
    return r;
}