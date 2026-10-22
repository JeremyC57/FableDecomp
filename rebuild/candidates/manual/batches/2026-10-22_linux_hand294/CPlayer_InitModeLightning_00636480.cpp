// CPlayer::InitModeLightning(EPlayerMode mode, CProcessedInput* input) -- entering the lightning mode attaches its twelve input processes, flags ZTargeting, drops a pending carried thing if the creature is in mode 0x22, puts the creature mode manager into the Z-target mode 25 (re-adding mode 12 when it was current), for player mode 2 asks the targeting component to pick a new lock target, and pushes the Z-target camera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00636480, length 548 bytes (+ int3 padding), `ret 8`.
// Callees: CAInputProcess::AddToInterface (virtual slot 1, twelve processes), CPlayer::GetHeroThing 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0 (const copy, the first mode-manager lookup) and 0x0040F020,
// CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 / RemoveMode 0x0073ABA0 / AddMode 0x0073AE10, CTCCarrying
// 0x006A1270 (bool query) / RemoveThingFromPrimarySlot 0x006A1E00, CTCTargetingPlayer::SetAsPickUpNewLockTarget
// 0x008023A0, the world's +0x34 object's slot 9 (GetCameraManager, +0x24), CGameCameraManager::PushZTargetMode 0x00698E40.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, World +0x1C, ZTargeting +0x20A)
// through the lane's CPlayer_Methods carrier; CThing interface bitset +0x20, interface map +0x44.
// Shape: the sibling InitModeZTarget 0x006355D0 body (same TU); the targeting component goes through the out-parameter
// GetInterface (one ternary entry pick), whose key temporary reuses the dead `mode` argument slot.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
struct CProcessedInput;
struct CTCBase;

struct CAInputProcess {
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline const T* GetConstTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};

struct CMode {
    virtual void v00();
    virtual unsigned long GetModeType();                // slot 1
};
struct CTCCreatureModeManager {                         // interface 0x31
    CMode* GetCurrentMode() const;                      // 0x0073A6E0
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};
struct CTCCarrying {                                    // interface 0x46
    bool HasPendingActions();                           // 0x006A1270
    CThing* RemoveThingFromPrimarySlot(bool drop);      // 0x006A1E00
};
struct CTCTargetingPlayer {                             // interface 0x08
    void SetAsPickUpNewLockTarget();                    // 0x008023A0
};
struct CGameCameraManager {
    void PushZTargetMode();                             // 0x00698E40
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
    void InitModeLightning(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeLightning(EPlayerMode mode, CProcessedInput* input)
{
    (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessStrafe_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessLightning_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockRightStickTargetSelect_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockCycleTargets_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->AddToInterface(PlayerInterface);
    ZTargeting = true;
    CThing* hero = GetHeroThing();
    if (hero->HasTC(0x31)) {
        if (hero->GetConstTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetModeType() == 0x22) {
            CTCCarrying* carrying = 0;
            if (hero->HasTC(0x46))
                carrying = hero->GetTC<CTCCarrying>(0x46);
            if (!carrying->HasPendingActions())
                carrying->RemoveThingFromPrimarySlot(false);
        }
    }
    CTCCreatureModeManager* modes = 0;
    if (hero->HasTC(0x31))
        modes = hero->GetTC<CTCCreatureModeManager>(0x31);
    unsigned long current = modes->GetCurrentMode()->GetModeType();
    if (current != 25)
        modes->AddMode(25);
    if (current == 12) {
        modes->RemoveMode(current);
        modes->AddMode(current);
    }
    if (mode == 2) {
        CTCTargetingPlayer* targeting = 0;
        hero->GetInterface(&targeting, 0x08);
        targeting->SetAsPickUpNewLockTarget();
    }
    CGameCameraManager* camera = 0;
    if (World->CameraSource->GetCameraManager(&camera))
        camera->PushZTargetMode();
}
