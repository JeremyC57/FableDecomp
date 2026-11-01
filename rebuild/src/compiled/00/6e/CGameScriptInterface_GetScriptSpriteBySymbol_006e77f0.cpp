// 006e77f0 CGameScriptInterface::GetScriptSpriteBySymbol
// Leaf: ?GetScriptSpriteBySymbol@CGameScriptInterface@@QBEJABVCCharString@@@Z
// Flags:
#define CGameScriptInterface CGameScriptInterfaceLayout
#include "engine/CGameScriptInterface.h"
#undef CGameScriptInterface
#define CPlayerGui CPlayerGuiLayout
#include "engine/CPlayerGui.h"
#undef CPlayerGui
#include "engine/CParentDefClassBase.h"
#define CPlayerGuiDef CPlayerGuiDefLayout
#include "engine/_quarantine/CPlayerGuiDef.h"
#undef CPlayerGuiDef
class CCharStringData;class CCharString { protected: CCharStringData* PStringData; };
namespace std {
template<class T> class allocator { };
template<class A,class B> struct pair { A first; B second; };
template<class T> struct less { };
template<class T> struct _Select1st { };
template<class T> struct _Const_traits { typedef const T& reference; };
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class T> struct _Rb_tree_node : public _Rb_tree_node_base { T _M_value_field; };
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator!=(const _Rb_tree_base_iterator& other)const{return _M_node!=other._M_node;}
};
template<class V,class Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node<V>* p) { _M_node=p; }
    const V* operator->() const { return &((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
};
template<class V,class T,class A> class _STLP_alloc_proxy : public A { public: V _M_data; };
template<class V,class A> struct _Rb_tree_base { protected: _STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header; };
template<class K,class V,class GetKey,class C,class A> class _Rb_tree : public _Rb_tree_base<V,A> {
    unsigned int _M_node_count;
    C _M_key_compare;
    _Rb_tree_node<V>* _M_find(const K&) const;
public:
    typedef _Rb_tree_iterator<V,_Const_traits<V> > const_iterator;
    const_iterator find(const K& k) const { return const_iterator(_M_find(k)); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
};
template<class K,class T,class C=less<K>,class A=allocator<pair<const K,T> > > class map {
    typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,C,A> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::const_iterator const_iterator;
    const_iterator find(const K& k) const { return _M_t.find(k); }
    const_iterator end() const { return _M_t.end(); }
};
}
// Layout: PDB struct_layouts_egor16748 ScriptSprites at2480; native006e77f0 +0x934, after retail container shrink. Original CParentDefClassBase is imported.
class CPlayerGuiDef : public CParentDefClassBase {
public:
    // Unobserved fields are storage only, not claimed recovered member names.
    unsigned char unobservedPrefix[0x934-0x39];
    std::map<CCharString,long> ScriptSprites;
};
// Layout: typed ScriptSprites matches the imported flattened header at +0x934.
#if defined(_MSC_VER)
typedef char ScriptSpritesOffsetCheck[(offsetof(CPlayerGuiDef,ScriptSprites)==offsetof(CPlayerGuiDefLayout,ScriptSprites))?1:-1];
#endif
class CPlayerGui : public CPlayerGuiLayout {
public: static const CPlayerGuiDef& __fastcall PeekGuiDef();
};
class CGameScriptInterface : public CGameScriptInterfaceLayout {
public: long GetScriptSpriteBySymbol(const CCharString&) const;
};
long CGameScriptInterface::GetScriptSpriteBySymbol(const CCharString& symbol) const
{
    const CPlayerGuiDef& def=CPlayerGui::PeekGuiDef();
    std::map<CCharString,long>::const_iterator it=def.ScriptSprites.find(symbol);
    if(it!=def.ScriptSprites.end()) return it->second;
    return 0;
}