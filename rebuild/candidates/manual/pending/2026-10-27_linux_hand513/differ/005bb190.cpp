// std::_List_base<CDefPointer<const CUnprovenDef_005baa41>, std::allocator<...> >::clear() -- destroy and free every node of the list, then relink the sentinel to itself
// Grade: DIFFER(49v51) -- retail destroys each value with `push 0 ; call ??_G` 0x005BAA41; the build calls the out-of-line ~CDefPointer (??1) then free. Tried (4): plain /Os, a same-TU delete helper (??_G emitted but unused), __forceinline _Destroy/deallocate, and a probe wrapping the value in a struct with an implicit destructor (GOTCHAS: an implicit non-trivial dtor goes through ??_G(0)) -- graded the wrong symbol, inconclusive. Next: confirm the wrapper probe with leaf ?clear@ and decide whether the value type is CDefPointer at all.
// Leaf: ?clear@?$_List_base@V?$CDefPointer@$$CBVCUnprovenDef_005baa41@@@@V?$allocator@V?$CDefPointer@$$CBVCUnprovenDef_005baa41@@@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BB190, 51 bytes, thiscall, plain `ret`.
// Identity (probable): STLport 4.6 _List_base::clear (_list.c) over a list whose value is a 4-byte CDefPointer: its only
//   caller 0x005BB5AE is the inline ~_List_base (clear(), then free the sentinel); each value is destroyed through the
//   non-virtual `??_G` 0x005BAA41 (landed as CDefPointer<const CUnprovenDef_005baa41>'s scalar deleting destructor). The
//   pointee def type is not proven (the manifest's CHeroAbilityDef and the label "CNavigatorQueueManager::FrameUpdate"
//   are propagated), so the landed callee's descriptive spelling is kept.
// Callees: CDefPointer<const CUnprovenDef_005baa41>::`scalar deleting destructor'(0) 0x005BAA41, free 0x00BFEA14.
// Shape: `#pragma optimize("s", on)`: the explicit `_Destroy` (`p->~T()`) of a value whose inline destructor is not
//   expanded becomes `push 0 ; call ??_G` (GOTCHAS, 0x5799DE); the cursor advances before the node is destroyed.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CDefPointeeBase { public:                         // vptr; reference count at +4
    virtual ~CDefPointeeBase();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    void Release() const { if (--RefCount == 0) Destroy(); }
};
class CUnprovenDef_005baa41;                            // descriptive: the def type is unproven
template <class T> class CDefPointer { public:          // Ego_r.pdb, 4 bytes
    const CDefPointeeBase* Object;                      // +0x00
    ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }
};
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
    _STLP_alloc_proxy<_List_node_base*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*) this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*) __cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template class _List_base<CDefPointer<const CUnprovenDef_005baa41>, allocator<CDefPointer<const CUnprovenDef_005baa41> > >;
}
// The delete expression that makes VC7.1 emit the ??_G COMDAT the explicit destructor call then uses (not a retail row).
void DeleteDefPointer_005baa41(CDefPointer<const CUnprovenDef_005baa41>* pointer) { delete pointer; }
