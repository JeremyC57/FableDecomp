// _global::UninitCopy_PairU64Long_00a7e630(const SPairU64Long_00a7e630* first, const SPairU64Long_00a7e630* last, SPairU64Long_00a7e630* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte pair<unsigned __int64, long>: placement copy-construct the key and value (not the tail padding), return the advanced result
// Leaf: UninitCopy_PairU64Long_00a7e630
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7E630, length 50 bytes, `ret 8`.
// Callers: one site, 0x00A82551 in the vector insert-n helper 0x00A82480 (catalog "Vector_InsertN_16ByteElements").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The 16-byte
//   stride copying only +0/+4/+8 is a pair<unsigned __int64, 4-byte value>: the pair is 8-aligned, so the word at +0x0C
//   is padding that a memberwise copy never touches (GOTCHAS: pair<u64, X> is 8-aligned). Descriptive name.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last
//   in edx, result + the tag reference on the stack). STLport's pair has an explicit memberwise copy constructor, so the
//   key's two words and the value go through one temporary (esi) instead of a 16-byte block copy. Placement-new null
//   test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SPairU64Long_00a7e630 {                  // std::pair<unsigned __int64, long>, 16 bytes (8-aligned)
    unsigned __int64 first;                     // +0x00
    long             second;                    // +0x08 (+0x0C padding)
    SPairU64Long_00a7e630(const SPairU64Long_00a7e630& o) : first(o.first), second(o.second) { }
};

SPairU64Long_00a7e630* __fastcall UninitCopy_PairU64Long_00a7e630(const SPairU64Long_00a7e630* first, const SPairU64Long_00a7e630* last, SPairU64Long_00a7e630* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SPairU64Long_00a7e630(*first);
    return result;
}