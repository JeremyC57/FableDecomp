// CPlayer::UninitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode, CProcessedInput* input) -- leaving PC third-person projectile aiming: detach fifteen input processes from the player's interface, clear PCProjectileWeaponThirdPersonAimingMode and take the controlled creature's mode manager out of creature mode 0x19
// Leaf: ?UninitModePCProjectileWeaponThirdPersonAiming@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00636D40, 264 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (abs store at 0x0121B868, the
//   Uninit beside 0x00636B70's Init store at 0x0121B85E). Retail order 0x00636B00 UninitModeTakePhotojournalScreenshot
//   (landed), 0x00636B70, 0x00636D40, 0x00636E50 InitModes (landed) matches both PDBs' source order
//   UninitModeTakePhotojournalScreenshot, Init/UninitModePCProjectileWeaponThirdPersonAiming, InitModes; the body clears
//   CPlayer::PCProjectileWeaponThirdPersonAimingMode (+0x21A) and detaches (RemoveFromInterface).
// Shape: the controlled creature is used without a null test and the manager pointer is NOT pre-zeroed (`T* tc;`): with
//   no interface bit retail passes the uninitialised local (its frame word, shared with the lookup key) to RemoveMode,
//   and VC duplicates the RemoveMode tail into both branches.
// Model: the landed sibling 0x006360A0 (layout-renamed engine/CPlayer.h, CThing::GetTC through the STLport bitset
//   reference proxy).
// Callees: CAInputProcess::RemoveFromInterface() 0x00A0D2D0 fifteen times, CPlayer::GetPControlledCreature() const
//   0x00487DC0, CVectorMap LowerBound 0x0040F020 (interface 0x31 = CTCCreatureModeManager),
//   CTCCreatureModeManager::RemoveMode(unsigned long) 0x0073ABA0.
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
    void RemoveMode(unsigned long mode);                  // 0x0073ABA0
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

class CInputProcessManager : public CBaseClassNonCopyable { };   // RTTI CGamePlayerInterface <- CInputProcessManager
class CGamePlayerInterface : public CInputProcessManager { };
class CAInputProcess : public CBaseClassNonCopyable {
public:
    void RemoveFromInterface();                             // 0x00A0D2D0
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
class CInputProcessStrafe : public CInputProcessGameBase { };
class CInputProcessDebugControls : public CInputProcessGameBase { };
class CInputProcessQuickAccessItems : public CInputProcessGameBase { };
class CInputProcessCombat : public CInputProcessGameBase { };
class CInputProcessSpecialAbilities : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
public:
    CThingPlayerCreature* GetPControlledCreature() const;     // 0x00487DC0 (Ego_r order: GetPControlledCreature, then PeekPControlledCreature at 0x00487DD0)
private:
    void UninitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModePCProjectileWeaponThirdPersonAiming(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessControlCreature_ptr->RemoveFromInterface();
    InputProcessControlCreatureActivateZTarget_ptr->RemoveFromInterface();
    InputProcessControlCreatureRightStick_ptr->RemoveFromInterface();
    InputProcessRightStickLookAround_ptr->RemoveFromInterface();
    InputProcessStrafe_ptr->RemoveFromInterface();
    InputProcessBlock_ptr->RemoveFromInterface();
    InputProcessDebugControls_ptr->RemoveFromInterface();
    InputProcessQuickAccessItems_ptr->RemoveFromInterface();
    InputProcessCombat_ptr->RemoveFromInterface();
    InputProcessUseRangedWeapon_ptr->RemoveFromInterface();
    InputProcessSpecialAbilities_ptr->RemoveFromInterface();
    InputProcessJumpingAndRolling_ptr->RemoveFromInterface();
    InputProcessSetRangedWeaponMode_ptr->RemoveFromInterface();
    InputProcessWatchForControlAngleChange_ptr->RemoveFromInterface();
    InputProcessWatchForRangedWeaponThirdPersonModeTermination_ptr->RemoveFromInterface();
    PCProjectileWeaponThirdPersonAimingMode = false;
    CThingPlayerCreature* creature = GetPControlledCreature();
    CTCCreatureModeManager* modes;
    creature->GetTC(&modes);
    modes->RemoveMode(0x19);
}