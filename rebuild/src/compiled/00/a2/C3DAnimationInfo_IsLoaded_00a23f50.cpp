// C3DAnimationInfo::IsLoaded()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23F50, oracle length 38 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+0Ch] ; test eax,eax ; je F ; mov ecx,[eax+28h] ; test ecx,ecx ; jne T ; mov eax,[eax+30h] ; test eax,eax ; je F ; mov ecx,eax ; call <task finished?> ; test al,al ; je F ; T: mov al,1 ; ret ; F: xor al,al ; ret
// engine/C3DAnimationInfo.h (PDB): +0x0C CIVCountedPointer<CAnimationData>::Object Data_Object.
// Loaded when the animation data exists and either already holds its decoded animation (+0x28)
// or its pending async load task (+0x30) reports completion (0x009D54E0, __fastcall, bool).
//
// Lane feedback: one `&&`/`||` expression reproduces the shared true/false exits; separate early returns
// differ. RELOCATION_MATCH.
#include "engine/C3DAnimationInfo.h"

class CBankFileAsyncData { public:
public:
    bool IsFinished() const;   // 0x009D54E0
};

// Call-surface view of C3DAnimationInfo::CAnimationData (counted pointee at this->Data_Object).
struct CAnimationDataCalls {
    unsigned char        _pad_0x00[0x28];
    void*                Animation;         // +0x28 decoded animation (name inferred)
    unsigned char        _pad_0x2C[4];
    CBankFileAsyncData* LoadTask;          // +0x30 pending load (name inferred)
};

struct C3DAnimationInfo_Methods : C3DAnimationInfo {
    bool IsLoaded();
};

bool C3DAnimationInfo_Methods::IsLoaded()
{
    // Loaded when the data block exists and either the decoded animation is present or its
    // async load task has finished. One expression => retail's shared `mov al,1` / `xor al,al`
    // exits; early returns duplicate the exits (34 bytes).
    CAnimationDataCalls* data = reinterpret_cast<CAnimationDataCalls*>(this->Data_Object);
    if (data && (data->Animation || (data->LoadTask && data->LoadTask->IsFinished())))
        return true;
    return false;
}