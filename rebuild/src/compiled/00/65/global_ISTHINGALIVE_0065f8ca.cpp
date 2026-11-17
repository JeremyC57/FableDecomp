// IS_THING_ALIVE<CIntelligentPointer<CThingBuilding> >(const CIntelligentPointer<CThingBuilding>&) -- true when the pointer is set and its building is not dead
// Leaf: ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingBuilding@@@@@@YI_NABV?$CIntelligentPointer@VCThingBuilding@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F8CA, 36 bytes, __fastcall (the reference in ecx):
//   push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ;
//   mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret   (CBaseIntelligentPointer::GetPItem 0x00A01B50 twice; CThing Dead +0x91 bit 0).
// Identity: FableWin.pdb 0x01D7C940 ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingBuilding@@@@@@YI_NABV?$CIntelligentPointer@VCThingBuilding@@@@@Z.
//   Second source: every caller passes a PDB CIntelligentPointer<CThingBuilding> -- CTCVillage::SetVillageLimbo 0x0065C36E (VillageBuildings,
//   list<CIntelligentPointer<CThingBuilding> >), CTCDummyVillager::OnInitialActivate 0x00730070 (HomeBuilding / WorkBuilding),
//   CAIStateGroup_GoToPub::IsRunnable 0x00949AB0 (PPub); all three landed callers bind this spelling.
// Name fix (agentNF55, 2026-11-17): re-authored as the explicit instance of engine/ThingAlive.h's IS_THING_ALIVE<T> over the shared
//   engine/CIntelligentPointer.h / engine/CThingBuilding.h; it was the stand-in ?IsThingAlive_0065f8ca@@YI_NAAV?$CIntelligentPointer@VCThing@@@@@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBuilding.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"

template bool FABLE_FASTCALL IS_THING_ALIVE<CIntelligentPointer<CThingBuilding> >(const CIntelligentPointer<CThingBuilding>& thing);
