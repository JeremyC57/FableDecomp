// CPlayer::InitModeNavigateInventoryMapScreen(EPlayerMode mode, CProcessedInput* input) -- entering the map hero-information screen mode: open the player character's CTCInventoryMap component (OpenInventory, slot 30), take the main input process off the interface, attach the hero-information-screens and map-screen input processes, then the alive check
// Leaf: ?InitModeNavigateInventoryMapScreen@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00634F60, 156 bytes, thiscall, `ret 8`.
// Identity (proven): its only reference is the CPlayer mode-table initialiser 0x0121AF20 (store at 0x0121B0D4), which pairs
//   every EPlayerMode's Init and Uninit member pointers; this is the Init of mode 8 (partner 0x00635000, which closes the same
//   component). Modes 4..8 are the five inventory screens in FableWin.pdb's order (NavigateInventory, Clothing, Weapons,
//   AbilitiesScreen, MapScreen), and the body opens interface 0x1B (CTCInventoryMap, as the landed
//   CInputProcessInventoryMapScreen::Init) and attaches CPlayer::InputProcessInventoryMapScreen (+0x120); both PDBs hold
//   ?InitModeNavigateInventoryMapScreen@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z.
// Model: the landed family J104 row CPlayer::InitModeNavigateInventoryExperienceScreen 0x00635080 (same GetTC, same alive
//   tail), plus CAInputProcess::RemoveFromInterface() 0x00A0D2D0 (Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ) on
//   InputProcessMain (+0x100) before the two AddToInterface calls.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (four calls), CVectorMap LowerBound 0x0040F020,
//   CTCInventoryMap::OpenInventory (virtual slot 30), RemoveFromInterface 0x00A0D2D0, AddToInterface (virtual slot 1).
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
namespace NInventory {
class CTCInventoryBase : public CTCBase {                   // partial view: 86 vtable slots (CTCInventoryMagic adds none)
public:
    virtual ~CTCInventoryBase();
    virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04(); virtual void Slot05();
    virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09(); virtual void Slot10();
    virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14(); virtual void Slot15();
    virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19(); virtual void Slot20();
    virtual void Slot21(); virtual void Slot22(); virtual void Slot23(); virtual void Slot24(); virtual void Slot25();
    virtual void Slot26(); virtual void Slot27(); virtual void Slot28(); virtual void Slot29();
    virtual void OpenInventory();                             // slot 30 (+0x78)
    virtual void CloseInventory();                            // slot 31 (+0x7C)
    virtual void Slot32(); virtual void Slot33(); virtual void Slot34(); virtual void Slot35(); virtual void Slot36();
    virtual void Slot37(); virtual void Slot38(); virtual void Slot39(); virtual void Slot40(); virtual void Slot41();
    virtual void Slot42(); virtual void Slot43(); virtual void Slot44(); virtual void Slot45(); virtual void Slot46();
    virtual void Slot47(); virtual void Slot48(); virtual void Slot49(); virtual void Slot50(); virtual void Slot51();
    virtual void Slot52(); virtual void Slot53(); virtual void Slot54(); virtual void Slot55(); virtual void Slot56();
    virtual void Slot57(); virtual void Slot58(); virtual void Slot59(); virtual void Slot60(); virtual void Slot61();
    virtual void Slot62(); virtual void Slot63(); virtual void Slot64(); virtual void Slot65(); virtual void Slot66();
    virtual void Slot67(); virtual void Slot68(); virtual void Slot69(); virtual void Slot70(); virtual void Slot71();
    virtual void Slot72(); virtual void Slot73(); virtual void Slot74(); virtual void Slot75(); virtual void Slot76();
    virtual void Slot77(); virtual void Slot78(); virtual void Slot79(); virtual void Slot80(); virtual void Slot81();
    virtual void Slot82(); virtual void Slot83(); virtual void Slot84(); virtual void Slot85();
};
}
class CTCInventoryMap : public NInventory::CTCInventoryBase {   // partial view
public:
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
class CInputProcessInventoryMapScreen : public CInputProcessInventoryBase { };
class CInputProcessMain : public CInputProcessGameBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
private:
    void InitModeNavigateInventoryMapScreen(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeNavigateInventoryMapScreen(EPlayerMode mode, CProcessedInput* input)
{
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCInventoryMap* inventory = 0;
    character->GetTC(TCI_INVENTORY_MAP, &inventory);
    inventory->OpenInventory();
    InputProcessMain_ptr->RemoveFromInterface();
    InputProcessHeroInformationScreens_ptr->AddToInterface(*PlayerInterface);
    InputProcessInventoryMapScreen_ptr->AddToInterface(*PlayerInterface);
    if (character.GetPItem() && !character->IsDead())
        character.GetPItem();
}