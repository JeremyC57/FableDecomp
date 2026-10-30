// NScript::CQ_HobbeToothContestScript::CQ_HobbeToothContestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase, the level-script Game / MasterScriptData pointers, the own vtable, then the WhisperCommentTimer timer registered with the script interface
// Leaf: ??0CQ_HobbeToothContestScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D99E40, 49 bytes (64 with int3 padding), thiscall `ret 8`.
// Identity (proven): CQ_HobbeToothContestScript::CQ_HobbeToothContestScript; evidence: the body stores the RTTI-less vtable 0x012D65CC, whose Main (slot 2, 0x00D9A190)
//   registers the entity scripts Q_HobbeToothHobbe, Q_HobbeToothWhisper, OrchardFarmFarmer, DrinkingHobbe ..., all nested in Ego_r.pdb NScript::CQ_HobbeToothContestScript (?Alloc@C<name>@CQ_HobbeToothContestScript@NScript@@);
//   FableWin.pdb has ??0CQ_HobbeToothContestScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. The registered timer lands at
//   +0x60, Ego_r.pdb CQ_HobbeToothContestScript::WhisperCommentTimer (CTimer, Ego_r +0x74; quest-script members sit 0x14 lower in retail).
// Label: the manifest's "CV_AmbushScamScript" is propagated across family H037 (seven quest-script constructors).
// Callees: CScriptBase::CScriptBase 0x00CB8110 (??0CScriptBase@@QAE@XZ), the script interface's RegisterTimer (slot 87, +0x15C)
//   through the global interface pointer 0x0143E8F8 (catalog ?g_pGameScriptInterface@@3PAUCGameScriptInterface@@A, reused).
// Model: Ego_r.pdb CLevelScriptBase<T> {CGameScriptInterfaceBase* Game, T* MasterScriptData} (retail +0x40 / +0x44), CTimer
//   {long TimerIndex} with an inline constructor; local classes -- Review at landing.
class CScriptDataBase;
class CGameScriptInterfaceBase;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

class CTimer {
public:
    long TimerIndex;                                    // +0x00
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer();
};

class CScriptBase {
public:
    CScriptBase();                                      // 0x00CB8110
    virtual ~CScriptBase();
    unsigned char _base[0x40 - 4];                      // Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame,
                                                        // PActiveThread, RuntimeScriptedEntityPersist
};

template <class TMasterData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TMasterData*              MasterScriptData;         // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : Game(&game), MasterScriptData((TMasterData*)data) { }
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HobbeToothContestScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_HobbeToothContestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_HobbeToothContestScript();
    unsigned char _members[0x60 - 0x48];           // POD members up to the timer
    CTimer        WhisperCommentTimer;                            // +0x60
};

CQ_HobbeToothContestScript::CQ_HobbeToothContestScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}