// std::_Rb_tree::_M_empty_initialize() -- STLport 4.6: make the header node an empty tree (red header, null root, leftmost = rightmost = header)
// Leaf: ?_M_empty_initialize@?$_Rb_tree@UCUnprovenKey_0042fdb8@@UCUnprovenValue_0042fdb8@@UCUnprovenKeyOfValue_0042fdb8@@U?$less@UCUnprovenKey_0042fdb8@@@std@@V?$allocator@UCUnprovenValue_0042fdb8@@@5@@std@@AAEXXZ
// Retail: Fable.exe @ 0x0042FDB8, 22 bytes, __fastcall, size-optimised TU (packed address, `and dword [eax+4],0`):
// `mov eax,[ecx] ; mov byte [eax],0 ; mov eax,[ecx] ; and [eax+4],0 ; mov eax,[ecx] ; mov [eax+8],eax ; mov ecx,[ecx] ;
// mov [ecx+0Ch],ecx` -- _M_header._M_data re-read for each store.
// Identity: the out-of-line copy of STLport 4.6 `_Rb_tree::_M_empty_initialize()` (stl/_tree.h, private) that every
// _Rb_tree constructor / clear() inlines. It has no direct caller in retail (gap-scan COMDAT), so the instantiation's
// key/value types are unproven: per-address placeholders CUnprovenKey/Value/KeyOfValue_0042fdb8 (descriptive).
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14

struct CUnprovenKey_0042fdb8 { long _M_unproven; };
inline bool operator<(const CUnprovenKey_0042fdb8& __x, const CUnprovenKey_0042fdb8& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenValue_0042fdb8 { CUnprovenKey_0042fdb8 _M_key; };
struct CUnprovenKeyOfValue_0042fdb8 { const CUnprovenKey_0042fdb8& operator()(const CUnprovenValue_0042fdb8& __v) const { return __v._M_key; } };

namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef bool _Color_type;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Color_type& __cdecl _S_color(_Link_type __x) { return (_Color_type&)(__x->_M_color); }
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
private:
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = false;      // _S_rb_tree_red
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
};

template void _Rb_tree<CUnprovenKey_0042fdb8, CUnprovenValue_0042fdb8, CUnprovenKeyOfValue_0042fdb8,
                       less<CUnprovenKey_0042fdb8>, allocator<CUnprovenValue_0042fdb8> >::_M_empty_initialize();
}