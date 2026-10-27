// std::list<NPlayerGui::CDrawDamage::SDamageValue>::_M_create_node(const SDamageValue& __x) -- allocate a list node and copy the record into it
// Leaf: ?_M_create_node@?$list@VSDamageValue@CDrawDamage@NPlayerGui@@V?$allocator@VSDamageValue@CDrawDamage@NPlayerGui@@@std@@@std@@QAEPAU?$_List_node@VSDamageValue@CDrawDamage@NPlayerGui@@@2@ABVSDamageValue@CDrawDamage@NPlayerGui@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065544C, 29 bytes, thiscall, `ret 4`.
// Identity (proven): its only caller is the list's insert(iterator, const value_type&) 0x00655AF3, called only by the
//   push_front 0x00655EB4 that NPlayerGui::CDrawDamage::AddDamageChange 0x006472B2 applies to its DamageValues list; Ego_r.pdb NPlayerGui::CDrawDamage::DamageValues is
//   list<NPlayerGui::CDrawDamage::SDamageValue> (a 12-byte record: the 0x14-byte node). The manifest's "std_allocate_and_copy_pair"
//   label is descriptive.
// Callees: malloc 0x00BFEA0E (the inline allocator).
// Shape (family M178, representative 0x0065544C): STLport 4.6 `_Node* __p = _M_node.allocate(1); _Construct(&__p->_M_data, __x);
//   return __p;` -- the placement copy null-tested, the record moved by three movsd. Size TU.
#pragma optimize("s", on)
#include <stdlib.h>
inline void* operator new(unsigned int, void* p) { return p; }
namespace NPlayerGui {
class CDrawDamage {
public:
    class SDamageValue {                                          // Ego_r.pdb, 12 bytes
    public:
        unsigned long Members[3];
    };
};
}
namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
};
}
template std::list<NPlayerGui::CDrawDamage::SDamageValue>::_Node* std::list<NPlayerGui::CDrawDamage::SDamageValue>::_M_create_node(const NPlayerGui::CDrawDamage::SDamageValue&);