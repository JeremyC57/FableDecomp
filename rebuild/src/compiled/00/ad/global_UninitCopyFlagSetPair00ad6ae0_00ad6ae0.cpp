// _global::UninitCopy_FlagSetPair_00ad6ae0(const SFlagSetPair_00ad6ae0* first, const SFlagSetPair_00ad6ae0* last, SFlagSetPair_00ad6ae0* result, const __false_type&) -- STLport __uninitialized_copy over 28-byte {flag, set<long>, set<long>} records: placement copy-construct [first, last), return the advanced result
// Leaf: UninitCopy_FlagSetPair_00ad6ae0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD6AE0, length 69 bytes, `ret 8`.
// Callers: 0x00AD7CD7 / 0x00AD7D36 (insert-fill 0x00AD7C70) and 0x00AD7F16 / 0x00AD7F6D (the landed
//   vector<Element>::_M_fill_insert 0x00AD7E80, whose same-TU stand-in `__uninitialized_copy` is this body).
// Callees: the out-of-line STLport set/tree copy constructor @ 0x00AD3A10 (twice; catalog "TreeNode_CopyConstructor").
// Identity: the catalog label `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. Layout as the
//   landed caller: Flag byte +0x00, set A +0x04, set B +0x10 (retail 12-byte tree: header, count, compare); 28 bytes.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx -> esi,
//   last in edx -> ebp, result + the tag reference on the stack -> edi, loaded before the empty test through the named
//   `cur` copy and returned from the shared epilogue). The implicit memberwise copy stores the flag byte and calls the
//   set copy twice; VC7.1 keeps `&dst->B` in ebx across the first call and addresses `&dst->A` as `ebx-0Ch`;
//   placement-new null test on the destination.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

struct STreeNode_00ad6ae0;

struct SRetailSet_00ad6ae0 {                    // retail 12-byte STLport set<long>
    STreeNode_00ad6ae0* Head;                   // +0x00
    unsigned int        Count;                  // +0x04
    unsigned char       Compare;                // +0x08
    SRetailSet_00ad6ae0(const SRetailSet_00ad6ae0& other);   // 0x00AD3A10
};

struct SFlagSetPair_00ad6ae0 {                  // 28 bytes
    unsigned char       Flag;                   // +0x00
    SRetailSet_00ad6ae0 A;                      // +0x04
    SRetailSet_00ad6ae0 B;                      // +0x10
};

SFlagSetPair_00ad6ae0* __fastcall UninitCopy_FlagSetPair_00ad6ae0(const SFlagSetPair_00ad6ae0* first, const SFlagSetPair_00ad6ae0* last, SFlagSetPair_00ad6ae0* result, const __false_type&)
{
    SFlagSetPair_00ad6ae0* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SFlagSetPair_00ad6ae0(*first);
    return cur;
}