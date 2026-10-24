// std::list<CCountedPointer<CModeBase> >::push_front(const CCountedPointer<CTCCreatureModeManager::CModeBase>& x) -- malloc a 0x10-byte node, copy the counted pointer into it (bumping the shared count) and link it in front of the first mode
// Leaf: ?push_front@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007421C0, 61 bytes, __fastcall `ret 4`.
// Callees: malloc @ 0x00BFEA0E (the STLport node allocator; MSVCR71 import thunk).
// List: CTCCreatureModeManager::PModes (Ego_r.pdb `list<CCountedPointer<CTCCreatureModeManager::CModeBase> >`; the
//   retail STLport list is the one sentinel pointer; its clear() is the landed 0x00742290).
// Label: the manifest's "ModeList_PushBack" is wrong about the end: the insert position is the sentinel's NEXT node
//   (`mov eax,[ecx] ; mov esi,[eax]`), i.e. begin() -- STLport `push_front(x) { insert(begin(), x); }`.
// Shape: the inline insert of the landed list push_back 0x00567FC0 at begin(): begin() is read before the node is
//   malloc'd, placement new tests &node->_M_data, the CCountedPointer copy constructor copies both words and bumps
//   Info->RefCount when Info is set, then the four link stores.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

struct CCPPointerInfo {
    unsigned long RefCount;                             // +0x00
    void (__cdecl* DeleteFunc)(void*);                  // +0x04
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

class CTCCreatureModeManager {
public:
    class CModeBase;
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;                           // the sentinel (retail list = one pointer)

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_front(const _Tp& __x);
};

template <class _Tp>
void list<_Tp>::push_front(const _Tp& __x)
{
    insert(begin(), __x);
}

template void list<CCountedPointer<CTCCreatureModeManager::CModeBase> >::push_front(const CCountedPointer<CTCCreatureModeManager::CModeBase>&);

}