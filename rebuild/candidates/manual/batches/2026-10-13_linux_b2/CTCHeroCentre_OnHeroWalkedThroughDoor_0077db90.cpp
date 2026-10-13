// CTCHeroCentre::OnHeroWalkedThroughDoor(EHeroCentreDoorType)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077DB90, oracle length 36 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; dec eax ; push esi ; mov esi,ecx ; jne R ; call <get hero> ; mov cl,[eax+0D6h] ; test cl,cl ; je R ; mov ecx,esi ; call <door reaction> ; R: pop esi ; ret 4
// Only door type 1 (the guild's main door) is interesting: fetch the hero creature through the
// global accessor at 0x00686A80 (__cdecl, no arguments) and, if the hero's flag byte at +0xD6 is
// set, run the reaction at 0x0077D4B0 (__thiscall, no arguments). Member names are inferred.
//
// Lane feedback: retail `mov eax,[esp+4] ; dec eax ; jne` is a one-case switch, not an `==` test. RELOCATION_MATCH.
#include "engine/CTCHeroCentre.h"

typedef long EHeroCentreDoorType;

// Call-surface view of the hero creature; +0xD6 is a pending-guild-entry flag (name inferred).
struct CHeroCreatureCalls {
    unsigned char _pad_0x00[0xD6];
    bool          GuildEntryPending;    // +0xD6
};

extern CHeroCreatureCalls* GFGetHeroCreature();    // 0x00686A80

struct CTCHeroCentre_Methods : CTCHeroCentre {
    void OnHeroEnteredGuildDoor();      // 0x0077D4B0
    void OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType);
};

void CTCHeroCentre_Methods::OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType)
{
    // `dec eax ; jne` is the compiler's single-case switch (case 1); `if (doorType == 1)`
    // emits `cmp dword ptr [esp+4],1` instead.
    switch (doorType) {
    case 1:
        if (GFGetHeroCreature()->GuildEntryPending)
            OnHeroEnteredGuildDoor();
        break;
    default:
        break;
    }
}
