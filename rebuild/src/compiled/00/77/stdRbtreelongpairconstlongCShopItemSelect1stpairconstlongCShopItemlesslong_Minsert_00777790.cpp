// std::_Rb_tree<long, pair<const long,CShopItem>, _Select1st<pair<const long,CShopItem> >, less<long> >::_M_insert(x, y, v, w) -- STLport tree insert below a found parent: allocate a node, copy the (key, shop item) value in, link it left or right of y keeping the header's root/leftmost/rightmost, rebalance and count; the new node's iterator through the hidden pointer
// Leaf: _M_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00777790, 201 bytes (+7 int3), `ret 14h` (hidden iterator result + 4
//   arguments).
// Identity: called by the landed insert_unique 0x00777860 and by insert_unique(hint) 0x00777BA0, the tree of
//   map<long, CShopItem> (CTCShop::Stock, retail +0x34; the landed CTCShop::CTCShop 0x007744A0 fills it through
//   operator[] 0x00778280). Label correction: catalog "RBTree_Insert" -> _Rb_tree<long, pair<const long, CShopItem>>::
//   _M_insert.
// Callees: malloc 0x00BFEA0E (the 0x30-byte node: 0x10 links + the 32-byte pair), _Rb_global<bool>::_Rebalance(node,
//   root&) 0x0042971D (__cdecl).
// Shape: the landed _M_insert of 0x006AE900 with the STLport pair copy constructor: the key is a dword move and the
//   28-byte CShopItem a `rep movsd` of 7 dwords, inside the placement-new null test.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

struct CShopItem {                                          // 28 bytes (landed CTCShop::CTCShop model)
    float Quantity;                                         // +0x00
    float PriceMultiplier;                                  // +0x04
    float MaxStock;                                         // +0x08
    long  RestockPeriod;                                    // +0x0c
    float RestockPeriodRandomness;                          // +0x10
    float SalesPerDay;                                      // +0x14
    float SalesPerDayRandomness;                            // +0x18
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }   // STLport's explicit copy constructor
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    long      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0c
};
template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                                   // +0x10
};
}

extern "C" void __cdecl Rebalance_0042971d(std::_Rb_tree_node_base* node, std::_Rb_tree_node_base*& root);   // 0x0042971D

typedef std::pair<const long, CShopItem> SStockValue;      // 32 bytes

class CStockTree {                                          // _Rb_tree<long, pair<const long, CShopItem>, _Select1st, less<long>>
public:
    typedef std::_Rb_tree_node_base* _Base_ptr;
    typedef std::_Rb_tree_node<SStockValue>* _Link_type;
    _Base_ptr     _M_header;                                // +0x00
    unsigned long _M_node_count;                            // +0x04
    struct iterator {
        _Base_ptr _M_node;
        iterator(_Base_ptr node) : _M_node(node) { }
    };
    _Link_type _M_create_node(const SStockValue& v)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(std::_Rb_tree_node<SStockValue>));
        new (&tmp->_M_value_field) SStockValue(v);
        return tmp;
    }
    iterator _M_insert(_Base_ptr x_, _Base_ptr y_, const SStockValue& v, _Base_ptr w_);
};

CStockTree::iterator CStockTree::_M_insert(_Base_ptr x_, _Base_ptr y_, const SStockValue& v, _Base_ptr w_)
{
    _Link_type w = (_Link_type)w_;
    _Link_type y = (_Link_type)y_;
    _Link_type x = (_Link_type)x_;
    _Link_type z;
    if (y == _M_header || (w == 0 && (x != 0 || v.first < y->_M_value_field.first))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_header->_M_parent = z;
            _M_header->_M_right = z;
        }
        else if (y == _M_header->_M_left)
            _M_header->_M_left = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_header->_M_right)
            _M_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    Rebalance_0042971d(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}