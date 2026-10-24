// _global::UninitCopy_PairByteLong_00bbd520(const SPairByteLong_00bbd520* first, const SPairByteLong_00bbd520* last, SPairByteLong_00bbd520* result, const __false_type&) -- STLport __uninitialized_copy over 8-byte {byte, long} pairs: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_PairByteLong_00bbd520
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBD520, length 44 bytes (+4 int3 pad), `ret 8`.
// Callers: 0x00BBD9B4 / 0x00BBDA18 in the vector insert-fill helper 0x00BBD950.
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. An 8-byte
//   element copied as one byte (+0) and one dword (+4) is a pair<1-byte, 4-byte> with STLport's memberwise copy
//   constructor (the 3 padding bytes are never copied); descriptive name.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack); `last` moves to esi so edx can carry the byte; placement-new null test
//   on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SPairByteLong_00bbd520 {                 // _STL::pair<bool, long>, 8 bytes
    bool first;                                 // +0x00
    long second;                                // +0x04
    SPairByteLong_00bbd520(const SPairByteLong_00bbd520& o) : first(o.first), second(o.second) { }
};

SPairByteLong_00bbd520* __fastcall UninitCopy_PairByteLong_00bbd520(const SPairByteLong_00bbd520* first, const SPairByteLong_00bbd520* last, SPairByteLong_00bbd520* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SPairByteLong_00bbd520(*first);
    return result;
}