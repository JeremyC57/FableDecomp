// CTCPerceiveThing::IsAwareOfThing()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643B3B, oracle length 25 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  fld [__real@0] ; fld [ecx+18h] ; fucompp ; fnstsw ax ; test ah,44h ; jnp T ; xor eax,eax ; ret ; T: xor eax,eax ; inc eax ; ret
// engine/CTCPerceiveThing.h (PDB): +0x18 float AwarenessTimer. Aware while the timer is non-zero.
// The 0.0f literal is loaded from .rdata (0x0122DEDC) and the result is materialised as a 32-bit
// 0/1 (xor/inc), i.e. an int-sized boolean rather than a `bool` in al.
#include "engine/CTCPerceiveThing.h"

struct CTCPerceiveThing_Methods : CTCPerceiveThing {
    long IsAwareOfThing();
};

long CTCPerceiveThing_Methods::IsAwareOfThing()
{
    if (this->AwarenessTimer != 0.0f)
        return 1;
    return 0;
}
