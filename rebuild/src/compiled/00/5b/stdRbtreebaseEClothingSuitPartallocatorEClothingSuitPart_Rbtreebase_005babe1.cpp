// std::_Rb_tree_base<EClothingSuitPart, allocator<EClothingSuitPart> >::_Rb_tree_base(const allocator<EClothingSuitPart>& a) -- the set<EClothingSuitPart> tree base: null the header pointer, then allocate the 0x14-byte header node
// Leaf: ??0?$_Rb_tree_base@W4EClothingSuitPart@@V?$allocator@W4EClothingSuitPart@@@std@@@std@@QAE@ABV?$allocator@W4EClothingSuitPart@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BABE1, 22 bytes, thiscall `ret 4`, size TU: `and [esi],0 ; push 14h ; call malloc ; mov [esi],eax`.
// Identity (sweep1, 2026-10-25): its only caller is the tree constructor 0x005BB025 (call at 0x005BB02C), itself called only by the set default
//   constructor 0x005BB3B3, whose three callers 0x005B4AA6/0x005B4F17/0x005B550A each build a local [ebp-48h] object
//   and pass it straight to the landed std::set<EClothingSuitPart>::insert 0x005BAF94 (five times each); the 0x14-byte
//   header = 0x10-byte node links + the 4-byte enum value. Catalog Allocate_20Bytes is a descriptive auto label.
// Shape: STLport 4.6 `_Rb_tree_base(const allocator_type& a) : _M_header(a, 0) { _M_header._M_data =
//   _M_header.allocate(1); }` over the malloc-backed allocator (no OOM path), n = 1 folded into `push 14h`.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE };

namespace std {
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> class allocator {
public:
    allocator() { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
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

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef allocator<_Tp> allocator_type;
    _Rb_tree_base(const allocator_type& __a);
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef typename _Base::allocator_type allocator_type;
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    protected:
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    public:
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // _S_rb_tree_red: the header is red
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    public:
    _Rb_tree(const _Compare& __comp, const allocator_type& __a);
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::allocator_type allocator_type;
    _Rep_type _M_t;                                         // 12 bytes
    set();
};

template <class _Tp, class _Alloc>
_Rb_tree_base<_Tp, _Alloc>::_Rb_tree_base(const allocator_type& __a)
    : _M_header(allocator<_Rb_tree_node<_Tp> >(__a), (_Rb_tree_node<_Tp>*)0)
{
    _M_header._M_data = _M_header.allocate(1);
}

template _Rb_tree_base<EClothingSuitPart, allocator<EClothingSuitPart> >::_Rb_tree_base(const allocator<EClothingSuitPart>&);
}
