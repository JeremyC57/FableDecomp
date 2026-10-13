// CTCHeroExperience::ApplyPenaltyForDeath()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6340, oracle length 26 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
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
