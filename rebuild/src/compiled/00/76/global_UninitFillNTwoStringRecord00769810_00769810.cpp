// _global::UninitFillN_TwoStringRecord_00769810(STwoStringRecord_00769810* first, unsigned int n, const STwoStringRecord_00769810& x, const __false_type&) -- STLport __uninitialized_fill_n over 20-byte {CCharString, CCharString, three longs} records: copy-construct n copies, return first + n
// Leaf: UninitFillN_TwoStringRecord_00769810
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769810, length 72 bytes, `ret 8`.
// Callers: one site, 0x0076B901 in the vector insert/reallocate helper 0x0076B850 (catalog
//   "Vector_Insert_CBrainUpdateZone_MoveAndRealloc").
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice).
// Identity: the catalog label `_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>*, ...>` is
//   propagated. Two strings (+0, +4) followed by three words (+8..+0x10 -- the size of Ego_r.pdb CBrainUpdateZone
//   {Distance, UpdateFrequency, InterruptionFrequency}, after the caller's label; unproven) -- descriptive names.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)`, `__fastcall` (first in ecx -> esi as `cur`,
//   n in edx -> ebx counted down, &x on the stack -> edi). The implicit memberwise copy constructor calls the out-of-line
//   CCharString copy on `this` and on `this+4`, then copies the three words; placement-new null test; cur in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};

struct STwoStringRecord_00769810 {              // 20 bytes
    CCharString m_first;                        // +0x00
    CCharString m_second;                       // +0x04
    long        m_8;                            // +0x08
    long        m_c;                            // +0x0C
    long        m_10;                           // +0x10
};

STwoStringRecord_00769810* __fastcall UninitFillN_TwoStringRecord_00769810(STwoStringRecord_00769810* first, unsigned int n, const STwoStringRecord_00769810& x, const __false_type&)
{
    STwoStringRecord_00769810* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) STwoStringRecord_00769810(x);
    return cur;
}