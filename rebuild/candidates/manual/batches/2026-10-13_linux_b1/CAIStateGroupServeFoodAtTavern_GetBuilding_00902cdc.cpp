// CAIStateGroup_ServeFoodAtTavern::GetBuilding()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902CDC, oracle length 17 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+4] ; mov eax,[eax+18h] ; lea ecx,[eax+0x170] ; jmp CBaseIntelligentPointer::GetPItem
// Shape template (landed byte-exact): CAIStateGroupCleanHouse::GetBuilding @ 0x00917AD0.
// Walks this->PBrain (+0x04, PDB CAIStateGroupBase) -> brain+0x18 (the owning creature/villager
// record) -> the CIntelligentPointer<CThingBuilding> at +0x170 of that record (the villager's
// tavern building) and resolves it through CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// The smart pointer's class is not modelled here because GetPItem is __thiscall with no stack
// arguments; a __fastcall free function with the pointer in ecx produces the same call site.
#include "engine/CAIStateGroup_ServeFoodAtTavern.h"

struct CThingBuilding;

// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (ecx = the CIntelligentPointer object).
CThingBuilding* __fastcall IntelligentPointer_GetPItem_00a01b50(void* intelligentPointer);

struct CAIStateGroup_ServeFoodAtTavern_Methods : CAIStateGroup_ServeFoodAtTavern {
    CThingBuilding* GetBuilding();
};

CThingBuilding* CAIStateGroup_ServeFoodAtTavern_Methods::GetBuilding()
{
    char* brain   = *(char**)&this->_base_0x04[0];   // CAIStateGroupBase::PBrain (+0x04)
    char* villager = *(char**)(brain + 0x18);          // brain -> villager/creature record
    return IntelligentPointer_GetPItem_00a01b50(villager + 0x170);   // CIntelligentPointer<CThingBuilding>
}
