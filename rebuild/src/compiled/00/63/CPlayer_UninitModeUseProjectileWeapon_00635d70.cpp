// CPlayer::UninitModeUseProjectileWeapon(EPlayerMode newMode, CProcessedInput* input) -- leaving the projectile-weapon mode: detach its eleven input processes, take a living hero's creature mode manager out of mode 25 and clear ProjectileTargetLocked, push the camera's current manual mode, and drop player mode 25 when the next mode is 25 or 2
// Leaf: ?UninitModeUseProjectileWeapon@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00635D70, 283 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser (`mov eax,635D70h ; mov [13B8FB8h],eax`
//   at 0x0121B477), which stores it as the Uninit member-function pointer of the entry whose Init (0x013B8FA8) is the
//   landed CPlayer::InitModeUseProjectileWeapon 0x00635BF0 (the table alternates Init/Uninit 16-byte pointers, as
//   0x00635080 / 0x00635120 at 0x013B8DE8 / 0x013B8DF8); both PDBs hold
//   ?UninitModeUseProjectileWeapon@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z.
// Callees: CAInputProcess::RemoveFromInterface 0x00A0D2D0 (eleven processes), CPlayer::GetPControlledCreature() const
//   0x00487DC0 (FableWin.pdb; every reloc binding of it is an invented free function), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020 (inline GetTC, interface 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::
//   RemoveMode(unsigned long) 0x0073ABA0, CAIGameCameraBase::GetCameraManager (virtual, slot 9, +0x24),
//   CGameCameraManager::PushCurrentManualMode 0x006988B0, CPlayer::RemoveMode(EPlayerMode) 0x00634190.
// Layout: engine/CPlayer.h (retail STLport layout; layout-renamed so the private method can be declared): World +0x1C,
//   the scoped_ptr input processes +0x104 .. +0x1FC, ProjectileTargetLocked +0x20B; engine/CWorld.h PCurrentGameCamera
//   +0x34; retail CThing UsingComponents +0x20, Components +0x44, Dead bit 0 of +0x91.
// Shape: the landed sibling CPlayer::UninitModeFirstPersonTargeting 0x00635B30: the mode manager zero-initialised and
//   filled by the inline out-parameter GetTC (the end / hit ternary), the camera manager out pointer zero-initialised in
//   its frame word and the getter's bool guarding the push.
#include <stddef.h>
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CWorld.h"

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CTCBase;

class CAInputProcess { public:
    void RemoveFromInterface();                             // 0x00A0D2D0
};

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> storage
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThingPlayerCreature { public:                        // partial view (retail CThing offsets)
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                       // +0x20 bitset<274>
    CInterfaceMap Components;                               // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char StateFlags;                               // +0x91 (bit 0: Dead)
    template <class T> __forceinline bool GetTC(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
inline bool IsThingAlive(const CThingPlayerCreature* thing)
{
    return thing && !(thing->StateFlags & 1);
}

class CTCCreatureModeManager { public:                      // interface 0x31
    void RemoveMode(unsigned long mode);                    // 0x0073ABA0
};
class CGameCameraManager { public:
    void PushCurrentManualMode();                           // 0x006988B0
};
class CAIGameCameraBase { public:                           // partial view: slots 0-8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};

class CPlayer : public CPlayerPdb { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
    void RemoveMode(EPlayerMode mode);                      // 0x00634190
private:
    void UninitModeUseProjectileWeapon(EPlayerMode newMode, CProcessedInput* input);
};

void CPlayer::UninitModeUseProjectileWeapon(EPlayerMode newMode, CProcessedInput* input)
{
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTargetOnPress_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessLeftStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessBButtonExitMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessTargetLockCycleTargets_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessProjectileTargetingAnalogueZoom_ptr)->RemoveFromInterface();
    CThingPlayerCreature* hero = GetPControlledCreature();
    if (IsThingAlive(hero)) {
        CTCCreatureModeManager* modes = 0;
        hero->GetTC(&modes, 0x31);
        modes->RemoveMode(25);
        ProjectileTargetLocked = false;
    }
    CGameCameraManager* camera = 0;
    if (World->PCurrentGameCamera->GetCameraManager(&camera))
        camera->PushCurrentManualMode();
    if (newMode == 25 || newMode == 2)
        RemoveMode((EPlayerMode)25);
}