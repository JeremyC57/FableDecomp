// CGameScriptInterface::ClearIsGossipForPlayer() -- empty CTCAIScratchpad's static IsGossipForPlayer map (map<CCharString, bool>::clear())
// Leaf: ?ClearIsGossipForPlayer@CGameScriptInterface@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A7930, 68 bytes, thiscall (this unused), plain `ret`.
// Identity (naming wave 26, family gossip): CGameScriptInterface vtable 0x01260F0C slot 892 (engine/CGameScriptInterface.h); Ego_r.pdb
//   ?ClearIsGossipForPlayer@CGameScriptInterface@@UAEXXZ (donor 0x008E6C70) and the catalog name. The map at 0x013BAE38 is
//   CTCAIScratchpad::IsGossipForPlayer (Ego_r.pdb dynamic initializer / atexit names; bound by 0x008A9810).
// Callees: _Rb_tree<CCharString, pair<const CCharString, bool>, ...>::_M_erase(_Link_type) 0x007ACC50 (private, Ego_r.pdb; c689 queue).
// Shape: STLport 4.6 _Rb_tree::clear() inlined -- when _M_node_count (+4) is non-zero: _M_erase(_M_root()), then _M_leftmost() = header,
//   _M_root() = 0, _M_rightmost() = header, _M_node_count = 0 (each store reloads the header word). Was a free __fastcall function over
//   GossipList / GossipNode (Release, g_gossipList 0x013BAE38, g_gossipPtr 0x013BAE3C) stand-ins.
class CCharString {
public:
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
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                       // +0x00
    _Base_ptr _M_parent;                                 // +0x04
    _Base_ptr _M_left;                                   // +0x08
    _Base_ptr _M_right;                                  // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    unsigned int _M_node_count;                                                    // +0x04
    _Compare _M_key_compare;                                                       // +0x08
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                                                 // 0x007ACC50 for this tree
public:
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
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    void clear() { _M_t.clear(); }
};
}   // namespace std
class CTCAIScratchpad {
public:
    static std::map<CCharString, bool > IsGossipForPlayer;   // 0x013BAE38
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::ClearIsGossipForPlayer()
{
    CTCAIScratchpad::IsGossipForPlayer.clear();
}
