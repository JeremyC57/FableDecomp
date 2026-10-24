// Grade: DIFFER(88v85) -- catalog row truncated (85 of 88 bytes, stops before the final `ret 4`); this source is a MATCH against the full 88-byte retail body (qx.py). Re-bound the row to 88 bytes, then land.
// CBasicString<char>::operator<(const CBasicString<char>& other) const -- lexicographic less-than of two strings
// Leaf: ??M?$CBasicString
// Retail: Fable.exe @ 0x00429950, 88 bytes, __thiscall `ret 4`. An inlined three-way Compare walks both buffers in
// step (one pointer plus the constant distance between them), signed char order; operator< is `Compare(...) < 0`,
// which VC7.1 tail-duplicates into each of the three return paths without folding the constant (`xor ecx,ecx ;
// test eax,eax ; setl cl`). Taking the other string's pointer first makes it the loop induction pointer (this
// string is read through the constant distance), as retail. Catalog row: only the first 85 bytes (stops before the last `ret 4`).
template <class T> class CBasicString {
public:
    T* m_pData;
    __forceinline long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};
template class CBasicString<char>;
