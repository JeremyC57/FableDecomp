// CPlayer::InitModeFirstPerson(EPlayerMode mode, CProcessedInput* input) -- entering first-person mode: attach the first-person, right-stick look-around and special-camera-cycle input processes, then push the special camera mode when the world hands out a camera manager
// Leaf: ?InitModeFirstPerson@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe @ 0x00633530, 90 bytes, thiscall `ret 8` (a CPlayer::PlayerModeInfo handler).
// Identity (probable, donor order + body): Ego_r.pdb orders CPlayer's mode handlers UninitModeControlCreature (0x00633420 here),
// UninitModeClickPastText, UninitModeCutScene, InitModeFirstPerson, UninitModeFreezeControls, UninitModeSpecialAbilityFreezeControls,
// ..., InitModeControlFreeCamera (0x006335E0 here); retail has 0x006334F0 / 0x00633510 (the two one-process removals: ClickPastText
// +0x140, CutScene +0x1A4), this body, then 0x00633590 / 0x006335A0 (the two FreezeControls removals) before 0x006335E0. Its
// mirror 0x00634410 (the same three processes removed, PushCurrentManualMode, RemoveMode(0x10)) sits where Ego_r.pdb puts
// UninitModeFirstPerson. The processes are Ego_r.pdb CPlayer::InputProcessFirstPerson / RightStickLookAround / CycleSpecialCameraModes.
// Callees: CAInputProcess::AddToInterface (virtual slot 1, x3), the world camera source's slot 9 GetCameraManager (+0x24),
// CGameCameraManager::PushSpecialCameraMode 0x00698F50 (Ego_r.pdb order PushZTargetMode 0x00698E40 (landed), PushSpecialCameraMode,
// SetAutomaticCameraControl 0x00699090 (landed)).
// Shape: the camera out pointer initialised to 0 in the frame slot and the push guarded by the returned bool.
// Layout: engine/CPlayer.h through the landed screen-mode donor include (InitModeNavigateInventoryExperienceScreen 0x00635080):
// PlayerInterface +0x0C, World +0x1C, PlayerCharacter +0x34, the scoped_ptr<CInputProcess*> members; CThing partial view with
// retail UsingComponents +0x20, Components +0x44, Dead (bit 0 of +0x91).
#include <stddef.h>
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

#include "engine/CVectorMap.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };

enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase : public CBaseClassNonCopyable { };              // partial view (RTTI CTCBase <- CBaseClassNonCopyable <- CBaseClass)
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
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
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
    virtual bool AddToInterface(CInputProcessManager& manager);   // slot 1 (+0x04)
    void RemoveFromInterface();                             // 0x00A0D2D0
};

class CInputProcessGameBase : public CAInputProcess { };   // partial views (bases not modelled)
class CInputProcessFirstPerson : public CInputProcessGameBase { };
class CInputProcessCameraLookAround : public CInputProcessGameBase { };
class CInputProcessCycleSpecialCameraModes : public CInputProcessGameBase { };

class CGameCameraManager {
public:
    void PushSpecialCameraMode();                           // 0x00698F50
    void SetAutomaticCameraControl();                       // 0x00699090
};
class CCameraManagerSource {                                // the world's +0x34 object (partial view)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
class CWorld {                                              // partial view
public:
    unsigned char         _pad00[0x34];
    CCameraManagerSource* CameraSource;                     // +0x34
};

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
private:
    void InitModeFirstPerson(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeFirstPerson(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    InputProcessFirstPerson_ptr->AddToInterface(*PlayerInterface);
    InputProcessRightStickLookAround_ptr->AddToInterface(*PlayerInterface);
    InputProcessCycleSpecialCameraModes_ptr->AddToInterface(*PlayerInterface);
    CGameCameraManager* camera = 0;
    if (World->CameraSource->GetCameraManager(&camera))
        camera->PushSpecialCameraMode();
}