// C3DMeshFileClothChunk::~C3DMeshFileClothChunk() -- the compiler-generated destructor: free the CompiledCode block, destroy ParticleCode and Name, then run ~CChunkedFileSuperChunk
// Leaf: ??1C3DMeshFileClothChunk@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9D3C0, 43 bytes, thiscall, tail `jmp ~CChunkedFileSuperChunk`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00AC69D0 of vtable 0x0129F008 (RTTI .?AVC3DMeshFileClothChunk@@ :
//   CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb ??1C3DMeshFileClothChunk@@
//   UAE@XZ (donor 0x033D4200, right after ??1C3DMeshFilePrimitiveChunk 0x033D41A0 -- retail has the same pair: 0x00A9D3A0
//   (called by C3DMeshFilePrimitiveChunk's ??_G 0x00ABEEB0) then this row). Ego_r.pdb C3DMeshFileClothChunk members Name
//   (CCharString), MaterialIndex, ParticleCode (CCharString), CompiledCode (CArray<char>) sit at retail +0x2C/+0x30/+0x34/+0x38
//   (CChunkedFileSuperChunk is 0x2C with 12-byte STLport arrays). No 0x0129F008 store: the destructor is the implicit one.
//   The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14, CCharString::~CCharString 0x0099EAE0 (twice), CChunkedFileSuperChunk::~CChunkedFileSuperChunk
//   0x00AA8490 (stores .?AVCChunkedFileSuperChunk@@ vtable 0x0129E4F0: a user dtor, RemoveAllSubChunks then the arrays).
// Layout: local -- engine/C3DMeshFileClothChunk.h / CChunkedFileSuperChunk.h / CChunkedFileChunk.h flatten the vptr.
// Shape: the landed sibling C3DMeshFileXUVTangentChunk 0x00AA7320 model; members in reverse order. DestroyOne (a destruction
//   by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"                         // retail model (8 bytes: Data, Info)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

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

class CChunkedFileSuperChunk : public CChunkedFileChunk {   // vtable 0x0129E4F0, 0x2c bytes in retail
public:
    virtual ~CChunkedFileSuperChunk();                      // 0x00AA8490
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;               // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes; // +0x1c
    unsigned long CurrentChunkIndex;                        // +0x28
};

class C3DMeshFileClothChunk : public CChunkedFileSuperChunk {   // vtable 0x0129F008, 0x44 bytes in retail
public:
    C3DMeshFileClothChunk();
    CCharString   Name;                                     // +0x2c
    long          MaterialIndex;                            // +0x30
    CCharString   ParticleCode;                             // +0x34
    CArray<char>  CompiledCode;                             // +0x38
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(C3DMeshFileClothChunk* chunk)
{
    chunk->C3DMeshFileClothChunk::~C3DMeshFileClothChunk();
}