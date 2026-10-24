// std::list<NUISystem::CComponent*>::~list() -- STLport list destructor: unlink and free every node (0x0042A18F, the inline _List_base::clear), then free the sentinel node
// Leaf: ??1?$list@PAVCComponent@NUISystem@@U?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@XZ
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042ABCA, 23 bytes (`ret`); 117 callers (NUISystem::CManager 0x0041E6D3 /
// 0x00423AF7.., CComponent destructors ..). Label correction (chunk 345 report): the manifest's "CFileInstaller::
// CActiveFile::OnReadFinished" is a propagated label shared by ~45 byte-identical list destructors (immediates masked:
// 0x00451763, 0x004DD118, 0x005B211F ..); this one belongs to the UI component list -- its clear callee 0x0042A18F frees
// 12-byte nodes with no element destructor (a pointer element) and its callers are the NUISystem::CManager / CComponent
// family that keeps list<CComponent*> children (retail CComponent children list, chunk 343/345).
// Callees: _List_base::clear @ 0x0042A18F (walks head->next freeing nodes with free 0x00BFEA14, then relinks the sentinel to
// itself; the manifest's "CGuiControlTreePane::SortTreeRecursively" is propagated), free @ 0x00BFEA14 for the sentinel.
// Layout: the list is STLport's one word, the sentinel-node pointer (_M_node); a node is {next, prev, CComponent* data}.
// Shape: size TU (`pop ecx` after the free); the sentinel is re-read after clear() and null-tested by
// allocator<T>::deallocate (`if (p) free(p)`, STLport's malloc allocator).
#pragma optimize("s", on)
extern "C" void __cdecl free(void*);
namespace NUISystem { class CComponent; }
namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct allocator {
    void deallocate(_List_node<T>* p, unsigned long) { if (p) ::free(p); }
};
template <class T, class A> class list {
public:
    _List_node<T>* _M_node;                      // the sentinel
    void clear();                                // 0x0042A18F (_List_base::clear)
    ~list();
};
template <class T, class A> list<T, A>::~list()
{
    clear();
    A().deallocate(_M_node, 1);
}
template class list<NUISystem::CComponent*, allocator<NUISystem::CComponent*> >;
}
