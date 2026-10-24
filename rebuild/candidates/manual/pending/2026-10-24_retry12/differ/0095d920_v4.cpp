// CTCEntityEvents::GetEvent(TEvent** out, const TFilter& filter) const [TEvent = NEntityEvents::CEventFiredShot, TFilter = NEntityEvents::CIsEventCreatedAfter] -- the newest event of type 0x26 (CEventFiredShot) created after the filter's frame
// Grade: DIFFER(137v137) -- register swap only: retail keeps the reverse iterator in edi and the event in ebx (ours ebx/edi), and pushes edi after `mov ebp,ecx`; tried a typed local, a function-scope typed local, direct-init copy
// Leaf: GetEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0095D920, 137 bytes (`ret 8`, bool in al).
// Callees: GetType (vtable slot 1), the count block DeleteFunc, operator delete 0x00BFE9BC.
// Identity: event type 0x26 is the constant GetType of CEventFiredShot (its RTTI vtable slot 1).
// Model: `this` is engine/CTCEntityEvents.h (Events +0x0c, opaque there: read through the local STLport list model);
// the counted pointer, the event base and the filters are local models (CEventBase from Ego_r.pdb).
// Shape: the loop copies each counted pointer (count ++ / -- with the DeleteFunc + operator delete release on
// both exits); rend() is re-read through `this` every iteration; *out is stored before the copy is released.
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

// NEntityEvents::CEventBase (Ego_r.pdb: vptr +0, WorldFrame +4); GetType is vtable slot 1.
struct CEventBase {
    virtual ~CEventBase();
    virtual long GetType() const;
    long WorldFrame;                                  // +0x04
};

// CTCEntityEvents::Events (+0x0c): STLport list<CCountedPointer<CEventBase> > (the node pointer at +0),
// walked newest first through a reverse_iterator (`*rit` is the node before the current one).
struct CEventList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CCountedPointer<CEventBase> _M_data; };
    SNode* _M_node;
    struct reverse_iterator {
        SNode* current;
        reverse_iterator(SNode* node) : current(node) {}
        const CCountedPointer<CEventBase>& operator*() const { SNode* tmp = current->_M_prev; return tmp->_M_data; }
        reverse_iterator& operator++() { current = current->_M_prev; return *this; }
    };
    reverse_iterator rbegin() const { return reverse_iterator(_M_node); }
    reverse_iterator rend() const { return reverse_iterator(_M_node->_M_next); }
};
inline bool operator!=(const CEventList::reverse_iterator& x, const CEventList::reverse_iterator& y) { return x.current != y.current; }

namespace NEntityEvents {
struct CIsEventCreatedAfter {
    long WorldFrame;
    bool operator()(const CEventBase& event) const { return event.WorldFrame > WorldFrame; }
};
struct CIsEventCreatedBeforeOrOn {
    long WorldFrame;
    bool operator()(const CEventBase& event) const { return event.WorldFrame <= WorldFrame; }
};
struct CIsEventOfType {
    long Type;
    bool operator()(const CEventBase& event) const { return event.GetType() == Type; }
};
}
template <class A, class B> struct CFilter_Bind2 {     // both filters, held by reference
    const A& First;
    const B& Second;
    template <class T> bool operator()(const T& event) const { return First(event) && Second(event); }
};
namespace NEntityEvents {
struct CEventFiredShot : CEventBase { enum { EventType = 0x26 }; };
}

struct CTCEntityEventsMethods : CTCEntityEvents {
    const CEventList& GetEvents() const { return *(const CEventList*)Events; }
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter& filter) const;
};

// The newest event of TEvent's type that passes the filter: stored to *out, true; false when none.
template <class TEvent, class TFilter>
bool CTCEntityEventsMethods::GetEvent(TEvent** out, const TFilter& filter) const
{
    for (CEventList::reverse_iterator it = GetEvents().rbegin(); it != GetEvents().rend(); ++it)
    {
        CCountedPointer<CEventBase> event = *it;
        if (event->GetType() == TEvent::EventType && filter(*(TEvent*)event.Data))
        {
            *out = (TEvent*)event.Data;
            return true;
        }
    }
    return false;
}

template bool CTCEntityEventsMethods::GetEvent<NEntityEvents::CEventFiredShot, NEntityEvents::CIsEventCreatedAfter >(NEntityEvents::CEventFiredShot**, const NEntityEvents::CIsEventCreatedAfter&) const;
