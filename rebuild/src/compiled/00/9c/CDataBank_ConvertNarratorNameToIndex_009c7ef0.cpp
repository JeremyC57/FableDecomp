// CDataBank::ConvertNarratorNameToIndex(const CCharString& name) -- index of `name` in the bank's Narrators array, or -1
// Leaf: ConvertNarratorNameToIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7EF0, length 120 bytes.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`), reached through the inlined CCharString equality
// (same data block, both blocks present with equal lengths, then Compare == 0 as `neg ; sbb al,al ; inc al`).
// Layout: Narrators is the CArray<CCharString> at retail +0x204 (Ego_r.pdb +0x284; retail CArray is 0x0c bytes:
// first/last/end) -- modelled locally.
// Shape: an index loop against Size() (`(last - first) >> 2`, unsigned compare, re-read every iteration because the
// loop calls out) with the element reached through a cursor pointer taken from the first element once (ebx, `add ebx,4`;
// indexing `Narrators[i]` re-reads the array base each iteration); the key's data block is read
// once after the entry test, i.e. copied into a local inside the non-empty arm of the rotated loop (the landed
// 0x004AF740 shape).
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
struct CCharString {
    CCharStringData* m_pData;
};
static __forceinline bool operator==(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pData;
    CCharStringData* b = sb.m_pData;
    if (a == b)
        return true;
    if (!a || !b)
        return false;
    if (a->m_length != b->m_length)
        return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}
struct CCharStringArray {                          // CArray<CCharString>: first/last/end
    CCharString* m_pFirst;
    CCharString* m_pLast;
    CCharString* m_pEnd;
    unsigned long Size() const { return m_pLast - m_pFirst; }
    const CCharString& operator[](unsigned long i) const { return m_pFirst[i]; }
};

struct CDataBank {
    unsigned char    _pad00[0x204];
    CCharStringArray Narrators;                    // +0x204
    long ConvertNarratorNameToIndex(const CCharString& name) const;
};

long CDataBank::ConvertNarratorNameToIndex(const CCharString& name) const
{
    unsigned long i = 0;
    if (i < Narrators.Size()) {
        CCharString key = name;
        const CCharString* it = &Narrators[0];
        do {
            if (*it == key)
                return i;
            ++i;
            ++it;
        } while (i < Narrators.Size());
    }
    return -1;
}