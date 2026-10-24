// NUISystem::CObservable::AddConcurrentExclusiveObserver(CObserver* observer) -- register a concurrent exclusive observer once: append it unless it is already listed
// Leaf: AddConcurrentExclusiveObserver
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055CA90, 69 bytes (`ret 4`).
// Callee: malloc @ 0x00BFEA0E (the list node).
// Label: CObservable vtable 0x01230044 slot 9 (rtti-vtables.tsv); Ego_r.pdb names
// ?AddConcurrentExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z. Slot 2 (0x0055CA40, AddObserver) is the
// same body on Observers.
// Layout: as the landed CObservable constructor 0x0042BE7B: Observers +0x04, m_pExclusiveObserver +0x08,
// ConcurrentExclusiveObservers +0x0c (each STLport list the 4-byte sentinel pointer).
// Shape: STLport `std::find` inlined over list iterators (value in edi, `nop` loop alignment) into a NAMED iterator (the
// find written inside the `if` compares with the cached end and jumps past the re-read); when it reaches `end()`
// (re-read into esi) the inlined push_back: node malloc, null-tested placement copy, four link stores before the sentinel.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);        // 0x00BFEA14
inline void* operator new(size_t, void* where) { return where; }

namespace NUISystem {
enum EEvent { EVENT_NONE };
class CObserver {                                  // vtable 0x01246020
public:
    virtual ~CObserver();                          // slot 0
    virtual void ProcessEvent(EEvent event) = 0;   // slot 1 (+0x04)
    virtual bool IsObservingEvent(EEvent event);   // slot 2 (+0x08), 0x0052D900
    virtual void ObserveEvent(EEvent event);       // slot 3 (+0x0c), 0x0052DA20
    virtual void IgnoreEvent(EEvent event);        // slot 4 (+0x10), 0x0052D940
    virtual void IgnoreAllEvents();                // slot 5 (+0x14), 0x0052D9A0
};
}

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> class list {                    // STLport list: the 4-byte sentinel pointer
public:
    typedef _List_node<T> _Node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* n) : _M_node(n) {}
        T& operator*() const { return ((_Node*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator==(const iterator& o) const { return _M_node == o._M_node; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        new (&p->_M_data) T(x);
        return p;
    }
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const T& x) { insert(end(), x); }
    iterator erase(iterator position)
    {
        _List_node_base* next = position._M_node->_M_next;
        _List_node_base* prev = position._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(position._M_node);
        return iterator(next);
    }
};
template <class I, class T> inline I find(I first, I last, const T& value)
{
    while (first != last && !(*first == value))
        ++first;
    return first;
}
}
namespace NUISystem {
class CObservable {                                // vtable 0x01230044
public:
    virtual void v000();
    std::list<CObserver*> Observers;               // +0x04
    CObserver*            m_pExclusiveObserver;    // +0x08
    std::list<CObserver*> ConcurrentExclusiveObservers;   // +0x0c
    void AddConcurrentExclusiveObserver(CObserver* observer);
};

void CObservable::AddConcurrentExclusiveObserver(CObserver* observer)
{
    std::list<CObserver*>::iterator it = std::find(ConcurrentExclusiveObservers.begin(), ConcurrentExclusiveObservers.end(), observer);
    if (it == ConcurrentExclusiveObservers.end())
        ConcurrentExclusiveObservers.push_back(observer);
}
}