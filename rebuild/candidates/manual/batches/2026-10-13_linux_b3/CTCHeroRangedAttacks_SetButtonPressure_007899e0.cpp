// CTCHeroRangedAttacks::SetButtonPressure(float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007899E0, oracle length 43 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; fld [__real@1.0] ; mov [ecx+34h],eax ; fsub [__real@1e-4] ; fld [esp+4] ; fcomp st(1) ; fnstsw ax ; test ah,41h ; jne K ; fstp [ecx+34h] ; ret 4 ; K: fstp st(0) ; ret 4
// engine/CTCHeroRangedAttacks.h (PDB): +0x34 float ButtonPressure. Stores the pressure and caps it
// just below full: anything above 1.0f - 0.0001f (constants at 0x0122DED8 / 0x0129BA3C) is
// replaced by that cap, so a fully-held trigger never reads as exactly 1.0.
#include "engine/CTCHeroRangedAttacks.h"

struct CTCHeroRangedAttacks_Methods : CTCHeroRangedAttacks {
    void SetButtonPressure(float pressure);
};

void CTCHeroRangedAttacks_Methods::SetButtonPressure(float pressure)
{
    this->ButtonPressure = pressure;
    float maximum = 1.0f - 0.0001f;
    if (pressure > maximum)
        this->ButtonPressure = maximum;
}
