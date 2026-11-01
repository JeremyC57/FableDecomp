// C3DMeshFileXUVTangentChunk::~C3DMeshFileXUVTangentChunk() -- the compiler-generated destructor: free the VTangents and UTangents blocks, then run ~CChunkedFileChunk
// Leaf: ??1C3DMeshFileXUVTangentChunk@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7320, 43 bytes, thiscall, tail `jmp ~CChunkedFileChunk`.
// Identity (proven): C3DMeshFileXUVTangentChunk's slot-0 `??_G` 0x00AA7370 (vtable 0x0129E3C0, RTTI
//   .?AVC3DMeshFileXUVTangentChunk@@ : CChunkedFileChunk) expands this same body inline, and the row sits in that class's run
//   (TriangleStripChunk's GetChunkDescription / ??_G, this row, then UVTangent's GetChunkDescription 0x00AA7350 and ??_G).
//   Ego_r.pdb C3DMeshFileXUVTangentChunk is CChunkedFileChunk (0x10) + UTangents CArray<C3DVector> +0x10 + VTangents
//   CArray<C3DVector> +0x1C, the two blocks freed here; no other chunk class has two trivially destructible arrays at those
//   offsets. FableWin.pdb spells ??1C3DMeshFileXUVTangentChunk@@UAE@XZ (0x0340EA40, apart from the class's own members:
//   compiler-generated). No callers (the ??_G inlines its own copy; this one is unreferenced). No vtable store: implicit.
//   The catalog label "~CPatchTesselationEdgeStrip" is propagated.
// Callees: free 0x00BFEA14, CChunkedFileChunk::~CChunkedFileChunk 0x00AA75C0 (Ego_r / FableWin ??1CChunkedFileChunk@@UAE@XZ).
// Shape: members in reverse order (VTangents, UTangents: CArray over the STLport vector, trivial _Destroy + ~_Vector_base's
//   null-tested free), then the base destructor as the tail jump. DestroyOne (a destruction by name, ungraded) makes VC7.1
//   emit this out-of-line implicit `??1` (chunks 656 / 657 / 696 / 697).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
};
#pragma pack(pop)

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
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
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

class CChunkedFileSuperChunk;

class CChunkedFileChunk : public CBaseClassNonCopyable {   // vtable 0x0129E474, 0x10 bytes
public:
    virtual ~CChunkedFileChunk();                           // 0x00AA75C0
    char ChunkType[5];                                      // +0x04
    CChunkedFileSuperChunk* Parent;                         // +0x0c
};

class C3DMeshFileXUVTangentChunk : public CChunkedFileChunk {   // vtable 0x0129E3C0, 0x28 bytes
public:
    C3DMeshFileXUVTangentChunk();
    CArray<C3DVector> UTangents;                            // +0x10
    CArray<C3DVector> VTangents;                            // +0x1c
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(C3DMeshFileXUVTangentChunk* chunk)
{
    chunk->C3DMeshFileXUVTangentChunk::~C3DMeshFileXUVTangentChunk();
}