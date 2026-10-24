// std::_Rb_tree<CCharString,pair<const CCharString,long>>::equal_range(const CCharString& k) -- the [lower_bound, upper_bound) iterator pair for a string key
// Leaf: equal_range
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BAF10, length 42 bytes, `ret 8` (hidden pair result + key).
// Callees: _M_upper_bound @ 0x004B9D50, _M_lower_bound @ 0x004B9D00 (both descend with the CCharString compare at node
// +0x10; the catalog labels them _Ubound / _Lbound).
// Label: the catalog "StdMap_EqualRange" is behavioural; STLport 4.x `equal_range(k) { return pair<iterator,iterator>(
// lower_bound(k), upper_bound(k)); }` -- VC evaluates the pair arguments right to left (upper bound first) and stores
// {first, second}. The 4-byte mapped type does not reach the bytes; `long` stands for it.
// Shape: default /O2 TU (the key is loaded once into edi and pushed for both calls); STLport's _Rb_tree_iterator has a
// user copy constructor (landed twin 0x0042C058), which keeps `pop edi` between the two stores.
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) {}
};
template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<V> iterator;
    _Base_ptr    _M_header;                              // +0x00
    unsigned int _M_node_count;                          // +0x04
    _Base_ptr _M_lower_bound(const K& k) const;          // 0x004B9D00
    _Base_ptr _M_upper_bound(const K& k) const;          // 0x004B9D50
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const K& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const K& k);
};
template <class K, class V> pair<typename _Rb_tree<K, V>::iterator, typename _Rb_tree<K, V>::iterator> _Rb_tree<K, V>::equal_range(const K& k)
{
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}
}

struct CCharStringData;
class CCharString {
private:
    CCharStringData* m_pData;
};

template class std::_Rb_tree<CCharString, std::pair<const CCharString, long> >;