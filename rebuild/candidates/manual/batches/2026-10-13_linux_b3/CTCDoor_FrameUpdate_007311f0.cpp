// CTCDoor::FrameUpdate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007311F0, oracle length 41 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,ecx ; call <update animation> ; mov eax,[esi+20h] ; mov eax,[eax+28h] ; test eax,eax ; jne A ; mov ecx,esi ; pop esi ; jmp <person trigger> ; A: cmp eax,1 ; jne R ; mov ecx,esi ; pop esi ; jmp <script trigger> ; R: pop esi ; ret
// engine/CTCDoor.h (PDB): +0x20 PDef_Object (CDefPointer<CDoorDef>). Per frame: advance the door
// animation (0x007308F0), then dispatch on the def's trigger type (+0x28 of CDoorDef): 0 = opened
// by nearby people (0x00730F90), 1 = opened by script (0x007310C0); other types do nothing. The
// callee names are the manifest's (CTCDoor::UpdateAnimation / UpdateOpenStatusTriggerType*).
#include "engine/CTCDoor.h"

struct CDoorDefCalls {
    unsigned char _pad_0x00[0x28];
    long          TriggerType;           // +0x28 EDoorTriggerType (name inferred)
};

struct CTCDoor_Methods : CTCDoor {
    void UpdateAnimation();                       // 0x007308F0
    void UpdateOpenStatusTriggerTypePerson();     // 0x00730F90
    void UpdateOpenStatusTriggerTypeScript();     // 0x007310C0
    void FrameUpdate();
};

void CTCDoor_Methods::FrameUpdate()
{
    UpdateAnimation();
    long triggerType = reinterpret_cast<CDoorDefCalls*>(this->PDef_Object)->TriggerType;
    if (triggerType == 0) {
        UpdateOpenStatusTriggerTypePerson();
        return;
    }
    if (triggerType == 1)
        UpdateOpenStatusTriggerTypeScript();
}
