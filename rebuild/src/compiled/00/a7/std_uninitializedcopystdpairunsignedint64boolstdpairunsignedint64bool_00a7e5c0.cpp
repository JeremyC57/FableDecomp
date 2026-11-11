// _global::UninitCopy_PairU64Bool_00a7e5c0(const SPairU64Bool_00a7e5c0* first, const SPairU64Bool_00a7e5c0* last, SPairU64Bool_00a7e5c0* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte pair<unsigned __int64, bool>: placement copy-construct key and flag, return the advanced result
// Leaf: UninitCopy_PairU64Bool_00a7e5c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7E5C0, length 50 bytes, `ret 8`.
// Callers: one site, 0x00A82451 in the vector insert-n helper 0x00A82380 (catalog "Vector_InsertN_16byte").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. A 16-byte
//   stride copying two words and one byte is a pair<unsigned __int64, bool> (8-aligned; +9..+0x0F padding).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last
//   in edx, result + the tag reference on the stack). The byte copy needs a byte register, so `last` moves to esi and
//   edx is the temporary (`mov dl,[ecx+8]`). STLport pair's memberwise copy constructor; placement-new null test on
//   the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SPairU64Bool_00a7e5c0 {                  // std::pair<unsigned __int64, bool>, 16 bytes (8-aligned)
    unsigned __int64 first;                     // +0x00
    bool             second;                    // +0x08
    SPairU64Bool_00a7e5c0(const SPairU64Bool_00a7e5c0& o) : first(o.first), second(o.second) { }
};

SPairU64Bool_00a7e5c0* __fastcall UninitCopy_PairU64Bool_00a7e5c0(const SPairU64Bool_00a7e5c0* first, const SPairU64Bool_00a7e5c0* last, SPairU64Bool_00a7e5c0* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SPairU64Bool_00a7e5c0(*first);
    return result;
}