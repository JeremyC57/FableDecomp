// NUISystem::CObservable::SetEvent(EEvent event) -- deliver an event: to the exclusive observer alone, else to each concurrent exclusive observer, else to every observer (each one only if it observes the event)
// Grade: DIFFER(204v195) -- the copied list's sentinel is loaded into eax and copied to ebx (retail loads ebx straight from
// the frame word and reads begin through it), and the second loop head gets a 16-byte alignment pad
// Leaf: SetEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055CB10, 195 bytes (`ret 4`).
// Callees: the observer's virtual slots 2 (+0x08, IsObservingEvent(EEvent)) and 1 (+0x04, ProcessEvent(EEvent));
// list<CObserver*>::list(const list&) @ 0x0055CF50 (manifest "LinkedList_Init_Copy": `_M_node = 0`, sentinel malloc, then the
// range insert) and list<CObserver*>::~list() @ 0x0042AC25 (manifest "CActiveFile::OnReadFinished" is propagated).
// Label: CObservable vtable 0x01230044 slot 0 (rtti-vtables.tsv); Ego_r.pdb names ?SetEvent@CObservable@NUISystem@@UAEXW4EEvent@2@@Z.
// Layout: Observers +0x04, m_pExclusiveObserver +0x08, ConcurrentExclusiveObservers +0x0c (landed constructor 0x0042BE7B).
// Shape: the observers are walked over a COPY of the list (an observer may unregister while handling the event); each arm
// has its own copy in the same frame word, its own loop with the copy's sentinel cached in ebx (an `end` local before the
// loop; `it != observers.end()` re-reads the frame word every pass because the copy's address escapes), and its own out-of-line
// destructor call and return. The exclusive path tests IsObservingEvent and shares the function's final exit.
namespace NUISystem {
enum EEvent { EVENT_NONE };
class CObserver {
public:
    virtual ~CObserver();                          // slot 0
    virtual void ProcessEvent(EEvent event) = 0;   // slot 1 (+0x04)
    virtual bool IsObservingEvent(EEvent event);   // slot 2 (+0x08)
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
template <class T> class list {
public:
    typedef _List_node<T> _Node;
    struct iterator {
        _List_node_base* _M_node;
        iterator(_List_node_base* n) : _M_node(n) {}
        T& operator*() const { return ((_Node*)_M_node)->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _List_node_base* _M_node;
    list(const list& other);                       // 0x0055CF50
    ~list();                                       // 0x0042AC25
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    bool empty() const { return _M_node->_M_next == _M_node; }
};
}
namespace NUISystem {
class CObservable {                                // vtable 0x01230044
public:
    virtual void SetEvent(EEvent event);           // slot 0
    std::list<CObserver*> Observers;               // +0x04
    CObserver*            m_pExclusiveObserver;    // +0x08
    std::list<CObserver*> ConcurrentExclusiveObservers;   // +0x0c
};

void CObservable::SetEvent(EEvent event)
{
    if (m_pExclusiveObserver) {
        if (m_pExclusiveObserver->IsObservingEvent(event))
            m_pExclusiveObserver->ProcessEvent(event);
    }
    else if (!ConcurrentExclusiveObservers.empty()) {
        std::list<CObserver*> observers(ConcurrentExclusiveObservers);
        std::list<CObserver*>::iterator it = observers.begin();
        std::list<CObserver*>::iterator end = observers.end();
        for (; it != end; ++it) {
            if ((*it)->IsObservingEvent(event))
                (*it)->ProcessEvent(event);
        }
    }
    else {
        std::list<CObserver*> observers(Observers);
        std::list<CObserver*>::iterator it = observers.begin();
        std::list<CObserver*>::iterator end = observers.end();
        for (; it != end; ++it) {
            if ((*it)->IsObservingEvent(event))
                (*it)->ProcessEvent(event);
        }
    }
}
}
