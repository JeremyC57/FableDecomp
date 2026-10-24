// _global::UninitCopy_CElement1F4_00889880(const CElement1F4* first, const CElement1F4* last, CElement1F4* result, const __false_type&) -- STLport __uninitialized_copy over 0x1F4-byte records with an out-of-line copy constructor: placement-copy each record, return the advanced result
// Leaf: UninitCopy_CElement1F4_00889880
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889880, length 52 bytes (+ int3 padding), `ret 8`.
// Callers: the vector helpers of the same TU: the copy constructor @ 0x008899C0 (call 0x00889A1C), the regrowth @ 0x00889F00
//   (calls 0x00889F72 / 0x0088A027) and the insert path at 0x0088993C (next to the landed __copy_backward @ 0x008898C0 of
//   the same 0x1F4-byte element, whose operator= is 0x006B5DF0).
// Callees: the element copy constructor @ 0x008894A0 (thiscall, source pushed; copies the first word, then a `rep movsd`
//   block and nested arrays).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack); first/last/cur live in esi/ebx/edi across the call,
//   placement new (null test) of the out-of-line copy constructor, stride 0x1F4.
// Label correction: the catalog's `_Uninit_copy<CFeatDef*,CFeatDef*,allocator<CFeatDef>>` is propagated (CFeatDef is 100
//   bytes); the element is modelled by its size and copy constructor.
inline void* operator new(unsigned int, void* where) { return where; }

struct CElement1F4 {
    unsigned char _body[0x1F4];
    CElement1F4(const CElement1F4& other);     // 0x008894A0
};

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

CElement1F4* __fastcall UninitCopy_CElement1F4_00889880(const CElement1F4* first, const CElement1F4* last, CElement1F4* result, const __false_type&)
{
    CElement1F4* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}