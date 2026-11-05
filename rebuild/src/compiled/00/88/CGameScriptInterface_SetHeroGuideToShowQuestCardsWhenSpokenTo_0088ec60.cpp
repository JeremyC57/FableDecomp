// Leaf: ?SetHeroGuideToShowQuestCardsWhenSpokenTo@CGameScriptInterface_Methods@@QAEXE@Z
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: char pad[0xda]; unsigned char b_da; };
struct CGameScriptInterface_Methods : CGameScriptInterface {
    void SetHeroGuideToShowQuestCardsWhenSpokenTo(unsigned char v);
};
void CGameScriptInterface_Methods::SetHeroGuideToShowQuestCardsWhenSpokenTo(unsigned char v){ this->World->b_da = v; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?SetHeroGuideToShowQuestCardsWhenSpokenTo@CGameScriptInterface@@UAEX_N@Z); evidence: RTTI vtable slot + PDB name set.
