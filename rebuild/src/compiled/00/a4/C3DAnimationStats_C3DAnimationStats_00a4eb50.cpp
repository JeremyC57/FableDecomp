// C3DAnimationStats::~C3DAnimationStats() -- empty user destructor: destroy the TimingEvents multimap (clear + free the header node), then run ~CBaseObject
// Leaf: ??1C3DAnimationStats@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4EB50, 83 bytes, thiscall, tail `jmp ~CBaseObject`.
// Identity (proven): stores vtable 0x0129D514 first (RTTI .?AVC3DAnimationStats@@ : CBaseObject, rtti-vtables.tsv /
//   rtti_bases.tsv); callers are that vtable's slot-0 `??_G` 0x00A555A0 and the tail jump of the derived C3DAnimation2's
//   destructor (0x00A4EF86, .?AVC3DAnimation2@@ : C3DAnimationStats). Ego_r.pdb (0x00BA4D90) and FableWin.pdb (0x03223540)
//   spell ??1C3DAnimationStats@@UAE@XZ; the binding ??1C3DAnimationStats@@UAE@XZ already exists.
// Callees: the multimap's _Rb_tree::_M_erase 0x00A54240 (bound PDB spelling, private STLport member), free 0x00BFEA14 (the
//   header node), CBaseObject::~CBaseObject 0x0099A430 (??1CBaseObject@@MAE@XZ).
// Shape: `virtual ~C3DAnimationStats() { }` (the vtable store is the user destructor's); TimingEvents is STLport 4.6
//   ~_Rb_tree = clear() (`if (count) { _M_erase(root); leftmost = header; root = 0; rightmost = header; count = 0; }`) then
//   ~_Rb_tree_base's null-tested header free (chunk 697).
// Layout: the landed ctor 0x00A4EB00 / Ego_r.pdb: AnimType +0x08, Duration +0x0c, LoopFlag +0x10, MovementFlag +0x11,
//   MovementVector +0x12 (retail C3DVector is 1-aligned), TimingEvents +0x20 (STLport tree: header +0x20, count +0x24).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    bool operator<(const CCharString& other) const;
};

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
};
#pragma pack(pop)

namespace std {

template <class _Tp> class allocator {
public:
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
    _Color_type _M_color;                                   // +0x00
    _Base_ptr   _M_parent;                                  // +0x04
    _Base_ptr   _M_left;                                    // +0x08
    _Base_ptr   _M_right;                                   // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
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
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    unsigned int _M_node_count;                             // +0x04
    _Compare _M_key_compare;                                // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                          // 0x00A54240 (this instantiation)
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
class multimap {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
private:
    _Rep_type _M_t;
};

}   // namespace std

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                             // : CBaseClass, ~CBaseObject 0x0099A430 (MAE)

enum EAnimationType { ANIMATION_TYPE_NONE = 0 };

class C3DAnimationStats : public CBaseObject {              // vtable 0x0129D514, 0x2C bytes
public:
    virtual ~C3DAnimationStats();
    EAnimationType AnimType;                                // +0x08
    float Duration;                                         // +0x0c
    bool LoopFlag;                                          // +0x10
    bool MovementFlag;                                      // +0x11
    C3DVector MovementVector;                               // +0x12
    std::multimap<CCharString, float> TimingEvents;         // +0x20
};

C3DAnimationStats::~C3DAnimationStats()
{
}