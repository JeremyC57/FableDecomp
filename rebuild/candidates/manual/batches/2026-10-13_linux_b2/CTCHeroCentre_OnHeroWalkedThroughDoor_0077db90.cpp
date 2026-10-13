// CTCHeroCentre::OnHeroWalkedThroughDoor(EHeroCentreDoorType)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077DB90, oracle length 36 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; dec eax ; push esi ; mov esi,ecx ; jne R ; call <get hero> ; mov cl,[eax+0D6h] ; test cl,cl ; je R ; mov ecx,esi ; call <door reaction> ; R: pop esi ; ret 4
// Only door type 1 (the guild's main door) is interesting: fetch the hero creature through the
// global accessor at 0x00686A80 (__cdecl, no arguments) and, if the hero's flag byte at +0xD6 is
// set, run the reaction at 0x0077D4B0 (__thiscall, no arguments). Member names are inferred.
#include "engine/CTCHeroCentre.h"

typedef long EHeroCentreDoorType;

struct CHeroCreatureCalls {
    unsigned char _pad_0x00[0xD6];
    bool          GuildEntryPending;    // +0xD6 (name inferred from use)
};

extern CHeroCreatureCalls* GFGetHeroCreature();    // 0x00686A80

struct CTCHeroCentre_Methods : CTCHeroCentre {
    void OnHeroEnteredGuildDoor();      // 0x0077D4B0
    void OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType);
};

void CTCHeroCentre_Methods::OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType)
{
    if (doorType == 1) {
        CHeroCreatureCalls* hero = GFGetHeroCreature();
        if (hero->GuildEntryPending)
            OnHeroEnteredGuildDoor();
    }
}
