// CTCHeroExperience::ApplyPenaltyForDeath()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6340, oracle length 26 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  push esi ; mov esi,[ecx+14h] ; mov eax,esi ; cdq ; sub eax,edx ; mov edx,[ecx+10h] ; sar eax,1 ; sub esi,eax ; sub edx,eax ; mov [ecx+14h],esi ; mov [ecx+10h],edx ; pop esi ; ret
// engine/CTCHeroExperience.h (PDB): +0x10 TotalExperiencePoints, +0x14 ExperiencePointsAvailableToSpend.
// Dying costs half of the unspent experience; the same amount is removed from the lifetime total
// (signed /2: cdq/sub/sar).
#include "engine/CTCHeroExperience.h"

struct CTCHeroExperience_Methods : CTCHeroExperience {
    void ApplyPenaltyForDeath();
};

void CTCHeroExperience_Methods::ApplyPenaltyForDeath()
{
    long penalty = this->ExperiencePointsAvailableToSpend / 2;
    this->ExperiencePointsAvailableToSpend -= penalty;
    this->TotalExperiencePoints -= penalty;
}
