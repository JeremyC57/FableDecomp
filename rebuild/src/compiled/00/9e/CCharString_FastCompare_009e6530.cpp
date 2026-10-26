// CCharString::FastCompare(const CCharString&) const -- cheap strict ordering for sorted containers: by block identity, then by length, then by the text compared one 32-bit word at a time
// Leaf: ?FastCompare@CCharString
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6530, 108 bytes (`ret 4`, bool in al).
// Callees: none.
// Shape: the same block (or both unassigned) is "not less"; an unassigned string sorts before any assigned one; a
// shorter string sorts first; equal lengths compare the buffers as unsigned dwords (the word read may run up to
// three bytes past the terminator inside the allocation), the first differing word deciding. VC7.1 strength-
// reduces the indexed loop to a byte counter (edi) plus one advancing pointer with the other buffer addressed
// through their difference (ebx). The order is not lexicographic -- it only needs to be strict and consistent.
// CCharString model as in the landed 0x0099FA50: a 4-byte handle on a packed 17-byte CCharStringData.
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    bool FastCompare(const CCharString& other) const;
};

bool CCharString::FastCompare(const CCharString& other) const
{
    const CCharStringData* b = other.PStringData;
    const CCharStringData* a = PStringData;
    if (a == b)
        return false;
    if (a == 0)
        return true;
    if (b == 0)
        return false;
    unsigned long length = a->Data.StringLength;
    if (length < b->Data.StringLength)
        return true;
    if (length > b->Data.StringLength)
        return false;
    const char* textA = a->Data.PData;
    const char* textB = b->Data.PData;
    for (unsigned long i = 0; i < length; i += 4) {
        unsigned long wordB = *(const unsigned long*)(textB + i);
        unsigned long wordA = *(const unsigned long*)(textA + i);
        if (wordA < wordB)
            return true;
        if (wordA > wordB)
            return false;
    }
    return false;
}