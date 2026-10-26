// C3DMeshFileUniqueVerticesChunk::~C3DMeshFileUniqueVerticesChunk() -- store the own vtable, free UniqueVerticesList's element block (the STLport vector destructor inline: trivial elements, then `if (_M_start) free(_M_start)`), then the CChunkedFileChunk destructor as a tail jump
// Leaf: ??1C3DMeshFileUniqueVerticesChunk@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABC590, 33 bytes (+ int3 padding to 48), thiscall, tail `jmp` to the base destructor.
// Identity (proven): C3DMeshFileUniqueVerticesChunk::~C3DMeshFileUniqueVerticesChunk() (Ego_r.pdb / FableWin.pdb ??1C3DMeshFileUniqueVerticesChunk@@UAE@XZ); evidence: it stores the C3DMeshFileUniqueVerticesChunk vtable 0x0129EC6C first (RTTI .?AVC3DMeshFileUniqueVerticesChunk@@; bases CChunkedFileChunk <- CBaseClassNonCopyable <- CBaseClass) and tail-jumps to the parent's destructor
// Label: the manifest's "OnDie" is propagated across byte-identical bodies (family J023: six destructors of a class
//   that adds one STLport vector of trivially destroyed elements; one retail address is one class's destructor, named
//   from the vtable it stores first).
// Callees: free 0x00BFEA14, CChunkedFileChunk::~CChunkedFileChunk() 0x00AA75C0 (catalog-bound ??1CChunkedFileChunk@@UAE@XZ (Ego_r.pdb; a QAE stand-in also)).
// Layout: CChunkedFileChunk is 0x10 bytes, UniqueVerticesList (CArray<unsigned long>) +0x10.
//   Local model: the generated engine headers for these classes carry a data-member vptr (and Dinkumware vector sizes).
#include <stddef.h>
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { __destroy_aux(this->_M_start, this->_M_finish); }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

class CBaseClass {
public:
    CBaseClass();                                               // 0x0099A2D0
    virtual ~CBaseClass();                                      // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                           // 0x0099A300
};
class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {   // 0x10 bytes (Ego_r.pdb, no STL member)
public:
    virtual ~CChunkedFileChunk();                               // 0x00AA75C0
    char                    ChunkType[5];                       // +0x04
    CChunkedFileSuperChunk* Parent;                             // +0x0C
};
class C3DMeshFileUniqueVerticesChunk : public CChunkedFileChunk {                              // RTTI: C3DMeshFileUniqueVerticesChunk <- CChunkedFileChunk <- CBaseClassNonCopyable <- CBaseClass
public:
    virtual ~C3DMeshFileUniqueVerticesChunk();
    CArray<unsigned long> UniqueVerticesList;                   // +0x10
};

C3DMeshFileUniqueVerticesChunk::~C3DMeshFileUniqueVerticesChunk()
{
}