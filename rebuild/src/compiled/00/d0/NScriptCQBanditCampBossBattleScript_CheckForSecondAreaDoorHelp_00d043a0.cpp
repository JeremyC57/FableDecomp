// Leaf: ?CheckForSecondAreaDoorHelp@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ
// NF190 (2026-11-20, q6261): the class is CQ_BanditCampBossBattleScript (Ego_r.pdb + FableWin.pdb name sets hold only
//   ?CheckForSecondAreaDoorHelp@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ; its spawn at 0x00D041B9 is inside the landed
//   CQ_BanditCampBossBattleScript::Main 0x00D038E0, which binds that spelling). The flag at +0x4D is Gate3Open (Ego_r +97 minus
//   the 0x14 base shift; the old local model named it ActivatedBanditRaid from CQ_BanditCampScript's record, same offset).
//   Ported onto engine/CQ_BanditCampBossBattleScript.h; was ?CheckForSecondAreaDoorHelp@CQ_BanditCampScript@NScript@@QAEXXZ.
// Revisit: inline-depth -- byte-exact, but reached with #pragma inline_depth regions retail most likely never wrote; redo pragma-free once the retail mechanism is found (owner 2026-11-02; docs/pipeline/INLINE_DEPTH_REVISIT.md).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Grade: RELOCATION_MATCH 948/948 on the corrected span 0x00D043A0+948 (code + embedded jump table; bounds audit) -- catalog row 976 B over-captured 0x00D04760; really CQ_BanditCampScript::CheckForSecondAreaDoorHelp (catalog: EndMission, propagated)
// CQ_BanditCampScript::CheckForSecondAreaDoorHelp() -- once the bandit raid is under way, while the hero lingers outside BanditCampResidential the guildmaster cycles through his THROUGH_GATE hints every 20 timer ticks; stops when the hero reaches BanditCampBoss
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D043A0, length 964 bytes, plain `ret` (+ a five-entry switch table).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CTimer (inline ctor/dtor on the global interface 0x0143E8F8, slots 87..90; out-of-line ~CTimer 0x00CD4470
//   at the return before the switch); CGameScriptInterface slots 7 NewScriptFrame, 11 IsLevelLoaded, 274
//   HeroReceiveMessageFromGuildMaster. Strings "BanditCampResidential", "BanditCampBoss",
//   "TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10" .. "_50".
// Layout: Ego_r.pdb CQ_BanditCampScript minus 0x14 (retail CScriptBase): ActivatedBanditRaid +0x4d.
// Identity: spawned as "CheckForSecondAreaDoorHelp" (string 0x012C8910) by the CQ_BanditCampScript spawn list at
//   0x00D041B9 (member pointer stored at 0x00D041E3); the catalog's CV_ChapelOfEvilScript::EndMission is a propagated label.
// Shape: generated script code; `for (;;)` with the terminate return first; the hint switch's last case loops back to
//   stage 3 (cases 2 and 4 share the store).
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// CTimer: the retail model from engine/CTimer.h; bodies the row expands inline follow the include (as at HEAD).
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }


void NScript::CQ_BanditCampBossBattleScript::CheckForSecondAreaDoorHelp()
{
    while (!Gate3Open) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    timer = 5;
    long stage = 0;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("BanditCampResidential")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsLevelLoaded("BanditCampBoss")) {
                if (IsActiveThreadTerminating())
                    return;
                return;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() <= 0) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            switch (stage) {
            case 0:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_10", "", true, true);
                stage = 1;
                break;
            case 1:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_20", "", true, true);
                stage = 2;
                break;
            case 2:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_30", "", true, true);
                stage = 3;
                break;
            case 3:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_40", "", true, true);
                stage = 4;
                break;
            case 4:
                Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_009_GUILDMASTER_THROUGH_GATE_50", "", true, true);
                stage = 3;
                break;
            }
            timer = 20;
        }
        Game->NewScriptFrame();
    }
}