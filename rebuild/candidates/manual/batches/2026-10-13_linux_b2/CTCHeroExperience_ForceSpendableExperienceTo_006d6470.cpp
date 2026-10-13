// CTCHeroExperience::ForceSpendableExperienceTo(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D6470, oracle length 38 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; jge A ; xor eax,eax ; A: mov [ecx+14h],eax ; mov eax,[ecx+10h] ; mov edx,[ecx+14h] ; cmp eax,edx ; jge B ; mov [ecx+10h],edx ; ret 4 ; B: mov eax,[ecx+10h] ; mov [ecx+10h],eax ; ret 4
// engine/CTCHeroExperience.h (PDB): +0x10 TotalExperiencePoints, +0x14 ExperiencePointsAvailableToSpend.
// Clamps the request at zero, stores it as the spendable amount and keeps the lifetime total at
// least that large. The retail else-branch re-stores TotalExperiencePoints to itself, which is
// what a conditional expression with the member on both arms compiles to.
//
// Lane feedback: the self-store and the re-read of the just-written member are the signature of a
// const-reference max template (std::max); a macro or `?:` on the members forwards. MATCH.
#include "engine/CTCHeroExperience.h"

// std::max as Dinkumware VC7.1 spells it: const-reference parameters, `left < right ? right : left`.
// The const references stop VC7.1 forwarding the store to ExperiencePointsAvailableToSpend into
// the compare, and the `left` arm re-reads TotalExperiencePoints and stores it back on itself --
// both visible in the retail bytes (`mov eax,[ecx+10h] ; mov [ecx+10h],eax`).
template<class T> inline const T& Max(const T& left, const T& right) { return (left < right) ? right : left; }

struct CTCHeroExperience_Methods : CTCHeroExperience {
    void ForceSpendableExperienceTo(long points);
};

void CTCHeroExperience_Methods::ForceSpendableExperienceTo(long points)
{
    if (points < 0)
        points = 0;
    this->ExperiencePointsAvailableToSpend = points;
    this->TotalExperiencePoints = Max(this->TotalExperiencePoints, this->ExperiencePointsAvailableToSpend);
}
