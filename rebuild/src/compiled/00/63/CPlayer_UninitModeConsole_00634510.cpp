// CPlayer::UninitModeConsole(EPlayerMode mode, CProcessedInput* input) -- leaving console mode: make the console singleton on first use, deactivate it, and detach the console input process
// Leaf: ?UninitModeConsole@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe @ 0x00634510, 75 bytes, thiscall `ret 8` (a CPlayer::PlayerModeInfo handler).
// Identity (probable, body + sibling): FableWin.pdb (the PC build) orders InitModeConsole, CConsole::SetActive(bool), UninitModeConsole;
// the retail sibling 0x006344C0 runs the same singleton fetch, stores true at the console's +0x78 and attaches InputProcessConsole
// (+0x1F8), and this body stores false and detaches the same process. CConsole::SetActive(bool) inlines to the +0x78 byte store.
// Callees: CTBaseSingleton<CConsole>::Get inlined (static 0x013CAA40; operator new 0x00BFEA1A of 0x9c bytes, CConsole::CConsole
// 0x009ECD80, CCountedPointer<CConsole>::Reset 0x00413520 -- the landed Get 0x00414C90 body), CAInputProcess::RemoveFromInterface
// 0x00A0D2D0 (Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ).
// Shape: the singleton word tested before `this` is saved; the console pointer re-read from the static after the reset.
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
class CInputProcessConsole : public CInputProcessGameBase { };

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
template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Info;                                             // CCPPointerInfo*
    void Reset(T* data);                                    // 0x00413520 for CConsole
};
template <class T> class CTBaseSingleton {
public:
    virtual ~CTBaseSingleton() { }
    static CCountedPointer<T> Instance;                     // 0x013CAA40 for CConsole
    static T& __fastcall Get()
    {
        if (!Instance.Data)
            Instance.Reset(new T);
        return *Instance.Data;
    }
};
class CConsole : public CTBaseSingleton<CConsole> {         // 0x9c bytes, vtable 0x0129C600
public:
    CConsole();                                             // 0x009ECD80
    unsigned char _pad04[0x78 - 0x04];
    bool Active;                                            // +0x78
    unsigned char _pad79[0x9c - 0x79];
    void SetActive(bool active) { Active = active; }
};
class CPlayer : public CPlayerPdb {
private:
    void UninitModeConsole(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModeConsole(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    CTBaseSingleton<CConsole>::Get().SetActive(false);
    InputProcessConsole_ptr->RemoveFromInterface();
}