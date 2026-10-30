// NScript::CQR_EscortTrader_ManagerScript::Activate() -- wait frame by frame while the LookoutPoint region is loaded, then (unless terminating) count one more Orchard Farm escort in the Sunnyvale master data and activate the QR_EscortTrader quest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E255D0, length 184 bytes, plain `ret`.
// Identity: Ego_r.pdb `?Activate@CQR_EscortTrader_ManagerScript@NScript@@QAEXXZ`; called six times from the manager's
// Main (0x00E24E10).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 12 IsRegionLoaded(const CCharString&) (+0x30), 276 ActivateQuest(const CCharString&) (+0x450); CCharString(const
// char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. Strings: "LookoutPoint" @ 0x012C3394, "QR_EscortTrader" @ 0x012E1600.
// Layout: retail quest base 0x48 bytes (CScriptBase 0x40, GSI +0x40, PMasterData +0x44); the Sunnyvale master data's
// OrchardFarmTraderEscortCounter is at +0x5C (Ego_r.pdb, no shift: the master data holds no debug containers).
// Shape: the loop test is an inline helper with a NAMED region string (built before the interface's vtable load) and a
// `? true : false` result (`setne bl` across the string destructor); VC rotates the loop so the helper is inlined at
// the top and at the bottom. The quest name is an implicit conversion temporary (re-addressed with `lea`).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0x5c];
    long          OrchardFarmTraderEscortCounter;       // +0x5c
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQR_EscortTrader_ManagerScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool IsLookoutPointLoaded()
    {
        CCharString region("LookoutPoint", -1);
        return GSI->IsRegionLoaded(region) ? true : false;
    }
    void Activate();
};

void CQR_EscortTrader_ManagerScript::Activate()
{
    while (IsLookoutPointLoaded()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ++PMasterData->OrchardFarmTraderEscortCounter;
    GSI->ActivateQuest("QR_EscortTrader");
}
}