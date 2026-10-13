// C3DAnimationInfo::IsLoaded()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23F50, oracle length 38 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+0Ch] ; test eax,eax ; je F ; mov ecx,[eax+28h] ; test ecx,ecx ; jne T ; mov eax,[eax+30h] ; test eax,eax ; je F ; mov ecx,eax ; call <task finished?> ; test al,al ; je F ; T: mov al,1 ; ret ; F: xor al,al ; ret
// engine/C3DAnimationInfo.h (PDB): +0x0C CIVCountedPointer<CAnimationData>::Object Data_Object.
// Loaded when the animation data exists and either already holds its decoded animation (+0x28)
// or its pending async load task (+0x30) reports completion (0x009D54E0, __thiscall, bool).
#include "engine/C3DAnimationInfo.h"

struct CAsyncLoadTaskCalls {
    bool IsFinished();                      // 0x009D54E0
};

struct CAnimationDataCalls {
    unsigned char        _pad_0x00[0x28];
    void*                Animation;         // +0x28 decoded animation (name inferred)
    unsigned char        _pad_0x2C[4];
    CAsyncLoadTaskCalls* LoadTask;          // +0x30 pending load (name inferred)
};

struct C3DAnimationInfo_Methods : C3DAnimationInfo {
    bool IsLoaded();
};

bool C3DAnimationInfo_Methods::IsLoaded()
{
    CAnimationDataCalls* data = reinterpret_cast<CAnimationDataCalls*>(this->Data_Object);
    if (!data)
        return false;
    if (data->Animation)
        return true;
    CAsyncLoadTaskCalls* task = data->LoadTask;
    if (!task)
        return false;
    return task->IsFinished();
}
