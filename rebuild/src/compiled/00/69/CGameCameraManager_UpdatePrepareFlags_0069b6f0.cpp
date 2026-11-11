// CGameCameraManager::UpdatePrepareFlags() -- per-frame camera bookkeeping: when the current mode reports itself done, fall back to automatic (or re-push the manual) mode; update the melee and building camera modes; and with no script module pending, count down ScriptedCameraRemoveTimer while the current script module is finished and no subtitle waits for a click, releasing scripted control when it runs out
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069B6F0, length 158 bytes.
// Callees: the current mode's virtual slot 13 (+0x34, a bool), PushCurrentManualMode @ 0x006988B0, UpdateObservedThingInMelee
// @ 0x0069AC30 (landed), UpdateObservedThingInBuilding @ 0x0069B650, CSubtitleRenderer::IsWaitingForClickPast @ 0x0048DB46 (the player
// GUI's renderer at +0x18; global g_pPlayerGui @ 0x013B8790), the current script module's virtual slot 2 (+8, a bool),
// ReleaseScriptedControl @ 0x00698950.
// Layout: retail CGameCameraManager with PDB names (see this chunk's ReleaseScriptedControl / PushCurrentManualMode): CurrentMode +0x98,
// NewAutoMode +0xa8, PCurrentScriptModule +0xd4, PPendingScriptModule +0xdc, ScriptedCameraRemoveTimer +0xe4, UsingCameraManually +0x12c.
// Shape: the subtitle query is a named bool taken BEFORE the script-module tests (it rides in bl); the timer clamp re-reads the member
// at the join and the decrement is `dec dword ptr [esi+0E4h]`.
class CSubtitleRenderer { public:
public:
    bool IsWaitingForClickPast() const;   // 0x0048DB46
};
class CPlayerGui { public:
    unsigned char     _pad[0x18];
    CSubtitleRenderer SubtitleRenderer;     // +0x18
};
// agentNF16: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass (?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A),
//   read through the inline static accessor Get() (name-only: the load is the same).
#include "engine/CTBaseSingletonNoDefaultAlloc.h"

namespace NCameraModes {
class CBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual bool IsFinished();              // slot 13 (+0x34)
};
class CScriptModuleBase { public:
    virtual void v00();
    virtual void v01();
    virtual bool IsFinished();              // slot 2 (+8)
};
}
struct SCountBlock;
#include "engine/CCountedPointer.h"

class CGameCameraManager { public:
    void*         __vftable;                                            // +0x00
    unsigned char _pad04[0x94];                                         // +0x04
    CCountedPointer<NCameraModes::CBase> CurrentMode;                   // +0x98
    CCountedPointer<NCameraModes::CBase> PendingMode;                   // +0xa0
    bool          NewAutoMode;                                          // +0xa8
    unsigned char _pada9[0x2b];                                         // +0xa9
    CCountedPointer<NCameraModes::CScriptModuleBase> PCurrentScriptModule; // +0xd4
    CCountedPointer<NCameraModes::CScriptModuleBase> PPendingScriptModule; // +0xdc
    long          ScriptedCameraRemoveTimer;                            // +0xe4
    unsigned char _pade8[0x44];                                         // +0xe8
    bool          UsingCameraManually;                                  // +0x12c
    void PushCurrentManualMode();                                       // 0x006988B0
    void ReleaseScriptedControl();                                      // 0x00698950
    void UpdateObservedThingInMelee();                                  // 0x0069AC30
    void UpdateObservedThingInBuilding();                               // 0x0069B650
    void UpdatePrepareFlags();
};

void CGameCameraManager::UpdatePrepareFlags()
{
    if (CurrentMode.Data && CurrentMode.Data->IsFinished()) {
        if (!UsingCameraManually)
            NewAutoMode = true;
        else
            PushCurrentManualMode();
    }
    UpdateObservedThingInMelee();
    UpdateObservedThingInBuilding();
    if (!PPendingScriptModule.Data) {
        bool waitingForSubtitle = CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer.IsWaitingForClickPast();
        if (PCurrentScriptModule.Data && PCurrentScriptModule.Data->IsFinished() && !waitingForSubtitle) {
            if (ScriptedCameraRemoveTimer < 0)
                ScriptedCameraRemoveTimer = 1;
            if (ScriptedCameraRemoveTimer == 0)
                ReleaseScriptedControl();
            --ScriptedCameraRemoveTimer;
        }
    }
}