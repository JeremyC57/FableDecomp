// CInputProcessControlCreatureRightStick::Update(CProcessedInput& processed_input) -- unless the right-stick game action (0x3d, any button event) is queued, ask the current game camera for its manual-mode owner (slot 9, out pointer), push the current manual mode when the stick was looking around, then clear the three stick flags
// Leaf: ?Update@CInputProcessControlCreatureRightStick@@UAEXAAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068D840, length 66 bytes (+ int3 padding). Vtable 0x01237A40 slot 6; the argument is unused.
// Identity: Ego_r.pdb + FableWin.pdb ?Update@CInputProcessControlCreatureRightStick@@UAEXAAVCProcessedInput@@@Z (public virtual, the CAInputProcess
//   slot 6 override; NF477 audit: one `.rdata` dword, no call).
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was ?Update@CInputProcessControlCreatureRightStick@@QAEXM@Z (a `float` argument) on a local
//   class model; respelt onto engine/CInputProcessControlCreatureRightStick.h FABLE_RETAIL_INPUTPROCESS_VTABLE. The base query is the PDB
//   two-argument overload ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@@Z (0x00687D50, declared
//   by the existing retail-view opt-in FABLE_RETAIL_COMBAT_INPUT_PASS26), was the made-up ?IsGameActionEventInQueue@CInputProcessGameBase@@QAE_NJJ@Z
//   (binding respelt, TABLE_CHANGES.tsv). No body change.
// Callees: CInputProcessGameBase::IsGameActionEventInQueue(EGameAction, EInputButtonEventType) @ 0x00687D50 (the two-argument overload),
// CGameCameraManager::PushCurrentManualMode @ 0x006988B0 (on the out pointer), the camera's vtable slot 9 (CWorld::PCurrentGameCamera +0x34).
// Layout: engine/CInputProcessControlCreatureRightStick.h (PushingCameraModeUp +0x28, PushingCameraModeDown +0x29, LookingAround +0x2a);
// World is CInputProcessGameBase::World (+0x14).
// Shape: one `push ecx` frame slot for the out pointer, zeroed from ebx (the shared zero that also feeds the second argument and the
// three byte clears); the LookingAround test is `cmp byte ptr [esi+2Ah],bl`.
class CGameCameraManager { public:
    void PushCurrentManualMode();                   // 0x006988B0
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual void GetCameraManager(CGameCameraManager** ppManager);   // slot 9
};
class CWorld { public:
    unsigned char      _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;          // +0x34
};
#include "engine/EGameAction.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#define FABLE_RETAIL_COMBAT_INPUT_PASS26   // the existing retail-view opt-in that declares the two-argument IsGameActionEventInQueue
#include "engine/CInputProcessControlCreatureRightStick.h"

void CInputProcessControlCreatureRightStick::Update(CProcessedInput&)
{
    if (!IsGameActionEventInQueue((EGameAction)0x3d, (EInputButtonEventType)0)) {
        CGameCameraManager* cameraManager = 0;
        World->PCurrentGameCamera->GetCameraManager(&cameraManager);
        if (LookingAround)
            cameraManager->PushCurrentManualMode();
        PushingCameraModeUp = false;
        PushingCameraModeDown = false;
        LookingAround = false;
    }
}
