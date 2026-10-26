// std::_Rb_tree<unsigned long, pair<const unsigned long, CSectionDesc>>::_M_insert(x, y, v, w) -- link a new section-descriptor node under y, rebalance, count it
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F0FA0, 153 bytes (160 with padding), `ret 14h` (hidden iterator result, x, y,
//   &v, w). Catalog label "RBTree_Insert" is descriptive; same SectionDesc tree as _M_create_node 0x009F0DE0.
// Callees: _M_create_node 0x009F0DE0 (declared only: retail calls it out of line), _Rb_global<bool>::_Rebalance(z,
//   root&) 0x0042971D (__cdecl).
// Shape: STLport 4.5 _Rb_tree::_M_insert verbatim: the left test `y == header || (w == 0 && (x != 0 || v.first <
//   key(y)))` (unsigned `jb`), header re-read after each child store, z's parent/left/right set, rebalance, ++count.
// Leaf: ?_M_insert@?$_Rb_tree@KU?$pair@$$CBKUCSectionDesc@@@std@@U?$_Select1st@U?$pair@$$CBKUCSectionDesc@@@std@@@2@U?$less@K@2@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBKUCSectionDesc@@@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBKUCSectionDesc@@@2@0@Z
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCharString
{
public:
    void* PStringData;
    CCharString(const CCharString& other);            // 0x0099EC30
    ~CCharString();                                   // 0x0099EAE0
};

struct CSectionDesc                                   // CCodeSectionManager::CSectionDesc, 16 bytes
{
    CCharString ShortName;                            // +0x00
    CCharString LongName;                             // +0x04
    bool        Loaded;                               // +0x08
    long        Size;                                 // +0x0c
};

namespace std {

template <class _T1, class _T2> struct pair { typedef _T1 first_type; _T1 first; _T2 second; };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _Rb_tree_node_base
{
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};

template <class _Dummy> struct _Rb_global
{
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);                     // 0x0042971D
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base
{
    _Value _M_value_field;                            // +0x10
};

template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
};

template <class _Tp> class allocator
{
public:
    allocator() {}
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc
{
public:
    _Value _M_data;
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare>
class _Rb_tree
{
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                      // +0x04
    _Compare     _M_key_compare;                                     // +0x08

    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    static _Base_ptr& _S_left(_Base_ptr __x) { return (_Base_ptr&)(__x->_M_left); }
    static _Base_ptr& _S_right(_Base_ptr __x) { return (_Base_ptr&)(__x->_M_right); }
    static _Base_ptr& _S_parent(_Base_ptr __x) { return (_Base_ptr&)(__x->_M_parent); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

protected:
    _Link_type _M_create_node(const _Value& __x);
private:
    iterator _M_insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v, _Base_ptr __w_);
};

} // namespace std

namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const _Value& __v, _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 &&                                  // if w != 0 the remainder fails to false
         (__x != 0 ||                                 // if x != 0 the remainder succeeds to true
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;                           // also makes _M_leftmost() = __z when __y == _M_header
        if (__y == this->_M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;                      // maintain _M_leftmost() pointing to the min node
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;                     // maintain _M_rightmost() pointing to the max node
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}
template std::_Rb_tree<unsigned long, std::pair<const unsigned long, CSectionDesc>,
                        std::_Select1st<std::pair<const unsigned long, CSectionDesc> >, std::less<unsigned long> >::iterator
std::_Rb_tree<unsigned long, std::pair<const unsigned long, CSectionDesc>,
               std::_Select1st<std::pair<const unsigned long, CSectionDesc> >, std::less<unsigned long> >::_M_insert(
    std::_Rb_tree_node_base*, std::_Rb_tree_node_base*, const std::pair<const unsigned long, CSectionDesc>&,
    std::_Rb_tree_node_base*);