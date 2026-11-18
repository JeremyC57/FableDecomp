// CTCTrophy::OnTrophyUseAction(CThingCreatureBase&) -- record a trophy, queue its show action or offer the witness minigame.
// Leaf: ?OnTrophyUseAction@CTCTrophy@@QAEXAAVCThingCreatureBase@@@Z
// Retail: 0x006DDD30, 841 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_SHOWTROPHY_USE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDRAWQUESTINFO_MINIGAMETYPE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGIONLOAD
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_SIZE_DECL
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CCreatureAction_ShowTrophy.h"
#include "engine/CTCTrophy.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_AICreatureHasProperty.h"

class CThingFilter_HasScriptSupervision : public std::unary_function<CThing, bool> {
    bool CheckForActive;
public:
    explicit CThingFilter_HasScriptSupervision(bool active) : CheckForActive(active) {}
};
class CThingFilter_IsLyingDown : public std::unary_function<CThing, bool> {};

template<class F> class CFilter_Not {
    const F& Filter;
public:
    explicit CFilter_Not(const F& filter) : Filter(filter) {}
};

template<class A, class B, class C, class D, class E> class CFilter_Bind5 {
    const A& First;
    const B& Second;
    const C& Third;
    const D& Fourth;
    const E& Fifth;
public:
    CFilter_Bind5(const A& a, const B& b, const C& c, const D& d, const E& e)
        : First(a), Second(b), Third(c), Fourth(d), Fifth(e) {}
};

void CTCTrophy::OnTrophyUseAction(CThingCreatureBase& creature_performing_action)
{
    if (!MinigameInProgress) {
        if (CPlayerGui::Get().GetQuestInfo().GetMiniGameType())
            return;
        EPlayerMode player_mode = GetPlayerManager().GetMainPlayer().GetCurrentMode();
        if (player_mode != PLAYER_MODE_CONTROL_CREATURE && player_mode != PLAYER_MODE_Z_TARGET
            && player_mode != PLAYER_MODE_QUICK_ACCESS_MENU && player_mode != PLAYER_MODE_LIVE_GUI)
            return;
        if (CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen()) {
            CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCInventoryTrade* tc_inventory;
            if (player && !player->IsDead() && player->GetTC(TCI_INVENTORY_TRADE, &tc_inventory))
                tc_inventory->TrophyUseNeeded = ParentThing->GetDefGlobalIndex();
            return;
        }

        AvailableWitnesses.clear();
        typedef CFilter_Bind5<CThingFilter_HasTC, CThingFilter_HasTC, CThingFilter_HasScriptSupervision,
            CFilter_Not<CThingFilter_AICreatureHasProperty>, CFilter_Not<CThingFilter_IsLyingDown> > WitnessFilter;
        GetWorld().GetThingSearchTools().GetAllThings<CThingAICreature>(
            WitnessFilter(CThingFilter_HasTC((ETCInterfaceType)0x4f), CThingFilter_HasTC(TCI_VILLAGE_MEMBER),
                CThingFilter_HasScriptSupervision(false),
                CFilter_Not<CThingFilter_AICreatureHasProperty>(CThingFilter_AICreatureHasProperty(3)),
                CFilter_Not<CThingFilter_IsLyingDown>(CThingFilter_IsLyingDown())), AvailableWitnesses);
        if (!AvailableWitnesses.empty()) {
            PlaySoundW(PDef->SoundStartGame);
            CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_PLAY_GAME");
            NGameText::ConstructString(text, AvailableWitnesses.size(), BestWitnessesAheadToDate);
            CWideString help(L"TEXT_GUI_HELP");
            StartDialogBox(text, 4, CWideString(L""), CWideString(L""), help);
        } else {
            CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_NO_PEOPLE");
            StartDialogBox(text, 10, CWideString(L""), CWideString(L""), CWideString(L""));
        }
    } else {
        CCountedPointer<CCreatureActionBase> paction;
        paction.Reset(new CCreatureAction_ShowTrophy(creature_performing_action, *ParentThing,
            PDef->ShowTrophyAnimName, 0));
        creature_performing_action.SetCurrentAction(paction);
    }
}