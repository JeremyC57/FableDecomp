// NScript::CV_AmbushScamScript::ResetRandomSpeechTime() -- Set the speech timer to maximum minus a random variation.
// Leaf: ?ResetRandomSpeechTime@CV_AmbushScamScript@NScript@@QAEXXZ
// Identity: Ego_r.pdb 0x00E77A70 and FableWin.pdb 0x0392E420, both
//   ?ResetRandomSpeechTime@CV_AmbushScamScript@NScript@@QAEXXZ. Retail Fable.exe 0x00E2E400 is 57 bytes
//   (push esi/edi through ret; int3 at 0x00E2E439). pGlobalScriptDef is 0x0143E90C, pDSTGame is 0x0143E8F8,
//   MSVCR71 rand thunk is 0x00BFEB16, SetTimer is vtable slot 89 (call [edx+164h]).
//   Timer index is [esi+7Ch] (engine/CV_AmbushScamScript.h). Def Max/Var are
//   AmbushScamRandomSpeechTimerMax/Var at +2B8h/+2BCh (engine/CScriptDef.h).
//   The 2026-10-28 draft is unchanged; that spelling loaded the vtable into ebx.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CV_AmbushScamScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"
extern CGameScriptInterfaceBase* pDSTGame; // 0x0143E8F8
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
namespace NScript {
void CV_AmbushScamScript::ResetRandomSpeechTime()
{
    const CScriptDef* definition = pGlobalScriptDef.Get();
    RandomSpeechTimer = pGlobalScriptDef->AmbushScamRandomSpeechTimerMax - rand() % definition->AmbushScamRandomSpeechTimerVar;
}
}
