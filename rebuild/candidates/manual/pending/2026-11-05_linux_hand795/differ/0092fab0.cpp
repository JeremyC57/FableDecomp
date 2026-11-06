// CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction(bool, CThingPlayerCreature*) -- test (or, with test false, perform) the creature's reaction to the hero: a sober shopkeeper whose shop is open offers trade (reaction 3); otherwise, by reaction slot, gossip (0), the wife's greeting with its yes / no confirmation (1), the opinion greeting (2) or the shopkeeper's hint after a trade (3); then a guard of a village in a region that uses crimes offers the hero a bribe or says he cannot afford one
// Grade: DIFFER(2508v2508) -- 311 diff lines, all frame offsets: size and every instruction match; the residue is the frame: retail sub esp,24h v 1Ch (two more dword slots: the empty L"" CWideString is in-frame at +0x08 where VC7.1 here reuses the player parameter slot, and the case 1/2 key and fps temps get their own slot +0x14 instead of sharing the TEXT_GUI_NO slot)
// Next idea: find what keeps the player parameter slot live across case 1 in retail (it is read raw in case 3): e.g. case 3 written before the others with gotos, or the post-switch code reading the raw parameter; then re-check the temp slots
// Leaf: ?TestOrDoInteraction@CAIStateGroup_MinionReactToPlayerInteraction@@AAE_N_NPAVCThingPlayerCreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092FAB0, 2508 bytes, `ret 8` (last at 0x00930466), then a 3-byte pad and the 4-entry switch
//   table 0x0093046C (case 0 0x0092FBE8, 1 0x0092FCDF, 2 0x0092FF3F, 3 0x0093002C; the switch is at 0x0092FBE1).
// Identity: Ego_r.pdb ?TestOrDoInteraction@CAIStateGroup_MinionReactToPlayerInteraction@@AAE_N_NPAVCThingPlayerCreature@@@Z (private);
//   called from the landed IsRunnable 0x00930A70.
// Callees: CBaseIntelligentPointer 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ~ 0x00A01C10; CVectorMap LowerBound
//   0x0040F020 (GetTC) / const 0x004365B0 (PeekTC); CTCShopKeeper::GetShopTC 0x00798F50 / GetHint 0x00798D30; CTCShop::
//   IsShopKeeperAvailable 0x00773AA0; CAIStateGroupBase GetPWorldSeed 0x008FCFD0 / GetWorld 0x008FCFC0 / PeekWorldMap 0x008FCFF0 /
//   GetPlayerManager 0x008FCF90 / AIAction_SpeakToTarget 0x00980BD0 / AIAction_SpeakAISpeechToTarget 0x00981EA0 (CCharString) and
//   0x009812C0 (unsigned long) / AIAction_LookAtThing 0x0097D6F0; GFFloatRandom 0x004978A0; CTCWife IsMarriedToThePlayer 0x006CEC80 /
//   GetGreetDialogueGroupName 0x006D0410 / GetGreetConfirmationText 0x006D0590; GetGossipPhrase 0x0092F310 (Ego_r order InteractionFailed
//   0x00952630 < GetGossipPhrase 0x00952710 < TestOrDoInteraction 0x00952D30, as retail 0x0092F240 < 0x0092F310 < 0x0092FAB0);
//   CIntelligentPointer<const CThing>(const CIntelligentPointer<CThingPlayerCreature>&) 0x00930D20; CIntelligentPointer<CThingCreatureBase>
//   (CThingCreatureBase*) 0x00668B60; IS_THING_ALIVE<CIntelligentPointer<CThingPlayerCreature> > 0x0048C740; CWorld::GetFrame 0x0049D870
//   / IsPerformingRegionLoad 0x0048DB13; CThing::GetPos 0x004C73D0; CSubtitleRenderer::SetText 0x00492F69 / 0x00492E4B; CWideString
//   0x0099AED0 / 0x0099B6B0 / ~ 0x0099B510; CCharString 0x0099E4B0 / 0x0099EBF0 / operator= 0x0099EFB0 / ~ 0x0099EAE0;
//   CTCCreatureOpinionOfHero GetGreetDialogueGroupIndex 0x00683C10 / ResetLastOpinionReactionFrame 0x006CEAE0; CTCVillageMember::
//   GetPVillage 0x007EE540; CTCVillage::GetCostToBribe 0x006588CF; CWorldMap GetLoadedRegion 0x004FB150 / GetRegion 0x004FC180;
//   CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0; NGameText::CDataBank::GetTextBySymbol
//   0x009C95E0; NGameText::ConstructString 0x009C7B70; __ftol2 0x00BFEA70.
// Literals: L"TEXT_GUI_NO" 0x01291DB8, L"TEXT_GUI_YES" 0x01291D9C, L"" 0x0122D70C, "" 0x0122D70E, "TEXT_GUI_CRIME_BRIBE_OFFER"
//   0x01291D80, "TEXT_GUI_CRIME_BRIBE_LACK_FUNDS" 0x01291D60; floats 0.2 0x01291D4C, 10.0 0x01291D50, 0.0 0x0122DEDC, 0.0027777.
// Layout: engine/CAIStateGroup_MinionReactToPlayerInteraction.h flattens its base, so the class is local (as the landed siblings):
//   NextReactionFrame[5] +0x14, ReactionType +0x28, WaitingForConfirmation +0x2C, DidTraderInteraction +0x2D,
//   PositionPlayerWasWhenWaitingForConfirmation +0x2E (Ego_r.pdb, packed). CTCDrunkenness::Drunk +0x0C (Ego_r IsDrunk), CTCBoastUI
//   ActivationRequested +0x2A (generated retail layout), CRegionDef +0x9D (descriptive: Ego_r UseCrimes +0xA9).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_BRIBE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_GREET
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOP_AVAILABLE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CDATABANK
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCWife.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCHeroStats.h"
#include "engine/C3DVector.h"
#include "fable_game_text_bank.h"

enum EAIActionReturn { AI_ACTION_RETURN_SUCCESS = 2 };                            // descriptive values
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 1 };                               // descriptive values

float __fastcall GFFloatRandom(float max, unsigned long* seed);                  // 0x004978A0 ?GFFloatRandom@@YIMMPAK@Z
template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);                 // 0x0048C740 (the CIntelligentPointer instantiation)
namespace NGameText { bool __fastcall ConstructString(CWideString& text, long value); }   // 0x009C7B70

// Retail keeps this instantiation's pointer constructor out of line (0x00668B60): a declared-only explicit specialization.
template<> CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase* pointer);

class CTCDrunkenness : public CTCBase { public:                    // TCI_DRUNKENNESS (0xC3)
    bool IsDrunk() const { return Drunk; }                          // Ego_r ?IsDrunk@CTCDrunkenness@@QBE_NXZ
    bool Drunk;                                                     // +0x0C
};
class CTCBoastUI : public CTCBase { public:                        // TCI_BOAST_UI (0xE8)
    unsigned char _pad_0x0c[0x2a - 0x0c];
    bool ActivationRequested;                                       // +0x2A
};
class CRegionDef : public CDefPointeeBase { public:
    unsigned char _pad[0x9d - sizeof(CDefPointeeBase)];
    bool UseCrimes;                                                 // +0x9D (descriptive)
};

#pragma pack(push, 1)
class CAIStateGroup_MinionReactToPlayerInteraction : public CAIStateGroupBase {
public:
    bool GetGossipPhrase(unsigned long& phrase);                    // 0x0092F310
private:
    bool TestOrDoInteraction(bool test, CThingPlayerCreature* player);
    unsigned long NextReactionFrame[5];                             // +0x14
    long          ReactionType;                                     // +0x28 EInteractionReactionType
    bool          WaitingForConfirmation;                           // +0x2c
    bool          DidTraderInteraction;                             // +0x2d
    C3DVector     PositionPlayerWasWhenWaitingForConfirmation;      // +0x2e
};
#pragma pack(pop)

bool CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction(bool test, CThingPlayerCreature* player)
{
    CIntelligentPointer<CThingPlayerCreature> p_player(player);
    if (test && !WaitingForConfirmation) {
        const CTCShopKeeper* shop_keeper;
        if (static_cast<const CThing&>(GetCreature()).PeekTC(TCI_SHOP_KEEPER, &shop_keeper)) {
            const CTCDrunkenness* drunkenness;
            if (!(static_cast<const CThing&>(GetCreature()).PeekTC(TCI_DRUNKENNESS, &drunkenness) && drunkenness->IsDrunk())) {
                if (p_player.Get() && !p_player->IsDead() && p_player->HasTC(TCI_INVENTORY_TRADE)
                        && shop_keeper->GetShopTC() && shop_keeper->GetShopTC()->IsShopKeeperAvailable()) {
                    ReactionType = 3;
                    return true;
                }
            }
        }
    }
    switch (ReactionType) {
    case 0:
        if (!test || GFFloatRandom(1.0f, GetPWorldSeed()) < 0.2f) {
            const CTCWife* wife;
            if (!(static_cast<const CThing&>(GetCreature()).PeekTC(TCI_WIFE, &wife) && wife->IsMarriedToThePlayer())) {
                unsigned long phrase = 0;
                if (GetGossipPhrase(phrase)) {
                    if (test)
                        return true;
                    if (AIAction_SpeakToTarget(AI_ACTION_REPEAT_ONCE, &GetCreature(), p_player, phrase, false, 0x93, false)
                            == AI_ACTION_RETURN_SUCCESS) {
                        NextReactionFrame[0] = (unsigned long)(CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 10.0f);
                        break;
                    }
                }
            }
        }
    case 1:
        if ((unsigned long)CWorld::GetFrame() > NextReactionFrame[1]) {
            ReactionType = 1;
            CTCWife* wife;
            if (GetCreature().GetTC(TCI_WIFE, &wife)) {
                CCharString group_name("");
                if (wife->GetGreetDialogueGroupName(group_name)) {
                    if (test)
                        return true;
                    CWideString confirmation;
                    bool has_confirmation = wife->GetGreetConfirmationText(confirmation);
                    if (AIAction_SpeakAISpeechToTarget(AI_ACTION_REPEAT_ONCE, &GetCreature(), p_player, group_name, false, 0x93)
                            == AI_ACTION_RETURN_SUCCESS) {
                        if (has_confirmation && IS_THING_ALIVE(p_player) && !GetWorld().IsPerformingRegionLoad()
                                && !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
                            const CTCBoastUI* boast_ui;
                            if (!(static_cast<const CThing*>(p_player.Get())->PeekTC(TCI_BOAST_UI, &boast_ui)
                                    && boast_ui->ActivationRequested)) {
                                {
                                    CWideString no_text(L"TEXT_GUI_NO");
                                    CWideString yes_text(L"TEXT_GUI_YES");
                                    CWideString empty_text(L"");
                                    CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(),
                                        confirmation, empty_text, yes_text, no_text, 4, 0.0f, false, true);
                                }
                                WaitingForConfirmation = true;
                                PositionPlayerWasWhenWaitingForConfirmation = p_player->GetPos();
                            }
                        }
                        NextReactionFrame[1] = (unsigned long)(CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 0.0f);
                        break;
                    }
                }
            }
        }
    case 2:
        if (!DidTraderInteraction) {
            if ((unsigned long)CWorld::GetFrame() > NextReactionFrame[2]) {
                ReactionType = 2;
                CTCCreatureOpinionOfHero* opinion;
                if (GetCreature().GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion)) {
                    unsigned long group = opinion->GetGreetDialogueGroupIndex((EOpinionAttitudeType)0x12);
                    if (group > 0) {
                        if (test)
                            return true;
                        if (AIAction_SpeakAISpeechToTarget(AI_ACTION_REPEAT_ONCE, &GetCreature(), p_player, group, false, 3)
                                == AI_ACTION_RETURN_SUCCESS) {
                            opinion->SetHasInteracted();
                            opinion->SetHasGreeted();
                            opinion->ResetLastOpinionReactionFrame();
                            NextReactionFrame[2] = (unsigned long)(CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 0.0f);
                            break;
                        }
                    }
                }
            }
        }
    case 3:
        if (DidTraderInteraction && player && !player->IsDead()) {
            CTCShopKeeper* shop_keeper;
            if (GetCreature().GetTC(TCI_SHOP_KEEPER, &shop_keeper)) {
                CCharString hint;
                hint = shop_keeper->GetHint();
                AIAction_SpeakAISpeechToTarget(AI_ACTION_REPEAT_ONCE, &GetCreature(), p_player, hint, false, 0x93);
            }
        }
    }
    if (!WaitingForConfirmation) {
        CTCVillageMember* member;
        if (GetCreature().HasTC(TCI_GUARD) && GetCreature().GetTC(TCI_VILLAGE_MEMBER, &member) && member->GetPVillage()) {
            CDefPointer<const CRegionDef> region_def = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetPRegionDef();
            if (!region_def.IsNull()) {
                if (region_def->UseCrimes && IS_THING_ALIVE(p_player)) {
                    CTCHeroStats* stats;
                    if (!(p_player->GetTC(TCI_HERO_STATS, &stats) && stats->TrainingStatus != 0)) {
                        if (test)
                            return true;
                        if (AIAction_LookAtThing(AI_ACTION_REPEAT_ONCE, p_player, 0x13, 0.0027777f) == AI_ACTION_RETURN_SUCCESS
                                && !GetWorld().IsPerformingRegionLoad()
                                && !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText() && IS_THING_ALIVE(p_player)) {
                            CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                            CTCVillage* village = member->GetPVillage();
                            CTCHeroStats* hero_stats;
                            long money;
                            if (hero->GetTC(TCI_HERO_STATS, &hero_stats) && (money = hero_stats->Money, money >= village->GetCostToBribe())) {
                                const CTCBoastUI* boast_ui;
                                if (!(static_cast<const CThing*>(p_player.Get())->PeekTC(TCI_BOAST_UI, &boast_ui)
                                        && boast_ui->ActivationRequested)) {
                                    CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_BRIBE_OFFER");
                                    NGameText::ConstructString(text, village->GetCostToBribe());
                                    ReactionType = 4;
                                    CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(),
                                        text, 0x24, 0.0f, false, true);
                                    WaitingForConfirmation = true;
                                }
                            } else {
                                const CTCBoastUI* boast_ui;
                                if (!(static_cast<const CThing*>(p_player.Get())->PeekTC(TCI_BOAST_UI, &boast_ui)
                                        && boast_ui->ActivationRequested)) {
                                    CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_BRIBE_LACK_FUNDS");
                                    ReactionType = 4;
                                    CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(),
                                        text, 0x22, 0.0f, false, true);
                                    WaitingForConfirmation = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}
