// _global::UninitCopy_Record12_00a7e990(const SRecord12_00a7e990* first, const SRecord12_00a7e990* last, SRecord12_00a7e990* result, const __false_type&) -- STLport __uninitialized_copy over 12-byte records: placement copy-construct [first, last) word by word, return the advanced result
// Leaf: UninitCopy_Record12_00a7e990
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7E990, length 50 bytes (+14 int3 pad), `ret 8`.
// Callers: one site, 0x00A8095B in the vector<12-byte record>::operator= at 0x00A80860 (`imul 2AAAAAABh ; sar edx,1` =
//   divide by 12; no element destructor calls, so the record is trivially destructible).
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated; the record
//   is not named by a landed relative -- a descriptive three-word record.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last
//   in edx, result + the tag reference on the stack). The three dword moves go through ONE temporary (esi) because the
//   record has a memberwise user copy constructor -- an implicit (trivial) copy of a 12-byte POD becomes an esi/edi
//   block copy (GOTCHAS wave 13). Placement-new null test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord12_00a7e990 {                     // 12 bytes
    long m_0, m_4, m_8;
    SRecord12_00a7e990(const SRecord12_00a7e990& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8) { }
};

SRecord12_00a7e990* __fastcall UninitCopy_Record12_00a7e990(const SRecord12_00a7e990* first, const SRecord12_00a7e990* last, SRecord12_00a7e990* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SRecord12_00a7e990(*first);
    return result;
}