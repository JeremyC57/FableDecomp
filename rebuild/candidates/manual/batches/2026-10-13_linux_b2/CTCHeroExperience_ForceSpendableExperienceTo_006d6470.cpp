// CTCHeroExperience::ForceSpendableExperienceTo(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6470, oracle length 38 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; jge A ; xor eax,eax ; A: mov [ecx+14h],eax ; mov eax,[ecx+10h] ; mov edx,[ecx+14h] ; cmp eax,edx ; jge B ; mov [ecx+10h],edx ; ret 4 ; B: mov eax,[ecx+10h] ; mov [ecx+10h],eax ; ret 4
// engine/CTCHeroExperience.h (PDB): +0x10 TotalExperiencePoints, +0x14 ExperiencePointsAvailableToSpend.
// Clamps the request at zero, stores it as the spendable amount and keeps the lifetime total at
// least that large. The retail else-branch re-stores TotalExperiencePoints to itself, which is
// what a conditional expression with the member on both arms compiles to.
#include "engine/CTCHeroExperience.h"

struct CTCHeroExperience_Methods : CTCHeroExperience {
    void ForceSpendableExperienceTo(long points);
};

void CTCHeroExperience_Methods::ForceSpendableExperienceTo(long points)
{
    if (points < 0)
        points = 0;
    this->ExperiencePointsAvailableToSpend = points;
    this->TotalExperiencePoints = (this->TotalExperiencePoints < this->ExperiencePointsAvailableToSpend)
                                      ? this->ExperiencePointsAvailableToSpend
                                      : this->TotalExperiencePoints;
}
