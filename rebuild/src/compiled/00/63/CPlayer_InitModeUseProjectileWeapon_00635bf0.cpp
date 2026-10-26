// CPlayer::InitModeUseProjectileWeapon(EPlayerMode mode, CProcessedInput* input) -- entering the projectile-weapon mode attaches its eleven input processes and, for a living hero, puts the creature mode manager into mode 25 (unless already there), resets the hero's ranged-attack zoom and pushes the projectile-weapon camera mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00635BF0, length 383 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?InitModeUseProjectileWeapon@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z` (a
// CPlayer::PlayerModeInfo InitFunc).
// Callees: CAInputProcess::AddToInterface(CGamePlayerInterface*) (virtual slot 1, eleven processes), CPlayer::GetHeroThing
// 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline lookups: type 0x31 = the creature
// mode manager, bit 17 of +0x24; type 0x81 = CTCHeroRangedAttacks, bit 1 of +0x30), CTCCreatureModeManager::GetCurrentMode
// 0x0073A6E0 (the mode's type is virtual slot 1), CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10,
// CTCHeroRangedAttacks::ResetProjectileTargetingZoomParameter 0x00789B00, the world's +0x34 object's slot 9
// (GetCameraManager out-parameter, +0x24), CGameCameraManager::PushProjectileWeaponMode 0x00698D10.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, World +0x1C) through the lane's
// CPlayer_Methods carrier; CThing interface bitset +0x20, interface map +0x44, dead bit +0x91 & 1 (the model of the
// landed sibling InitModeFirstPersonTargeting 0x00635A50).
// Shape: the mode manager is zero-initialised and filled by the out-parameter GetInterface (`xor ebx,ebx` before the
// bitset test, the component kept in ebx); the ranged-attack component is HasTC + the landed ternary GetTC (straight
// into ecx); the camera manager out pointer is zero-initialised in its frame word and the getter's bool guards the push.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
class CProcessedInput;
class CTCBase;

class CAInputProcess { public:
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

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
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead)
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

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CMode {
    virtual void v00();
    virtual unsigned long GetModeType();                // slot 1
};
class CTCCreatureModeManager { public:                         // interface 0x31
    CMode* GetCurrentMode();                            // 0x0073A6E0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};

class CTCHeroRangedAttacks { public:                           // interface 0x81
    void ResetProjectileTargetingZoomParameter();       // 0x00789B00
};

class CGameCameraManager { public:
    void PushProjectileWeaponMode();                    // 0x00698D10
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

struct CPlayer_Methods : CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
    void InitModeUseProjectileWeapon(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeUseProjectileWeapon(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTargetOnPress_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessLeftStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBButtonExitMode_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockCycleTargets_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessProjectileTargetingAnalogueZoom_ptr)->AddToInterface(PlayerInterface);
    CThing* hero = GetHeroThing();
    if (IsThingAlive(hero)) {
        CTCCreatureModeManager* modes = 0;
        hero->GetInterface(&modes, 0x31);
        if (modes->GetCurrentMode()->GetModeType() != 25)
            modes->AddMode(25);
        if (hero->HasTC(0x81))
            hero->GetTC<CTCHeroRangedAttacks>(0x81)->ResetProjectileTargetingZoomParameter();
        CGameCameraManager* camera = 0;
        if (World->CameraSource->GetCameraManager(&camera))
            camera->PushProjectileWeaponMode();
    }
}