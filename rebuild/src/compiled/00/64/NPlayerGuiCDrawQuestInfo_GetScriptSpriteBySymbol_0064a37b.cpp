// NPlayerGui::CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString& symbol) const -- look a script sprite symbol up in the player GUI def's script-sprite map: its graphic index, or 0 when the symbol is unknown
// Leaf: ?GetScriptSpriteBySymbol@CDrawQuestInfo@NPlayerGui@@QBEJABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064A37B, 38 bytes, thiscall (this unused), `ret 4`, /O1 TU.
// Identity (probable): the catalog label, Ego_r.pdb ?GetScriptSpriteBySymbol@CDrawQuestInfo@NPlayerGui@@QBEJABVCCharString@@@Z (donor
//   0x0068C6C0); the body reads CPlayerGuiDef::ScriptSprites, a map<CCharString, long> (PDB +0x9b0, retail +0x934: the def's
//   STLport containers are smaller), exactly what the name says.
// Callees: CPlayerGui::PeekGuiDef() 0x00438570 (static __fastcall, Ego_r.pdb), the STLport 4.6 _Rb_tree<CCharString, pair<const
//   CCharString, long>, _Select1st, less, allocator>::_M_find(const CCharString&) const 0x0042B467 (node in eax). reloc_bindings
//   holds a spelling without _Select1st and several invented map finds for 0x0042B467 -> Review at landing.
// Shape: STLport `find` inline (`const_iterator(_M_find(k))`), compared with end() (the header node), the mapped long at node +0x14.
#pragma optimize("s", on)
class CCharString { public: void* PData; };

namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Value> struct _Rb_tree_const_iterator {
    _Rb_tree_node_base* _M_node;
    __forceinline _Rb_tree_const_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    __forceinline const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    __forceinline bool operator!=(const _Rb_tree_const_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_const_iterator<_Value> const_iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    __forceinline const_iterator end() const { return const_iterator(_M_header._M_data); }
    __forceinline const_iterator find(const _Key& __x) const { return const_iterator(_M_find(__x)); }
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;   // 0x0042B467
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    __forceinline const_iterator end() const { return _M_t.end(); }
    __forceinline const_iterator find(const _Key& __x) const { return _M_t.find(__x); }
};
}

class CPlayerGuiDef {
public:
    unsigned char _pad[0x934];
    std::map<CCharString, long> ScriptSprites;     // +0x934 (PDB +0x9b0)
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};

namespace NPlayerGui {

class CDrawQuestInfo {
public:
    long GetScriptSpriteBySymbol(const CCharString& symbol) const;
};

long CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString& symbol) const
{
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    std::map<CCharString, long>::const_iterator it = def.ScriptSprites.find(symbol);
    if (it != def.ScriptSprites.end())
        return it->second;
    return 0;
}

}