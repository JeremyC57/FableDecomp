// NScript::CQR_EscortTraderScript::WatchForFailure() -- watch thread: frame by frame until the hero is killed or the escort ends (failed, succeeded, trader killed, trader lost), then mark the mission failed unless it succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1F2D0, length 198 bytes, plain `ret`.
// Identity: Ego_r.pdb `?WatchForFailure@CQR_EscortTraderScript@NScript@@QAEXXZ`; the escort script's main
// (0x00E1F0C0..) spawns it as a CSpawnedFunc0 thread (0x00E1F270 stores this address next to "WatchForFailure").
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7 NewScriptFrame (+0x1C) and
// 70 GetHero (+0x118, returns CScriptThing&); CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48);
// CCharString(const char*, long) 0x0099EBF0 and ~CCharString 0x0099EAE0. String: "" @ 0x0122D70E (killed by anyone).
// Layout: retail CScriptBase is 0x40 bytes, the quest base adds GSI (+0x40) and its owner word (+0x44); the
// CQR_EscortTraderScript members sit 0x14 below Ego_r.pdb (debug containers in the base): MissionSucceeded +0x48,
// MissionFailed +0x49, TraderDepartedLost +0x4C, TraderKilled +0x4E (PDB +0x5C/+0x5D/+0x60/+0x62).
// Shape: one script frame first, then a wait loop whose condition is a single `||` chain led by the hero's killed
// message; the CCharString("") temporary lives in a conditional-temporary full expression, so VC keeps a destruction
// flag (ebx, `or ebx,1` / `test bl,1`) and parks the condition's result in a frame byte across the destructor.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQR_EscortTraderScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Owner;                        // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool HiredByTrader;                                 // +0x4a
    bool TraderAtDestination;                           // +0x4b
    bool TraderDepartedLost;                            // +0x4c
    bool GreetedHero;                                   // +0x4d
    bool TraderKilled;                                  // +0x4e
    void WatchForFailure();
};

void CQR_EscortTraderScript::WatchForFailure()
{
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        bool waiting = !(GSI->GetHero().MsgIsKilledBy("") || MissionFailed || MissionSucceeded || TraderKilled
                         || TraderDepartedLost);
        if (!waiting)
            break;
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
}