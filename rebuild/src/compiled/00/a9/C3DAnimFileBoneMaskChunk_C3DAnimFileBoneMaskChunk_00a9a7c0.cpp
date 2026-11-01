// C3DAnimFileBoneMaskChunk::~C3DAnimFileBoneMaskChunk() -- compiler-generated destructor: free the EnableMask array block, then ~CChunkedFileChunk
// Leaf: ??1C3DAnimFileBoneMaskChunk@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A9A7C0, 27 bytes, thiscall, tail jump to CChunkedFileChunk::~CChunkedFileChunk 0x00AA75C0.
// Identity (proven): the sole caller 0x00AB0B53 is the scalar deleting destructor 0x00AB0B50, slot 0 of vtable 0x0129E980
//   (rtti-vtables.tsv, RTTI .?AVC3DAnimFileBoneMaskChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass). No vtable
//   store inside: the destructor is compiler-generated (Ego_r.pdb ??1C3DAnimFileBoneMaskChunk@@UAE@XZ). The member freed
//   (+0x10) is Ego_r C3DAnimFileBoneMaskChunk::EnableMask (CArray<bool>). The catalog label "CTCInventoryItem::OnDie" is
//   propagated. The tail target 0x00AA75C0 stores the CChunkedFileChunk vtable 0x0129E474 (its own user dtor,
//   ??1CChunkedFileChunk@@UAE@XZ).
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate), ??1CChunkedFileChunk@@UAE@XZ 0x00AA75C0 (tail jump).
// Layout: Ego_r.pdb C3DAnimFileBoneMaskChunk: CChunkedFileChunk (ChunkType char[5] +0x04, Parent +0x0C), EnableMask +0x10
//   (retail CArray<bool> = 12-byte STLport vector, one byte per element: the ctor 0x00AB0700 zeroes +0x10/+0x14/+0x18 and
//   WriteToFile 0x00AB06B0 reads `byte [start+i]` up to finish-start). engine/C3DAnimFileBoneMaskChunk.h and
//   engine/CChunkedFileChunk.h are flattened records (vptr as data), so the polymorphic chain is a local model.
// TU: the PDB constructor ??0C3DAnimFileBoneMaskChunk@@QAE@XZ is defined with an empty body only to emit the vtable, the ??_G
//   and this implicit ??1 (not graded).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: the 12-byte STLport vector
};

#include "engine/CBaseClassNonCopyable.h"

class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {    // vtable 0x0129E474
protected:
    CChunkedFileChunk(const char* chunkType);               // 0x00AA7590
public:
    virtual ~CChunkedFileChunk();                           // 0x00AA75C0
    char                    ChunkType[5];                   // +0x04
    CChunkedFileSuperChunk* Parent;                         // +0x0C
};

class C3DAnimFileBoneMaskChunk : public CChunkedFileChunk { // vtable 0x0129E980
public:
    C3DAnimFileBoneMaskChunk();                             // 0x00AB0700
    static const char* GetStaticChunkType();
    CArray<bool> EnableMask;                                // +0x10
};

C3DAnimFileBoneMaskChunk::C3DAnimFileBoneMaskChunk()
    : CChunkedFileChunk(GetStaticChunkType())
{
}