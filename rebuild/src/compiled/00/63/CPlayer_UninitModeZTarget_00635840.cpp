// CPlayer::UninitModeZTarget(EPlayerMode mode, CProcessedInput* input) -- leaving the Z-target mode removes its sixteen input processes from the player interface, drops creature mode 25 from the hero, clears the ZTargeting flag, gives the camera back to automatic control and, unless the next mode is 0x12, clears the hero's locked target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00635840, length 370 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?UninitModeZTarget@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z` (a CPlayer::PlayerModeInfo
// UninitFunc).
// Callees: CAInputProcess::RemoveFromInterface 0x00A0D2D0 (non-virtual, sixteen processes), CPlayer::GetPControlledCreature
// 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline GetInterface: type 0x31 = the creature
// mode manager, bit 17 of +0x24; type 8 = CTCTargetingPlayer, bit 8 of +0x20), CTCCreatureModeManager::RemoveMode
// 0x0073ABA0, the world's +0x34 camera source slot 9 (+0x24, GetCameraManager out pointer),
// CGameCameraManager::SetAutomaticCameraControl 0x00699090, CTCTargetingPlayer::ClearLockedTarget 0x00802650.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, World +0x1C, ZTargeting +0x20A) through the lane's
// CPlayer carrier; CThing interface bitset +0x20, interface map +0x44 (as the landed InitModeFirstPersonTargeting
// 0x00635A50).
// Shape: HasTC + the landed ternary GetTC (0x00847FB0) so each component lands straight in ecx (the out-parameter
// GetInterface leaves it in eax + `mov ecx,eax`); the mode-manager pointer is left uninitialised (retail's miss arm
// loads the dead `mode`-argument frame word), the targeting pointer is zero-initialised (`xor ecx,ecx` before the bit
// test); no alive test on the hero.
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

typedef long EPlayerMode;
class CProcessedInput;
class CTCBase;

class CAInputProcess { public: void RemoveFromInterface(); };          // 0x00A0D2D0

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
class CThingPlayerCreature : public CThing { };

class CTCCreatureModeManager { public:                         // interface 0x31
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
};
class CTCTargetingPlayer { public:                             // interface 8
    void ClearLockedTarget();                           // 0x00802650
};

class CGameCameraManager { public:
    void SetAutomaticCameraControl();                   // 0x00699090
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                 // +0x34
};

class CPlayer : public CPlayerPdb { public:
    CThingPlayerCreature* GetPControlledCreature() const;                             // 0x00487DC0
    void UninitModeZTarget(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModeZTarget(EPlayerMode mode, CProcessedInput* input)
{
    (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessZTarget_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessStrafe_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockRightStickTargetSelect_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseEnvironment_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockCycleTargets_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->RemoveFromInterface();
    CThing* hero = GetPControlledCreature();
    CTCCreatureModeManager* modes;
    if (hero->HasTC(0x31))
        modes = hero->GetTC<CTCCreatureModeManager>(0x31);
    modes->RemoveMode(25);
    ZTargeting = false;
    CGameCameraManager* camera = 0;
    if (World->CameraSource->GetCameraManager(&camera))
        camera->SetAutomaticCameraControl();
    if (mode != 0x12) {
        CTCTargetingPlayer* targeting = 0;
        if (hero->HasTC(8))
            targeting = hero->GetTC<CTCTargetingPlayer>(8);
        targeting->ClearLockedTarget();
    }
}