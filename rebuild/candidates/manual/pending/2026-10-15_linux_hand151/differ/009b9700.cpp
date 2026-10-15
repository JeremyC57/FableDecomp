// CParsedItem::InitAsInteger(const char*, unsigned long) -- parse `length` characters as a decimal integer (a '-' anywhere flags it negative), set Type to the integer kind
// Grade: DIFFER(91v103) -- zero-length path: retail threads the known-false minus flag (separate `mov [ecx],3 ; ret` exit, bl zeroed by `xor bl,bl` with edx as the shared zero/index) where the lane keeps one tail with `cmp bl,1`; also `[edx+edi+1]` vs `[edi+edx+1]` operand order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B9700, length 103 bytes (padded to 112).
// Callees: none.
// Layout: Ego_r.pdb CParsedItem (12 bytes): Type +0 (EParsedItemType; 3 = integer), the value union +4 (long Integer /
// float Float), CCharString String +8.
// Shape: the union word is zeroed first and accumulated in memory (`Integer = Integer * 10 + c - '0'` re-reads it
// each step: lea x5 / lea x2-0x30); the minus flag is a `bool` local in bl tested with `== true` (`cmp bl,1`);
// Type is stored after the loop on both paths (VC duplicates the store into the zero-length exit).
enum EParsedItemType { PIT_NONE, PIT_STRING, PIT_QUOTED_STRING, PIT_INTEGER, PIT_FLOAT };

struct CCharString {
    void* m_pData;
};

struct CParsedItem {
    EParsedItemType Type;                   // +0
    union {
        long  Integer;                      // +4
        float Float;
    };
    CCharString String;                     // +8
    void InitAsInteger(const char* text, unsigned long length);
};

void CParsedItem::InitAsInteger(const char* text, unsigned long length)
{
    Integer = 0;
    unsigned long i = 0;
    bool negative = false;
    char c = text[0];
    for (; i < length; ++i) {
        if (c == '-')
            negative = true;
        else
            Integer = Integer * 10 + c - '0';
        c = text[i + 1];
    }
    Type = PIT_INTEGER;
    if (negative == true)
        Integer = -Integer;
}
