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

class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
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