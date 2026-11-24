// CAIStateGroup_ServeFoodAtHome::GetBuilding()
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902CA7, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+4] ; mov eax,[eax+18h] ; lea ecx,[eax+0x168] ; jmp CBaseIntelligentPointer::GetPItem
// Shape template (landed byte-exact): CAIStateGroupCleanHouse::GetBuilding @ 0x00917AD0.
// Walks this->PBrain (+0x04, PDB CAIStateGroupBase) -> brain+0x18 (the owning creature/villager
// record) -> the CIntelligentPointer<CThingBuilding> at +0x168 of that record (the villager's
// home building) and resolves it through CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Leaf: ?GetBuilding@CAIStateGroup_ServeFoodAtHome@@MAEPAVCThingBuilding@@XZ
// Class-model move (NF327, 2026-11-24, NF325 Q2 ruling): defined on the real class of engine/CAIStateGroup_ServeFoodAtHome.h under
//   FABLE_RETAIL_CAISTATEGROUP_SERVEFOODATHOME_VIRTUAL_MODEL over the new engine/CAIStateGroup_ServeFood.h opt-in model (slots 14-16 pure, as retail's
//   _purecall slots) instead of the flat alias CAIStateGroup_ServeFoodAtHome_Methods (was ?GetBuilding@CAIStateGroup_ServeFoodAtHome_Methods@@QAEPAVCThingBuilding@@XZ); the retail RTTI
//   vftable placement (slot 15, protected: both PDBs MAE) names this PDB member here. The walk is HEAD's, now starting from the base's
//   CAIStateGroupBase::PBrain: CAIBrain::Creature (+0x18) -> CThingAICreature::HomeBuilding (+0x168, CIntelligentPointer<CThingBuilding>).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_SERVEFOOD_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_SERVEFOODATHOME_VIRTUAL_MODEL
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_ServeFoodAtHome.h"
class CThingBuilding;

// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ecx = the CIntelligentPointer object).
CThingBuilding* CAIStateGroup_ServeFoodAtHome::GetBuilding()
{
    char* villager = *(char**)((char*)PBrain + 0x18);   // CAIBrain::Creature (+0x18), the villager
    return ((CThingBuilding*)(((CBaseIntelligentPointer*)(villager + 0x168))->GetPItem()));   // its home building
}
