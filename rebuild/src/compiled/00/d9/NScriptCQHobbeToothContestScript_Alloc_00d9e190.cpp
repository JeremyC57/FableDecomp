// NScript::CQ_HobbeToothContestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script factory: new CQ_HobbeToothContestScript(data, game) with the constructor inlined (CScriptBase, Game / MasterScriptData, the own vtable, the WhisperCommentTimer timer)
// Leaf: ?Alloc@CQ_HobbeToothContestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9E190, 71 bytes (+ int3 padding to 80), static `__fastcall` (data in ecx, game in edx), plain `ret`.
// Identity (proven): NScript::CQ_HobbeToothContestScript::Alloc; evidence: the inlined constructor stores the RTTI-less vtable 0x012D65CC, whose Main (slot 2, 0x00D9A190) registers the entity scripts Q_HobbeToothHobbe, Q_HobbeToothWhisper, OrchardFarmFarmer, DrinkingHobbe ..., all nested in Ego_r.pdb NScript::CQ_HobbeToothContestScript (?Alloc@C<name>@CQ_HobbeToothContestScript@NScript@@); the timer lands at +0x60 = Ego_r CQ_HobbeToothContestScript::WhisperCommentTimer (CTimer, +0x74) - 0x14 and `new` asks 0x68 = sizeof Ego_r CQ_HobbeToothContestScript (0x7C) - 0x14; this factory is stored at 0x00CD6C8E (CScriptInfoManager registration 0x00CD52D0); the Ego_r/FableWin.pdb name sets hold ?Alloc@CQ_HobbeToothContestScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Label: the manifest's "Alloc" is generic (family J034: six one-timer quest-script factories).
// Callees: operator new 0x00BFEA1A (0x68 bytes), CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), the script
//   interface's RegisterTimer (slot 87, +0x15C; ?RegisterTimer@CGameScriptInterface@@UAEJXZ) through the global interface pointer
//   0x0143E8F8 (no PDB data name; the catalog spelling ?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A, reused).
// Layout: Ego_r.pdb CQ_HobbeToothContestScript (0x7C; retail 0x68 = Ego_r - 0x14, the script-base difference): CLevelScriptBase<T>
//   {CGameScriptInterfaceBase* Game, T* MasterScriptData} at retail +0x40 / +0x44, WhisperCommentTimer (CTimer) Ego_r +0x74 -> retail +0x60.
// Model: agent952 H037 (the out-of-line constructor of the same classes); local classes -- Review at landing.
class CScriptDataBase;
class CGameScriptInterfaceBase;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeToothContestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HobbeToothContestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_HobbeToothContestScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x60 - 0x48];          // POD members up to the timer
    CTimer        WhisperCommentTimer;                            // +0x60
    unsigned char _tail[0x4];                           // members after the timer
};
CScriptBase* __fastcall CQ_HobbeToothContestScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_HobbeToothContestScript(data, game);
}
}