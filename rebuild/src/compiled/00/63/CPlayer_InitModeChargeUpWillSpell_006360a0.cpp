// CPlayer::InitModeChargeUpWillSpell(EPlayerMode mode, CProcessedInput* input) -- entering the will-charge-up mode: attach nine creature-control input processes (control creature, Z-target, look-around, right stick, jump/roll, block, movement, the third-person termination watcher, special abilities) to the player's interface, then put the live controlled creature into creature mode 0x19 unless it is already current
// Leaf: ?InitModeChargeUpWillSpell@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006360A0, 245 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (store at 0x0121B65C): the Init of
//   mode 35 (Uninit 0x006361A0). Retail places it between UninitModeUseProjectileWeapon 0x00635D70 / the mode-34 pair
//   0x00635E90 / 0x00635FC0 and InitModeNavigateInGameMenu 0x00636270, where both PDBs' source order has
//   UseProjectileWeaponThirdPerson then ChargeUpWillSpell; the body attaches
//   CPlayer::InputProcessWatchForWillChargeUpThirdPersonModeTermination (+0x1C4). Both PDBs hold
//   ?InitModeChargeUpWillSpell@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z.
// Model: the landed J104 CPlayer rows (0x00635080: layout-renamed engine/CPlayer.h, CThing::GetTC through the STLport
//   bitset reference proxy, `T* tc = 0`).
// Callees: AddToInterface (virtual slot 1) nine times, CPlayer::GetPControlledCreature() const 0x00487DC0 (Ego_r.pdb; the byte-identical 0x00487DD0 is bound as GetPControlledCreature),
//   CVectorMap LowerBound 0x0040F020 (interface 0x31 = CTCCreatureModeManager), CTCCreatureModeManager::GetCurrentMode()
//   const 0x0073A6E0, CModeBase::GetType (virtual slot 1), CTCCreatureModeManager::AddMode(unsigned long) 0x0073AE10.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include <stddef.h>
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase : public CBaseClassNonCopyable { };              // partial view (RTTI CTCBase <- CBaseClassNonCopyable <- CBaseClass)
class CTCCreatureModeManager : public CTCBase {             // partial view
public:
    class CModeBase {
    public:
        virtual ~CModeBase();
        virtual unsigned long GetType() const;            // slot 1 (+0x04)
    };
    CModeBase& GetCurrentMode() const;                    // 0x0073A6E0
    bool AddMode(unsigned long mode);                     // 0x0073AE10
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
class CInputProcessWatchForWillChargeUpThirdPersonModeTermination : public CInputProcessGameBase { };
class CInputProcessSpecialAbilities : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
public:
    CThingPlayerCreature* GetPControlledCreature() const;     // 0x00487DC0 (Ego_r order: GetPControlledCreature, then PeekPControlledCreature at 0x00487DD0)
private:
    void InitModeChargeUpWillSpell(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeChargeUpWillSpell(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessControlCreature_ptr->AddToInterface(*PlayerInterface);
    InputProcessControlCreatureActivateZTarget_ptr->AddToInterface(*PlayerInterface);
    InputProcessRightStickLookAround_ptr->AddToInterface(*PlayerInterface);
    InputProcessControlCreatureRightStick_ptr->AddToInterface(*PlayerInterface);
    InputProcessJumpingAndRolling_ptr->AddToInterface(*PlayerInterface);
    InputProcessBlock_ptr->AddToInterface(*PlayerInterface);
    InputProcessCreatureMovement_ptr->AddToInterface(*PlayerInterface);
    InputProcessWatchForWillChargeUpThirdPersonModeTermination_ptr->AddToInterface(*PlayerInterface);
    InputProcessSpecialAbilities_ptr->AddToInterface(*PlayerInterface);
    CThingPlayerCreature* creature = GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CTCCreatureModeManager* modes = 0;
        creature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        if (modes->GetCurrentMode().GetType() != 0x19)
            modes->AddMode(0x19);
    }
}