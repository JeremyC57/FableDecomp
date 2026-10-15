// CTCTavernGameCardPairs::vfunc_57() -- returns the selected card value while exactly one card is turned, else 0
// Retail: Fable.exe @ 0x007D5F90, 18 bytes (vtable 0x0126F6FC slot 57; byte-identical to the
//         CTCTavernGameCardSorting slot-57 override at 0x007D4E10).
// Shape: `mov edx,[ecx+300h] ; xor eax,eax ; dec edx ; jne ; mov eax,[ecx+304h]` -- the `dec ; jne`
//        test on the count is a one-case `switch` into a zero-initialised result local
//        (a `return` inside the case splits the exits; an `== 1` compare emits `cmp edx,1`).
// The retail card-game classes are much larger than the PDB's CTCTavernGame (0x108) -- the derived
// members at +0x300/+0x304 are modelled locally with descriptive names.
struct CTCTavernGameCardPairs {
    unsigned char _pad[0x300];
    long m_numTurnedCards;      // +0x300
    long m_turnedCard;          // +0x304
    long vfunc_57();
};

long CTCTavernGameCardPairs::vfunc_57()
{
    long card = 0;
    switch (m_numTurnedCards) {
    case 1:
        card = m_turnedCard;
        break;
    }
    return card;
}
