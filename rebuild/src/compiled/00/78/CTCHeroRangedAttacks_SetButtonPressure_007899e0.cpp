// CTCHeroRangedAttacks::SetButtonPressure(float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007899E0, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; fld [__real@1.0] ; mov [ecx+34h],eax ; fsub [__real@1e-4] ; fld [esp+4] ; fcomp st(1) ; fnstsw ax ; test ah,41h ; jne K ; fstp [ecx+34h] ; ret 4 ; K: fstp st(0) ; ret 4
// engine/CTCHeroRangedAttacks.h (PDB): +0x34 float ButtonPressure. Stores the pressure and caps it
// just below full: anything above 1.0f - 0.0001f (constants at 0x0122DED8 / 0x0129BA3C) is
// replaced by that cap, so a fully-held trigger never reads as exactly 1.0.
//
// Lane feedback: the cap is `1.0f - <extern epsilon>` evaluated at run time; folding it to a literal
// (0x3F7FF972) is 9 bytes shorter. RELOCATION_MATCH.
#include "engine/CTCHeroRangedAttacks.h"

// Shared float epsilon in .rdata; declared extern so the compiler cannot fold `1.0f - epsilon`
// (retail computes it at run time: `fld 1.0 ; fsub [0129BA3Ch]`).
extern const float g_FloatEpsilon;      // 0x0129BA3C

struct CTCHeroRangedAttacks_Methods : CTCHeroRangedAttacks {
    void SetButtonPressure(float pressure);
};

void CTCHeroRangedAttacks_Methods::SetButtonPressure(float pressure)
{
    // Store the pressure, then cap it just below full so a held trigger never reads exactly 1.0.
    this->ButtonPressure = pressure;
    float maximum = 1.0f - g_FloatEpsilon;
    if (pressure > maximum)
        this->ButtonPressure = maximum;
}