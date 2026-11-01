// NThingMap::CThingMapManager::~CThingMapManager() -- compiler-generated thing-map manager destructor: release the active game-map set, both thing-map arrays, then the non-copyable base
// Leaf: ??1CThingMapManager@NThingMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051A0A0, 109 bytes, `jmp` to the base destructor.
// Identity (proven): the sole caller is the scalar deleting destructor 0x0051A080, slot 0 of vtable 0x01244678 (RTTI
//   NThingMap::CThingMapManager : CBaseClassNonCopyable); the tail jump is ~CBaseClassNonCopyable 0x0099A300 (the direct
//   base, RTTI). Ego_r.pdb ??1CThingMapManager@NThingMap@@UAE@XZ (0x004E7700) / ??_G (0x004E7CB0), FableWin.pdb the same
//   pair (0x01C78890 / 0x01C78850). The catalog label CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: std::_Rb_tree<long, long, _Identity<long>, less<long>, allocator<long> >::_M_erase 0x004A9CB0 (set<long>),
//   free 0x00BFEA14, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Layout: engine/CThingMapManager.h (retail STLport offsets, flattened): PWorld +0x04, ThingMaps +0x08 (CArray<CThingMapInfo>),
//   ActiveThingMaps +0x14 (CArray<CThingMapInfo*>), ActiveGameMaps +0x20 (set<long>), NoThings +0x2c, Dimensions +0x30;
//   modelled locally to carry the real base class and the STLport containers (the header keeps them as byte arrays).
// Shape: no vtable store -- the destructor is compiler-generated (a user-declared one stores the class vtable first);
//   members die in reverse order: ~set (clear + header free), then each CArray (STLport ~vector: trivial element
//   destroy, `if (_M_start) free(_M_start)`), then the out-of-line virtual base destructor as a tail jump.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                // +0x00
    _Base_ptr   _M_parent;                               // +0x04
    _Base_ptr   _M_left;                                 // +0x08
    _Base_ptr   _M_right;                                // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                             // +0x04
    _Compare  _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                       // out of line, one instantiation per tree type
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
} // namespace std
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp> inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray over the STLport vector (12 bytes)

#include "engine/CBaseClassNonCopyable.h"               // ~CBaseClassNonCopyable 0x0099A300

class CWorld;
class C2DExtentsI { public: long X; long Y; };

namespace NThingMap {
class CThingMapManager : public CBaseClassNonCopyable {
public:
    class CThingMapInfo;
    CThingMapManager();                                 // 0x005191E0
    CWorld*                  PWorld;                    // +0x04
    CArray<CThingMapInfo>    ThingMaps;                 // +0x08
    CArray<CThingMapInfo*>   ActiveThingMaps;           // +0x14
    std::set<long>           ActiveGameMaps;            // +0x20
    long                     NoThings;                  // +0x2c
    C2DExtentsI              Dimensions;                // +0x30
};
class CThingMap;
class CThingMapManager::CThingMapInfo { public: CThingMap* PThingMap; long RefCount; };   // Ego_r.pdb, 8 bytes
} // namespace NThingMap

// TU harness (not a retail function): destroying a local by name makes VC7.1 emit the implicit ??1 COMDAT.
void TUHarness_CThingMapManager()
{
    NThingMap::CThingMapManager manager;
}