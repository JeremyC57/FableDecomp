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
// The smart pointer's class is not modelled here because GetPItem is __fastcall with no stack
// arguments; a __fastcall free function with the pointer in ecx produces the same call site.
#include "engine/CAIStateGroup_ServeFoodAtHome.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingBuilding;

// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ecx = the CIntelligentPointer object).

struct CAIStateGroup_ServeFoodAtHome_Methods : CAIStateGroup_ServeFoodAtHome {
    CThingBuilding* GetBuilding();
};

CThingBuilding* CAIStateGroup_ServeFoodAtHome_Methods::GetBuilding()
{
    char* brain   = *(char**)&this->_base_0x04[0];   // CAIStateGroupBase::PBrain (+0x04)
    char* villager = *(char**)(brain + 0x18);          // brain -> villager/creature record
    return ((CThingBuilding*)(((CBaseIntelligentPointer*)(villager + 0x168))->GetPItem()));   // CIntelligentPointer<CThingBuilding>
}