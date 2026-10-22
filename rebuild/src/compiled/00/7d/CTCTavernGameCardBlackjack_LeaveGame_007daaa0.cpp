// CTCTavernGameCardBlackjack::vfunc_30() -- LeaveGame override: card-base LeaveGame, then clears the card vector at +0x318 (erase(begin(), end()))
// Retail: Fable.exe @ 0x007DAAA0, 59 bytes (vtable 0x0126F8F4 slot 30 = the LeaveGame slot; the
//         CardSorting/CardPairs slot-30 overrides carry the LeaveGame label).
// Callees: CTCTavernGameCardBase::LeaveGame 0x008F20F0, memmove 0x00BFEAE6.
// Shape: Dinkumware `erase(first, last)` = `m_last = copy(last, end(), first)` with the copy-down
//        helper as an early-out (`if (src == srcEnd) return dest;`, GOTCHAS wave 17): `last` and
//        `end()` are the same load, giving the retail `cmp eax,eax ; jne`; the never-taken slow path
//        moves the byte count and adds it unscaled to memmove's result (as the landed
//        CByteVector_00567ac0::Clear). Elements are the card pointers HasGoneBust (0x007DA8B0) reads.
#include <string.h>

struct CTavernCard;

struct CCardVector {                    // retail 12-byte vector<CTavernCard*>
    CTavernCard** m_first;
    CTavernCard** m_last;
    CTavernCard** m_endOfStorage;

    static CTavernCard** CopyDown(CTavernCard** src, CTavernCard** srcEnd, CTavernCard** dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (char*)srcEnd - (char*)src;
        return (CTavernCard**)((char*)memmove(dest, src, bytes) + bytes);
    }
    void Erase(CTavernCard** eraseFirst, CTavernCard** eraseLast)
    {
        m_last = CopyDown(eraseLast, m_last, eraseFirst);
    }
    void Clear() { Erase(m_first, m_last); }
};

struct CTCTavernGameCardBase {
    unsigned char _pad[0x318];
    void LeaveGame();                   // 0x008F20F0
};

struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    CCardVector m_cards;                // +0x318
    void vfunc_30();
};

void CTCTavernGameCardBlackjack::vfunc_30()
{
    CTCTavernGameCardBase::LeaveGame();
    m_cards.Clear();
}