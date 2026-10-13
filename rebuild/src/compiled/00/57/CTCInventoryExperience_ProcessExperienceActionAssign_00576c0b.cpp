#pragma optimize("s",on)
// CTCInventoryExperience::ProcessExperienceActionAssign()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00576C0B, oracle length 46 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; xor esi,esi ; L1: push esi ; mov ecx,edi ; call SpendExperienceOnStat ; inc esi ; cmp esi,0Ah ; jl L1 ; xor esi,esi ; inc esi ; L2: push esi ; mov ecx,edi ; call SpendExperienceOnAbility ; inc esi ; cmp esi,14h ; jl L2 ; mov ecx,edi ; pop edi ; pop esi ; jmp UpdateExperienceSpendLevelUp
// Experience screen "assign" action: spend the queued experience on every trainable stat (0..9,
// EHeroTrainableStatType) and every ability (1..19, EHeroAbility; 0 is "none"), then refresh the
// level-up display (0x005721FA, tail call). Callee names are the manifest's.
#include "engine/CTCInventoryExperience.h"

struct CTCInventoryExperience_Methods : CTCInventoryExperience {
    void SpendExperienceOnStat(long stat);          // 0x005746F7 (EHeroTrainableStatType)
    void SpendExperienceOnAbility(long ability);    // 0x005747A3 (EHeroAbility)
    void UpdateExperienceSpendLevelUp();            // 0x005721FA
    void ProcessExperienceActionAssign();
};

void CTCInventoryExperience_Methods::ProcessExperienceActionAssign()
{
    long i;
    for (i = 0; i < 10; ++i)
        SpendExperienceOnStat(i);
    for (i = 1; i < 20; ++i)
        SpendExperienceOnAbility(i);
    UpdateExperienceSpendLevelUp();
}