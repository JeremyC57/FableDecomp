// std::list::remove_if(NEntityEvents::CConvFilterToCP<NEntityEvents::CIsEventOfType> pred) -- drop every entity event whose GetType() equals the wrapped filter's type
// Instantiation: std::list<CCountedPointer<NEntityEvents::CEventBase> >::remove_if
// Leaf: ??$remove_if@V?$CConvFilterToCP@VCIsEventOfType@NEntityEvents@@@NEntityEvents@@@?$list@V?$CCountedPointer@VCEventBase@NEntityEvents@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC890, 106 bytes, thiscall, `ret 4` (the one-word predicate by value).
// Identity: FableWin.pdb `??$remove_if@V?$CConvFilterToCP@VCIsEventOfType@NEntityEvents@@@NEntityEvents@@@?$list@V?$CCountedPointer
//   @VCEventBase@NEntityEvents@@@@...`; CTCEntityEvents::RemoveEvents<CIsEventOfType>(const CIsEventOfType&) wraps the filter's
//   address (callers 0x008A7A59 CGameScriptInterface::EntityStopFollowing, 0x00981E61, and the jmp thunks 0x008AD460 / 0x008AD703).
// Callees: CEventBase virtual slot 1 GetType (+0x04), the count block's DeleteFunc, operator delete @ 0x00BFE9BC, free @ 0x00BFEA14.
// Layout: Ego_r.pdb CConvFilterToCP<CIsEventOfType> { CIsEventOfType* Filter +0 }, CIsEventOfType { EEventType Type +0 }.
// Shape: STLport 4.6 `remove_if` + 4.6 `erase` (as 0x0073E790); `(*Filter)(p.Data)` = `e->GetType() == Type`, the filter
//   pointer read after the virtual call.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CCPPointerInfo {                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    T* operator->() const { return Data; }
    bool operator==(const CCountedPointer& other) const { return Data == other.Data; }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

class CTCEntityEvents;
namespace NEntityEvents {
enum EEventType { EVENT_TYPE_NONE };
class CEventBase {                                          // Ego_r.pdb: vptr +0, WorldFrame +4 (vtable 0x0124F2C4)
public:
    virtual ~CEventBase();                                  // slot 0
    virtual EEventType GetType() const = 0;                 // slot 1 (pure in the base)
    virtual void OnAdd(const CTCEntityEvents& events);      // slot 2
    virtual long GetLifeTime() const;                       // slot 3 (base: the frame rate, one second)
    virtual bool IsReadyToDie() const;                      // slot 4 (base: true)
    long GetWorldFrame() const { return WorldFrame; }
    bool IsExpired(long worldFrame) const { return worldFrame > WorldFrame + GetLifeTime(); }
    long WorldFrame;                                        // +0x4 creation frame
};
}
namespace NEntityEvents {
class CIsEventOfType {                                      // Ego_r.pdb: Type +0
public:
    EEventType Type;
    bool operator()(const CEventBase* e) const { return e->GetType() == Type; }
};
template <class F> class CConvFilterToCP {                  // Ego_r.pdb: F* Filter +0
public:
    F* Filter;
    bool operator()(const CCountedPointer<CEventBase>& e) const { return (*Filter)(e.Data); }
};
}

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
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
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
        return iterator(__next_node);
    }
    void remove(const _Tp& __val)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first)
                erase(__first);
            __first = __next;
        }
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};
}

template void std::list<CCountedPointer<NEntityEvents::CEventBase> >::remove_if(NEntityEvents::CConvFilterToCP<NEntityEvents::CIsEventOfType>);