// Leaf: ?SetHeroGuideToShowQuestCardsWhenSpokenTo@CGameScriptInterface@@UAEX_N@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT with the PDB parameter / return types (was ?SetHeroGuideToShowQuestCardsWhenSpokenTo@CGameScriptInterface_Methods@@QAEXE@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CWorld { public: char pad[0xda]; unsigned char b_da; };
void CGameScriptInterface::SetHeroGuideToShowQuestCardsWhenSpokenTo(bool v) { this->World->b_da = v; }
