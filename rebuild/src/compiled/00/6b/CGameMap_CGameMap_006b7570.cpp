// CGameMap::~CGameMap() -- the user destructor (empty body): reset the CGameMap vtable, free the Cells block, then ~CBaseClassNonCopyable
// Leaf: ??1CGameMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B7570, 33 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): vtable 0x0125D558 stored first (RTTI .?AVCGameMap@@ : CBaseClassNonCopyable : CBaseClass); its slot-0
//   `??_G` 0x006B81A0 inlines the same body. Ego_r.pdb (0x006F3890) and FableWin.pdb (0x0249FF30) spell ??1CGameMap@@UAE@XZ
//   inside the class's own TU (Ego_r order Clear / SetDimensions / ~CGameMap / ctor): a user-declared destructor, as the
//   vtable store shows. Ego_r.pdb CGameMap: Cells (vector<CGameMapCell>) +0x04, Map, World, WorldSeed, SizeX, SizeY (all
//   trivially destructible); the landed-style ctor 0x006B7760 zeroes Cells at +0x04/+0x08/+0x0C. No callers (the deleting
//   destructor has its own copy). The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE).
// Shape: `virtual ~CGameMap() {}` defined out of line in the class TU: vtable store, Cells' STLport ~vector (trivial
//   CGameMapCell _Destroy, null-tested free), then the base destructor as the tail jump.
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
template <class T> class CArray : public std::vector<T> { };   // Lionhead CArray over STLport vector (0xc)

#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

class CMap;
class CWorld;
class CGameMapInit;
class CGameMapCell {                                    // Ego_r.pdb CGameMapCell, 7 bytes (trivially destructible)
public:
    unsigned char AtmosThemes[3];                       // +0x00
    unsigned char AtmosBlends[2];                       // +0x03
    signed char   SoundTheme;                           // +0x05
    unsigned char Music;                                // +0x06
};
class CGameMap : public CBaseClassNonCopyable {         // vtable 0x0125D558, 0x24 bytes in retail
public:
    virtual ~CGameMap();
private:
    CGameMap(CWorld& world, CMap& map, const CGameMapInit& init);   // 0x006B7760 (Ego_r AAE)
    std::vector<CGameMapCell> Cells;                    // +0x04
    CMap*          Map;                                 // +0x10
    CWorld*        World;                               // +0x14
    unsigned long* WorldSeed;                           // +0x18
    long           SizeX;                               // +0x1c
    long           SizeY;                               // +0x20
};

CGameMap::~CGameMap()
{
}