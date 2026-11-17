// CV_FisticuffsClubScript::SetAllowedHits(long hits) -- sets the hits allowed in the coming fight: the region's base allowance minus hits / FightCount scaled by the definition's factor, truncated; an unknown region simply takes the given hits
// Leaf: ?SetAllowedHits@CV_FisticuffsClubScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E8A140, length 116 bytes (jump table at 0x00E8A1B4 inside the row), `ret 4`.
// Callees: __ftol2 @ 0x00BFEA70. Data: the script definition @ 0x0143E90C (allowances +0x5ec / +0x5f0 / +0x5f4 / +0x5f8 for
// regions 1..4, the scale at +0x5fc).
// Layout (retail = Ego_r.pdb CV_FisticuffsClubScript - 0x14): RelevantRegion +0x48, FightCount +0x88, AllowedHits +0x90.
// Shape: every case is the whole statement; VC7.1 cross-jumps the common `fild hits ; fidiv ; fimul ; fsubp ; ftol`
// tail, the cases in source order 1, 4, 2, 3.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: FisticuffsAllowedHitsBowerstone -> FC_BowerstoneAllowedHits (+0x5ec), FisticuffsAllowedHitsOakvale -> FC_OakvaleAllowedHits (+0x5f0), FisticuffsAllowedHitsBanditCamp -> FC_BanditCampAllowedHits (+0x5f4), FisticuffsAllowedHitsKnotholeGlade -> FC_KnotholeGladeAllowedHits (+0x5f8), FisticuffsAllowedHitsScale -> FC_HitsDepletionOverSession (+0x5fc), FisticuffsFightCountBowerstone -> FC_BowerstoneFightCount (+0x600), FisticuffsFightCountOakvale -> FC_OakvaleFightCount (+0x604), FisticuffsFightCountBanditCamp -> FC_BanditCampFightCount (+0x608), FisticuffsFightCountKnotholeGlade -> FC_KnotholeGladeFightCount (+0x60c).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
namespace NScript {
class CV_FisticuffsClubScript {
public:
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    long RelevantRegion;                           // +0x48 (1 Bowerstone, 2 Oakvale, 3 Bandit Camp, 4 Knothole Glade)
    unsigned char _pad4c[0x88 - 0x4c];
    long FightCount;                               // +0x88
    bool EverybodyAttack;                          // +0x8c
    bool LostWhileOutside;                         // +0x8d
    bool FisticuffsActive;                         // +0x8e
    long AllowedHits;                              // +0x90
    void SetAllowedHits(long hits);
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;
void NScript::CV_FisticuffsClubScript::SetAllowedHits(long hits)
{
    switch (RelevantRegion)
    {
    case 1:
        AllowedHits = (long)(pGlobalScriptDef->FC_BowerstoneAllowedHits - (float)hits / FightCount * pGlobalScriptDef->FC_HitsDepletionOverSession);
        return;
    case 4:
        AllowedHits = (long)(pGlobalScriptDef->FC_KnotholeGladeAllowedHits - (float)hits / FightCount * pGlobalScriptDef->FC_HitsDepletionOverSession);
        return;
    case 2:
        AllowedHits = (long)(pGlobalScriptDef->FC_OakvaleAllowedHits - (float)hits / FightCount * pGlobalScriptDef->FC_HitsDepletionOverSession);
        return;
    case 3:
        AllowedHits = (long)(pGlobalScriptDef->FC_BanditCampAllowedHits - (float)hits / FightCount * pGlobalScriptDef->FC_HitsDepletionOverSession);
        return;
    }
    AllowedHits = hits;
}