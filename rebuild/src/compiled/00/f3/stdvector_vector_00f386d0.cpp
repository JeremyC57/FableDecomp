// std::vector::~vector() -- (vector<CGPUScratchBufferTextureManager::CResourcePage >) out-of-line destructor: destroy every 0x1C-byte element (its implicit destructor frees the inner ResourceList array at +0x4 when it has a block), then free the vector's own block
// Leaf: ??1?$vector@UCResourcePage@CGPUScratchBufferTextureManager@@V?$allocator@
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F386D0, 58 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: vector<CGPUScratchBufferTextureManager::CResourcePage >::~vector -- probable; evidence: only the unreferenced jmp thunk 0x00F38760 (the CArray-level destructor) reaches it, in the CResourcePage vector TU: the landed _M_clear 0x00F38710 of vector<CGPUScratchBufferTextureManager::CResourcePage> beside it has the same bytes (0x1C-byte pages, ResourceList's block at +4); FableWin.pdb spells CResourcePage a struct (U).
// Label: the manifest's "sub_00f386d0" is descriptive / propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (twice: the element's inner block, then the vector's block).
// Body: STLport 4.6 `~vector() { _Destroy(_M_start, _M_finish); }` with the inline `~_Vector_base() { if (_M_start) deallocate(_M_start, ...); }`; the element destructors are implicit (the inner vector's
//   trivially destructible elements need no loop: STLport's __type_traits<T>::has_trivial_destructor picks the empty
//   __destroy_aux).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
class CGPUScratchBufferTextureManager {
public:
    struct CResourcePage {                                    // 28 bytes (Ego_r.pdb 32 with its 16-byte CArray)
        long           ScratchPool;                           // +0x00
        CArray<long>   ResourceList;                          // +0x04
        unsigned char* NextFreeBufferAddress;                 // +0x10
        long           FreeSpaceSize;                         // +0x14
        bool           EnableAutoRestore;                     // +0x18
    };
};
template class std::vector<CGPUScratchBufferTextureManager::CResourcePage >;