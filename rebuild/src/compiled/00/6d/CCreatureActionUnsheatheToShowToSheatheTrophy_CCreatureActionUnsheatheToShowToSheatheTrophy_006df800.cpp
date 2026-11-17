// CCreatureAction_UnsheatheToShowToSheatheTrophy::CCreatureAction_UnsheatheToShowToSheatheTrophy(CThingCreatureBase&, CThing&, const CCharString&) -- base CCreatureAction_UnsheatheItemFromInventory(creature, trophy, no follow-up, 1000), own vtable (0x0125F8D4), the animation name copied at +0xb8, Trophy at +0xbc default-built then set to the trophy
// Leaf: ??0CCreatureAction_UnsheatheToShowToSheatheTrophy@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DF800, length 84 bytes, `ret 0Ch`.
// Identity: Ego_r.pdb 0x0070D1D0 + FableWin.pdb 0x0249AFD0 ??0CCreatureAction_UnsheatheToShowToSheatheTrophy@@QAE@AAVCThingCreatureBase@@AAVCThing@@ABVCCharString@@@Z
//   (engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h); its one caller CTCTrophy::GlobalUpdate 0x006DEEE0 uses that header.
// Callees: CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase&, CThing&, CCreatureActionBase*, long) @ 0x006A0150 (Ego_r.pdb +
//   FableWin.pdb ??0CCreatureAction_UnsheatheItemFromInventory@@QAE@AAVCThingCreatureBase@@AAVCThing@@PAVCCreatureActionBase@@J@Z),
//   CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the
//   CIntelligentPointer<CThing> vtable 0x01238C6C), CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the trophy rides in ebx (used for the base call and the SetPItem), the string argument is read from its slot after the base
//   call, the pointer member is addressed once in edi; Trophy.Set is a body statement after the init list.
// Name fix (agentNF55, 2026-11-17): moved onto the shared headers (references, as both PDBs spell the constructor and its base); it
//   modelled CCharString, the intelligent pointer and the base action locally with pointer parameters.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventory.h"
#include "engine/CCreatureAction_UnsheatheToShowToSheatheTrophy.h"

CCreatureAction_UnsheatheToShowToSheatheTrophy::CCreatureAction_UnsheatheToShowToSheatheTrophy(CThingCreatureBase& creature, CThing& trophy, const CCharString& animation)
    : CCreatureAction_UnsheatheItemFromInventory(creature, trophy, 0, 1000)
    , ShowAnimName(animation)
    , Trophy()
{
    Trophy.Set(&trophy);
}
