// CPlayer::UninitModeTakePhotojournalScreenshot(EPlayerMode mode, CProcessedInput* input) -- leaving the photojournal-screenshot mode: close the player character's photojournal capture component, detach the capture input process and re-attach the main one
// Leaf: ?UninitModeTakePhotojournalScreenshot@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe @ 0x00636B00, 105 bytes, thiscall `ret 8` (a CPlayer::PlayerModeInfo handler).
// Identity (probable, donor order + mirror): FableWin.pdb orders InitModeConsole, SetActive, UninitModeConsole,
// InitModeTakePhotojournalScreenshot, UninitModeTakePhotojournalScreenshot; the retail sibling 0x00636A80 opens the same
// component (interface 0x10E) and swaps InputProcessMain (+0x100) out for InputProcessPhotojournalCapture (+0x200), and this
// body closes it and swaps them back.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020,
// CTCPhotojournalCapture::ClosePhotojournalCapture 0x0055D770 (landed; Ego_r.pdb ?ClosePhotojournalCapture@CTCPhotojournalCapture@@QAEXXZ),
// CAInputProcess::RemoveFromInterface 0x00A0D2D0, CAInputProcess::AddToInterface (slot 1).
// Shape: the GetTC head with the dword +0x40 loaded and `test ch,40h` (bit 0x10E).
// Layout: engine/CPlayer.h through the landed screen-mode donor include (InitModeNavigateInventoryExperienceScreen 0x00635080):
// PlayerInterface +0x0C, World +0x1C, PlayerCharacter +0x34, the scoped_ptr<CInputProcess*> members; CThing partial view with
// retail UsingComponents +0x20, Components +0x44, Dead (bit 0 of +0x91).
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
    void RemoveFromInterface();                             // 0x00A0D2D0
};

class CInputProcessGameBase : public CAInputProcess { };   // partial views (bases not modelled)
class CInputProcessPhotojournalCapture : public CInputProcessGameBase { };
class CInputProcessMain : public CInputProcessGameBase { };

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
class CTCPhotojournalCapture : public CTCBase {              // partial view
public:
    enum { INTERFACE_TYPE = 0x10E };
    void ClosePhotojournalCapture();                        // 0x0055D770
};
class CPlayer : public CPlayerPdb {
private:
    void UninitModeTakePhotojournalScreenshot(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModeTakePhotojournalScreenshot(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCPhotojournalCapture* capture = 0;
    character->GetTC(&capture);
    capture->ClosePhotojournalCapture();
    InputProcessPhotojournalCapture_ptr->RemoveFromInterface();
    InputProcessMain_ptr->AddToInterface(*PlayerInterface);
}