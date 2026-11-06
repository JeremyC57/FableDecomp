// NScript::CQ_AmbushTradersScript::PhaseTwo() -- the Ambush Traders phase 2: wait for the hero near the M_AT_ConvoyPhase2Trigger marker, remove it and set ConvoyTrigger2, find a living CREATURE_BANDIT_GRUNT among the hero's followers and have it speak TEXT_QST_003_PHASE2_TRIGGER, then each frame until the quest fails or finishes: on entering Bowerstone Jail activate Q_AmbushTraders_ActivatePhase3 once; on entering the Gibbet Woods recreate every recorded convoy member (def name, seed, health, script name, items) at M_AT_ConvoyTeleport facing M_AT_ConvoyLookAt and clear TeleportInfo
// Grade: DIFFER(2208v2285) -- exit-block residue: retail keeps the pre-loop and loop-bottom IsActiveThreadTerminating exits hot with their inline vector/CScriptThing dtor expansions and the post-loop vector dtor inline, ours moves them cold and calls ~vector; TeleportInfo.size() is inlined in retail
// Next idea: an Inline-split do{}while(0) on the pre-loop terminate check combined with the x4 frame shape; reconstruct the sibling AT rows that share this TU (DoQuest 0x00F0B691 caller) to settle the vector dtor spelling first
// Leaf: ?PhaseTwo@CQ_AmbushTradersScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F0C140, 2285 bytes. Identity: called from CQ_AmbushTradersScript::DoQuest at 0x00F0B691; Ego_r.pdb + FableWin.pdb ?PhaseTwo@CQ_AmbushTradersScript@NScript@@QAEXXZ. Layout: engine/CQ_AmbushTradersScript.h PHASETWO opt-in (TeleportInfo vector<TeleportData>, ConvoyTrigger2, ActivatedPhase3). Callees: vector<TeleportData>::swap 0x00F0CA30 (landed under a wrong name, see NAME_FIXES.tsv), IsDistanceBetweenThingsUnder 0x00CBE2FF.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_PHASETWO
#define FABLE_RETAIL_STLP_VECTOR_SWAP_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"
template<class T> __declspec(noinline) long __fastcall CBasicString<T>::Compare(const T* a,const T* b)
{
    while(*a || *b) {
        if(*a<*b)return -1;
        if(*a>*b)return 1;
        ++a; ++b;
    }
    return 0;
}
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }

void NScript::CQ_AmbushTradersScript::PhaseTwo()
{
    CCharString region;
    CScriptThing trigger = Game->GetThingWithScriptName("M_AT_ConvoyPhase2Trigger");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), trigger, pGlobalScriptDef->AmbushTradersConvoyTriggerProximity)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(trigger, false, true);
    ConvoyTrigger2 = true;
    std::vector<CScriptThing> followers;
    CScriptThing bandit;
    long count = Game->GetFollowingEntityList(Game->GetHero(), followers);
    if (count > 0)
    for (long i = 0; i < count; ++i) {
        if (followers[i].IsAlive() && followers[i].GetDefName() == "CREATURE_BANDIT_GRUNT") {
            if (IsActiveThreadTerminating())
                return;
            bandit = followers[i];
            break;
        }
    }
    if (bandit.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        long conversation = Game->AddNewConversation(bandit, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        {
            CCharString line("TEXT_QST_003_PHASE2_TRIGGER");
            Game->AddLineToConversation(conversation, line, false, bandit, Game->GetHero());
        }
    }
    if (IsActiveThreadTerminating())
        return;
    bool done = false;
    while (!done) {
        if (QuestFinished || QuestFailed) { done = true; continue; }
        if (Game->MsgOnRegionLoaded(region)) {
            if (IsActiveThreadTerminating())
                return;
            if (region == "BowerstoneJail") {
                if (IsActiveThreadTerminating())
                    return;
                if (!ActivatedPhase3) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->ActivateQuest("Q_AmbushTraders_ActivatePhase3");
                    ActivatedPhase3 = true;
                }
            } else if (region == "DesertedFarm") {
                if (IsActiveThreadTerminating())
                    return;
            } else if (region == "GibbetWoods") {
                if (IsActiveThreadTerminating())
                    return;
                long teleport_count = TeleportInfo.size();
                for (long t = 0; t < teleport_count; ++t) {
                    TeleportData data = TeleportInfo[t];
                    CCharString def_name = data.defName;
                    unsigned long seed = data.appearanceSeed;
                    float health = data.health;
                    CCharString script_name = data.scriptName;
                    CScriptThing look_at = Game->GetThingWithScriptName("M_AT_ConvoyLookAt");
                    CScriptThing teleport = Game->GetThingWithScriptName("M_AT_ConvoyTeleport");
                    C3DVector pos = teleport.GetPos();
                    CScriptThing creature = Game->CreateCreatureNearby(def_name, pos, 10.0f, script_name, false);
                    long item_count = data.itemsCarried.size();
                    for (long j = 0; j < item_count; ++j)
                        Game->AddItemToContainer(creature, data.itemsCarried[j]);
                    Game->EntitySetAppearanceSeed(creature, seed);
                    Game->ModifyThingHealth(creature, health - Game->GetHealth(creature), false);
                    Game->EntitySetFacingAngleTowardsThing(creature, look_at, true);
                    Game->EntityAttachToScript(creature, "Q_AmbushTraders");
                }
                std::vector<TeleportData> empty;
                TeleportInfo.swap(empty);
            }
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
}
