// std::_Rb_tree::_M_erase(_Rb_tree_node<value_type>* x) -- (map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo >) STLport 4.6 recursive subtree release without rebalancing: for each node erase its right subtree, destroy the value, free the node and continue with its left child
// Leaf: ?_M_erase@?$_Rb_tree@PAVCThingCreatureBase@@U?$pair@QAVCThingCreatureBase@@UPerCreatureInfo@CTCDivineWrath@@@std@@U?$_Select1st@U?$pair@QAVCThingCreatureBase@@UPerCreatureInfo@CTCDivineWrath@@@std@@@3@U?$less@PAVCThingCreatureBase@@@3@V?$allocator@U?$pair@QAVCThingCreatureBase@@UPerCreatureInfo@CTCDivineWrath@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@QAVCThingCreatureBase@@UPerCreatureInfo@CTCDivineWrath@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00792300, 70 bytes (+ int3 padding), thiscall, `ret 4`; recursive.
// Identity (proven): std::_Rb_tree<CThingCreatureBase*, pair<const CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo >, _Select1st<...>, less<CThingCreatureBase* >, allocator<...> >::_M_erase;
//   evidence: its caller ~CTCDivineWrath 0x0078F5A0 (and the CTCDivineWrath TU's clear / erase helpers 0x00792730, 0x00792880, 0x00792B70) releases CTCDivineWrath::LastFrameCreaturesHit, Ego_r.pdb map<CThingCreatureBase*, CTCDivineWrath::PerCreatureInfo>; the node destroys two intelligent pointers at +0x14 / +0x20 after the 4-byte key, PerCreatureInfo's ActualCreature +0 and Effect +0xC.
// Label: the manifest's "BuildTreeArray" is propagated from a byte-identical-shape body elsewhere (family J090: the 70-byte
//   _M_erase bodies whose value has two out-of-line destructors; retail has no identical-code folding, so each address is its
//   own tree type).
// Callees: itself (the right subtree), ~CBaseIntelligentPointer 0x00A01C10 (??1CBaseIntelligentPointer@@UAE@XZ) on Effect +0x20, then on ActualCreature +0x14, free 0x00BFEA14 (allocator<_Node>::deallocate).
// Model: agent952 H079's STLport _tree.c `_M_erase` (private); the value's implicit destructor inline.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#pragma pack(push, 1)
class CBaseIntelligentPointer {                            // 8 bytes, 1-aligned in retail
public:
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    void* PData;                                           // +0x04 (CBaseObjectPointer*)
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#pragma pack(pop)
class CThing;
class CThingCreatureBase;
class CEnginePrimitiveThemeChannelDesc {                   // 4 bytes (Ego_r.pdb)
public:
    unsigned char ChannelIndex0;                           // +0x00
    unsigned char ChannelIndex1;                           // +0x01
    unsigned char ChannelBlendFactor;                      // +0x02
    unsigned char Pad;                                     // +0x03
};
class CTCDivineWrath {                                     // partial view: only the nested type is used here
public:
    struct PerCreatureInfo {                               // 24 bytes (Ego_r.pdb)
        CIntelligentPointer<CThingCreatureBase> ActualCreature;   // +0x00
        long FramesToGetHit;                               // +0x08
        CIntelligentPointer<CThing> Effect;                // +0x0C
        CEnginePrimitiveThemeChannelDesc DefaultChannel;   // +0x14
    };
};
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
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
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                  // +0x00
    _Base_ptr   _M_parent;                                 // +0x04
    _Base_ptr   _M_left;                                   // +0x08
    _Base_ptr   _M_right;                                  // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                            // +0x04
    _Compare     _M_key_compare;                           // +0x08
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
private:
    void _M_erase(_Link_type __x);
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}
} // namespace std
typedef std::pair<CThingCreatureBase* const, CTCDivineWrath::PerCreatureInfo > TValue_00792300;
template class std::_Rb_tree<CThingCreatureBase*, TValue_00792300, std::_Select1st<TValue_00792300 >, std::less<CThingCreatureBase* >, std::allocator<TValue_00792300 > >;