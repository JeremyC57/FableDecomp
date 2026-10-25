// NUISystem::CObservable::SetOwnedEvent(CObserver* owner, EEvent event) -- forward event to the observer that owns it: copy the Observers list, then for each entry that is the owner and is observing the event call its ProcessEvent(event)
// Leaf: ?SetOwnedEvent@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055CBE0, 141 bytes, `ret 8` (thiscall: owner, event); vtable 0x01230044 slot 1.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the local list's sentinel and nodes), the list's out-of-line
//   `_M_insert_dispatch(pos, first, last, const __false_type&)` 0x0055CE90 (`ret 10h`, catalogued
//   "LinkedList_CopyElements"; modelled as a same-TU noinline body that never touches `this`, so, as in retail, ecx and
//   the tag reference both point at the `event` argument slot, and `event` is re-read from it after the call),
//   CObserver::IsObservingEvent (slot 2, +8), CObserver::ProcessEvent (slot 1, +4).
// Label correction: catalogued as "LinkedList_FilterByPredicate"; Ego_r.pdb
//   `?SetOwnedEvent@CObservable@NUISystem@@UAEXPAVCObserver@2@W4EEvent@2@@Z`.
// Layout: CObservable { vptr, STLport list<CObserver*> Observers +0x04 (the sentinel pointer only) } as in the landed
//   CObservable::RemoveAllObservers 0x0055C950. Local model (no engine header for NUISystem::CObservable).
// Shape: STLport 4.x list copy constructor inlined (`_List_base` allocates the self-linked sentinel, then
//   `insert(begin(), x.begin(), x.end())` -> `_M_insert_dispatch`), the loop re-reads the node data for the second
//   virtual call, and `~_List_base` is the inline clear + sentinel free (recipe of the landed 0x00A84470).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace NUISystem {
enum EEvent { EVENT_NONE };
class CObserver {
public:
    virtual ~CObserver();
    virtual void ProcessEvent(EEvent event);            // slot 1 (+0x04)
    virtual bool IsObservingEvent(EEvent event);        // slot 2 (+0x08)
};
}
using NUISystem::CObserver;

namespace std {
struct __false_type { __false_type() { } };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct _List_node : _List_node_base {                   // 12 bytes
    CObserver* _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
    CObserver*& operator*() const { return ((_List_node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& y) const { return _M_node != y._M_node; }
};
struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_const_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
};
template <class T> struct allocator {
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
struct _List_base {
    _STLP_alloc_proxy<_List_node> _M_node;
    _List_base(const allocator<_List_node>& a) : _M_node(a, (_List_node*)0)
    {
        _List_node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _List_node* cur = (_List_node*)_M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _List_node* tmp = cur;
            cur = (_List_node*)cur->_M_next;
            _M_node.deallocate(tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};
struct list : _List_base {
    allocator<_List_node> get_allocator() const { return allocator<_List_node>(); }
    _List_iterator begin() { return _List_iterator(_M_node._M_data->_M_next); }
    _List_iterator end() { return _List_iterator(_M_node._M_data); }
    _List_const_iterator begin() const { return _List_const_iterator(_M_node._M_data->_M_next); }
    _List_const_iterator end() const { return _List_const_iterator(_M_node._M_data); }
    __declspec(noinline) void _M_insert_dispatch(_List_iterator pos, _List_const_iterator first, _List_const_iterator last,
                                                 const __false_type&)   // 0x0055CE90 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _List_node* n = _M_node.allocate(1);
            new (&n->_M_data) CObserver*(((_List_node*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(_List_iterator pos, _List_const_iterator first, _List_const_iterator last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};
}

namespace NUISystem {
class CObservable {
public:
    virtual ~CObservable();
    virtual void SetOwnedEvent(CObserver* owner, EEvent event);   // slot 1 (+0x04)
    std::list Observers;                               // +0x04
};

void CObservable::SetOwnedEvent(CObserver* owner, EEvent event)
{
    std::list observers(Observers);
    for (std::_List_iterator it = observers.begin(); it != observers.end(); ++it) {
        if (*it == owner && (*it)->IsObservingEvent(event))
            (*it)->ProcessEvent(event);
    }
}
}