# gen_find.py <addr> <out.cpp>: STLport 4.6 _Rb_tree::find<K> definer for the four invented-class `find` rows (D1).
import sys
A = sys.argv[1]; OUT = sys.argv[2]
ROWS = {
 "006dab90": dict(K="EDamageAttribute", V="float", const=True, pre="enum EDamageAttribute { };",
   ident="Binders: the seven landed CTCHero* damage-factor readers 0x008688C0..0x00868A10 call it as the const find of map<EDamageAttribute, float> (`QBE`, _Const_traits); keys compare signed (`jl` / `jge`).",
   cmp="signed"),
 "00990320": dict(K="unsigned long", V="CThreadedFileManager::CFileObject*", const=True, pre="class CThreadedFileManager {\npublic:\n    class CFileObject;\n};",
   ident="Binder: CThreadedFileManager 0x0098E590 (landed) calls it as the const find of map<unsigned long, CFileObject*> Objects (+0x13C); keys compare unsigned (`jb` / `jae`).",
   cmp="unsigned"),
 "00a7f880": dict(K="long", V="CANavQuadTreeNode*", const=False, pre="class CANavQuadTreeNode;",
   ident="Binders: 0x00A77CA0 and 0x00A80500 (landed) call it as the non-const find of map<long, CANavQuadTreeNode*>; keys compare signed (`jl` / `jge`).",
   cmp="signed"),
 "006bd0f0": dict(K="EWorldMapNameGraphic", V="unsigned long", const=True, pre="enum EWorldMapNameGraphic { };",
   ident="Binder: CRegion::GetWorldMapNameGraphic 0x006BC380 (landed) calls it as the const find of NUISystem::CUILocaleGraphicsDef::WorldMapRegionName, map<EWorldMapNameGraphic, unsigned long>; keys compare signed (`jl` / `jge`). Replaces the CUnprovenKey/Value_006bd0f0 placeholders.",
   cmp="signed"),
 "005937e1": dict(K="long", V="long", const=False, pre="", size=True,
   ident="Binder: CGameScriptInterface::DeregisterTimer 0x006E7690 (landed) calls it as the non-const find of map<long, long> ScriptTimers; its _M_find is 0x00592F92 (same instantiation, landed).",
   cmp="signed"),
}
WAS = {'005937e1':'CRegion::SetMiniMapRegionExitTextOffsetX (extern C __stdcall forwarder)','006bd0f0':'_Rb_tree<CUnprovenKey_006bd0f0, ...>::find'}
r = ROWS[A]; K, V = r["K"], r["V"]; P = f"pair<const {K}, {V} >"
tree = f"_Rb_tree<{K}, {P}, _Select1st<{P} >, less<{K} >, allocator<{P} > >"
cq = " const" if r["const"] else ""; it = "const_iterator" if r["const"] else "iterator"
size = r.get("size")
hdr = f"""// std::_Rb_tree::find<_KT>(const _KT& k){cq} -- map<{K}, {V}> lookup (STLport 4.6.1 _tree.h 458-459: `template <class _KT> {it} find(const _KT& __x){cq} {{ return {it}(_M_find(__x)); }}`)
// Leaf: LEAF
// Retail: Fable.exe (TLC, VC7.1) @ 0x{A.upper()}, thiscall, `ret 8` (hidden iterator result + key reference).
// Identity (owner decision 2026-11-03, D1: STLport find / _M_find are member templates): {r['ident']}
//   Was the generated stand-in "{WAS.get(A,'CRetailTree_*::find')}"; retail has no identical-code folding, so this
//   address is one instantiation (value type and constness from the binders above).
"""
if size:
    hdr += "// Shape: size TU; `_M_find` stays out of line (call 0x00592F92 with `this` still in ecx), the iterator is built in the hidden result.\n#pragma optimize(\"s\", on)\n"
else:
    hdr += f"// Shape: `{it}(_M_find(__x))` with _M_find inlined: walk keeping the last node not less than k, then the header when nothing was\n//   found or k < key(y). Mapped types are opaque (find never reads them).\n"
body = f"""{r['pre']}
namespace std {{
template <class _Tp> struct less {{
    bool operator()(const _Tp& __x, const _Tp& __y) const {{ return __x < __y; }}
}};
template <class _Tp> class allocator {{ }};
template <class _T1, class _T2> struct pair {{
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
}};
template <class _Pair> struct _Select1st {{
    const typename _Pair::first_type& operator()(const _Pair& __x) const {{ return __x.first; }}
}};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {{
public:
    _Tp _M_data;
}};

struct _Rb_tree_node_base {{
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
}};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {{
    _Value _M_value_field;                                  // +0x10
}};

struct _Rb_tree_base_iterator {{
    _Rb_tree_node_base* _M_node;
}};
template <class _Tp> struct _Nonconst_traits {{ }};
template <class _Tp> struct _Const_traits {{ }};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {{
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) {{ _M_node = __x; }}
}};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {{
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
protected:
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type& _M_root() const {{ return (_Link_type&)_M_header._M_data->_M_parent; }}
    static _Link_type& _S_left(_Base_ptr __x) {{ return (_Link_type&)(__x->_M_left); }}
    static _Link_type& _S_right(_Base_ptr __x) {{ return (_Link_type&)(__x->_M_right); }}
    static const _Key& _S_key(_Base_ptr __x) {{ return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }}
public:
    template <class _KT> iterator find(const _KT& __x) {{ return iterator(_M_find(__x)); }}
    template <class _KT> const_iterator find(const _KT& __x) const {{ return const_iterator(_M_find(__x)); }}
private:
    template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT& __k) const{';' if size else ''}
"""
if not size:
    body += """    {
        _Link_type __y = _M_header._M_data;                 // last node which is not less than k
        _Link_type __x = _M_root();                         // current node
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == _M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = _M_header._M_data;
        return __y;
    }
"""
body += f"""}};

typedef {tree} _Tree;
template _Tree::{it} _Tree::find<{K} >(const {K}&){cq};
}}
"""
open(OUT, "w", encoding="utf-8").write(hdr + body)
