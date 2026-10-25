// std::_Rb_tree<unsigned long, pair<const unsigned long, CSectionDesc>>::erase(iterator) -- unlink one section-descriptor node, destroy its strings and free it
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F0C90, 78 bytes (80 with padding), `ret 4`. Catalog label
//   "CMemoryAllocatorVariableSize_EraseElement" is wrong: this is the STLport erase(iterator) of
//   CCodeSectionManager::SectionDesc (map<unsigned long, CSectionDesc>; node 0x24 bytes, value pair at +0x10).
// Callees: _Rb_global<bool>::_Rebalance_for_erase(z, root&, leftmost&, rightmost&) 0x0042A34E (__cdecl), ~CCharString
//   0x0099EAE0 (LongName then ShortName), free 0x00BFEA14 (allocator::deallocate's null test).
// Shape: STLport 4.x _Rb_tree::erase verbatim: rebalance, _Destroy(&y->_M_value_field), deallocate(y, 1),
//   --_M_node_count (re-read, dec, store).
// Leaf: ?erase@?$_Rb_tree@KU?$pair@$$CBKUCSectionDesc@@@std@@U?$less@K@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBKUCSectionDesc@@@std@@@2@@Z
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CCharString
{
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};

struct CSectionDesc                                  // CCodeSectionManager::CSectionDesc, 16 bytes
{
    CCharString ShortName;                           // +0x00
    CCharString LongName;                            // +0x04
    bool        Loaded;                              // +0x08
    long        Size;                                // +0x0c
};

namespace std {

template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

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
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root, _Base_ptr& __leftmost,
                                                  _Base_ptr& __rightmost);    // 0x0042A34E
};
typedef _Rb_global<bool> _Rb_global_inst;

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base
{
    _Value _M_value_field;                           // +0x10
};

template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
};

template <class _Tp> class allocator
{
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc
{
public:
    _Value _M_data;
};

template <class _Key, class _Value, class _Compare>
class _Rb_tree
{
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                      // +0x04
    _Compare     _M_key_compare;                                     // +0x08

    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                           this->_M_header._M_data->_M_parent,
                                                                           this->_M_header._M_data->_M_left,
                                                                           this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
};

template class _Rb_tree<unsigned long, pair<const unsigned long, CSectionDesc>, less<unsigned long> >;

} // namespace std