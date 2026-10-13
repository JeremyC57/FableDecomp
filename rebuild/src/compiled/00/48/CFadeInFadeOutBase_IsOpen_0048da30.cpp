// CFadeInFadeOutBase::IsOpen()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DA30, oracle length 28 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  cmp byte ptr [ecx],0 ; je F ; mov al,[ecx+0Dh] ; test al,al ; jne F ; mov al,[ecx+0Ch] ; test al,al ; jne F ; mov eax,1 ; ret ; F: xor eax,eax ; ret
// engine/CFadeInFadeOutBase.h (PDB): +0x00 bool Active, +0x0C bool Closing, +0x0D bool Opening.
// Fully open = active and neither fading in (Opening) nor fading out (Closing). The result is
// materialised as a 32-bit 1/0 (`mov eax,1`), i.e. an int-sized boolean.
#include "engine/CFadeInFadeOutBase.h"

struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    long IsOpen();
};

long CFadeInFadeOutBase_Methods::IsOpen()
{
    if (this->Active && !this->Opening && !this->Closing)
        return 1;
    return 0;
}