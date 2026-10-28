// CGameScriptInterface::SetGossipVillages(CCharString name,CCharString value,long index) -- Set one gossip entry.
// Leaf: ?SetGossipVillages@CGameScriptInterface@@UAEXVCCharString@@0J@Z
// Identity: original virtual API and FableWin static CTCAIScratchpad overload; category013bae50.
#define CGameScriptInterface CGameScriptInterface_HeaderLayout
#include "engine/CGameScriptInterface.h"
#undef CGameScriptInterface
#ifndef _MSC_VER
#define __fastcall
#endif
// Layout: active script header is flattened; genuine virtual declaration below preserves the PDB virtual API. No receiver fields accessed. Shared header retained as layout evidence.
class CCharString {
public:
    CCharString& operator=(const CCharString&);
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
    _Tp& operator[](long n) {return _M_start[n];}
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
// Layout: static CTCAIScratchpad category only; no object used.
class CTCAIScratchpad {public:static std::map<CCharString,std::vector<CCharString> > GossipVillages;
static void __fastcall SetGossipVillages(CCharString name,CCharString value,long index){GossipVillages[name][index]=value;}};
class CGameScriptInterface {public:virtual void SetGossipVillages(CCharString name,CCharString value,long index);unsigned char OwnerFields[0x4c];};
void CGameScriptInterface::SetGossipVillages(CCharString name,CCharString value,long index){CTCAIScratchpad::SetGossipVillages(name,value,index);}