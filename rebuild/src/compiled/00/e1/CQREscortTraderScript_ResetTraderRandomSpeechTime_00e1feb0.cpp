// NScript::CQR_EscortTraderScript::ResetTraderRandomSpeechTime() -- restart the trader's random-speech timer (TraderRandomSpeechTimer +0x84) with the def's maximum minus a random amount below the def's range
// Leaf: ?ResetTraderRandomSpeechTime@CQR_EscortTraderScript@NScript@@QAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?ResetTraderRandomSpeechTime@CQR_EscortTraderScript@NScript@@QAEXXZ; the shared header
//   engine/NScript_CQR_EscortTraderScript.h declares it next to TraderRandomSpeechTimer, and its callers are the escort-trader
//   script's own entity code (0x00E1FCF0 / 0x00E1FEF0 call it at +0x84). Was the stand-in CV_AmbushScamScript::ResetRandomSpeechTime
//   on a local struct (name-fix NF38: moved onto the shared header).
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe @ 0x00E1FEB0, 60 bytes (+ int3 padding). Called from eleven places in its script (0x00E1FD9C..).
// Callees: rand @ 0x00BFEB16, CGameScriptInterface::SetTimer (slot 89, +0x164) through the interface global @ 0x0143E8F8.
// Data: the script def pointer @ 0x0143E90C (CDefPointer<CScriptDef>): +0x9c8 the timer maximum, +0x9cc the random range
// (descriptive names; the retail CScriptDef offsets do not line up with Ego_r.pdb here).
// Shape: the def pointer is read into a local BEFORE rand() (kept in edi for the `idiv`), while the maximum is read
// through the global again after the call, and the difference is a named local (that keeps the `mov eax,[g] ;
// mov eax,[eax+9c8h]` reload in eax; computing it inside the call argument hoists the interface fetch into ebx).
extern "C" int __cdecl rand();                           // 0x00BFEB16

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#include "engine/NScript_CQR_EscortTraderScript.h"
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)

// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: RandomSpeechTimeMax -> TraderRandomSpeechTimerMax (+0x9c8), RandomSpeechTimeRange -> TraderRandomSpeechTimerVar (+0x9cc).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)

void NScript::CQR_EscortTraderScript::ResetTraderRandomSpeechTime()
{
    const CScriptDef* def = pGlobalScriptDef.Get();
    long time = pGlobalScriptDef->TraderRandomSpeechTimerMax - rand() % def->TraderRandomSpeechTimerVar;
    pDSTGame->SetTimer(TraderRandomSpeechTimer.TimerIndex, time);
}
