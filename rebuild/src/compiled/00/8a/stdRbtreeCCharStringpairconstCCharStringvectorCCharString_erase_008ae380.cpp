// std::_Rb_tree<CCharString, pair<const CCharString, vector<CCharString> >, ...>::erase(iterator first, iterator last) -- STLport map range erase: clear() when the range is the whole tree, else erase node by node
// Leaf: ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@_STL@@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@_STL@@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@_STL@@@_STL@@@3@@_STL@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@_STL@@@_STL@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$vector@VCCharString@@@_STL@@@_STL@@@2@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AE380, 153 bytes, thiscall, `ret 8` (two iterators by value).
// Identity: the manifest's "std::map::_Erase_range" is descriptive. The only caller is 0x008AE420 (this tree's
//   erase(const key&): equal_range + erase(first, last)), the CTCAIScratchpad gossip maps' tree
//   (map<CCharString, vector<CCharString> >; landed operator[] 0x007AEBE0, _M_erase 0x007AE0E0).
// Callees: _Rb_tree::_M_erase(_Link_type) 0x007AE0E0 (landed), _Rb_tree::erase(iterator) 0x008AE1E0 (this chunk).
// Shape: STLport 4.6 `if (first == begin() && last == end()) clear(); else while (first != last) erase(first++);`
//   with clear() inline (count test, _M_erase(root), leftmost = header, root = 0, rightmost = header, count = 0, the
//   header re-read through `this` between the stores) and the post-increment inline: the iterator's SGI
//   `_M_increment` (right then leftmost descent, else climb while we are a right child, then the `right != y` test),
//   the old node passed by value through the iterator's user copy constructor (`push ecx ; mov ecx,esp`).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14 (allocator::deallocate, unused here)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    ~CCharString();                                         // 0x0099EAE0
};

namespace _STL {

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;   // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
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

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment()
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
    }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self operator++(int)
    {
        _Self __tmp = *this;
        _M_increment();
        return __tmp;
    }
};

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;

    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08

    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }

    void _M_erase(_Link_type __x);                          // 0x007AE0E0
    void erase(iterator __position);                        // 0x008AE1E0
    void erase(iterator __first, iterator __last);
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __first, iterator __last)
{
    if (__first == begin() && __last == end())
        clear();
    else
        while (__first != __last)
            erase(__first++);
}

typedef pair<const CCharString, vector<CCharString> > _GossipValue;
template void _Rb_tree<CCharString, _GossipValue, _Select1st<_GossipValue>, less<CCharString> >::erase(iterator, iterator);

} // namespace _STL