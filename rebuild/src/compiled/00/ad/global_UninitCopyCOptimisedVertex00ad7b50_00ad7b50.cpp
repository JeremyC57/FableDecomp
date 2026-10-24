// _global::UninitCopy_COptimisedVertex_00ad7b50(const COptimisedVertex* first, const COptimisedVertex* last, COptimisedVertex* result, const __false_type&) -- STLport __uninitialized_copy over QuadricOptimiserInternals::COptimisedVertex (0xB8 bytes): placement-copy each vertex through its out-of-line copy constructor, return the advanced result
// Leaf: UninitCopy_COptimisedVertex_00ad7b50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD7B50, length 52 bytes (+ int3 padding), `ret 8`.
// Callers: the vector<COptimisedVertex> regrowth @ 0x00AD81F0 (calls 0x00AD825C / 0x00AD82B2) and the landed
//   vector::resize @ 0x00AD85D0 (calls 0x00AD8662 / 0x00AD86BA).
// Callees: QuadricOptimiserInternals::COptimisedVertex::COptimisedVertex(const COptimisedVertex&) @ 0x00AD77F0 (landed).
// Layout: retail COptimisedVertex is 0xB8 bytes (the PDB's 0xC0 counts 16-byte maps; retail STLport maps are smaller), so
//   the element is modelled by its size and copy constructor.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack); first/last/cur in esi/ebx/edi across the call, placement
//   new (null test) of the out-of-line copy constructor, stride 0xB8.
// Label correction: the catalog's `_Uninit_copy<CFeatDef*,CFeatDef*,allocator<CFeatDef>>` is propagated.
inline void* operator new(unsigned int, void* where) { return where; }

namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    COptimisedVertex(const COptimisedVertex& other);   // 0x00AD77F0
    unsigned char _body[0xB8];
};
}
typedef QuadricOptimiserInternals::COptimisedVertex COptimisedVertex;

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

COptimisedVertex* __fastcall UninitCopy_COptimisedVertex_00ad7b50(const COptimisedVertex* first, const COptimisedVertex* last, COptimisedVertex* result, const __false_type&)
{
    COptimisedVertex* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}