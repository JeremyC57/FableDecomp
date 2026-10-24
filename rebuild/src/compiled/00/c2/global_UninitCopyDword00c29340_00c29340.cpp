// _global::UninitCopy_Dword_00c29340(const SDword_00c29340* first, const SDword_00c29340* last, SDword_00c29340* dest, SAllocator_00c29340& al, SNonscalarPtrTag_00c29340) -- Dinkumware-style _Uninit_copy over 4-byte elements: construct [first, last) into raw storage at dest through the allocator, return the advanced dest
// Leaf: UninitCopy_Dword_00c29340
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29340, length 38 bytes, `ret 0Ch`.
// Callers: the forwarding wrapper 0x00C294F0 (`ret 8`: pushes its second stack argument twice -- the allocator
//   reference and the uninitialised empty tag slot -- then the destination; first/last pass through ecx/edx, i.e. the
//   `_Uninitialized_copy(first, last, dest, al)` -> `_Uninit_copy(..., _Ptr_cat(first, dest))` front end), the
//   vector insert-range helpers 0x00C295B0 / 0x00C298D0 and their catch funclet 0x00C29A3C.
// Identity: this TU is the EH-enabled Dinkumware vector code of 0x00C29xxx (vector<float> / vector<unsigned long>
//   `_Ufill`, catch funclets); the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is a
//   propagated VC8 name but the shape is the Dinkumware `_Uninit_copy(first, last, dest, _Alloc&,
//   _Nonscalar_ptr_iterator_tag)`: three stack arguments (dest, the allocator reference, the 1-byte tag by value)
//   = `ret 0Ch`. The try/catch around the construct loop is dropped by VC7.1 because nothing in it can throw.
// Shape: the same body as the STLport __uninitialized_copy 0x0082C420 -- empty-range test first, placement-new null
//   test on dest, one dword copy through esi, both pointers stepped by 4, dest returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct SNonscalarPtrTag_00c29340 { };           // _Nonscalar_ptr_iterator_tag: empty, passed by value (one slot)

struct SDword_00c29340 {                        // 4 bytes, trivially copyable
    unsigned long m_value;
};

struct SAllocator_00c29340 {                    // allocator<T>: construct = placement copy
    void construct(SDword_00c29340* p, const SDword_00c29340& v) { new (p) SDword_00c29340(v); }
};

SDword_00c29340* __fastcall UninitCopy_Dword_00c29340(const SDword_00c29340* first, const SDword_00c29340* last, SDword_00c29340* dest, SAllocator_00c29340& al, SNonscalarPtrTag_00c29340)
{
    for (; first != last; ++dest, ++first)
        al.construct(dest, *first);
    return dest;
}