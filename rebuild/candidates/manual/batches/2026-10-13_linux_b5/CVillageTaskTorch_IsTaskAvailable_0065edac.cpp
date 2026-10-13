// CVillageTaskTorch::IsTaskAvailable()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065EDAC, oracle length 14 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
//
// Retail bytes:  add ecx,10h ; call CBaseIntelligentPointer::GetPItem ; neg eax ; sbb eax,eax ; inc eax ; ret
// engine/CVillageTaskTorch.h (PDB): +0x10 CIntelligentPointer<CThing const> CreatureAssignedToTask.
// The task is available while no creature is assigned (the intelligent pointer resolves to null).
// The int-width `neg ; sbb ; inc` is the plain `return p == 0;` form (bool via a full register).
#include "engine/CVillageTaskTorch.h"

struct CBaseIntelligentPointerCalls {
    void* GetPItem();                       // 0x00A01B50
};

struct CVillageTaskTorch_Methods : CVillageTaskTorch {
    bool IsTaskAvailable();
};

bool CVillageTaskTorch_Methods::IsTaskAvailable()
{
    return reinterpret_cast<CBaseIntelligentPointerCalls*>(&this->CreatureAssignedToTask[0])->GetPItem() == 0;
}
