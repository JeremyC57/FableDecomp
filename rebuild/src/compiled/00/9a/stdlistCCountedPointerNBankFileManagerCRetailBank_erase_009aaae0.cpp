// std::list<CCountedPointer<NBankFileManager::CRetailBank> >::erase(iterator position) -- STLport single-node erase: unlink the node, release its counted pointer (deleter + count block freed at zero) and clear it, free the node, return the iterator to the following node
// Leaf: ?erase@?$list@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AAAE0, 85 bytes, thiscall, `ret 8` (hidden iterator result + the iterator by value).
// Identity (probable): its only call is from the unreferenced pop_back COMDAT 0x009AAB40 (`erase(iterator(_M_node->_M_prev))`); it sits in the CBankFileManager TU right after the landed list<CCountedPointer<NBankFileManager::CRetailBank> >::push_back 0x009AAAA0 (CBankFileManager::RetailBanks), the TU's only list of counted pointers.
// Callees: the count block's __fastcall DeleteFunc (`call [Info+4]`, Data in ecx), operator delete 0x00BFE9BC (the block),
//   free 0x00BFEA14 (the node allocator's deallocate).
// Shape (family K018, representative 0x00719210): agent952 H020's STLport erase (next/prev read from the node,
//   prev->_M_next = next, next->_M_prev = prev, `_Destroy(&n->_M_data)`, deallocate, `return iterator(next)`), in a speed
//   TU so CCountedPointer's destructor is inline: the release re-reads Info after the decrement, then both words are zeroed.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CCPPointerInfo {                                    // Ego_r.pdb, 12 bytes
public:
    unsigned long RefCount;                               // +0x00
    void (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                           // +0x08
};
template <class T> class CCountedPointer {                // Ego_r.pdb: Data, Info
public:
    T*              Data;                                 // +0x00
    CCPPointerInfo* Info;                                 // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ::operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
namespace NBankFileManager { class CRetailBank; }
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

template list<CCountedPointer<NBankFileManager::CRetailBank > >::iterator list<CCountedPointer<NBankFileManager::CRetailBank > >::erase(iterator);

} // namespace std