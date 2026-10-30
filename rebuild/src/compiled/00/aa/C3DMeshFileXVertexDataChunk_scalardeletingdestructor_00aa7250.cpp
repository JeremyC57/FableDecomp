// C3DMeshFileXVertexDataChunk::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~C3DMeshFileXVertexDataChunk inlined (VertexData freed, then ~CChunkedFileChunk), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GC3DMeshFileXVertexDataChunk@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7250, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: C3DMeshFileXVertexDataChunk -- proven; evidence: vtable 0x0129E33C slot 0 (RTTI .?AVC3DMeshFileXVertexDataChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb names ??_GC3DMeshFileXVertexDataChunk@@UAEPAXI@Z; the freed vector at +0x40 is Ego_r.pdb VertexData (CArray<char>, after FormatID, VertexCount, VertexSize, InitFlags and the 0x20-byte VBCParams), after CChunkedFileChunk's 0x10 bytes; 0x00AA75C0 stores the CChunkedFileChunk vtable 0x0129E474 (RTTI .?AVCChunkedFileChunk@@)
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CChunkedFileChunk::~CChunkedFileChunk 0x00AA75C0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and C3DMeshFileXVertexDataChunk's is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more. KillOne's qualified explicit destructor call makes VC7.1 define the implicit destructor
//   before it generates the vtable's deleting destructor, which then inlines it (without it the deleting destructor
//   calls an out-of-line ??1 whose body holds the inline free: 30 bytes against 48).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {             // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                                  // 0x0099A2F0 (FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {      // vtable 0x0129E474, 0x10 bytes
protected:
    CChunkedFileChunk();
public:
    virtual ~CChunkedFileChunk();                             // 0x00AA75C0 (stores 0x0129E474, tail jmp ~CBaseClassNonCopyable)
    char ChunkType[5];                                        // +0x04
    CChunkedFileSuperChunk* Parent;                           // +0x0c
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<char> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
class CVertexCompressionParams {                          // 0x20 bytes
public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;         // +0x00
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;             // +0x10
};
class C3DMeshFileXVertexDataChunk : public CChunkedFileChunk { // 0x4c bytes
public:
    unsigned long            FormatID;                        // +0x10
    unsigned long            VertexCount;                     // +0x14
    unsigned long            VertexSize;                      // +0x18
    unsigned long            InitFlags;                       // +0x1c
    CVertexCompressionParams VBCParams;                       // +0x20
    CArray<char>             VertexData;                      // +0x40
};
void* MakeOne() { return new C3DMeshFileXVertexDataChunk; }
void KillOne(C3DMeshFileXVertexDataChunk* p) { p->C3DMeshFileXVertexDataChunk::~C3DMeshFileXVertexDataChunk(); }