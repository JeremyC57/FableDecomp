// NScript::CQ_TraderEscortScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game) -- script factory: `new CQ_TraderEscortScript(data, game)` with the constructor inlined (registered at 0x00CD77E5 as "S_QTE" / "Q_TraderEscort")
// Leaf: ?Alloc@CQ_TraderEscortScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0AF60, 79 bytes, static __fastcall (data in ecx, game in edx). PDB:
//   ?Alloc@CQ_TraderEscortScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x7c bytes), CScriptBase::CScriptBase 0x00CB8110, g_pGameScriptInterface (0x0143E8F8)->RegisterTimer for CommentTimer, CCharString::CCharString() 0x0099E4B0. Vtable 0x012DF16C.
// Inline constructor: CommentTimer (+0x48) and the TraderToTalk string (+0x78); the rest stays uninitialised.
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
class CCharString { public: void* PStringData; CCharString(); };   // default ctor 0x0099E4B0
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_TraderEscortScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CTimer CommentTimer;                                // +0x48
    long BalverinesToSurpriseHeroNeeded;                // +0x4c
    long TradersStillAliveCounter;                      // +0x50
    bool InfectedTraderCanGetUp;                        // +0x54
    bool TradersShouldBeScared;
    bool IntroFinished;
    bool FollowInfoGiven;
    bool EndStarted;
    bool SavedInMiddle;
    bool SavedNearEnd;                                  // +0x5a
    long IncubationTimeLow;                             // +0x5c
    long IncubationTimeMedium;                          // +0x60
    long IncubationTimeHigh;                            // +0x64
    long TraderPickpocketedCount;                       // +0x68
    bool MissionSucceeded;                              // +0x6c
    bool MissionFailed;
    bool QuestStartScreened;
    bool GoneWrongWay;
    bool ShownBalverine;
    bool ShownEarthTroll;                               // +0x71
    long TimesBantered;                                 // +0x74
    CCharString TraderToTalk;                           // +0x78
    CQ_TraderEscortScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_TraderEscortScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CQ_TraderEscortScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CQ_TraderEscortScript(data, game);
}
}