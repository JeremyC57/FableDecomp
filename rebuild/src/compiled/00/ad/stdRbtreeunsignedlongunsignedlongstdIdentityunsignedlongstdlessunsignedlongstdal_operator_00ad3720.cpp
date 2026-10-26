// std::_Rb_tree<unsigned long, unsigned long, _Identity<unsigned long>, less<unsigned long>, allocator<unsigned long> >::
// operator=(const _Rb_tree&) -- the set<unsigned long> under QuadricOptimiserInternals::CIndexList: unless self-assigned,
// clear() (the recursive _M_erase @ 0x004CF810, header re-linked to itself), count zeroed, then either the header reset
// for an empty source or the source's root cloned under the header (_M_copy @ 0x00AD3230), leftmost/rightmost re-derived
// and the count copied.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD3720, oracle length 178 bytes, `ret 4`.
// Identity: its _M_copy 0x00AD3230 allocates 0x14-byte nodes (16-byte node base + an unsigned long: set<unsigned long>,
//   agent424); the COptimisedVertex copy 0x00AD7050 calls it on +0x84 = Ego_r.pdb COptimisedVertex::FaceList (CIndexList)
//   and then 0x00AD3E30 on the next member Groups (map<unsigned long,float>). Was catalogued `CRetailTree_00ad3230::
//   operator=` (VC6 Dinkumware comment); retail is STLport 4.6 (`_M_header` pointer, `_M_node_count`).
// Shape: STLport 4.6 `_Rb_tree::operator=` verbatim with clear(), _M_root/_M_leftmost/_M_rightmost and _S_minimum /
//   _S_maximum inlined; the empty _M_key_compare copy emits nothing.
// Leaf: ??4?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@QAEAAV01@ABV01@@Z
namespace std {
template <class _Tp> struct _Identity { };
template <class _Tp> struct less { };
template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    char      _M_color;                              // +0x00
    _Base_ptr _M_parent;                             // +0x04
    _Base_ptr _M_left;                               // +0x08
    _Base_ptr _M_right;                              // +0x0C
    static _Base_ptr _S_minimum(_Base_ptr __x) { while (__x->_M_left != 0) __x = __x->_M_left; return __x; }
    static _Base_ptr _S_maximum(_Base_ptr __x) { while (__x->_M_right != 0) __x = __x->_M_right; return __x; }
};

template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};

template <class _Tp, class _Alloc>
struct _Rb_tree_base {
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                      // +0x04
    _Compare     _M_key_compare;                     // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type _S_minimum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(__x); }
    static _Link_type _S_maximum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(__x); }
private:
    _Link_type _M_copy(_Link_type __x, _Link_type __p);   // 0x00AD3230
private:
    void _M_erase(_Link_type __x);                   // 0x004CF810
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = (_Link_type)this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = (_Link_type)this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    _Rb_tree& operator=(const _Rb_tree& __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>&
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x)
{
    if (this != &__x) {
        clear();
        _M_node_count = 0;
        _M_key_compare = __x._M_key_compare;
        if (__x._M_root() == 0) {
            _M_root() = 0;
            _M_leftmost() = (_Link_type)this->_M_header._M_data;
            _M_rightmost() = (_Link_type)this->_M_header._M_data;
        }
        else {
            _M_root() = _M_copy(__x._M_root(), (_Link_type)this->_M_header._M_data);
            _M_leftmost() = _S_minimum(_M_root());
            _M_rightmost() = _S_maximum(_M_root());
            _M_node_count = __x._M_node_count;
        }
    }
    return *this;
}

typedef _Rb_tree<unsigned long, unsigned long, _Identity<unsigned long>,
                 less<unsigned long>, allocator<unsigned long > > _CULongSetTree;
template _CULongSetTree& _CULongSetTree::operator=(const _CULongSetTree&);   // emit just this member
}
