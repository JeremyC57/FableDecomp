// NScript::CQ_PrisonEscapeRescueMotherScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game) -- script factory: `new CQ_PrisonEscapeRescueMotherScript(data, game)` with the constructor inlined (registered at 0x00CD71EB as "S_QPERM" / "Q_PrisonEscapeRescueMother")
// Leaf: ?Alloc@CQ_PrisonEscapeRescueMotherScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDACB0, 74 bytes, static __fastcall (data in ecx, game in edx). PDB:
//   ?Alloc@CQ_PrisonEscapeRescueMotherScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x80 bytes), CScriptBase::CScriptBase 0x00CB8110, g_pGameScriptInterface (0x0143E8F8)->RegisterTimer for RunningTimer. Vtable 0x012DB86C.
// Inline constructor: only RunningTimer (+0x7C) has a constructor; the longs and bools stay uninitialised.
// Layout: PDB (Ego_r) offsets minus 0x14 (the retail script base is 0x14 bytes smaller), STLport vectors 12 bytes (PDB 16).
// Model: the landed quest-script Alloc (NScript::CQ_HeroSoulsArenaScript::Alloc 0x00D80BA0): CLevelScriptBase<T> is the
//   `__declspec(novtable)` argument-carrying base (Game +0x40, MasterScriptData +0x44, stored before the class's own vtable
//   store), the CGameScriptInterface model is the landed 88-slot one (RegisterTimer = slot 87, +0x15C).
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CScriptDataBase;
class CScriptBase { public:                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterface& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonEscapeRescueMotherScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long CurrentTask;                                   // +0x48
    long TasksDone[3];                                  // +0x4c
    long TargetNumber[3];                               // +0x58
    bool HeroAgreesToTask;                              // +0x64
    bool HeroHasFoundWeapons;
    bool MotherCellDoorOpen;
    bool MotherFollowing;
    bool MotherDied;
    bool WatchingHero;
    bool PrisonCutsceneFinished;
    bool RaceStarting;                                  // +0x6b
    long YearsPassedInCell;                             // +0x6c
    long PrisonerConversationIndex;                     // +0x70
    long ConversationIndex;                             // +0x74
    bool HeroEscapedCell;                               // +0x78
    bool AlarmSounded;
    bool MotherRescuedFromPrisonAutoSaveCheckPoint;     // +0x7a
    CTimer RunningTimer;                                // +0x7c
    CQ_PrisonEscapeRescueMotherScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_PrisonEscapeRescueMotherScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CQ_PrisonEscapeRescueMotherScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CQ_PrisonEscapeRescueMotherScript(data, game);
}
}