// CPlayer::InitModeZTarget(EPlayerMode mode, CProcessedInput* input) -- entering Z-targeting attaches its sixteen input processes, flags ZTargeting, drops a pending carried thing if the creature is in mode 0x22, puts the creature mode manager into the Z-target mode 25 (re-adding mode 12 when it was current), switches the player to aggressive mode, pushes the Z-target camera and asks the targeting component to pick a new lock target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006355D0, length 611 bytes (+ int3 padding), `ret 8`.
// Callees: CAInputProcess::AddToInterface (virtual slot 1, sixteen processes), CPlayer::GetPControlledCreature 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0 (const copy, the first mode-manager lookup) and 0x0040F020,
// CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 / RemoveMode 0x0073ABA0 / AddMode 0x0073AE10, CTCCarrying
// 0x006A1270 (bool query) / RemoveThingFromPrimarySlot 0x006A1E00, CPlayer::SetAggressiveMode 0x00487EB0, the world's
// +0x34 object's slot 9 (GetCameraManager, +0x24), CGameCameraManager::PushZTargetMode 0x00698E40,
// CTCTargetingPlayer::SetAsPickUpNewLockTarget 0x008023A0.
// Layout: engine/CPlayer.h (scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, World +0x1C, ZTargeting +0x20A)
// through the lane's CPlayer carrier (the landed InitModeUseProjectileWeapon 0x00635BF0 model); CThing interface
// bitset +0x20, interface map +0x44.
// Shape: optional components are `T* p = 0; if (HasTC(t)) p = GetTC<T>(t);` (the miss zero kept in a callee-saved
// register and the call made either way); the mode-0x22 test reads the manager through the const GetTC.
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer

typedef long EPlayerMode;
class CProcessedInput;
class CThingPlayerCreature;
class CPlayer : public CPlayerLayout
{
public:
    void SetAggressiveMode(bool);   // 0x00487EB0
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
    void InitModeZTarget(EPlayerMode mode, CProcessedInput* input);
};

class CTCBase;

class CAInputProcess { public:
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    template <class T> __forceinline const T* GetConstTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (const T*)it->second;
    }
};
class CThingPlayerCreature : public CThing { };

struct CMode {
    virtual void v00();
    virtual unsigned long GetModeType();                // slot 1
};
class CTCCreatureModeManager { public:                         // interface 0x31
    CMode* GetCurrentMode() const;                      // 0x0073A6E0
    void RemoveMode(unsigned long mode);                // 0x0073ABA0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};
class CTCCarrying { public:                                    // interface 0x46
    CThing* RemoveThingFromPrimarySlot(bool drop);      // 0x006A1E00
public:
    bool IsPrimarySlotFree() const;   // 0x006A1270
};
class CTCTargetingPlayer { public:                             // interface 0x08
    void SetAsPickUpNewLockTarget();                    // 0x008023A0
};
class CGameCameraManager { public:
    void PushZTargetMode();                             // 0x00698E40
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


void CPlayer::InitModeZTarget(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessZTarget_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessStrafe_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockRightStickTargetSelect_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseEnvironment_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockCycleTargets_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->AddToInterface(PlayerInterface);
    ZTargeting = true;
    CThing* hero = GetPControlledCreature();
    if (hero->HasTC(0x31)) {
        if (hero->GetConstTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetModeType() == 0x22) {
            CTCCarrying* carrying = 0;
            if (hero->HasTC(0x46))
                carrying = hero->GetTC<CTCCarrying>(0x46);
            if (!carrying->IsPrimarySlotFree())
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
    SetAggressiveMode(true);
    CGameCameraManager* camera = 0;
    if (World->CameraSource->GetCameraManager(&camera))
        camera->PushZTargetMode();
    CTCTargetingPlayer* targeting = 0;
    hero->GetInterface(&targeting, 0x08);
    targeting->SetAsPickUpNewLockTarget();
}