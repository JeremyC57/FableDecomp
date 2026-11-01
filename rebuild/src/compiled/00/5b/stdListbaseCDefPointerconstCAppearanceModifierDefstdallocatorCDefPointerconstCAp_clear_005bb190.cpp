// 005bb190 std::_List_base<CDefPointer<const CAppearanceModifierDef>,std::allocator<CDefPointer<const CAppearanceModifierDef> > >::clear
// Leaf: ?clear@?$_List_base@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@V?$allocator@V?$CDefPointer@$$CBVCAppearanceModifierDef@@@@@std@@@std@@QAEXXZ
// Flags:
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointeeBase.h"
class CAppearanceModifierDef;                            // original donor list element and native armor-value list evidence
#include "engine/CDefPointer.h"
namespace std {
template <class _Tp> class allocator { public:
    __forceinline void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> __forceinline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp, class _Alloc> class _List_base { public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*) this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*) __cur->_M_next;
// Fidelity: suppress only the element destruction, leaving the list walk inlined.
#pragma inline_depth(0)
        __tmp->_M_data.~_Tp();
#pragma inline_depth(255)
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template class _List_base<CDefPointer<const CAppearanceModifierDef>, allocator<CDefPointer<const CAppearanceModifierDef> > >;
}