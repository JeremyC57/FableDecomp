// Leaf: ?GetTradingPriceMult@CGameScriptInterface@@UAEMXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_GetTradingPriceMult@@YIMPBUCGameScriptInterface@@@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
class CGameScriptInterface;
extern float g_TradingPriceMult;

float CGameScriptInterface::GetTradingPriceMult()
{
    return g_TradingPriceMult;
}