// CTCShop::GetStockList() const -- copy the shop's stock into a map of item type to whole quantity (each float quantity rounded up to a long), returned by value
// Grade: DIFFER(348v282) -- retail_asm candidate: retail rounds each quantity through an inline-asm block (`fld q; fadd 0.5; fistp i; fild i; fsub 1.0; fstp t`, the 0.5 / 1.0 constants and q / t spilled to ebp locals, then `if (q bits == t bits) --i`), which also forces the ebp frame; C++ cannot emit the bare fistp, so this is the C++ reference body
// Leaf: ?GetStockList@CTCShop@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00774610, length 282 bytes (thiscall, `ret 4`, ebp frame; result through the
//   hidden pointer).
// Identity: Ego_r.pdb ?GetStockList@CTCShop@@QBE?BV?$map@JJU?$less@J@std@@V?$allocator@U?$pair@$$CBJJ@std@@@2@@std@@XZ
//   (callers 0x005A3113, 0x006CF6A3). The catalog label "CTCShop_RebuildCacheMap" is descriptive.
// Callees: malloc 0x00BFEA0E (the local map's header), map<long,long>::operator[] 0x005B30BB, map<long,long> copy ctor
//   0x00778100 (into the return slot; VC7.1 has no NRVO), _Rb_tree<long,pair<const long,long> >::_M_erase 0x004DA2AC (the
//   inlined clear() of the local map), free 0x00BFEA14 (its header).
// Layout: retail CTCShop Stock (map<long, CShopItem>) at +0x34 (Ego_r +0x3C less two 4-byte-shorter STLport vectors);
//   node {color +0, parent +4, left +8, right +0x0C, key +0x10, CShopItem +0x14}; CShopItem.Quantity +0x00.
// Shape: `map<long,long> stock; for (it = Stock.begin(); it != Stock.end(); ++it) stock[(*it).first] =
//   CeilToLong((*it).second.Quantity); return stock;` -- the iterator increment is STLport's _M_increment inlined.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

// Retail rounds with inline asm (fld/fadd 0.5/fistp/fild/fsub 1.0/fstp + a bitwise compare): a ceiling to long.
inline long CeilToLong(float q)
{
    long i = (long)(q + 0.5f + (q + 0.5f >= 0.0f ? 0.5f : -0.5f));   // stand-in for the asm's round-to-nearest of q + 0.5
    float t = (float)i - 1.0f;
    if (*(long*)&q == *(long*)&t)
        --i;
    return i;
}

class CShopItem {                                       // 28 bytes (Ego_r.pdb)
public:
    float Quantity;                                     // +0x00
    float PriceMultiplier;                              // +0x04
    float MaxStock;                                     // +0x08
    long  RestockPeriod;                                // +0x0c
    float RestockPeriodRandomness;                      // +0x10
    float SalesPerDay;                                  // +0x14
    float SalesPerDayRandomness;                        // +0x18
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
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

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr);
};

template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    } else {
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

typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value> struct _Rb_tree_const_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_const_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_const_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_const_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;
    _Rb_tree_base(const _Alloc& __a) : _M_header(allocator<_Rb_tree_node<_Tp> >(), (_Rb_tree_node<_Tp>*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_const_iterator<_Value> const_iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Rb_tree_node_base*& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;      // _S_rb_tree_red
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0), _M_key_compare() { _M_empty_initialize(); }
    _Rb_tree(const _Rb_tree& __x);
    ~_Rb_tree() { clear(); }
    void _M_erase(_Link_type __x);                      // 0x004DA2AC
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
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    map() : _M_t() { }
    map(const map& __x) : _M_t(__x._M_t) { }
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
    _Tp& operator[](const _Key& __k);                   // 0x005B30BB (map<long,long>)
};

}

class CTCShop {
public:
    unsigned char                    _pad00[0x34];
    std::map<long, CShopItem>        Stock;             // +0x34
    const std::map<long, long> GetStockList() const;
};

const std::map<long, long> CTCShop::GetStockList() const
{
    std::map<long, long> stock;
    for (std::map<long, CShopItem>::const_iterator it = Stock.begin(); it != Stock.end(); ++it)
        stock[(*it).first] = CeilToLong((*it).second.Quantity);
    return stock;
}
