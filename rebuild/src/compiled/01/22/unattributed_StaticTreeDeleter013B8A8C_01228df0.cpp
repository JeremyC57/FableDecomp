// unattributed::StaticTreeDeleter013B8A8C() -- the dynamic atexit destructor of a static STLport map at 0x013B8A8C: clear it (erase from the root, relink the empty header, zero the count), then free the header node
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01228DF0, 84 bytes, in `.text$yd`; registered with atexit by its dynamic
//   initializer @ 0x0121A670 (`malloc(0x20)` header, colour/parent/left/right initialised, count zeroed, then
//   `push 0x01228DF0 ; call atexit`). VC7.1 names the pair `$E1` (initializer) / `$E2` (this thunk) in the TU.
// Identity: the tree (header +0, node count +4, empty comparator +8) is referenced by nothing but these two functions, so
//   its element type is not recoverable; the 0x20-byte node says the value is 16 bytes of POD (the out-of-line
//   `_M_erase` 0x00527950 frees each node with no element destructor). The catalog name "Database_BuildTree_Deleter" is
//   invented. Candidate name (unconfirmed): FableWin.pdb's atexit list runs `AttitudeSymbols` ->
//   `REACTION_MANAGER_DEF_INSTANTIATION_NAME` -> `CThing::TCsToBeRemoved`, and retail's thunks run AttitudeSymbols
//   (0x01228DD0) -> a CCharString (0x01228DE0) -> this tree.
// Callees: _Rb_tree::_M_erase @ 0x00527950 (`__fastcall`, `ret 4`), free @ 0x00BFEA14 (the import thunk).
// Shape: STLport 4.6 `~_Rb_tree() { clear(); }` + `~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }`
//   inlined into the compiler's atexit thunk: every header store re-reads the static header pointer (the stores may alias
//   it), and the malloc allocator's deallocate null-tests before `free`.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* p);                    // 0x00BFEA14 (import thunk)
namespace _STL {
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* p, unsigned int) { if (p != 0) free(p); }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef allocator<_Node> _M_node_allocator_type;
    _Rb_tree_base(const _Alloc& __a) : _M_header(_M_node_allocator_type(), 0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, _M_node_allocator_type> _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                         // 0x00527950 (out of line)
    void _M_empty_initialize() {
        this->_M_header._M_data->_M_color = 0;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    void clear() {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _M_t;
};
}

struct SStaticTreeValue013B8A8C {                  // the unrecoverable 12-byte mapped value (16-byte node value with its key)
    long m_a;
    long m_b;
    long m_c;
};

static _STL::map<long, SStaticTreeValue013B8A8C> g_StaticTree013B8A8C;   // 0x013B8A8C