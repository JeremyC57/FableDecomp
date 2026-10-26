// std::list::erase(iterator position) -- (list<CCountedPointer<const CFileInstaller::CFileInfo> >) STLport single-node erase: unlink the node, release its counted pointer (deleter and count block freed at zero), free the node and return the iterator to the following node
// Leaf: ?erase@?$list@V?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@V?$allocator@V?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@@std@@@std@@QAE?AU?$_List_iterator@V?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@U?$_Nonconst_traits@V?$CCountedPointer@$$CBVCFileInfo@CFileInstaller@@@@@std@@@2@U32@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6C60, 85 bytes (+ int3 padding), thiscall, `ret 8` (hidden iterator result +
//   the iterator by value), returns the result pointer.
// Identity (proven): std::list<CCountedPointer<const CFileInstaller::CFileInfo> >::erase(iterator); evidence: its only caller is the unreferenced pop_front 0x009C6E30 in the CFileInstaller code (CActiveFile / CFileInstaller helpers around it); FableWin.pdb holds pop_front for exactly four list<CCountedPointer<T> > types -- CCreatureActionBase, CLevelLoader::CObject and NCreatureMode::CModeHoverBase::CHoverAnim (the three other rows' pop_fronts) and const CFileInstaller::CFileInfo, the element of CFileInstaller::FilesToCopy / FilesToFinish (Ego_r.pdb), the installer's only counted-pointer lists.
// Label: the manifest's "sub_009c6c60" is propagated (family J110: five byte-identical counted-pointer list erases; retail has no
//   identical-code folding).
// Callees: the count block's deleter (CCPPointerInfo slot +4, __fastcall on the object), operator delete 0x00BFE9BC (the block),
//   free 0x00BFEA14 (the node allocator's deallocate).
// Shape: agent952's STLport erase (H020, 0x00660117) with CCountedPointer's destructor inline (the landed CCountedPointer
//   destructor form: test Info, decrement, at zero DeleteFunc(Data) and delete Info, then null Data and Info).
// Layout: STLport list node {next +0, prev +4, CCountedPointer<T> {Data +8, Info +0xC}}; CCPPointerInfo {RefCount, DeleteFunc,
//   Data} (Ego_r.pdb, 12 bytes).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CFileInstaller { public: class CFileInfo; };
class CCPPointerInfo {                                     // Ego_r.pdb 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {               // 8 bytes: the object, the shared count block
public:
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    T*              Data;                                  // +0x0
    CCPPointerInfo* Info;                                  // +0x4
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
};
template list<CCountedPointer<const CFileInstaller::CFileInfo> >::iterator list<CCountedPointer<const CFileInstaller::CFileInfo> >::erase(iterator);
}   // namespace std