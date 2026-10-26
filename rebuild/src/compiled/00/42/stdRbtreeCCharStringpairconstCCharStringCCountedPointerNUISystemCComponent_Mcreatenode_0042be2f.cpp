// std::_Rb_tree<CCharString, pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, ...>::_M_create_node(const value_type&) -- mallocs a 0x1c-byte node and copy-constructs the pair into it
// Leaf: ?_M_create_node@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@3@@std@@IAEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@2@ABU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@2@@Z
// Retail: Fable.exe @ 0x0042BE2F, 33 bytes, `ret 4`. Catalog label NScript::CDanyalTestScriptsData::Alloc is wrong. STLport
// _M_create_node: malloc import 0x00BFEA0E (`push 1Ch`), placement new (null test) with the pair copy ctor 0x0042A961
// (CCharString copy 0x0099EC30 then CCountedPointer copy 0x00428928) on node +0x10. Callers: the map's _M_insert 0x0042CC03,
// reached from NUISystem::CManager::CreateMenuEntryComponent (0x00428494) via operator[] 0x0042D45D; Ego_r.pdb names the
// member NUISystem::CManager::BastardChildren, map<CCharString, CCountedPointer<NUISystem::CComponent> >.
// Opt: size TU (`pop ecx` after the cdecl malloc, `push dword ptr [esp+8]`).
#pragma optimize("s", on)
#include <stdlib.h>
#include <new>
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099EC30
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& p);           // 0x00428928 (out of line, folded across T)
};
namespace NUISystem { class CComponent; }
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    size_t              _M_node_count;
protected:
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
};
}

template std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_Link_type std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_M_create_node(const std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >&);