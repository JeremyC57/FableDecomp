// CPlayer::UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input) -- leaving the live-GUI mode: close the player character's PC inventory, end the live controlled creature's blocking, take the hero-information-screens and inventory input processes off the interface and give the main input process back
// Leaf: ?UninitModeLiveGUI@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006369B0, 194 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (store at 0x0121B82C): the Uninit of
//   mode 43, between the OracleMinigame (41: Uninit 0x00633BB0, landed) / FireheartMinigame (42) pairs and the Console pair
//   (44: Uninit 0x00634510, landed); FableWin.pdb's source order is OracleMinigame, FireheartMinigame, LiveGUI, Console,
//   and retail places this body right after the mode-43 Init 0x006368D0 (which opens the PC inventory and attaches the same
//   two processes) as both PDBs place UninitModeLiveGUI after InitModeLiveGUI.
// Model: the landed J104 CPlayer rows (0x00635080: layout-renamed engine/CPlayer.h, CThing::GetTC through the STLport
//   bitset reference proxy, `T* tc = 0`).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CVectorMap LowerBound 0x0040F020 (interfaces 0x11 =
//   CTCInventory and 0x30 = CTCCombat), CTCInventory::ClosePCInventory(bool) 0x00587847, CPlayer::GetPControlledCreature()
//   const 0x00487DC0, CTCCombat::UnsetBlocking() 0x007338A0, CAInputProcess::RemoveFromInterface() 0x00A0D2D0 (twice),
//   AddToInterface (virtual slot 1).
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
class CTCInventory : public CTCBase {                       // partial view
public:
    enum { INTERFACE_TYPE = 0x11 };                       // GetInterfaceType
    void ClosePCInventory(bool immediately);              // 0x00587847
};
class CTCCombat : public CTCBase {                          // partial view
public:
    enum { INTERFACE_TYPE = 0x30 };                       // GetInterfaceType
    void UnsetBlocking();                                 // 0x007338A0
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
    void RemoveFromInterface();                                   // 0x00A0D2D0
};
class CInputProcessGameBase : public CAInputProcess { };
class CInputProcessHeroInformationScreens : public CInputProcessGameBase { };
class CInputProcessInventoryBase : public CInputProcessGameBase { };
class CInputProcessInventory : public CInputProcessInventoryBase { };
class CInputProcessMain : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
public:
    CThingPlayerCreature* GetPControlledCreature() const;     // 0x00487DC0 (Ego_r order: GetPControlledCreature, then PeekPControlledCreature at 0x00487DD0)
private:
    void UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input)
{
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCInventory* inventory = 0;
    character->GetTC(TCI_INVENTORY, &inventory);
    inventory->ClosePCInventory(true);
    CThingPlayerCreature* creature = GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CTCCombat* combat = 0;
        creature->GetTC(TCI_COMBAT, &combat);
        combat->UnsetBlocking();
    }
    InputProcessHeroInformationScreens_ptr->RemoveFromInterface();
    InputProcessInventory_ptr->RemoveFromInterface();
    InputProcessMain_ptr->AddToInterface(*PlayerInterface);
}