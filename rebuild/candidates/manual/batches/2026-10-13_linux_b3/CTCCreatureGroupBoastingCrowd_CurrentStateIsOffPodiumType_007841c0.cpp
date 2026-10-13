// CTCCreatureGroupBoastingCrowd::CurrentStateIsOffPodiumType()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007841C0, oracle length 41 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+2Ch] ; test eax,eax ; je T ; cmp eax,1 ; je T ; cmp eax,7 ; je T ; cmp eax,8 ; je T ; cmp eax,9 ; je T ; cmp eax,0Ah ; je T ; xor eax,eax ; ret ; T: mov eax,1 ; ret
// engine/CTCCreatureGroupBoastingCrowd.h (PDB) has CrowdState at +0x30; retail reads it at +0x2C
// (the CIntelligentPointer PHero before it is 4 bytes shorter in the shipped build). States 0, 1
// and 7..10 of EBoastingCrowdState are the "hero is off the podium" states. 32-bit 1/0 result.

struct CTCCreatureGroupBoastingCrowd_Retail {
    unsigned char _pad_0x00[0x2C];
    long          CrowdState;        // +0x2C EBoastingCrowdState (PDB +0x30)
    long CurrentStateIsOffPodiumType();
};

long CTCCreatureGroupBoastingCrowd_Retail::CurrentStateIsOffPodiumType()
{
    long state = this->CrowdState;
    if (state == 0 || state == 1 || state == 7 || state == 8 || state == 9 || state == 10)
        return 1;
    return 0;
}
