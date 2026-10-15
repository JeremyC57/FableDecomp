// CTCTavernGameCardBlackjack::HasGoneBust() -- sums the current hand (aces count 11), demotes aces to 1 while over 21, and returns whether the hand is still over 21
// Retail: Fable.exe @ 0x007DA8B0, 71 bytes, no callees.
// +0x33C points at the hand being scored (one of the three 12-byte card vectors the constructor at
// 0x007DB280 zeroes at +0x318/+0x324/+0x330); each element is a card pointer whose value sits at +8
// (11 = ace).
// Shape: `xor eax,eax ; cmp ecx,15h ; setg al` is a `long` result (a bool drops the xor); the
//        ace-demotion loop is a top-tested `while (total > 21 && aces > 0)` that VC rotates into
//        `cmp ; jle` + `test edi,edi ; jle ... jg` (with the aligned loop head `nop`). The hand
//        pointer must be a named local: reading the member twice in the loop header makes VC reuse
//        ecx for it and puts the total in edx (retail: pointer in edx, total in ecx).
struct CTavernCard {
    unsigned char _pad[8];
    long m_value;                       // +0x08 (11 = ace)
};

struct CCardHand {
    CTavernCard** m_first;              // vector<CTavernCard*> begin
    CTavernCard** m_last;               // end
    CTavernCard** m_endOfStorage;
};

struct CTCTavernGameCardBlackjack {
    unsigned char _pad[0x33c];
    CCardHand* m_pCurrentHand;          // +0x33c
    long HasGoneBust();
};

long CTCTavernGameCardBlackjack::HasGoneBust()
{
    CCardHand* hand = m_pCurrentHand;
    long total = 0;
    long aces = 0;
    for (CTavernCard** it = hand->m_first; it != hand->m_last; ++it) {
        long value = (*it)->m_value;
        if (value == 11)
            ++aces;
        total += value;
    }
    while (total > 21 && aces > 0) {
        total -= 10;
        --aces;
    }
    return total > 21;
}
