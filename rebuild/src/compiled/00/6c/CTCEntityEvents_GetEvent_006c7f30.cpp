// CTCEntityEvents::GetEvent(TEvent** out, const TFilter& filter) const [TEvent = NEntityEvents::CEventHeroHasBeenSpottedStealing, TFilter = NEntityEvents::CIsEventOfType] -- the newest event of type 0x15 (CEventHeroHasBeenSpottedStealing) whose type is also the filter's type
// Leaf: GetEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C7F30, 141 bytes (`ret 8`, bool in al).
// Callees: GetType (vtable slot 1, twice: the type test and CIsEventOfType), the count block DeleteFunc, operator delete 0x00BFE9BC.
// Identity: the catalog labels every one of these byte-similar bodies CEventChildHurtGoToMother; the event type
// 0x15 is the constant GetType of CEventHeroHasBeenSpottedStealing (its RTTI vtable slot 1).
// Model: `this` is engine/CTCEntityEvents.h (Events +0x0c, opaque there: read through the local STLport list model);
// the counted pointer, the event base and the filters are local models (CEventBase from Ego_r.pdb).
// Shape: the loop copies each counted pointer (count ++ / -- with the DeleteFunc + operator delete release on
// both exits); rend() is re-read through `this` every iteration; *out is stored before the copy is released.
#include "engine/CTCEntityEvents.h"

void __cdecl operator delete(void* block);            // 0x00BFE9BC

// CCountedPointer<T>: the object and its shared count block {RefCount, __fastcall DeleteFunc(Data), Data}.
class CCPPointerInfo { public:
    long  RefCount;                                   // +0x00
    void (__fastcall* DeleteFunc)(void* data);        // +0x04
    void* Data;                                       // +0x08
};
template <class T> class CCountedPointer { public:
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
        bool operator!=(const reverse_iterator& o) const { return current != o.current; }
    };
    reverse_iterator rbegin() const { return reverse_iterator(_M_node); }
    reverse_iterator rend() const { return reverse_iterator(_M_node->_M_next); }
};

namespace NEntityEvents {
class CIsEventCreatedAfter { public:
    long WorldFrame;
    bool operator()(const CEventBase& event) const { return event.WorldFrame > WorldFrame; }
};
class CIsEventCreatedBeforeOrOn { public:
    long WorldFrame;
    bool operator()(const CEventBase& event) const { return event.WorldFrame <= WorldFrame; }
};
class CIsEventOfType { public:
    long Type;
    bool operator()(const CEventBase& event) const { return event.GetType() == Type; }
};
}
template <class A, class B> class CFilter_Bind2 { public:     // both filters, held by reference
    const A& First;
    const B& Second;
    template <class T> bool operator()(const T& event) const { return First(event) && Second(event); }
};
namespace NEntityEvents {
class CEventHeroHasBeenSpottedStealing : public CEventBase { public: enum { EventType = 0x15 }; };
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

template bool CTCEntityEventsMethods::GetEvent<NEntityEvents::CEventHeroHasBeenSpottedStealing, NEntityEvents::CIsEventOfType >(NEntityEvents::CEventHeroHasBeenSpottedStealing**, const NEntityEvents::CIsEventOfType&) const;