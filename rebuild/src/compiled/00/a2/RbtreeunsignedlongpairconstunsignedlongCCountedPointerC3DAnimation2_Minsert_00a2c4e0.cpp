// _Rb_tree<unsigned long, pair<const unsigned long, CCountedPointer<C3DAnimation2>>>::_M_insert(x, y, v, w) -- link a new node holding v under parent y (left or right by the hint and key order), then rebalance
// Leaf: ?_M_insert@?$_Rb_tree@KU?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@U?$_Select1st@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2C4E0, 211 bytes, `ret 14h` (hidden iterator pointer + x, y, &v, w). Nine
//   call sites in the tree's insert_unique(v) 0x00A2C5C0 and insert_unique(hint, v) 0x00A2CCF0, which serves
//   map::operator[] 0x00A2D810 from CMeshDataBank's BuildAnimationInstance (0x00A26BDE, map at CAnimationData+0x1C).
//   The catalog label "std_set_insert" is descriptive.
// Identity: the tree under C3DAnimationInfo::CAnimationData::Animations (Ego_r.pdb: map<unsigned long,
//   CCountedPointer<C3DAnimation2>>); node 0x1C = header 0x10 + key + CCountedPointer (Data, Info) whose copy addrefs
//   CCPPointerInfo::RefCount (+0). Rebalance is STLport's static _Rb_global<bool>::_Rebalance (cdecl, 0x0042971D).
// Shape: STLport 4.6 `_Rb_tree::_M_insert` verbatim: the right-link arm laid out first, placement-new null test on the
//   value field, `++_M_node_count` as load/inc/store; the iterator has a user copy constructor, so it returns through
//   the hidden pointer.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class C3DAnimation2;

class CCPPointerInfo {
public:
    unsigned long RefCount;                             // +0x00
    void (__cdecl* DeleteFunc)(void*);                  // +0x04
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {              // 8 bytes
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair& __p) : first(__p.first), second(__p.second) {}
};
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    typedef _Value value_type;
    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;

protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
public:
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Rb_tree_node_base*& _S_left(_Rb_tree_node_base* __x) { return __x->_M_left; }
    static _Rb_tree_node_base*& _S_right(_Rb_tree_node_base* __x) { return __x->_M_right; }
    static _Rb_tree_node_base*& _S_parent(_Rb_tree_node_base* __x) { return __x->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

private:
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const value_type& __v, _Rb_tree_node_base* __w = 0);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v, _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 &&
         (__x != 0 ||
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}

template class std::_Rb_tree<unsigned long, std::pair<const unsigned long, CCountedPointer<C3DAnimation2> >,
    std::_Select1st<std::pair<const unsigned long, CCountedPointer<C3DAnimation2> > >, std::less<unsigned long> >;