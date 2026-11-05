// CPlayer::InitModeNavigateInventoryTradeScreen(EPlayerMode mode, CProcessedInput* input) -- entering the Trade hero-information screen mode: open the player character's CTCInventoryTrade component (OpenInventoryPC, slot 86), attach the hero-information-screens and Trade-screen input processes to the player interface, then the alive check
// Leaf: ?InitModeNavigateInventoryTradeScreen@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00635190, 148 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): CPlayer::InitModeNavigateInventoryTradeScreen(EPlayerMode, CProcessedInput*), private; evidence: its only
//   reference is the CPlayer mode-table initialiser 0x0121AF20, which stores it as the Init member-function pointer of one
//   EPlayerMode entry beside its Uninit partner 0x00635230 (which closes the same component); it looks up TC interface 0x17
//   (= CTCInventoryTrade::GetInterfaceType, vtable 0x01253034) and calls that class's slot 86 (OpenInventoryPC, slot 86), and it attaches the
//   CPlayer member at +0x130 = Ego_r.pdb CPlayer::InputProcessInventoryTradeScreen (scoped_ptr<CInputProcessInventoryTradeScreen>); FableWin.pdb and Ego_r.pdb both hold
//   ?InitModeNavigateInventoryTradeScreen@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z.
// Label: the manifest's "sub_00635190" is a gap-scan placeholder (family J104: five same-shape screen-mode initialisers that
//   differ in the interface id, the opened slot and the input-process member; retail has no identical-code folding).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ;
//   four calls), CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::LowerBound(const
//   ETCInterfaceType&) 0x0040F020 (Ego_r.pdb names it; retail STLport returns the element pointer, the spelling the J018 rows
//   landed with), CTCInventoryTrade::OpenInventoryPC (virtual, slot 86, +0x158; the class's first own virtual after the 86 base slots; FableWin.pdb ?OpenInventoryPC@CTCInventoryTrade@@UAEXXZ, the port audit's BSim match at the slot's body, and the Uninit partner calls slot 87 (?CloseInventoryPC@CTCInventoryTrade@@UAEXXZ)), CAInputProcess::AddToInterface(CInputProcessManager&)
//   (virtual, slot 1, +0x04; Ego_r.pdb ?AddToInterface@CAInputProcess@@UAE_NAAVCInputProcessManager@@@Z).
// Shape: `T* tc = 0; PlayerCharacter->GetTC(&tc); tc->OpenInventoryPC(); <two AddToInterface(*PlayerInterface)>; if
//   (PlayerCharacter.GetPItem() && !PlayerCharacter->IsDead()) PlayerCharacter.GetPItem();` -- CThing::GetTC<T>(T**) (Ego_r.pdb
//   ??$GetTC@VCTCInventoryTrade@@@CThing@@... in FableWin.pdb) tests UsingComponents through the STLport bitset reference proxy and looks
//   the type up through LowerBound (key copy in the frame slot, the end / greater check against the constant); the opened
//   component is not null-tested; the alive tail keeps a third, discarded character fetch (retail's call is not removable).
// Layout: engine/CPlayer.h (retail STLport layout: PlayerInterface +0x0C, PlayerCharacter +0x34 CIntelligentPointer<
//   CThingPlayerCreature>, InputProcessHeroInformationScreens_ptr +0x138, InputProcessInventoryTradeScreen +0x130), layout-renamed so the private
//   method can be declared; retail CThing UsingComponents (bitset<274>) +0x20, Components (CVectorMap, 0x10) +0x44, Dead
//   (bit 0 of +0x91; Ego_r.pdb +0xA1 with Dinkumware containers). The input-process, inventory-component and thing chains are
//   local polymorphic partial views with their RTTI bases (engine/CAInputProcess.h and engine/CTCBase.h model the vptr as a
//   data member; no CThing header exists).
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
class CTCInventoryTrade : public NInventory::CTCInventoryBase {   // partial view (its second base NHeroInformationScreens::CBase is at +0x130)
public:
    virtual void OpenInventoryPC();                           // slot 86 (+0x158)
    virtual void CloseInventoryPC();                          // slot 87 (+0x15C)
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
class CInputProcessHeroInformationScreens : public CInputProcessGameBase { };
class CInputProcessInventoryBase : public CInputProcessGameBase { };
class CInputProcessInventoryTradeScreen : public CInputProcessInventoryBase { };

enum EPlayerMode { EPlayerMode_Force32 = 0x7fffffff };     // enumerators not needed
class CProcessedInput;
class CPlayer : public CPlayerPdb {
private:
    void InitModeNavigateInventoryTradeScreen(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeNavigateInventoryTradeScreen(EPlayerMode mode, CProcessedInput* input)
{
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCInventoryTrade* inventory = 0;
    character->GetTC(TCI_INVENTORY_TRADE, &inventory);
    inventory->OpenInventoryPC();
    InputProcessHeroInformationScreens_ptr->AddToInterface(*PlayerInterface);
    InputProcessInventoryTradeScreen_ptr->AddToInterface(*PlayerInterface);
    if (character.GetPItem() && !character->IsDead())
        character.GetPItem();
}