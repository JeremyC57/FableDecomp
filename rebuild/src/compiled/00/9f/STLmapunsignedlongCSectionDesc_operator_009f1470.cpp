// std::map<unsigned long, CSectionDesc>::operator[](const unsigned long&) -- find a section's descriptor by handle, inserting a default one when absent
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F1470, 208 bytes (`ret 4`). Catalog label "CCodeSectionManager_RegisterSection"
//   is wrong (RegisterSection is the landed 0x009F05A0); this is the STLport operator[] of CCodeSectionManager::SectionDesc
//   (map<unsigned long, CSectionDesc>), the twin of map<char, CCharString>::operator[] 0x009EFCF0.
// Callees: CCharString() 0x0099E4B0 (twice: the default CSectionDesc), CCharString copy ctor 0x0099EC30 (twice: the pair),
//   _Rb_tree::insert_unique(iterator, const value_type&) 0x009F1180, ~CCharString 0x0099EAE0 (four times).
// Shape: STLport operator[] with an unsigned key compare (`jb` / `jae`); _STLP_DEFAULT_CONSTRUCTED as an inline
//   __default_constructed((_Tp*)0) and map::insert as an inline wrapper over insert_unique (both load-bearing, see
//   0x009EFCF0); the default CSectionDesc only constructs its strings (Loaded/Size are copied uninitialised).
// Leaf: ??A?$map@KUCSectionDesc@@U?$less@K@std@@@std@@QAEAAUCSectionDesc@@ABK@Z
class CCharString
{
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

struct CSectionDesc                                 // CCodeSectionManager::CSectionDesc, 16 bytes; implicit special members
{
    CCharString ShortName;                          // +0x00
    CCharString LongName;                           // +0x04
    bool        Loaded;                             // +0x08
    long        Size;                               // +0x0c
};

namespace std {

template <class _T1, class _T2> struct pair
{
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less
{
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base
{
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base
{
    _Value _M_value_field;                          // +0x10
};

template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map
{
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    typedef _Rb_tree_node<value_type> _Node;

    _Rb_tree_node_base* _M_header;                  // +0x00 (alloc proxy)
    unsigned int        _M_node_count;              // +0x04
    _Compare            _M_key_compare;             // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field.first; }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const _Key& __k)
    {
        _Rb_tree_node_base* __y = _M_header;        // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_header->_M_parent;
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const value_type& __x);    // 0x009F1180 (_Rb_tree::insert_unique)
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

template class map<unsigned long, CSectionDesc>;

} // namespace std