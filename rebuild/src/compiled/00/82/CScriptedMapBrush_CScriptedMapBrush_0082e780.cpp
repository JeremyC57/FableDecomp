// CScriptedMapBrush::~CScriptedMapBrush() -- scripted map brush destructor: free the brush name, the things-to-add / move / delete trees and the cell vector, then the CBaseObject base
// Leaf: ??1CScriptedMapBrush@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082E780, 215 bytes, `jmp` to the base destructor.
// Identity (proven): the body stores vtable 0x01272518 (RTTI CScriptedMapBrush : CBaseObject) and ends in ~CBaseObject
//   0x0099A430 (direct base, RTTI); callers: the scalar deleting destructor 0x008270C0 (slot 0 of 0x01272518) and
//   CMap::CreateScriptedMapBrush 0x00820F80 (a by-value brush destroyed by name). Ego_r.pdb ??1CScriptedMapBrush@@UAE@XZ
//   (0x00735640, beside ??0CScriptedMapBrush@@QAE@XZ), FableWin.pdb the same (0x027C9350). Virtual (UAE): the CBaseObject
//   destructor is virtual. The catalog label CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation;
//   reloc_bindings' ??1CScriptedMapBrush@@QAE@XZ for 0x0082E780 is the wrong (non-virtual) spelling (NAME_FIXES).
// Callees: ~CCharString 0x0099EAE0; std::_Rb_tree<unsigned __int64, unsigned __int64, _Identity, ...>::_M_erase 0x00824830
//   (set<unsigned __int64>, ThingsToAdd and ThingsToDelete share it); std::_Rb_tree<unsigned __int64, pair<const unsigned
//   __int64, CScriptedMapBrush::CThingPosAndOrientation>, ...>::_M_erase 0x00824870; free 0x00BFEA14;
//   CBaseObject::~CBaseObject 0x0099A430.
// Layout: engine/CScriptedMapBrush.h names (its containers are Ego_r's 16 bytes; retail STLport 12): Cells +0x08,
//   ThingsToDelete +0x14, ThingsToMove +0x20, ThingsToAdd +0x2c, GameArea +0x38, BrushName +0x48,
//   CreationTimeSeconds +0x4c, CreationPattern +0x50 (0x54 bytes); modelled locally with the retail sizes.
// Shape: user destructor (vtable store first, empty body); members in reverse order: ~CCharString, three STLport trees
//   (clear() with the shared zero register, header free), the trivially destroyed cell vector's storage (free).
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

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                         // : CBaseClass; ~CBaseObject 0x0099A430 (protected virtual)

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;                       // +0x00
};
#include "engine/C3DVector.h"
class C3DAngles { public: float AngleXY; float AngleYZ; float AngleXZ; };
class C2DBoxI { public: long TLX; long TLY; long BRX; long BRY; };

class CScriptedMapBrush : public CBaseObject {
public:
    class CScriptedMapCell {                            // Ego_r.pdb, 0x14 bytes
    public:
        float         Height;                           // +0x00
        unsigned long Theme[3];                         // +0x04
        unsigned char ThemeBlend[2];                    // +0x10
    };
    class CThingPosAndOrientation {                     // Ego_r.pdb, 0x18 bytes
    public:
        C3DVector Position;                             // +0x00
        C3DAngles FacingAngles;                         // +0x0c
    };
    CScriptedMapBrush();
    virtual ~CScriptedMapBrush();
    std::vector<CScriptedMapCell>                              Cells;            // +0x08
    std::set<unsigned __int64>                                 ThingsToDelete;   // +0x14
    std::map<unsigned __int64, CThingPosAndOrientation>        ThingsToMove;     // +0x20
    std::set<unsigned __int64>                                 ThingsToAdd;      // +0x2c
    C2DBoxI                                                    GameArea;         // +0x38
    CCharString                                                BrushName;        // +0x48
    float                                                      CreationTimeSeconds;   // +0x4c
    long                                                       CreationPattern;  // +0x50
};

CScriptedMapBrush::~CScriptedMapBrush()
{
}