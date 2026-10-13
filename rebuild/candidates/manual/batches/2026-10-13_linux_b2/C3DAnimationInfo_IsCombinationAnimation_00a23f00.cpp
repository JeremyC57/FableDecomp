// C3DAnimationInfo::IsCombinationAnimation()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23F00, oracle length 38 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,ecx ; mov ecx,[eax+4] ; mov eax,[eax+8] ; push eax ; call CBankFile::GetEntryDataType ; and eax,0Fh ; cmp eax,7 ; je T ; cmp eax,8 ; je T ; cmp eax,9 ; je T ; xor al,al ; ret ; T: mov al,1 ; ret
// engine/C3DAnimationInfo.h leaves +0x04 and +0x08 unnamed; retail uses them as the owning bank
// (a CBankFile, whose GetEntryDataType @ 0x009CC210 is called on it) and this animation's bank
// entry index. Data types 7, 8 and 9 (low nibble) are the combination-animation kinds.
#include "engine/C3DAnimationInfo.h"

struct CBankFileCalls {
    unsigned long GetEntryDataType(unsigned long entryIndex);   // 0x009CC210
};

struct C3DAnimationInfo_Methods : C3DAnimationInfo {
    bool IsCombinationAnimation();
};

bool C3DAnimationInfo_Methods::IsCombinationAnimation()
{
    CBankFileCalls* bank  = *(CBankFileCalls**)&this->_pad_0x04[0];   // +0x04 owning bank
    unsigned long   entry = *(unsigned long*)&this->_pad_0x08[0];     // +0x08 bank entry index
    unsigned long   type  = bank->GetEntryDataType(entry) & 0xF;
    if (type == 7 || type == 8 || type == 9)
        return true;
    return false;
}
