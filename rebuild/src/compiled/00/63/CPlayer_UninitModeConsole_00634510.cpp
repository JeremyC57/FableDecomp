// CPlayer::UninitModeConsole(EPlayerMode mode, CProcessedInput* input) -- leaving console mode: make the console singleton on first use, deactivate it, and detach the console input process
// Leaf: ?UninitModeConsole@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
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
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#include <stddef.h>
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

#include "engine/CVectorMap.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { };

#include "engine/CThing.h"
class CTCBase : public CBaseClassNonCopyable { };              // partial view (RTTI CTCBase <- CBaseClassNonCopyable <- CBaseClass)

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
#include "engine/CCountedPointer.h"
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