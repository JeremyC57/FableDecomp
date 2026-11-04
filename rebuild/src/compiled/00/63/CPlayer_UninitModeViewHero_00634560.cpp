// CPlayer::UninitModeViewHero(EPlayerMode mode, CProcessedInput* input) -- leaving view-hero mode: detach its three input processes, show the bottom half of the GUI again, hand the camera back to automatic control and drop player mode 0x2F
// Leaf: ?UninitModeViewHero@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x00634560, 88 bytes, thiscall `ret 8` (a CPlayer::PlayerModeInfo handler).
// Identity (probable, donor order + mirror): Ego_r.pdb orders RemoveMode (0x00634190 here), UninitModeFirstPerson (0x00634410),
// UninitModeChargeQuickAccess (0x00634470, landed), UninitModeViewHero, AddMode (0x006345C0, landed); this body undoes the landed
// InitModeViewHero 0x00636E90 exactly (the same three processes LeftStickLookAround +0x190, BButtonExitMode +0x1D8,
// ToggleViewHeroMode +0x204; DisplayBottomHalfGUI back to true).
// Callees: CAInputProcess::RemoveFromInterface 0x00A0D2D0 (x3), CPlayerGui::SetDisplayBottomHalfGUI(bool) inlined (FableWin.pdb
// places it right after InitModeViewHero; (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790, DisplayBottomHalfGUI +0x330), the world camera source's
// GetCameraManager (slot 9), CGameCameraManager::SetAutomaticCameraControl 0x00699090, CPlayer::RemoveMode(EPlayerMode) 0x00634190.
// Shape: the camera out pointer is not initialised and its bool result is ignored (as InitModeViewHero).
// Layout: engine/CPlayerGui.h (DisplayBottomHalfGUI +0x330, donor include) and engine/CPlayer.h through the landed screen-mode donor include (InitModeNavigateInventoryExperienceScreen 0x00635080):
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
    void RemoveFromInterface();                             // 0x00A0D2D0
};

class CInputProcessGameBase : public CAInputProcess { };   // partial views (bases not modelled)
class CInputProcessFirstPersonLookAround : public CInputProcessGameBase { };
class CInputProcessBButtonExitMode : public CInputProcessGameBase { };
class CInputProcessToggleViewHeroMode : public CInputProcessGameBase { };

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
class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
#define CPlayerGui CPlayerGuiDonor
#include "engine/CPlayerGui.h"
#undef CPlayerGui
class CPlayerGui : public CPlayerGuiDonor {
public:
    void SetDisplayBottomHalfGUI(bool display) { DisplayBottomHalfGUI = display; }
};
class CPlayer : public CPlayerPdb {
private:
public:
    void RemoveMode(EPlayerMode mode);                     // 0x00634190
private:
    void UninitModeViewHero(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModeViewHero(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    InputProcessLeftStickLookAround_ptr->RemoveFromInterface();
    InputProcessBButtonExitMode_ptr->RemoveFromInterface();
    InputProcessToggleViewHeroMode_ptr->RemoveFromInterface();
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SetDisplayBottomHalfGUI(true);
    CGameCameraManager* camera;
    World->CameraSource->GetCameraManager(&camera);
    camera->SetAutomaticCameraControl();
    RemoveMode((EPlayerMode)0x2f);
}