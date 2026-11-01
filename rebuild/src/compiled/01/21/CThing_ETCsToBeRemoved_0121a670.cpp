// CThing::$E_TCsToBeRemoved() -- dynamic initializer of CThing::TCsToBeRemoved: the inline STLport 4.6 _Rb_tree default constructor (allocate the header node, colour it red, root 0, leftmost/rightmost = header, count 0), then its destructor is registered with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A670, 60 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x013742FC, plain `ret`. Registers the landed atexit destructor 0x01228DF0 (`_$E2`: clear() through the out-of-line
//   _Rb_tree::_M_erase 0x00527950, then free of the header node) over the same tree at 0x013B8A8C.
// Identity (proven, two sources):
//   (1) initializer order: FableWin.pdb runs `AttitudeSymbols` -> `REACTION_MANAGER_DEF_INSTANTIATION_NAME` ->
//       `CThing::TCsToBeRemoved` (03FDF480 / 03FDF6E0 / 03FDF810; the CWorldMap statics between them have no retail
//       dynamic initializer); retail's .CRT$XCU runs AttitudeSymbols 0x0121A630 (CCRCSymbolMap at 0x013B8A54) ->
//       0x0121A650 (CCharString "OPINION_REACTION_MANAGER_INSTANCE" at 0x013B8A7C) -> this tree; the atexit thunks run
//       in the same order (0x01228DD0, 0x01228DE0, 0x01228DF0).
//   (2) the tree sits in the thing-manager .bss block (0x013B8A80 / 0x013B8A84 used by CThingManager::GetThingSearchTools
//       and WorldUpdateThings, 0x013B8A88 the copying-things flag of CThingManager_LoadMorphFromScript) and its
//       _M_erase 0x00527950 sits among the thing-manager STLport instantiations (0x00527xxx-0x0052Axxx).
//   Retail never reads it (only this initializer and its atexit destructor reference 0x013B8A8C).
// Types: unproven -- the element type does not reach any retail byte (node 0x20 = 16-byte trivially destructible value);
//   the per-instantiation placeholders CUnprovenKey/Value/KeyOfValue_00527950 are those of the landed rows of the same
//   instantiation (0x00528E50 _Rb_tree ctor, 0x00529EA0), whose out-of-line _M_erase is 0x00527950.
// Callees: malloc 0x00BFEA0E (STLport node allocator), _atexit 0x004012BC.
// Model: STLport 4.6 stl/_tree.h text (as the landed 0x00528E50), namespace std.
// Shape: every header-node store re-reads the static header pointer (the stores through _M_data may alias the global).
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
struct CUnprovenKey_00527950 { long _M_unproven; };
inline bool operator<(const CUnprovenKey_00527950& __x, const CUnprovenKey_00527950& __y) { return __x._M_unproven < __y._M_unproven; }
struct CUnprovenValue_00527950 { CUnprovenKey_00527950 _M_key; unsigned char _M_unproven[12]; };
struct CUnprovenKeyOfValue_00527950 { const CUnprovenKey_00527950& operator()(const CUnprovenValue_00527950& __v) const { return __v._M_key; } };
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
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef bool _Color_type;
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef typename _Base::allocator_type allocator_type;
    enum { _S_rb_tree_red = false, _S_rb_tree_black = true };
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Color_type& __cdecl _S_color(_Link_type __x) { return (_Color_type&)(__x->_M_color); }
private:
    void _M_erase(_Link_type __x);                       // 0x00527950 (out of line, called from the atexit destructor)
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
private:
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = _S_rb_tree_red;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
}

class CThing {
public:
    static std::_Rb_tree<CUnprovenKey_00527950, CUnprovenValue_00527950, CUnprovenKeyOfValue_00527950,
                         std::less<CUnprovenKey_00527950>, std::allocator<CUnprovenValue_00527950> > TCsToBeRemoved;   // 0x013B8A8C
};

std::_Rb_tree<CUnprovenKey_00527950, CUnprovenValue_00527950, CUnprovenKeyOfValue_00527950,
              std::less<CUnprovenKey_00527950>, std::allocator<CUnprovenValue_00527950> > CThing::TCsToBeRemoved;