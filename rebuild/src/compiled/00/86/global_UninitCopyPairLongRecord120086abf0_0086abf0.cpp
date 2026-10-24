// _global::UninitCopy_PairLongRecord12_0086abf0(const SPairLongRecord12_0086abf0* first, const SPairLongRecord12_0086abf0* last, SPairLongRecord12_0086abf0* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte pair<long, 12-byte POD>: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_PairLongRecord12_0086abf0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086ABF0, length 65 bytes (+15 int3 pad), `ret 8`.
// Callers: 0x0086CC9B / 0x0086CCFA (vector reallocation 0x0086CC50, `sar eax,4` = 16-byte elements) and 0x0086CDE2 /
//   0x0086CE47 (insert-range 0x0086CD70, catalog "Insert_range_C3DClothQuad").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. A key word
//   copied directly followed by a 12-byte block copy is STLport's pair<4-byte key, trivially copyable 12-byte value>
//   (GOTCHAS chunk 354: "STLport pair user ctor = key then `lea` block"); the value (a C3DVector-sized record) is not
//   named by a landed relative -- descriptive names.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last in
//   edx, result + the tag reference on the stack). The pair's memberwise copy constructor copies the key through edi
//   and the value as an inline block (`mov ebx,esi` / `lea edi,[eax+4]`, ebp the temporary; esi the strength-reduced
//   source + 4); placement-new null test on the destination; the advanced result returned in eax. The named `cur`
//   copy of result keeps the destination block address a per-iteration `lea edi,[eax+4]`; looping on `result` itself
//   strength-reduces it into a fourth induction register and spills `last` (78 v 65 bytes).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SRecord12_0086abf0 {                     // 12-byte trivially copyable value
    long m_0, m_4, m_8;
};

struct SPairLongRecord12_0086abf0 {             // _STL::pair<long, SRecord12>, 16 bytes
    long               first;                   // +0x00
    SRecord12_0086abf0 second;                  // +0x04
    SPairLongRecord12_0086abf0(const SPairLongRecord12_0086abf0& o) : first(o.first), second(o.second) { }
};

SPairLongRecord12_0086abf0* __fastcall UninitCopy_PairLongRecord12_0086abf0(const SPairLongRecord12_0086abf0* first, const SPairLongRecord12_0086abf0* last, SPairLongRecord12_0086abf0* result, const __false_type&)
{
    SPairLongRecord12_0086abf0* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SPairLongRecord12_0086abf0(*first);
    return cur;
}