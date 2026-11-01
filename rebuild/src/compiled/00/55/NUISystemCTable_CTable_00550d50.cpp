// NUISystem::CTable::~CTable() -- table component destructor: release the sprite map (STLport map<ETableSprites, CCountedPointer<CComponent> >), then the changing-state component base
// Leaf: ??1CTable@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00550D50, 100 bytes, `jmp` to the base destructor.
// Identity (proven): the sole caller is the scalar deleting destructor 0x005540D0, slot 0 of vtable 0x0124A224 (RTTI
//   NUISystem::CTable); the body stores CTable's three vtables (0x0124A224 / 0x0124A1FC / 0x0124A1F4 at +0 / +4 / +0x18:
//   CUIInterface, CObserver, CActionParam, RTTI bases) and chains to ~CChangingStateComponent 0x0052CCF0 (its direct base,
//   RTTI). Ego_r.pdb ??1CTable@NUISystem@@UAE@XZ (0x005CB2B0), FableWin.pdb the same (0x0228E530). The catalog label
//   CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: std::_Rb_tree<...>::_M_erase 0x00553690 (landed under its STLport spelling), free 0x00BFEA14,
//   NUISystem::CChangingStateComponent::~CChangingStateComponent 0x0052CCF0.
// Layout: engine/CTable.h is the flattened Ego_r/retail byte layout (Sprites +0x15c, PHorizontalSeparations +0x168,
//   PVerticalSeparations +0x16c); the destructor needs the real polymorphic base chain (three vtable stores and the
//   base destructor call), so the class is modelled locally with the header's offsets and names; engine/CObserver.h is
//   likewise a flattened Ego_r struct (no vptr), so the CObserver base (vptr +0x04, 0x14 bytes) is local too.
// Shape: the user-declared empty destructor (a compiler-generated one would drop the vtable stores); Sprites' implicit
//   ~map runs STLport ~_Rb_tree (clear(): _M_erase(root), leftmost = header, root = 0, rightmost = header, count = 0)
//   then ~_Rb_tree_base (allocator<_Node>::deallocate of the header node: `if (p) free(p)`).
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

#include "engine/CCountedPointer.h"                    // retail model (8 bytes: Data, Info)

namespace NUISystem {
enum ETableSprites { };
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _pad04[0x14 - 0x04];
};
class CActionParam { public: virtual ~CActionParam(); };
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    unsigned char _pad1c[0x134 - 0x1c];
};
class CChangingStateComponent : public CComponent {
public:
    unsigned char _pad134[0x15c - 0x134];
    virtual ~CChangingStateComponent();                 // 0x0052CCF0
};
class CTable : public CChangingStateComponent {
public:
    std::map<ETableSprites, CCountedPointer<CComponent> > Sprites;   // +0x15c
    void* PHorizontalSeparations;                       // +0x168 vector<unsigned long>*
    void* PVerticalSeparations;                         // +0x16c vector<unsigned long>*
    virtual ~CTable();
};

CTable::~CTable()
{
}
} // namespace NUISystem