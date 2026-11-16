// NScript::CQ_OrchardFarmRaidScript::MakeTeamMemberComment(const CCharString& line, CScriptThing& speaker, ERaidComment comment) -- if the comment timer has run out, start a conversation in which the speaker says "<TextSystemScriptCode><speaker data>_<line>" to the hero, then restart the timer
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCDA80, length 251 bytes, `ret 0xc` (the third argument, the nested ERaidComment, is unused).
// Callees: the interface global 0x0143E8F8 GetTimer(long) (slot 90, +0x168, 0x006E7710) and SetTimer(long, long) (slot 89,
// +0x164, 0x006E76E0); CScriptBase::IsActiveThreadTerminating 0x00CB7940 (result unused); through the game script
// interface (+0x40): AddNewConversation (slot 364, +0x5b0), GetHero (slot 70, +0x118, x2), AddPersonToConversation (slot
// 365, +0x5b4), AddLineToConversation (slot 366, +0x5b8); CScriptThing::GetDataString (virtual slot 3, +0xc, by value);
// operator+(const CCharString&, const CCharString&) 0x0099F570 and operator+(const CCharString&, const char*) 0x0099F600
// (both `__fastcall`, hidden result in ecx); ~CCharString 0x0099EAE0 (x4). Literal "_" 0x01244DB4.
// Layout: Ego_r.pdb CQ_OrchardFarmRaidScript - 0x14, -4 after CrateList: CommentTimer +0x4c, TextSystemScriptCode +0x84.
// Shape: the conversation index is a named local (ebx); the line is one `code + data + "_" + line` expression whose
// last temporary lands in the dead `speaker` argument slot (the reference is held in edi).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// Leaf: ?MakeTeamMemberComment@CQ_OrchardFarmRaidScript@NScript@@QAE_NABVCCharString@@AAVCScriptThing@@W4ERaidComment@12@@Z
// Name: Ego_r.pdb + FableWin.pdb ?MakeTeamMemberComment@CQ_OrchardFarmRaidScript@NScript@@QAE_NABVCCharString@@AAVCScriptThing@@W4ERaidComment@12@@Z
//   (non-const CScriptThing&, third parameter the nested enum ERaidComment); the landed callers (CCrateTeamMember::Main 0x00DCE230 and
//   0x00DCD350's family) already bind it.
// NF41 (2026-11-16): was a local NScript::CQ_OrchardFarmRaidScript model (const CScriptThing&, bool) over CGameScriptInterface; now the
//   shared engine/CQ_OrchardFarmRaidScript.h (its Game pointer +0x40 is the CGameScriptInterfaceBase whose slots 70 / 364..366 the
//   row calls), with the include chain of the landed sibling 0x00DCD350.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
#include "engine/CTimer.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

bool NScript::CQ_OrchardFarmRaidScript::MakeTeamMemberComment(const CCharString& line, CScriptThing& speaker, ERaidComment comment)
{
    if (pDSTGame->GetTimer(CommentTimer.TimerIndex) > 0) {
        IsActiveThreadTerminating();
        return false;
    }
    long conversation = Game->AddNewConversation(speaker, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, TextSystemScriptCode + speaker.GetDataString() + "_" + line, false, speaker, Game->GetHero());
    pDSTGame->SetTimer(CommentTimer.TimerIndex, 5);
    return true;
}
