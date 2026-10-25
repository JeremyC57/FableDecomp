// std::map::operator[](const long& k) -- STLport subscript of CTCShop's map<long, CShopItem>: lower_bound the key, insert {key, CShopItem()} at that hint when absent, return the stock record
// Leaf: ??A?$map@JVCShopItem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00778280, 189 bytes, __fastcall `ret 4`.
// Identity: callers CTCShop::CTCShop 0x007744A0, CTCShop_OnInitialActivate 0x00775350, CPersistentShop_TransferStockItems
//   0x00775C60 (14 sites); the landed _Rb_tree<long, pair<const long, CShopItem> >::_M_insert 0x00777790 names the tree.
//   CShopItem is the 28-byte Ego_r.pdb record (as landed CTCShop::CTCShop 0x007744A0 models it).
// Callees: insert_unique(iterator, const value_type&) @ 0x00777BA0 (the hint insert; iterator built in its slot, result
//   through the dead `k` slot).
// Shape: STLport 4.6 map::operator[] with _M_lower_bound inlined (the landed twin 0x009913D0 template); CShopItem's
//   default ctor stores {0, 1.0f, 0, 1, 0, 0, 0} into the frame temporary, and its implicit copy is the 7-dword `rep movsd`
//   into the value_type temporary.
#include <stddef.h>

class CShopItem {                               // 28 bytes (Ego_r.pdb; a class in the PDB name sets)
public:
    float Quantity;                             // +0x00
    float PriceMultiplier;                      // +0x04
    float MaxStock;                             // +0x08
    long  RestockPeriod;                        // +0x0c
    float RestockPeriodRandomness;              // +0x10
    float SalesPerDay;                          // +0x14
    float SalesPerDayRandomness;                // +0x18
    CShopItem()
        : Quantity(0.0f), PriceMultiplier(1.0f), MaxStock(0.0f), RestockPeriod(1), RestockPeriodRandomness(0.0f),
          SalesPerDay(0.0f), SalesPerDayRandomness(0.0f) { }
};

namespace std {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                       // +0x10
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
    Value& operator*() const { return static_cast<_Rb_tree_node<Value>*>(_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& other) const { return _M_node == other._M_node; }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    iterator end() { return iterator(_M_header); }
    Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const Key& k)
    {
        _Rb_tree_node_base* y = _M_header;      // last node which is not less than k
        _Rb_tree_node_base* x = _M_root();      // current node
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return iterator(y);
    }
    iterator insert_unique(iterator position, const Value& v);   // 0x00777BA0
};

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef Key key_type;
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare, Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;

    iterator lower_bound(const key_type& k) { return _M_t.lower_bound(k); }
    iterator end() { return _M_t.end(); }
    Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator position, const value_type& v) { return _M_t.insert_unique(position, v); }
    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        // i->first is greater than or equivalent to k.
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};

}   // namespace std

template CShopItem& std::map<long, CShopItem>::operator[](const long&);