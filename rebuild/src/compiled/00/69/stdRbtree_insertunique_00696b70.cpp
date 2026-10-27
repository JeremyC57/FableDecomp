// std::_Rb_tree::insert_unique(const value_type& __v) -- (map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase>>) STLport unique insert: descend comparing CCharString keys, then insert unless the predecessor's key equals __v's; returns (iterator, inserted)
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCActionEventBase@NCreatureAction@@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00696B70, 254 bytes, thiscall, pair<iterator, bool> through the hidden pointer, `ret 8`.
// Identity (proven): its _M_insert 0x006969D0 is landed as map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> >'s; its callers are RegisterActionEventTypes 0x006932B0 (seven inserts of {name, counted new CActionEvent*} into the global map 0x013BA8BC).
// Callees: CBasicString<char>::operator< 0x00429950 (CCharString::operator< inline), _Rb_tree::_M_insert 0x006969D0,
//   _Rb_tree_iterator::operator-- 0x00695C80.
// Shape (family M197, representative 0x00696B70): STLport 4.6 _tree.c `insert_unique(const value_type&)` verbatim over the
//   landed CCharString-key tree model (the hinted insert_unique 0x00756790): CCharString::operator< inline (equal blocks false,
//   null this true, null other false, else the CBasicString compare as a same-TU noinline body); the iterator's operator--
//   noinline. Speed TU.
template <class T> class CBasicString {
public:
    T* m_pData;
#pragma auto_inline(off)
    // 0x00429950 (landed): the real body as a same-TU noinline function.
    __declspec(noinline) bool operator<(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return false;
            if (*a < *b)
                return true;
            if (*a > *b)
                return false;
            ++a;
            ++b;
        }
    }
#pragma auto_inline(on)
};
class CCharString {
public:
    CBasicString<char>* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        return *PStringData < *other.PStringData;
    }
};
class CCPPointerInfo {
public:
    long RefCount;                                        // +0x00
    void (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                           // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes: Data, Info
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
};
namespace NCreatureAction { class CActionEventBase; }
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    __declspec(noinline) _Self& operator--()           // 0x00695C80: out of line in retail (per instantiation)
    {
        if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
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
        return *this;
    }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }
    iterator begin() { return iterator(_M_leftmost()); }

    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x006969D0
    pair<iterator, bool> insert_unique(const value_type& __v);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp) {
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x */ __y, __y, __v), true);
        else
            --__j;
    }
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
}
typedef std::pair<const CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > TValue;
typedef std::_Rb_tree<CCharString, TValue, std::_Select1st<TValue>, std::less<CCharString>, std::allocator<TValue> > TTree;
template std::pair<TTree::iterator, bool> TTree::insert_unique(const TValue&);