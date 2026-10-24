// CShopDef::Test() const -- the def's self-check: walks every entry of the attitude buy- and sell-price multiplier maps (the per-entry checks are compiled out of retail, so only the two tree walks remain)
// Leaf: ?Test@CShopDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007773A0, 140 bytes (`ret`, `this` in ecx).
// Name: FableWin.pdb `?Test@CShopDef@@QBEXXZ` (void CShopDef::Test() const); the one caller is CTCShop::CTCShop 0x007744A0
//   (landed), which calls it on the shop def fetched with GetSubDef. Label correction: catalog "CTCShop_ClearLists" ->
//   CShopDef::Test (nothing is cleared).
// Layout (retail CShopDef with 12-byte containers; FableWin.pdb member names, Ego_r offsets shifted by the 16->12-byte
//   vectors): DefaultStock +0x40, StockCategoryDefaults +0x4c, BuyPriceMultiplier +0x58, SellPriceMultiplier +0x5c,
//   AttitudeBuyPriceMultiplierDefault +0x60, AttitudeBuyPriceMultiplier +0x64 (map<EOpinionAttitudeType, float>: header,
//   node count, key compare), AttitudeSellPriceMultiplierDefault +0x70, AttitudeSellPriceMultiplier +0x74.
// Shape: two `for (it = m.begin(); it != m.end(); ++it)` walks with empty bodies; the STLport tree increment
//   (`_Rb_global<bool>::_M_increment`) is inlined in each.
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template<class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                        // +0x00
    _Base_ptr _M_parent;                                  // +0x04
    _Base_ptr _M_left;                                    // +0x08
    _Base_ptr _M_right;                                   // +0x0c
};
template<class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                                 // +0x10
};

template<class Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        }
        else {
            _Base_ptr y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};
template<class Value> struct _Rb_tree_const_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_const_iterator(_Base_ptr x) { _M_node = x; }
    const Value& operator*() const { return ((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    _Rb_tree_const_iterator& operator++()
    {
        _M_increment();
        return *this;
    }
};
template<class Value>
inline bool operator!=(const _Rb_tree_const_iterator<Value>& x, const _Rb_tree_const_iterator<Value>& y)
{
    return x._M_node != y._M_node;
}

template<class Key, class T, class Compare = less<Key> > class map {
public:
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                        // +0x00 (the allocator proxy's node pointer)
    unsigned int _M_node_count;                           // +0x04
    Compare _M_key_compare;                               // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

class CShopDef {
public:
    unsigned char _pad_0x00[0x60];
    float AttitudeBuyPriceMultiplierDefault;                                    // +0x60
    std::map<EOpinionAttitudeType, float> AttitudeBuyPriceMultiplier;          // +0x64
    float AttitudeSellPriceMultiplierDefault;                                   // +0x70
    std::map<EOpinionAttitudeType, float> AttitudeSellPriceMultiplier;         // +0x74
    void Test() const;
};

void CShopDef::Test() const
{
    for (std::map<EOpinionAttitudeType, float>::const_iterator it = AttitudeBuyPriceMultiplier.begin();
         it != AttitudeBuyPriceMultiplier.end(); ++it) {
    }
    for (std::map<EOpinionAttitudeType, float>::const_iterator it2 = AttitudeSellPriceMultiplier.begin();
         it2 != AttitudeSellPriceMultiplier.end(); ++it2) {
    }
}