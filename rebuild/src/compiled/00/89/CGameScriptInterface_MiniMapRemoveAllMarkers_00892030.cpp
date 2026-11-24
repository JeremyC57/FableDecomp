// Leaf: ?MiniMapRemoveAllMarkers@CGameScriptInterface@@UBEXXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_MiniMapRemoveAllMarkers@@YIXPAX@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
struct CObj; struct CSub;
extern CObj* g_obj;
extern "C" CSub* __fastcall get_sub(CObj* self);
extern "C" void __fastcall sub_method(CSub* self);

void CGameScriptInterface::MiniMapRemoveAllMarkers() const
{
    CSub* s = get_sub(g_obj);
    sub_method(s);
}