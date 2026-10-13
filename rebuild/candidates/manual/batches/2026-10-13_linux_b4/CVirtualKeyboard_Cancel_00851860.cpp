// CVirtualKeyboard::Cancel()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00851860, oracle length 46 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov al,[ecx+5] ; test al,al ; jne R ; mov al,1 ; mov [ecx+5],al ; mov [ecx+4],al ; call <IME singleton> ; mov ecx,eax ; call CIME::IsIMEActive ; test al,al ; je R ; push 0 ; call <IME singleton> ; mov ecx,eax ; call CIME::FinaliseString ; R: ret
// engine/CVirtualKeyboard.h (PDB): +0x04 Canceled, +0x05 Done. Cancelling a keyboard that is not
// done marks it done and cancelled, and if the input-method editor is mid-composition
// (CIME::IsIMEActive @ 0x00404C50) the composition is finalised without committing
// (CIME::FinaliseString(false) @ 0x00404B50). The IME singleton accessor is 0x00404A80 (__cdecl,
// no arguments; its manifest label is a propagated name).
#include "engine/CVirtualKeyboard.h"

struct CIMECalls {
    bool IsIMEActive();                     // 0x00404C50
    void FinaliseString(bool commit);       // 0x00404B50
};

extern CIMECalls* GFGetIME();               // 0x00404A80

struct CVirtualKeyboard_Methods : CVirtualKeyboard {
    void Cancel();
};

void CVirtualKeyboard_Methods::Cancel()
{
    if (!this->Done) {
        this->Done     = true;
        this->Canceled = true;
        if (GFGetIME()->IsIMEActive())
            GFGetIME()->FinaliseString(false);
    }
}
