// CFaction::~CFaction() -- compiler-generated faction destructor: release the enemy and allied faction-name sets, then the faction name
// Leaf: ??1CFaction@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BA6D0, 129 bytes, `jmp` to ~CCharString.
// Identity (proven): FableWin.pdb ??1CFaction@@QAE@XZ (0x02444350) with the non-virtual ??_GCFaction@@QAEPAXI@Z (0x02445980);
//   retail's non-virtual scalar deleting destructor 0x006BA760 calls this body, and 0x006BAA50 (`jmp` here, std::_Destroy)
//   sits after the ctor thunk 0x006BAA40 (calls ??0CFaction@@QAE@XZ 0x006BA420): the ??1 / ??_G / _Destroy triple of an
//   implicit destructor (chunk 661). Ego_r.pdb CFaction { CCharString Name +0; set<CCharString> AlliedFactions;
//   set<CCharString> EnemyFactions } at retail STLport sizes = +0x00 / +0x04 / +0x10 (0x1C bytes), exactly the three
//   members this body destroys; reloc_bindings already binds ??1CFaction@@QAE@XZ to 0x006BA6D0. The catalog label
//   CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: std::_Rb_tree<CCharString, CCharString, _Identity<CCharString>, less<CCharString>, allocator<CCharString> >::
//   _M_erase 0x004ABA70 (both sets), free 0x00BFEA14, ~CCharString 0x0099EAE0.
// Layout: no engine header (CFaction is not polymorphic); Ego_r.pdb names.
// Shape: no vtable; members in reverse order: two STLport sets (clear() with the shared zero register, header free),
//   then the name's out-of-line destructor as a tail jump.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                // +0x00
    _Base_ptr   _M_parent;                               // +0x04
    _Base_ptr   _M_left;                                 // +0x08
    _Base_ptr   _M_right;                                // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                             // +0x04
    _Compare  _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                       // out of line, one instantiation per tree type
public:
    ~_Rb_tree() { clear(); }
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
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
} // namespace std

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                     // 0x0099EAE0
    CCharStringData* PStringData;                       // +0x00
};

class CFaction {
public:
    CFaction();                                         // 0x006BA420
    CCharString           Name;                         // +0x00
    std::set<CCharString> AlliedFactions;               // +0x04
    std::set<CCharString> EnemyFactions;                // +0x10
};

// TU harness (not a retail function): destroying a local by name makes VC7.1 emit the implicit ??1 COMDAT.
void TUHarness_CFaction()
{
    CFaction faction;
}