// CHeightMap::~CHeightMap() -- clear the world-seed pointer, then free the Cells vector storage and chain to ~CBaseClassNonCopyable
// Leaf: ??1CHeightMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F6170, 40 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`; no caller (the
//   scalar deleting destructor inlines it; an unreferenced COMDAT the linker kept).
// Identity (proven): (1) it stores vtable 0x01288BEC (RTTI .?AVCHeightMap@@), and CHeightMap's scalar deleting destructor 0x008F6DE0
//   (slot 0) inlines exactly this body (store, WorldSeed = 0, free [+0x10], 0x0099A300); (2) retail function order mirrors
//   Ego_r.pdb's CHeightMap block: AlterHeightBy (retail 0x008F60C0 landed / Ego_r 0x00854180), then ??1CHeightMap@@UAE@XZ
//   (this row / Ego_r 0x00854230), then the constructor (landed 0x008F63C0 / Ego_r 0x00854280). Ego_r.pdb CHeightMap: World +4,
//   Map +8, WorldSeed (unsigned long*) +0xC, Cells (vector<CHeightMapCell>) +0x10 -- the landed constructor builds the same
//   retail layout. The catalog label "CParticleEffect::~CParticleEffect" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE).
// Shape: a user destructor (it stores the vtable) whose body clears WorldSeed (`WorldSeed = 0;`, the only statement the retail
//   bytes show); then the Cells STLport vector (CHeightMapCell has a trivial destructor) frees its block under the null test;
//   the C2DBoxI array and scalars emit nothing; then the base destructor.
// Layout: CHeightMapCell from engine/CHeightMapCell.h; C2DBoxI / the member list as the landed constructor 0x008F63C0 (Ego_r
//   names); the real STLport 4.6 vector (chunk 699/700 model) in place of that row's hand vector. No engine header exists for
//   CHeightMap.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
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
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std
#include "engine/CBaseClassNonCopyable.h"               // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

class CWorld;
class CMap;
#include "engine/CHeightMapCell.h"                     // CHeightMapCell (Ego_r.pdb, 12 bytes; trivial destructor)
class C2DBoxI {
public:
    long MinX, MinY, MaxX, MaxY;
};

class CHeightMap : public CBaseClassNonCopyable {       // vtable 0x01288BEC, retail 0x214 bytes
public:
    virtual ~CHeightMap();
    CWorld*                     World;                  // +0x004
    CMap*                       Map;                    // +0x008
    unsigned long*              WorldSeed;              // +0x00c
    std::vector<CHeightMapCell> Cells;                  // +0x010
    C2DBoxI                     MapUpdateAreas[30];     // +0x01c
    long                        NoMapUpdateAreas;       // +0x1fc
    bool                        Changed;                // +0x200
    long                        SizeX;                  // +0x204
    long                        SizeY;                  // +0x208
    long                        SizeXIncludingExtraCells;   // +0x20c
    long                        SizeYIncludingExtraCells;   // +0x210
};

CHeightMap::~CHeightMap()
{
    WorldSeed = 0;
}