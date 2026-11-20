// CBriarRose::DoOnHitSpeech() -- Briar Rose's line when hit: three times in four the default hit line, otherwise one of three lines picked by the minion-camp script's timer (more than 48: _START, more than 24: _MIDDLE, else _END); said through DoSpeech unless the script is done
// Leaf: ?DoOnHitSpeech@CBriarRose@CQ_MinionCampScript@NScript@@AAEXXZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA26B0, 244 bytes (plain `ret`).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (line prefix 0x012D711C), rand @ 0x00BFEB16, IsDone
//          @ 0x00F35B30 (manifest label propagated), the timer service (global 0x0143E8F8) slot 90 (+0x168, GetTimer),
//          CCharString::operator+=(const char*) @ 0x0099F100 (suffixes 0x012CD3B8 / 0x012D7114 / 0x012D710C),
//          CCharString::operator=(const char*) @ 0x0099EFE0 (the default line 0x012D70E8), CBriarRose::DoSpeech
//          @ 0x00DA2610, ~CCharString @ 0x0099EAE0.
// Layout: the landed sibling DoOnSpeakToSpeech (0x00DA27B0) model (Ego_r.pdb CBriarRose, ParentClass +0x14 whose timer
//          sits at +0x60).
// Shape: `rand() % 4 == 0` (signed remainder: `and 80000003h ; jns ; dec ; or -4 ; inc`) guards the timer lines,
//          which repeat DoOnSpeakToSpeech's chain after one extra IsDone test; the default arm is the `else` (laid out last).
// NF200 (2026-11-20, q6500): the DoSpeech family (0x00DA2610 / 0x00DA26B0 / 0x00DA27B0) moved together onto the shared
//   engine/CQ_MinionCampScript.h (FABLE_RETAIL_NSCRIPT), the prelude of the landed sibling 0x00DA0EF0 CBriarRose::Main: the nested class is
//   NScript::CQ_MinionCampScript::CBriarRose, so DoSpeech is the private Ego_r.pdb 0x00DC9700 + FableWin.pdb 0x037EF460
//   ?DoSpeech@CBriarRose@CQ_MinionCampScript@NScript@@AAEXABVCCharString@@@Z (was the global-class stand-in
//   ?DoSpeech@CBriarRose@@QAEXABVCCharString@@@Z). The row-local CBriarRose / CEntityScriptBase / CQ_MinionCampScript / CGameScriptInterface /
//   CTimerManager / CCharString views are gone; CTimer::GetTime / operator=(long) are the CTimer.h inline bodies retail expands
//   (pDSTGame slots 90 GetTimer / 89 SetTimer), defined after the include as the header asks.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CQ_MinionCampScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CTimer.h"
extern CGameScriptInterfaceBase* pDSTGame;                                // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
extern "C" int __cdecl rand();                  // 0x00BFEB16

void NScript::CQ_MinionCampScript::CBriarRose::DoOnHitSpeech()
{
    CCharString line("TEXT_QST_B08_BRIAR_ONHIT");  // 0x012D711C
    if (rand() % 4 == 0) {
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass.ChantTimer.GetTime() > 48) {
            if (IsActiveThreadTerminating())
                return;
            line += "_START";                      // 0x012CD3B8
        } else if (ParentClass.ChantTimer.GetTime() > 24) {
            if (IsActiveThreadTerminating())
                return;
            line += "_MIDDLE";                     // 0x012D7114
        } else {
            if (IsActiveThreadTerminating())
                return;
            line += "_END";                        // 0x012D710C
        }
    } else {
        if (IsActiveThreadTerminating())
            return;
        line = "TEXT_QST_B08_BRIAR_ONHIT_DEFAULT";   // 0x012D70E8
    }
    DoSpeech(line);
}
