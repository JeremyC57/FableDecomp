// _global::UninitCopy_Dword_0082c420(const SDword_0082c420* first, const SDword_0082c420* last, SDword_0082c420* result, const __false_type&) -- STLport __uninitialized_copy over 4-byte elements: placement copy-construct [first, last) into raw storage, return the advanced result
// Leaf: UninitCopy_Dword_0082c420
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082C420, length 38 bytes (+10 int3 pad), `ret 8`.
// Callers: one site, 0x0082CA48 inside the vector reallocation helper 0x0082C980 (catalog "std_Move_UintVector").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,...>>, unsigned int*, ...>`
//   is a propagated VC8-style name; the body is STLport 4.x `__uninitialized_copy(first, last, result,
//   const __false_type&)` in a /Gr TU (`__fastcall`: first in ecx, last in edx, result + the tag reference on the
//   stack). The 4-byte element is not named by a landed relative: a descriptive one-dword record.
// Shape: `cmp ecx,edx` before the result load (the empty-range test hoisted above the loop), the placement-new null
//   test on the destination (`test eax,eax ; je`), one dword copy through esi, both pointers stepped by 4, the
//   advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct SDword_0082c420 {                        // 4 bytes, trivially copyable
    unsigned long m_value;
};

SDword_0082c420* __fastcall UninitCopy_Dword_0082c420(const SDword_0082c420* first, const SDword_0082c420* last, SDword_0082c420* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) SDword_0082c420(*first);
    return result;
}