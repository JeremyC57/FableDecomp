// NScript::CQ_TraderEscortScript::MakeTraderComment(const CCharString&, const CScriptThing&, ETraderComment) -- a Darkwood trader comment: pick a live trader (else the nearest DarkwoodTrader to the hero); for the attacked speech find the nearest / furthest other trader (swap unless the nearest is SCARED), roar the rock troll for ROCK_TROLL_CLOSE, and queue the TEXT_QST_067_<trader>_<comment> conversation with the intro responses; otherwise, once the comment timer has run out, queue the single comment and (MULTIPLE) the nearest other trader's _RESPONSE line if the text exists; rearm the comment timer
// Grade: DIFFER(2641v2697) -- 110 lines: retail expands CScriptThing::operator= (the ShareData body) at the first `trader = GetNearestWithScriptName(...)` and pushes edi in the prologue; VC7.1 calls 0x008AB980 there (plain inline, implicit or CCountedPointer::operator= alone never expand, even in a tiny body; __forceinline expands every site); plus the attacked-branch three-dtor return pad is laid out at the else-branch test instead of the first test
// Next idea: find the construct retail used for the first assignment (a member or helper whose body is the share, e.g. an inline CScriptThing setter on PImp) so only that site expands, then recheck the pad placement
// Leaf: ?MakeTraderComment@CQ_TraderEscortScript@NScript@@QAE_NABVCCharString@@ABVCScriptThing@@W4ETraderComment@12@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E01900, 2697 bytes, thiscall `ret 0Ch`, frame 0x40.
// Identity: Ego_r.pdb / FableWin.pdb name set (engine/NScript_CQ_TraderEscortScript.h declares it; callers 0x00E06050
//   WatchForSurprisingBalverines, 0x00E0A820 TurnToBalv, 0x00E05360 CTraderComment::Main pass the comment, a trader and the type).
// Callees: IsActiveThreadTerminating 0x00CB7940, CScriptThing copy 0x004ABE90 / operator= 0x008AB980 / () 0x006E7B40 / ~ 0x004AA840 /
//   IsAlive 0x004AB130 (slot 75 through the vtable on the copied parameter) / GetDataString 0x004AA900, CCharString 0x0099EBF0 /
//   copy 0x0099EC30 / ~ 0x0099EAE0 / != 0x0099E960, operator+ 0x0099F690 / 0x0099F600 / 0x0099F570, CBasicString::Compare 0x00411570;
//   interface slots 70 GetHero, 72 GetThingWithScriptName, 77 / 78 GetNearest / GetFurthestWithScriptName, 89 / 90 Set / GetTimer
//   (pDSTGame), 358 TextEntryExists, 364-366 conversation, 376 Pause, 689 PlayCriteriaSoundOnThing.
// Data: CommentTimer +0x48 rearmed from pGlobalScriptDef->TE_TraderCommentDelay (+0xDF0); strings "DarkwoodTrader", "SCARED",
//   "ROCK_TROLL_CLOSE", "RockTrollTrigger", "EARTH_TROLL_OFFSCREEN_ROAR", "TEXT_QST_067_", "_", "_INTRO_RESPONSE_NO/_OATH/_BAD_NEWS/
//   _REWARD", "_RESPONSE". The flag dword at [esp+4] is the conditional-temporary mask of the TextEntryExists condition.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/NScript_CQ_TraderEscortScript.h"
#include "engine/fable_script_def_global.h"
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing::~CScriptThing() { }
inline CCharString CScriptThing::GetDataString() const { if(PImp.IsNull())return NullString;return PImp->GetDataString(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
inline bool CScriptThing::operator!=(const CScriptThing& other) const { return !(*this == other); }
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
static inline bool IsThingAlive(const CScriptThing& thing) { return thing.IsAlive(); }

bool NScript::CQ_TraderEscortScript::MakeTraderComment(const CCharString& comment, const CScriptThing& thing, ETraderComment type)
{
    CScriptThing trader = thing;
    if (!IsThingAlive(trader)) {
        if (IsActiveThreadTerminating()) return false;
        trader = Game->GetNearestWithScriptName(Game->GetHero(), "DarkwoodTrader");
    }
    if (!IsThingAlive(trader)) {
        if (IsActiveThreadTerminating()) return false;
        return false;
    }
    if (type == TRADER_COMMENT_ATTACKED_SPEECH) {
        if (IsActiveThreadTerminating()) return false;
        CScriptThing nearest = Game->GetNearestWithScriptName(trader, "DarkwoodTrader");
        CScriptThing furthest = Game->GetFurthestWithScriptName(trader, "DarkwoodTrader");
        if (nearest == furthest) {
            if (IsActiveThreadTerminating()) return false;
            furthest = CScriptThing();
        }
        if (nearest.GetDataString() != "SCARED") {
            if (IsActiveThreadTerminating()) return false;
            CScriptThing swap(nearest);
            nearest = furthest;
            furthest = swap;
        }
        if (comment == "ROCK_TROLL_CLOSE") {
            if (IsActiveThreadTerminating()) return false;
            Game->PlayCriteriaSoundOnThing(Game->GetThingWithScriptName("RockTrollTrigger"), "EARTH_TROLL_OFFSCREEN_ROAR");
            Game->Pause(1.0f);
        }
        long conversation = Game->AddNewConversation(thing, false, false);
        Game->AddPersonToConversation(conversation, Game->GetHero());
        if (nearest.IsAlive()) {
            if (IsActiveThreadTerminating()) return false;
            Game->AddPersonToConversation(conversation, nearest);
            if (furthest.IsAlive()) {
                if (IsActiveThreadTerminating()) return false;
                Game->AddPersonToConversation(conversation, furthest);
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + trader.GetDataString() + "_" + comment, false, trader, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + nearest.GetDataString() + "_INTRO_RESPONSE_NO", false, nearest, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + furthest.GetDataString() + "_INTRO_RESPONSE_OATH", false, furthest, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + nearest.GetDataString() + "_INTRO_RESPONSE_BAD_NEWS", false, nearest, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + trader.GetDataString() + "_INTRO_RESPONSE_REWARD", false, trader, Game->GetHero());
            } else {
                if (IsActiveThreadTerminating()) return false;
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + trader.GetDataString() + "_" + comment, false, trader, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + nearest.GetDataString() + "_INTRO_RESPONSE_NO", false, nearest, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_067_" + trader.GetDataString() + "_INTRO_RESPONSE_REWARD", false, trader, Game->GetHero());
            }
        }
        CommentTimer = pGlobalScriptDef->TE_TraderCommentDelay;
        return true;
    }
    if (CommentTimer.GetTime() > 0) {
        IsActiveThreadTerminating();
        return false;
    }
    long conversation = Game->AddNewConversation(trader, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    {
        CCharString data = trader.GetDataString();
        Game->AddLineToConversation(conversation, "TEXT_QST_067_" + data + "_" + comment, false, trader, Game->GetHero());
    }
    if (type == TRADER_COMMENT_MULTIPLE) {
        if (IsActiveThreadTerminating()) return false;
        CScriptThing other = Game->GetNearestWithScriptName(trader, "DarkwoodTrader");
        if (other.IsAlive() && !(other == trader) && Game->TextEntryExists("TEXT_QST_067_" + other.GetDataString() + "_" + comment + "_RESPONSE")) {
            if (IsActiveThreadTerminating()) return false;
            Game->AddPersonToConversation(conversation, other);
            Game->AddLineToConversation(conversation, "TEXT_QST_067_" + other.GetDataString() + "_" + comment + "_RESPONSE", false, other, Game->GetHero());
        }
    }
    CommentTimer = pGlobalScriptDef->TE_TraderCommentDelay;
    return true;
}
