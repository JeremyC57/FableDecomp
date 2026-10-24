// CTCEntityEvents::HasEvent(const TFilter& filter) const [TFilter = NEntityEvents::CIsEventOfTypeCreatedAfter] -- true when any event of the filter type created after the filter frame is in the event list
// Leaf: HasEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956650, 70 bytes (`ret 4`, bool in al).
// Callees: GetType (vtable slot 1) only; every filter is inline.
// Model: `this` is engine/CTCEntityEvents.h (Events +0x0c, opaque there: read through the local STLport list model);
// the event base and the filters are local models (CEventBase from Ego_r.pdb; CFilter_Bind3 holds its filters by reference).
// Shape: the same newest-first walk as the GetEvent siblings, but the event is read straight from the list
// node (`(*it).Data`), no counted-pointer copy, so there is no release block; rend() is re-read through `this`.
#include "engine/CTCEntityEvents.h"

void __cdecl operator delete(void* block);            // 0x00BFE9BC

// CCountedPointer<T>: the object and its shared count block {RefCount, __fastcall DeleteFunc(Data), Data}.
struct CCPPointerInfo {
    long  RefCount;                                   // +0x00
    void (__fastcall* DeleteFunc)(void* data);        // +0x04
    void* Data;                                       // +0x08
};
template <class T> struct CCountedPointer {
    T*              Data;                             // +0x00
    CCPPointerInfo* Info;                             // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    T* operator->() const { return Data; }
};

namespace NEntityEvents {
// NEntityEvents::CEventBase (Ego_r.pdb: vptr +0, WorldFrame +4); GetType is vtable slot 1.
struct CEventBase {
    virtual ~CEventBase();
    virtual long GetType() const;                     // slot 1
    virtual void OnAdd(const CTCEntityEvents& events);
    virtual long GetLifeTime() const;
    virtual bool IsReadyToDie() const;
    long WorldFrame;                                  // +0x04
};
struct CIsEventOfTypeCreatedAfter {                // one functor: the type and the creation frame
    long Type;
    long WorldFrame;
    bool operator()(const CEventBase* event) const { return event->GetType() == Type && event->WorldFrame > WorldFrame; }
};
}
// CTCEntityEvents::Events (+0x0c): STLport list<CCountedPointer<CEventBase> > (the node pointer at +0),
// walked newest first through STLport's reverse_iterator over the list iterator (`*rit` copies the base
// iterator and decrements the copy: that STLport form, not a hand-folded `current->_M_prev`, gives retail's
// register choice here; the hand-folded form is kept where it is the one that matches).
struct CEventList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CCountedPointer<NEntityEvents::CEventBase> _M_data; };
    SNode* _M_node;
    struct const_iterator {                           // STLport _List_iterator
        SNode* _M_node;
        const_iterator(SNode* node) : _M_node(node) {}
        const CCountedPointer<NEntityEvents::CEventBase>& operator*() const { return _M_node->_M_data; }
        const_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    struct reverse_iterator {                         // STLport reverse_iterator: *it is *--(copy of base)
        const_iterator current;
        reverse_iterator(const_iterator it) : current(it) {}
        const CCountedPointer<NEntityEvents::CEventBase>& operator*() const { const_iterator tmp = current; return *--tmp; }
        reverse_iterator& operator++() { --current; return *this; }
        const_iterator base() const { return current; }
        bool operator!=(const reverse_iterator& o) const { return base() != o.base(); }
    };
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }
};


struct CTCEntityEventsMethods : CTCEntityEvents {
    const CEventList& GetEvents() const { return *(const CEventList*)Events; }
    template <class TFilter> bool HasEvent(const TFilter& filter) const;
};

// True when any event, newest first, passes the filter (the counted pointers are only read, not copied).
template <class TFilter>
bool CTCEntityEventsMethods::HasEvent(const TFilter& filter) const
{
    for (CEventList::reverse_iterator it = GetEvents().rbegin(); it != GetEvents().rend(); ++it)
    {
        if (filter((*it).Data))
            return true;
    }
    return false;
}

template bool CTCEntityEventsMethods::HasEvent<NEntityEvents::CIsEventOfTypeCreatedAfter >(const NEntityEvents::CIsEventOfTypeCreatedAfter&) const;