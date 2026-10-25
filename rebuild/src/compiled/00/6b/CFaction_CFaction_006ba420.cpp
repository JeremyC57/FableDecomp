// CFaction::CFaction() -- default constructor: an empty Name and the two empty STLport faction-name sets (AlliedFactions, EnemyFactions), each allocating and initialising its 0x14-byte header node
// Leaf: ??0CFaction@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BA420, 91 bytes, thiscall, plain `ret`, returns this.
// Identity: the manifest's "std::pair<CCharString,CCharString>::pair" is propagated. The landed
//   CPersistContext::Transfer<CFaction> 0x006BABD0 (its traits DefaultVal), CPersistContext_TransferableComponent<CFaction>
//   0x006BACE0 / 0x006BAD20 and 0x006BAB80 / 0x006BAED0 construct the 0x1C-byte record with it; Ego_r.pdb CFaction is
//   { CCharString Name; set<CCharString> AlliedFactions; set<CCharString> EnemyFactions; } (36 bytes there with 16-byte
//   Dinkumware sets; retail STLport sets are 12 bytes: Name +0, AlliedFactions +4, EnemyFactions +0x10).
// Callees: CCharString::CCharString() 0x0099E4B0, malloc 0x00BFEA0E (each set's header node: colour, parent, left,
//   right, one CCharString).
// Shape: STLport 4.6 `_Rb_tree() : _Rb_tree_base(a), _M_node_count(0), _M_key_compare(comp) { _M_empty_initialize(); }`
//   with `_Rb_tree_base(a) : _M_header(a, (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }`: the proxy's null
//   store, the header published right after the call, the count zeroed, then colour = red (0) through the returned
//   pointer and root / leftmost / rightmost with the header re-read through `this` between the stores.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                           // +0x00
    CCharString();                                          // 0x0099E4B0
};

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
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

template <class _Value, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Value> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;   // +0x00
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08

    _Rb_tree(const _Compare& __comp = _Compare(), const _Alloc& __a = _Alloc())
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // _S_rb_tree_red: tells the header from the root
        this->_M_header._M_data->_M_parent = 0;             // root
        this->_M_header._M_data->_M_left = this->_M_header._M_data;    // leftmost
        this->_M_header._M_data->_M_right = this->_M_header._M_data;   // rightmost
    }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
    set() : _M_t(_Compare(), _Alloc()) { }
};

} // namespace _STL

class CFaction {
public:
    CCharString             Name;                           // +0x00
    _STL::set<CCharString>  AlliedFactions;                 // +0x04
    _STL::set<CCharString>  EnemyFactions;                  // +0x10
    CFaction();
};

CFaction::CFaction()
{
}