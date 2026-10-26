// std::_Rb_tree::insert_unique(iterator position, const value_type& v) -- (map<CCharString, CScriptGameResourceObjectScriptedThingBase >) STLport 4.6 hinted unique insert: begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3AB6, 456 bytes, thiscall `ret 0Ch` (hidden iterator result, position, v), ebp frame (a size-optimised /Os module: unaligned entry).
// Identity (proven): std::_Rb_tree<CCharString, pair<const CCharString, CScriptGameResourceObjectScriptedThingBase>, _Select1st<...>, less<CCharString>, allocator<...> >::insert_unique(iterator, const value_type&); evidence: the tree is map<CCharString, CScriptGameResourceObjectScriptedThingBase> (the J035 cutscene cast map): _M_insert 0x00CD3867 / node factory 0x00CD3604 -> pair copy 0x00CD2CAB (families J044 / J041); the hinted insert 0x00CD3AB6 serves operator[] 0x00CD3D2E (through 0x00CD3CAE); this body calls that tree's _M_insert 0x00CD3867 and its insert_unique(v) 0x00CD38FC; its call sites: 0x00CD3CBF
// Label: the manifest's "RBTree_TransferAllocateString_Alt" is a descriptive placeholder (family J046: five byte-similar hinted inserts over CCharString keys, each its own
//   COMDAT; retail has no identical-code folding, so each address is one tree type).
// Callees: std::less<CCharString>::operator() 0x0042AAB3 (??R?$less@VCCharString@@@std@@QBE_NABVCCharString@@0@Z), _M_insert 0x00CD3867 (family J044),
//   insert_unique(const value_type&) 0x00CD38FC (family J045), std::_Rb_global<bool>::_M_increment 0x004292C0 / _M_decrement 0x004294E2 (__cdecl).
// Shape: STLport 4.6 _tree.c verbatim (as the landed 0x0042C082 hinted insert of map<NUISystem::CComponent*, long>).
// Model: less<CCharString>::operator() (0x0042AAB3) and CBasicString<char>::operator< (0x00429950) are given their bodies (noinline, as the landed
//   0x0042AAB3 / 0x00429950 sources) so VC7.1 sees the compare writes no memory and keeps the header pointer in a register across it
//   (declared-only compare: 153v166 on the J045 representative); the _Rb_global step functions (0x004292C0 / 0x004294E2) are defined in
//   the TU as STLport's template statics are, for the same reason (declared-only steps: 449v456 on this family's representative).
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
template <class C> class CBasicString {
public:
    C* PData;                                              // +0x00
    __forceinline long Compare(const CBasicString& other) const
    {
        const C* b = other.PData;
        const C* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    __declspec(noinline) bool operator<(const CBasicString& other) const { return Compare(other) < 0; }   // 0x00429950
};
class CCharString {                                        // 4 bytes (Ego_r)
public:
    CBasicString<char>* PData;                             // +0x0
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
namespace std {
template <class _Tp> struct less {
    __declspec(noinline) bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }   // 0x0042AAB3 for CCharString (body visible: it writes no memory)
};
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);   // 0x0042971D
    static _Base_ptr __cdecl _M_increment(_Base_ptr __x);                // 0x004292C0
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);                // 0x004294E2
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
    void _M_decrement() { _M_node = _Rb_global_inst::_M_decrement(_M_node); }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_increment(); return *this; }
    _Self& operator--() { _M_decrement(); return *this; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
    iterator begin() { return iterator(_M_leftmost()); }
    unsigned int size() const { return _M_node_count; }
protected:
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00CD3867
public:
    pair<iterator, bool> insert_unique(const value_type& __x);   // 0x00CD38FC
    iterator insert_unique(iterator __position, const value_type& __x);
};
template <class _Dummy> _Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == false && (_M_node->_M_parent)->_M_parent == _M_node)
        _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
        _Base_ptr __y = _M_node->_M_left;
        while (__y->_M_right != 0)
            __y = __y->_M_right;
        _M_node = __y;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_left) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        _M_node = __y;
    }
    return _M_node;
}
template <class _Dummy> _Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const value_type& __v)
{
    if (__position._M_node == this->_M_header._M_data->_M_left) {     // begin()
        if (size() <= 0)
            return insert_unique(__v).first;
        if (_M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        else {
            bool __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if (__comp_pos_v == false)                                 // equal keys
                return __position;
            iterator __after = __position;
            ++__after;
            if (__after._M_node == this->_M_header._M_data)
                return _M_insert(0, __position._M_node, __v, __position._M_node);
            if (_M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else
                return insert_unique(__v).first;
        }
    }
    else if (__position._M_node == this->_M_header._M_data) {          // end()
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            return _M_insert(0, _M_rightmost(), __v, __position._M_node);
        else
            return insert_unique(__v).first;
    }
    else {
        iterator __before = __position;
        --__before;
        bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));
        if (__comp_v_pos && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node);
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
        }
        else {
            iterator __after = __position;
            ++__after;
            bool __comp_pos_v = !__comp_v_pos;
            if (!__comp_v_pos)
                __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if ((!__comp_v_pos) && __comp_pos_v &&
                (__after._M_node == this->_M_header._M_data || _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else {
                if (__comp_v_pos == __comp_pos_v)
                    return __position;
                else
                    return insert_unique(__v).first;
            }
        }
    }
}
typedef pair<const CCharString, CScriptGameResourceObjectScriptedThingBase > _MapValue;
typedef _Rb_tree<CCharString, _MapValue, _Select1st<_MapValue>, less<CCharString>, allocator<_MapValue> > _MapTree;
template _MapTree::iterator _MapTree::insert_unique(_MapTree::iterator, const _MapValue&);
} // namespace std