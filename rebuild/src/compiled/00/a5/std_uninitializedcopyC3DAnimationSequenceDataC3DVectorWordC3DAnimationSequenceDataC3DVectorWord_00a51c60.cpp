// _global::UninitCopy_C3DTriangle2_00a51c60(const C3DTriangle2* first, const C3DTriangle2* last, C3DTriangle2* result, const __false_type&) -- STLport __uninitialized_copy<C3DTriangle2*>: placement copy-construct 6-byte index triangles, return the advanced result
// Leaf: UninitCopy_C3DTriangle2_00a51c60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A51C60, length 52 bytes, `ret 8`.
// Callers: 0x00A55155 (insert-n helper 0x00A55070, catalog "Vector_InsertN_uint48") and 0x00A55326 / 0x00A55390 (insert-n
//   helper 0x00A552B0, catalog "Vector_InsertN_Triangle2").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. The 6-byte
//   stride is Ego_r.pdb C3DTriangle2 (unsigned short VertexIndex[3]; alignment 2). Byte twins: 0x00A52040, 0x00A6E350.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx, last
//   in edx, result + the tag reference on the stack). The implicit (trivial) copy of a 6-byte POD is VC7.1's inline block
//   copy through esi/edi (`mov esi,ecx ; mov ebx,[esi] ; mov edi,eax ; mov [edi],ebx ; mov si,[esi+4] ; mov [edi+4],si`);
//   placement-new null test on the destination; the advanced result returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

class C3DTriangle2 { public:                           // 6 bytes (Ego_r.pdb)
    unsigned short VertexIndex[3];              // +0x00
};

C3DTriangle2* __fastcall UninitCopy_C3DTriangle2_00a51c60(const C3DTriangle2* first, const C3DTriangle2* last, C3DTriangle2* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) C3DTriangle2(*first);
    return result;
}