// CGameScriptInterface::SetIsGossipForPlayer(const CCharString& name, bool isGossip) const -- record whether the named gossip is for the player in CTCAIScratchpad's static map
// Leaf: ?SetIsGossipForPlayer@CGameScriptInterface@@UBEXABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A96F0, 47 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 724; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order)
//   names it ?SetIsGossipForPlayer@CGameScriptInterface@@UBEXABVCCharString@@_N@Z (slot 723 is the by-value overload 0x008A97D0); the
//   copy is built in the dead `name` slot, as an inlined by-value parameter.
// Callees: CCharString copy constructor 0x0099EC30, map<CCharString, bool>::operator[] 0x008ADF10, CCharString::~CCharString 0x0099EAE0.
// Global: CTCAIScratchpad::IsGossipForPlayer 0x013BAE38 (reloc_bindings ?IsGossipForPlayer@CTCAIScratchpad@@2V?$map@...@std@@A; the landed
//   GetIsGossipForPlayer 0x008A9810 model).
// Shape: the body is Ego_r.pdb's static __fastcall CTCAIScratchpad::SetIsGossipForPlayer(CCharString, bool) (FableWin donor 0x02AAFEA0) inlined:
//   its by-value CCharString parameter is the copy retail builds, then `IsGossipForPlayer[name] = isGossip`, then the copy dies.
class CCharString {
public:
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool operator<(const CCharString& other) const;
    void* PStringData;                                   // +0x00
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st { };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector, 12 bytes
public:
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;   // +0x08
};
struct _Rb_tree_node_base {
    bool _M_color;                                       // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    unsigned int _M_node_count;                                                    // +0x04
    _Compare _M_key_compare;                                                       // +0x08
    iterator end() { return iterator(this->_M_header._M_data); }
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    _Link_type _M_find(const _Key& __k) const;                                     // 0x007ACC90 for this tree
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    _Tp& operator[](const _Key& __k);                                              // 0x008ADF10 for this map
};
}   // namespace std
class CTCAIScratchpad {
public:
    static std::map<CCharString, bool > IsGossipForPlayer;   // 0x013BAE38
    static void __fastcall SetIsGossipForPlayer(CCharString name, bool isGossip)   // Ego_r.pdb ?SetIsGossipForPlayer@CTCAIScratchpad@@SIXVCCharString@@_N@Z (inlined)
    {
        IsGossipForPlayer[name] = isGossip;
    }
};
class CGameScriptInterface {
public:
    virtual ~CGameScriptInterface();
    virtual void SetIsGossipForPlayer(const CCharString& name, bool isGossip) const;
};

void CGameScriptInterface::SetIsGossipForPlayer(const CCharString& name, bool isGossip) const
{
    CTCAIScratchpad::SetIsGossipForPlayer(name, isGossip);
}