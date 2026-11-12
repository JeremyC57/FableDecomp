// CTCFineDialogue::GlobalUpdate(bool) -- while the pay-fine prompt waits for confirmation, takes the newest yes/no conversation
// answer from the world's message queue; on "yes" it pays the fine (money taken and crimes cleared when the hero remembers crimes),
// adds it to the hero's FinePaid stat and online score, clears the village's security sweeps and, when the hero was trespassing or
// is a murderer, kicks the hero to the last safe region with a fade-out and a subtitle; on any answer the message is removed, the
// wait ends and the component leaves the global update list.
// Leaf: ?GlobalUpdate@CTCFineDialogue@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2A60, 707 bytes, `ret 4`; CTCFineDialogue vtable 0x01270274 slot 26 (Ego_r.pdb /
//   FableWin.pdb ?GlobalUpdate@CTCFineDialogue@@UAEX_N@Z, override of CTCBase::GlobalUpdate).
// Callees: CTCBase::GetWorld @ 0x00686A70, CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> @ 0x004AB8B0
//   (COMDAT, out of line), CWideString() @ 0x0099AED0, CVectorMap::LowerBound @ 0x0040F020 (inside CThing::GetTC),
//   CTCTrespasser::IsCurrentlyTrespassing @ 0x007BAE70, CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//   NGameText::CDataBank::GetTextBySymbol @ 0x009C95E0, CWideString::operator= @ 0x0099B7D0 / ~ @ 0x0099B510,
//   CTCVillage::IsHeroMurderer @ 0x0065A1FD, CTCHeroStats::AddMoney @ 0x0057B327, CTCVillage::ClearCrimes @ 0x0065C2D0,
//   CPlayerManager::GetMainPlayer const @ 0x00449960, CPlayer::GetPControlledCreature @ 0x00487DC0,
//   CTCHeroOnlineScoreboard::UpdateScoreForActivity @ 0x0055F3B0, CTCVillage::EndSecuritySweep @ 0x006590BF
//   (Ego_r.pdb / FableWin.pdb; body `jmp` to the private InvalidateAndClearAllSecuritySweepTasks 0x006590A0), CTCHeroStats::KickHeroToMostRecentSafeRegion @ 0x0057DF85, CDisplayEngine::StartScreenFadeOut @ 0x00434C00,
//   CSubtitleRenderer::SetText @ 0x00492E4B, CMessageEventManager::RemoveMessage @ 0x00493B5B, CTCBase::RemoveFromGlobalTCs @ 0x00686DF0.
// Shape: the FinePaid / TimesThrownOut stats go through the inline Ego_r accessors Get/SetFinePaid, Get/SetTimesThrownOut (the
//   value is re-read into a register and spilled for the fild); the hero-stats and scoreboard components are fetched without a
//   fallback (retail reads the dead `messages` slot when absent); the presence bools are materialised (`const bool&` HasTC, own opt-in FABLE_RETAIL_CTHING_HASTC_MATERIALISED:
//   `and cl,1 ; mov [esp+13h],cl` at 0x007E2AC1 / 0x007E2B9E / 0x007E2C27); the
//   colour is built in its argument slot by CRGBColour's Initialise copy constructor. FindMostRecentMessage is declaration only
//   (retail calls the COMDAT). Layout: Ego_r.pdb CTCFineDialogue (28 bytes) over CTCBase. No pragmas or flags.
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_MATERIALISED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CDISPLAYENGINE_STARTSCREENFADEOUT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_FINEPAID
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CRGBColour.h"
#include "engine/CWorld.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CDisplayEngine.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCVillage.h"

class CTCTrespasser : public CTCBase {
public:
    bool IsCurrentlyTrespassing();                      // 0x007BAE70
};
class CTCHeroOnlineScoreboard : public CTCBase {
public:
    enum EOnlineScoreboardActivity { };
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float value, bool flag);   // 0x0055F3B0
};

class CTCFineDialogue : public CTCBase {
public:
    virtual void GlobalUpdate(bool paused);
private:
    long        Fine;                              // +0x0c
    long        Money;                             // +0x10
    bool        GivePlayerChanceToPayFine;         // +0x14
    bool        WaitingForConfirmation;            // +0x15
    CTCVillage* Village;                           // +0x18
};

void CTCFineDialogue::GlobalUpdate(bool paused)
{
    if (!WaitingForConfirmation)
        return;
    CMessageEventManager& messages = GetWorld().GetMessageEventManager();
    const CMessageEvent* message = messages.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
    if (!message)
        return;
    if (message->ExtraData == 1) {
        bool kicked_out = false;
        CWideString text;
        CTCTrespasser* trespasser;
        if (ParentThing->GetTC(TCI_TRESPASSER, &trespasser) && trespasser->IsCurrentlyTrespassing()) {
            kicked_out = true;
            text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_KICKED_OUT_TRESPASS");
        }
        if (Village->IsHeroMurderer()) {
            kicked_out = true;
            text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_KICKED_OUT_MURDER");
        }
        CThing* thing = ParentThing;
        CTCHeroStats* stats;
        thing->GetTC(TCI_HERO_STATS, &stats);
        if (thing->HasTC(TCI_HERO_REMEMBER_CRIMES)) {
            stats->AddMoney(-Fine, true);
            Village->ClearCrimes();
        }
        GivePlayerChanceToPayFine = true;
        stats->SetFinePaid(stats->GetFinePaid() + Fine);
        CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard);
        scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0x24, (float)stats->GetFinePaid(), false);
        Village->EndSecuritySweep();
        if (kicked_out) {
            stats->KickHeroToMostRecentSafeRegion();
            stats->SetTimesThrownOut(stats->GetTimesThrownOut() + 1);
            CMainGameComponent::Get()->GetDisplayEngine().StartScreenFadeOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255), -1.0f);
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), text, 0xa, 0.0f, false, true);
        }
    } else {
        GivePlayerChanceToPayFine = false;
    }
    messages.RemoveMessage(message);
    WaitingForConfirmation = false;
    RemoveFromGlobalTCs();
}