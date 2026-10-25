// _STL::list::erase(_STL::list<CMeshBankManager::CMeshDataBankInfo>::iterator position) -- list<CMeshDataBankInfo>: unlink one mesh-bank record, release its bank pointer and handle string, free the node and return the next position
// Leaf: ?erase@?$list@VCMeshDataBankInfo@CMeshBankManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A650, 97 bytes, `__fastcall` (the list in ecx, unused), hidden
//   iterator result + position on the stack, `ret 8`.
// Identity: the node's data (+8) is a 12-byte {CCharString +0; CCountedPointer +4} = Ego_r.pdb
//   CMeshBankManager::CMeshDataBankInfo {CCharString BankHandle; CCountedPointer<CMeshDataBank> MeshDataBank}, the
//   element of CMeshBankManager::MeshDataBanks; both callers (0x00A09E3A, 0x00A09E9F) sit in CMeshBankManager's
//   CloseGraphicBank / FlushUnusedBanks run (0x00A09DC0..), next to _Cons_val<allocator<CMeshDataBankInfo>, ...>.
//   The manifest's "list<CGraphicBankManager::CGraphicDataBankInfo>::erase" is propagated from 0x009F8920.
// Callees: operator delete @ 0x00BFE9BC (the count block), CCharString::~CCharString @ 0x0099EAE0, free @ 0x00BFEA14
//   (the node); the bank is deleted through the count block's DeleteFunc.
// Shape: the same STLport `list::erase` as 0x009F8920 over the smaller record (the data address formed once in esi).
extern "C" void __cdecl free(void* block);               // 0x00BFEA14 (import thunk)
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
struct CCPPointerInfo {                                  // CCountedPointer count block
    long  RefCount;                                      // +0x00
    void  (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                          // +0x08
};
template <class T> class CCountedPointer {               // the landed model (STL_partialsort_00cbd0f0)
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CMeshDataBank;
class CMeshBankManager {
public:
    class CMeshDataBankInfo {                            // 12 bytes
    public:
        CCharString                    BankHandle;       // +0x00
        CCountedPointer<CMeshDataBank> MeshDataBank;     // +0x04
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }   // malloc-backed node allocator: no null test
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                            // +0x00
    _List_node_base* _M_prev;                            // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                         // +0x08
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00 the header node
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
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
    void pop_back()
    {
        iterator __tmp = end();
        erase(--__tmp);
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first)) erase(__first);
            __first = __next;
        }
    }
};
}

template _STL::list<CMeshBankManager::CMeshDataBankInfo>::iterator
    _STL::list<CMeshBankManager::CMeshDataBankInfo>::erase(_STL::list<CMeshBankManager::CMeshDataBankInfo>::iterator);