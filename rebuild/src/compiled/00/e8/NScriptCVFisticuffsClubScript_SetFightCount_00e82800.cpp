// CV_FisticuffsClubScript::SetFightCount() -- sets the number of fights for the club of the current region from the script definition (Bowerstone also for an unknown region)
// Leaf: ?SetFightCount@CV_FisticuffsClubScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E82800, length 104 bytes (jump table at 0x00E82858 inside the row), `ret`.
// Data: the script definition @ 0x0143E90C (fight counts at +0x600 / +0x604 / +0x608 / +0x60c for regions 1..4).
// Layout (retail = Ego_r.pdb CV_FisticuffsClubScript - 0x14): RelevantRegion +0x48, FightCount +0x88.
// Shape: the region-1 case is the default arm's body (merged by VC7.1, laid out last); the cases follow the source
// order 1, 4, 2, 3.
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
    void SetFightCount();
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;
void NScript::CV_FisticuffsClubScript::SetFightCount()
{
    switch (RelevantRegion)
    {
    case 1:
        FightCount = pGlobalScriptDef->FC_BowerstoneFightCount;
        return;
    case 4:
        FightCount = pGlobalScriptDef->FC_KnotholeGladeFightCount;
        return;
    case 2:
        FightCount = pGlobalScriptDef->FC_OakvaleFightCount;
        return;
    case 3:
        FightCount = pGlobalScriptDef->FC_BanditCampFightCount;
        return;
    }
    FightCount = pGlobalScriptDef->FC_BowerstoneFightCount;
}