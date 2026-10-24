// _global::UninitCopy_NamedRecord12_00786e40(const SNamedRecord12_00786e40* first, const SNamedRecord12_00786e40* last, SNamedRecord12_00786e40* result, const __false_type&) -- STLport __uninitialized_copy over 12-byte {long, CCharString, long} records: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_NamedRecord12_00786e40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786E40, length 60 bytes, `ret 8`.
// Callers: 0x0078723E / 0x007872EA in the landed vector<12-byte>::operator= 0x007871D0 (catalog "CCombatWheel_Assign",
//   modelled there by size only) and 0x00786FCC.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The record is a
//   word, a CCharString (+4) and a word (+8) with an implicit memberwise copy constructor; Ego_r.pdb has several 12-byte
//   records of that shape (e.g. CTCSkeletalMorph::CSkeletalMorph {ID, Name, Strength}), none proven here -- descriptive.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx -> esi,
//   last in edx -> ebx, result + the tag reference on the stack -> edi). The named `cur = result` copy makes VC7.1 load
//   result into edi before the empty-range test and return it from the one shared epilogue (GOTCHAS 009f6c00). The
//   implicit copy constructor copies the first word, calls the out-of-line CCharString copy on `this+4`, then copies the
//   last word; placement-new null test on the destination.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct CCharString {
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other);      // 0x0099EC30
};

struct SNamedRecord12_00786e40 {                // 12 bytes
    long        m_id;                           // +0x00
    CCharString m_name;                         // +0x04
    long        m_value;                        // +0x08
};

SNamedRecord12_00786e40* __fastcall UninitCopy_NamedRecord12_00786e40(const SNamedRecord12_00786e40* first, const SNamedRecord12_00786e40* last, SNamedRecord12_00786e40* result, const __false_type&)
{
    SNamedRecord12_00786e40* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SNamedRecord12_00786e40(*first);
    return cur;
}