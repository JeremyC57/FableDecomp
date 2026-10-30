// CGameScriptInterface::GetGossipVillages(CCharString name) -- the GossipVillages entry for name in CTCAIScratchpad's static map, or null when there is none
// Leaf: ?GetGossipVillages@CGameScriptInterface@@UAEPBV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@VCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AA1B0, 108 bytes, thiscall, `ret 4` (the CCharString by value, destroyed here).
// Identity (proven): CGameScriptInterface vtable 0x01260F0C slot 902; Ego_r.pdb ?GetGossipVillages@CGameScriptInterface@@UAEPBV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@VCCharString@@@Z
//   (donor 0x008E9250) and the manifest label agree; the map at 0x013BAE50 is CTCAIScratchpad::GossipVillages (Ego_r.pdb dynamic
//   initializer / atexit destructor names).
// Callees: CCharString copy constructor 0x0099EC30, _Rb_tree<CCharString, pair<const CCharString, std::vector<CCharString>>, ...>::_M_find
//   0x008AC200, map<CCharString, std::vector<CCharString>>::operator[] 0x007AEBE0, CCharString::~CCharString 0x0099EAE0 (x2 per exit).
// Shape (family K033, representative 0x008A9760): the body is Ego_r.pdb's static __fastcall
//   CTCAIScratchpad::GetGossipVillages(CCharString) (donor 0x008EB830) inlined: its by-value copy of name, the found iterator
//   (a local: find runs before end() is read) against the map's header word, then `&map[name]`; the copy and the
//   argument die on both exits.
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
    _Link_type _M_find(const _Key& __k) const;                                     // 0x008AC200 for this tree
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
    _Tp& operator[](const _Key& __k);                                              // 0x007AEBE0 for this map
};
}   // namespace std
class CTCAIScratchpad {
public:
    static std::map<CCharString, std::vector<CCharString> > GossipVillages;   // 0x013BAE50
    static const std::vector<CCharString>* __fastcall GetGossipVillages(CCharString name)
    {
        std::map<CCharString, std::vector<CCharString> >::iterator it = GossipVillages.find(name);
        if (it == GossipVillages.end())
            return 0;
        return &GossipVillages[name];
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

const std::vector<CCharString>* CGameScriptInterface::GetGossipVillages(CCharString name)
{
    return CTCAIScratchpad::GetGossipVillages(name);
}