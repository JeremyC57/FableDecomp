// CTCInventoryClothing::InitialiseMannequin() -- create the inventory mannequin from the inventory def's clothing doll at the owner, copy the owner's appearance components onto it and start its DEFAULT animation
// Leaf: ?InitialiseMannequin@CTCInventoryClothing@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B37F7, 760 bytes, size-optimised TU, plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?InitialiseMannequin@CTCInventoryClothing@@AAEXXZ (private; caller ConstructScreenPC 0x005B8743).
// Callees (PDB spellings): CThing::GetPos 0x004C73D0, CTCDNone::Create 0x00835A20 (static __fastcall), CThing::AddAndCopyTC(const
//   CTCBase&, bool) 0x004C9F60, the forced-inline CThing::GetTC (bitset<274>::test 0x00410DE0, CVectorMap::LowerBound 0x0040F020),
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CThing::AddTC 0x004C9D60, CDefClassBase::GetSubDef<CAppearanceDef>
//   0x0042B0A2, CAnimationSet::GetAnim 0x005DC340, CAnimationEntry::GetAnimateThingFlags 0x005DC390, CAnimInit::Init 0x0070C050 (the inline
//   argument constructor), CTCAnimationComplex::GetAnimation 0x0070B460, CAnimateThing::SetMainAnim 0x0070D580,
//   CTCHeroAttachableAppearanceModifiers::EditFrameUpdate 0x00707930, the defs' virtual release (slot 1).
// Layout: NInventory::CTCInventoryBase PDef +0x68 (CDefPointer<const CInventoryDef>, GetPDef by value), CInventoryDef ClothingDoll +0xD4
//   (FABLE_RETAIL_CINVENTORYDEF_DOLLS), CTCInventoryClothing PMannequin +0x158, CThing PTCGraphicAppearance +0x64 / PBaseDef +0x70.
// Shape: TCI_SKELETAL_MORPH (0x78) is fetched through HasTC + GetTC(id) (the expanded Find, `cmp [eax],ebx ; jg`), the other three
//   through GetTC<CTCBase>(id, &tc) (`jle`); the graphic-appearance TC is null-initialised and passed whether or not it was found.
// Name fix (agentNF12 2026-11-11): moved off the local CThing / CDefPointer / CThingBaseDef / CAppearanceDef / CAnimationSet / CAnimInit /
// CAnimateThing / CTCInventoryClothing stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers. 0x0042B0A2 binds
// GetSubDef<CAppearanceDef> (was ?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@VCAppearanceDef@@@@@Z), 0x00835A20 CTCDNone::Create
// (was the free ?CreateThing@@YIPAVCThing@@JABVC3DVector@@_N@Z), 0x004C9F60 AddAndCopyTC(const CTCBase&, bool) (was the pointer form),
// 0x005DC340 GetAnim (was GetAnimation returning a pointer), 0x0070C050 CAnimInit::Init (was Set), 0x00707930 EditFrameUpdate on
// CTCHeroAttachableAppearanceModifiers; the row's own symbol is the PDB private AAEXXZ (was QAEXXZ); the def member is ClothingDoll
// (was MannequinDef).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_PDEF
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN
#define FABLE_RETAIL_CINVENTORYDEF_DOLLS
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_COVERS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CANIMATETHING_MAINANIM
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CInventoryDef.h"
#include "engine/CTCDNone.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimateThing.h"
#include "engine/CDefinitionManager.h"

void CTCInventoryClothing::InitialiseMannequin()
{
    if (GetPDef()->ClothingDoll == 0) {
        PMannequin = 0;
        return;
    }
    CThing* owner = ParentThing;
    PMannequin = CTCDNone::Create(GetPDef()->ClothingDoll, owner->GetPos(), true);
    if (PMannequin) {
        PMannequin->AddAndCopyTC(*ParentThing->GetTCGraphicAppearance(), false);
        CTCBase* tc;
        if (ParentThing->GetTC(TCI_APPEARANCE_MORPH, &tc))
            PMannequin->AddAndCopyTC(*tc, false);
        if (ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &tc))
            PMannequin->AddAndCopyTC(*tc, false);
        CThing* parent = ParentThing;
        if (parent->HasTC(TCI_SKELETAL_MORPH)) {
            CTCBase& morph = parent->GetTC(TCI_SKELETAL_MORPH);
            PMannequin->AddAndCopyTC(morph, false);
        }
        CTCBase* basic = 0;
        ParentThing->GetTC(TCI_GRAPHIC_APPEARANCE_BASIC_COMPONENT, &basic);
        PMannequin->AddAndCopyTC(*basic, false);
        PMannequin->AddTC("CTCAnimationComplex", false, 0);
        CTCAnimationComplex* complex = 0;
        PMannequin->GetTC(&complex);
        CDefPointer<const CAppearanceDef> appearance;
        PMannequin->GetPBaseDef()->GetSubDef(&appearance);
        const CAnimationEntry& entry = appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
        CAnimInit init(6, 0, entry.BankIndex, ANIM_UPDATE_FROM_SPEED, true, entry.GetAnimateThingFlags(), 100);
        complex->GetAnimation().SetMainAnim(init, 1, 0);
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (PMannequin->GetTC(&modifiers))
            modifiers->EditFrameUpdate();
    }
}
