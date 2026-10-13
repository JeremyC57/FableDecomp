// CResourceBank::FrameEnd()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC430, oracle length 15 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
//
// Retail bytes:  inc [ecx+30h] ; xor eax,eax ; mov [ecx+3Ch],eax ; inc [ecx+6Ch] ; mov [ecx+78h],eax ; ret
// PDB CResourceBank: +0x04 CResourceList ResourceList, +0x40 CResourceList LoadingQueue (0x3C bytes
// each). engine/CResourceList.h: +0x2C ulong CurrentFrame, +0x38 ulong UnloadedThisFrame. Both lists
// step their frame counter and clear this frame's unload tally.
#include "engine/CResourceList.h"

// Per-list frame step, inlined twice: writing the four statements out on the bank hoists both
// loads above the stores (23 bytes); the member-function form keeps retail's `inc [mem]` pairs.
struct CResourceList_Methods : CResourceList {
    void FrameEnd() { CurrentFrame++; UnloadedThisFrame = 0; }
};

struct CResourceBank_Retail {
    void*                 __vftable;        // +0x00
    CResourceList_Methods ResourceList;     // +0x04
    CResourceList_Methods LoadingQueue;     // +0x40
    void FrameEnd();
};

void CResourceBank_Retail::FrameEnd()
{
    this->ResourceList.FrameEnd();
    this->LoadingQueue.FrameEnd();
}
