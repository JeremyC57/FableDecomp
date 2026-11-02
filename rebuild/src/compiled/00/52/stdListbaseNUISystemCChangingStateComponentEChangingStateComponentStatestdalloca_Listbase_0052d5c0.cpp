// std::_List_base<NUISystem::CChangingStateComponent::EChangingStateComponentState, std::allocator<..> >::~_List_base() -- STLport 4.6: free every node (clear() expanded), re-link the sentinel to itself,
//   free the sentinel
// Leaf: ??1?$_List_base@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@V?$allocator@W4EChangingStateComponentState@CChangingStateComponent@NUISystem@@@std@@@std@@QAE@XZ
// Naming wave 30 (family stl): was the free __fastcall stand-in ?SortTreeRecursively_00436900@@YIXPAUTreeList@@@Z (catalog
//   "CGuiControlTreePane::SortTreeRecursively", propagated across the byte-identical list destructors 0x0052D5C0 / 0x00B09E40 / 0x00B43BB0).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052D5C0, thiscall, plain `ret`; the out-of-line COMDAT called by ~CChangingStateComponent 0x0052CCF0 on StatesToDo (+0x13C, Ego_r.pdb list<EChangingStateComponentState>).
// Shape: the node loop (`mov eax,esi; mov esi,[esi]; push eax; call free`) compares against the reloaded sentinel; the element destructor
//   is trivial; the sentinel's two self-links are stored through the reloaded pointer, then the sentinel is freed when non-null
//   (allocator::deallocate). Model: the landed 0x00B42E70 source's STLport list. DestroyOne (a destruction by name, ungraded) makes VC7.1
//   emit the out-of-line `??1` (the chunk 656/657 idiom).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

}   // namespace std

namespace NUISystem {
class CChangingStateComponent {                             // only the nested enum (its enumerators are not used here)
public:
    enum EChangingStateComponentState { };
};
}   // namespace NUISystem

// Emission helper: destroying the base by name is what makes VC7.1 emit the out-of-line COMDAT.
void DestroyOne(std::_List_base<NUISystem::CChangingStateComponent::EChangingStateComponentState, std::allocator<NUISystem::CChangingStateComponent::EChangingStateComponentState > >* list)
{
    list->~_List_base();
}