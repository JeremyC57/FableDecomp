// std::_List_iterator_base::_List_iterator_base(_List_node_base* x) -- store the node and return this (the optimised copy every list iterator constructor calls)
// Leaf: ??0_List_iterator_base@std@@QAE@PAU_List_node_base@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C780, 11 bytes, thiscall `ret 4`: `mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],ecx`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 455 "0x41C780 '__EH_epilog3' is an iterator-base ctor"): reloc_bindings
//   already binds ??0_List_iterator_base@std@@QAE@PAU_List_node_base@1@@Z -> 0041c780 from the landed
//   CVsyncCallbackTimer::ClearAllQueries 0x009F7250; the landed list-iterator copy ctor 0x009F7760, list<CPendingQuery>::size
//   0x009F7DC0 and OnPostSwapScreens 0x009F73C0 call it from begin()/end(); 40+ callers, all list-iterator constructions
//   (0x009F7284, 0x009F72E4, 0x009F7403, ... in the unoptimised CVsyncCallbackTimer TU, where the inline constructor is a
//   real call). STLport 4.6 _list.h `_List_iterator_base(_List_node_base* __x) : _M_node(__x) {}`; the optimised COMDAT the
//   linker kept sits in an early /O2 TU. The landed `T::Assign(int)` / "__EH_epilog3" names were placeholders.
// Namespace: std as the existing binding.
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct _List_iterator_base {
    _List_node_base* _M_node;                       // +0x00
    _List_iterator_base(_List_node_base* __x);
};
_List_iterator_base::_List_iterator_base(_List_node_base* __x)
    : _M_node(__x)
{
}
}
