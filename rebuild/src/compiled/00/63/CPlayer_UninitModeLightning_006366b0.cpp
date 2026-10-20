// CPlayer::UninitModeLightning(EPlayerMode, CProcessedInput*) -- detach the lightning-mode input processes, take the hero's creature mode manager out of mode 25, clear Z-targeting and hand the camera back to automatic control
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006366B0, length 255 bytes, `ret 8` (a CPlayer::PlayerModeInfo UninitFunc,
// table slot at 0x0121B7F0).
// Callees: CAInputProcess::RemoveFromInterface 0x00A0D2D0 (x12), CPlayer::GetHeroThing 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (interface 0x31, bit 17 of +0x24),
// CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0, the world's +0x34 object's slot 9 (GetCameraManager
// out-parameter, +0x24), CGameCameraManager::SetAutomaticCameraControl 0x00699090.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, World +0x1C, ZTargeting +0x20A) through the lane's
// CPlayer_Methods carrier; CThing interface bitset +0x20, interface map +0x44.
// Shape: `T* p = 0; if (HasTC) p = GetTC;` (retail's `xor ecx,ecx` preload, edi pushed only around the map lookup; the
// out-parameter GetInterface form keeps the manager in edi); the camera out pointer zeroed in its frame word as in the
// landed CPlayer::InitModeFirstPersonTargeting (0x00635A50), the getter's bool guarding the call.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
struct CProcessedInput;
struct CTCBase;

struct CAInputProcess {
    void RemoveFromInterface();                         // 0x00A0D2D0 (non-virtual)
};

struct CTCEntry {                                       // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     Type;
    CTCBase* TC;
};
struct CTCMap {                                         // CThing::Components: a sorted vector of pairs (12 bytes)
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type);    // 0x0040F020
    __forceinline CTCBase* Find(long type)
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                   // +0x20 which TC types the thing holds
    unsigned char _pad40[4];
    CTCMap        Components;                           // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                             // +0x91 bit 0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) { return Components.Find(type); }
};

struct CTCCreatureModeManager {                         // interface 0x31
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};

struct CGameCameraManager {
    void SetAutomaticCameraControl();                   // 0x00699090
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                 // +0x34
};

struct CPlayer_Methods : CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
    void UninitModeLightning(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::UninitModeLightning(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessStrafe_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessLightning_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockRightStickTargetSelect_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockCycleTargets_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->RemoveFromInterface();
    CThing* hero = GetHeroThing();
    CTCCreatureModeManager* modes = 0;
    if (hero->HasTC(0x31))
        modes = (CTCCreatureModeManager*)hero->GetTC(0x31);
    modes->RemoveMode(25);
    ZTargeting = false;
    CGameCameraManager* camera = 0;
    if (((CWorld*)World)->CameraSource->GetCameraManager(&camera))
        camera->SetAutomaticCameraControl();
}