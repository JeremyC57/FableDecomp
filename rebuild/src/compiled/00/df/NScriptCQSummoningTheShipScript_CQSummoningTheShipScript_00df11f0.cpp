// NScript::CQ_SummoningTheShipScript::CQ_SummoningTheShipScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase, the level-script Game / MasterScriptData pointers, the own vtable, then the CommentaryTimer timer registered with the script interface
// Leaf: ??0CQ_SummoningTheShipScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF11F0, 49 bytes (64 with int3 padding), thiscall `ret 8`.
// Identity (proven): CQ_SummoningTheShipScript::CQ_SummoningTheShipScript; evidence: the body stores the RTTI-less vtable 0x012DE10C, whose Main (slot 2, 0x00DF13A0)
//   registers the entity scripts STS_BriarRose, SummonerAttacker, SummonerMinion, FireHeartHolder ..., all nested in Ego_r.pdb NScript::CQ_SummoningTheShipScript (?Alloc@C<name>@CQ_SummoningTheShipScript@NScript@@);
//   FableWin.pdb has ??0CQ_SummoningTheShipScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z. The registered timer lands at
//   +0x54, Ego_r.pdb CQ_SummoningTheShipScript::CommentaryTimer (CTimer, Ego_r +0x68; quest-script members sit 0x14 lower in retail).
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

// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"


namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_SummoningTheShipScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_SummoningTheShipScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_SummoningTheShipScript();
    unsigned char _members[0x54 - 0x48];           // POD members up to the timer
    CTimer        CommentaryTimer;                            // +0x54
};

CQ_SummoningTheShipScript::CQ_SummoningTheShipScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}