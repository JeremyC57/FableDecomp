// CTCTavernGame::OnInitialActivate() -- reset the game state, drop the game master and load the three fonts the tavern-game def names
// Leaf: ?OnInitialActivate@CTCTavernGame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EA650, 654 bytes, plain `ret`. Vtable 0x012560BC slot 6; Ego_r.pdb / FableWin.pdb
//   ?OnInitialActivate@CTCTavernGame@@UAEXXZ. The derived games call it non-virtually (landed CTCTavernGameCardBase 0x008F1250,
//   CTCTavernGameSpotTheAddition 0x007D83C0).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (the
//   CIntelligentPointer<CThingAICreature> temporary, vptr 0x0123E7E0), CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (through
//   the inline GetDef), CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828 (the inline CDefString conversion),
//   CFontManager::GetFont 0x009E2C80 on PFontManager 0x013B838C, ~CCharString 0x0099EAE0, operator delete 0x00BFE9BC (the
//   count blocks of the inline CCountedPointer<CFontBank> assignments / temporaries); the def's release (slot 1).
// Shape: one full expression per font (`PScoreFont = PFontManager->GetFont(def->ScoreFont);`): the CCharString temporary lives
//   until after the counted-pointer temporary's release; the def's release has no null test (the def was dereferenced).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTavernGameDef.h"
#include "engine/CDefinitionManager.h"
// GetSubDef<CTavernGameDef> 0x005ED4A0 (landed, same body; the landed FrameUpdate 0x005EB9E0 defines it the same way): retail's TU
// holds it, so VC7.1 sees the def slot is not captured -- the def stays known non-null after `def->ScoreFont` and its release has
// no null test (0x005EA8B8..0x005EA8D7).
template <>
bool CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#include "engine/CTCTavernGame.h"
#include "engine/CThingAICreature.h"
#include "engine/CFontManager.h"

void CTCTavernGame::OnInitialActivate()
{
    IsBeingPlayed = false;
    TavernGameState = ETGS_NONE;
    PGameMaster = CIntelligentPointer<CThingAICreature>(0);
    GameInSession = false;
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    BaseBet = 0;
    PScoreFont = PFontManager->GetFont(def->ScoreFont);
    PTargetFont = PFontManager->GetFont(def->TargetFont);
    PStatsFont = PFontManager->GetFont(def->StatsFont);
    HasPrizeBeenGiven = false;
    Draw = false;
}