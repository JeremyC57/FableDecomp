// C3DAnimFileLoader::~C3DAnimFileLoader() -- animation-file loader destructor: restore the vtable, free the ChunkTypes vector and release the nine registered chunk-type counted pointers in reverse order, then ~CChunkedFileLoader
// Leaf: ??1C3DAnimFileLoader@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A996D0, 357 bytes, thiscall, tail jump to ~CChunkedFileLoader 0x00AA7540.
// Identity (proven): the body stores vtable 0x0129E060 first, whose RTTI COL names .?AVC3DAnimFileLoader@@ (rtti-vtables.tsv slot 0
//   = 0x00A9A7A0, the scalar deleting destructor and first caller); the second caller 0x00AA4A23 is the slot-0 ??_G 0x00AA4A20
//   of C3DAnimFileXLoader (vtable 0x0129E1E4), a derived class with nothing of its own to destroy. Ego_r.pdb + FableWin.pdb
//   ??1C3DAnimFileLoader@@UAE@XZ (virtual, public). The catalog label is right.
// Layout: Ego_r.pdb C3DAnimFileLoader (0x60): CChunkedFileLoader base 0x10 (vptr, ReadUnknownChunkTypes, SupportedFileType[5]),
//   RootChunkType +0x10, ObjectChunkType +0x18, SequenceChunkType +0x20, BoneMaskChunkType +0x28, HelpersChunkType +0x30,
//   MovementVectorChunkType +0x38, MovementTrackChunkType +0x40, TimingEventChunkType +0x48 (CCountedPointer, 8 bytes),
//   ChunkTypes +0x50 (std::vector<CChunkedFileChunk*>: the PDB's 16-byte Dinkumware vector is STLport's 12 bytes); retail 0x5C.
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate), CCountedPointer release inline (engine/CCountedPointer.h:
//   DeleteFunc, operator delete 0x00BFE9BC, Data = Info = 0), ~CChunkedFileLoader 0x00AA7540 (out of line: stores its own
//   vtable 0x0129E464 only; the catalog's "CChunkedFileLoader::CChunkedFileLoader" label on it is wrong).
// Shape: a user destructor with an empty body (it stores the vtable; an implicit one would not -- GOTCHAS wave 25); the
//   members are destroyed in reverse declaration order. No engine/C3DAnimFileLoader.h or engine/CChunkedFileLoader.h exists.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "engine/CCountedPointer.h"
namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_vector.h (the members this row needs)
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CChunkedFileLoader { public:                          // vtable 0x0129E464, 0x10 bytes
    CChunkedFileLoader(const char* supportedFileType);
    virtual ~CChunkedFileLoader();                          // 0x00AA7540
    int  ReadUnknownChunkTypes;                             // +0x04
    char SupportedFileType[5];                              // +0x08
};
class CChunkedFileChunk;
class C3DAnimFileRootChunk;
class C3DAnimFileLoader : public CChunkedFileLoader { public:   // vtable 0x0129E060
    C3DAnimFileLoader();
    virtual ~C3DAnimFileLoader();
    CCountedPointer<C3DAnimFileRootChunk> RootChunkType;    // +0x10
    CCountedPointer<CChunkedFileChunk> ObjectChunkType;     // +0x18
    CCountedPointer<CChunkedFileChunk> SequenceChunkType;   // +0x20
    CCountedPointer<CChunkedFileChunk> BoneMaskChunkType;   // +0x28
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;    // +0x30
    CCountedPointer<CChunkedFileChunk> MovementVectorChunkType;   // +0x38
    CCountedPointer<CChunkedFileChunk> MovementTrackChunkType;    // +0x40
    CCountedPointer<CChunkedFileChunk> TimingEventChunkType;      // +0x48
    std::vector<CChunkedFileChunk*> ChunkTypes;             // +0x50
};

C3DAnimFileLoader::~C3DAnimFileLoader()
{
}
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileLoader) == 0x5C);