// CPlayer::InitModeUseProjectileWeaponThirdPerson(EPlayerMode mode, CProcessedInput* input) -- entering third-person ranged-weapon mode: attach twelve creature-control input processes (control creature, Z-target activation, ranged weapon use, look-around, jumping/rolling, block, right stick, movement, ranged-weapon mode, the third-person termination watcher, special abilities, the control-angle watcher) to the player's interface, then put the live controlled creature into creature mode 0x19 unless it is already current
// Leaf: ?InitModeUseProjectileWeaponThirdPerson@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00635E90, 290 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (abs store at 0x0121B648, the
//   Init slot of the mode-34 pair whose Uninit is 0x00635FC0). Retail order 0x00635BF0 InitModeUseProjectileWeapon
//   (landed), 0x00635D70, 0x00635E90, 0x00635FC0, 0x006360A0 InitModeChargeUpWillSpell (landed) matches both PDBs'
//   source order Init/UninitModeUseProjectileWeapon, Init/UninitModeUseProjectileWeaponThirdPerson,
//   InitModeChargeUpWillSpell; this body attaches (AddToInterface) where 0x00635FC0 detaches the same set, and it
//   attaches InputProcessWatchForRangedWeaponThirdPersonModeTermination (+0x1C0).
// Model: the landed sibling 0x006360A0 verbatim (layout-renamed engine/CPlayer.h, CThing::GetTC through the STLport
//   bitset reference proxy, `T* tc = 0`); only the attached process list differs.
// Callees: AddToInterface (virtual slot 1) twelve times, CPlayer::GetPControlledCreature() const 0x00487DC0,
//   CVectorMap LowerBound 0x0040F020 (interface 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::GetCurrentMode()
//   const 0x0073A6E0, CModeBase::GetType (virtual slot 1), CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10.
#include <stddef.h>
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase : public CBaseClassNonCopyable { };              // partial view (RTTI CTCBase <- CBaseClassNonCopyable <- CBaseClass)
class CTCCreatureModeManager : public CTCBase {             // partial view
public:
    enum { INTERFACE_TYPE = 0x31 };                       // GetInterfaceType
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const;            // slot 1 (+0x04)
    };
    CModeBase& GetCurrentMode() const;                    // 0x0073A6E0
    bool AddMode(unsigned long mode);                     // 0x0073AE10
};

class CThing {                                              // partial view, retail offsets
public:
    unsigned char      _pad00[0x20];                        // CClass / CBaseObject / CBaseClass, UniqueID (+0x18)
    std::bitset<274>   UsingComponents;                     // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x91 - 0x54];                 // UpdateComponentsList (list, 4) ... PlayerNumber (+0x90)
    unsigned char      Dead : 1;                            // +0x91 bit 0
    unsigned char      CreatedByOwnerThingCreation : 1;
    unsigned char      IncludeInThingScans : 1;
    unsigned char      Activated : 1;
    unsigned char      GloballyLoaded : 1;
    unsigned char      InLimbo : 1;
    unsigned char      KillLocked : 1;
    unsigned char      CreatedBySerialisation : 1;
    bool IsDead() const { return Dead; }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::iterator it = map->LowerBound(type);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { };                   // RTTI CThingPlayerCreature <- CThingCreatureBase <- CThingGameObject
class CThingGameObject : public CThingPhysical { };         //   <- CThingPhysical <- CThing
class CThingCreatureBase : public CThingGameObject { };
class CThingPlayerCreature : public CThingCreatureBase { };

class CBaseObject;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                             // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void* PData;                                            // CBaseObjectPointer*
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

class CInputProcessManager : public CBaseClassNonCopyable { };   // RTTI CGamePlayerInterface <- CInputProcessManager
class CGamePlayerInterface : public CInputProcessManager { };
class CAInputProcess : public CBaseClassNonCopyable {
public:
    virtual bool AddToInterface(CInputProcessManager& manager);   // slot 1 (+0x04)
};
class CInputProcessGameBase : public CAInputProcess { };
class CInputProcessControlCreature : public CInputProcessGameBase { };
class CInputProcessControlCreatureActivateZTarget : public CInputProcessGameBase { };
class CInputProcessCameraLookAround : public CInputProcessGameBase { };
class CInputProcessControlCreatureRightStick : public CInputProcessGameBase { };
class CInputProcessJumpingAndRolling : public CInputProcessGameBase { };
class CInputProcessBlock : public CInputProcessGameBase { };
class CInputProcessCreatureMovement : public CInputProcessGameBase { };
class CInputProcessUseRangedWeapon : public CInputProcessGameBase { };
class CInputProcessSetRangedWeaponMode : public CInputProcessGameBase { };
class CInputProcessWatchForRangedWeaponThirdPersonModeTermination : public CInputProcessGameBase { };
class CInputProcessCreatureMovementWatchForControlAngleChange : public CInputProcessGameBase { };
class CInputProcessSpecialAbilities : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
public:
    CThingPlayerCreature* GetPControlledCreature() const;     // 0x00487DC0 (Ego_r order: GetPControlledCreature, then PeekPControlledCreature at 0x00487DD0)
private:
    void InitModeUseProjectileWeaponThirdPerson(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeUseProjectileWeaponThirdPerson(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessControlCreature_ptr->AddToInterface(*PlayerInterface);
    InputProcessControlCreatureActivateZTarget_ptr->AddToInterface(*PlayerInterface);
    InputProcessUseRangedWeapon_ptr->AddToInterface(*PlayerInterface);
    InputProcessRightStickLookAround_ptr->AddToInterface(*PlayerInterface);
    InputProcessJumpingAndRolling_ptr->AddToInterface(*PlayerInterface);
    InputProcessBlock_ptr->AddToInterface(*PlayerInterface);
    InputProcessControlCreatureRightStick_ptr->AddToInterface(*PlayerInterface);
    InputProcessCreatureMovement_ptr->AddToInterface(*PlayerInterface);
    InputProcessSetRangedWeaponMode_ptr->AddToInterface(*PlayerInterface);
    InputProcessWatchForRangedWeaponThirdPersonModeTermination_ptr->AddToInterface(*PlayerInterface);
    InputProcessSpecialAbilities_ptr->AddToInterface(*PlayerInterface);
    InputProcessWatchForControlAngleChange_ptr->AddToInterface(*PlayerInterface);
    CThingPlayerCreature* creature = GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CTCCreatureModeManager* modes = 0;
        creature->GetTC(&modes);
        if (modes->GetCurrentMode().GetType() != 0x19)
            modes->AddMode(0x19);
    }
}