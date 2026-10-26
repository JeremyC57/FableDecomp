// CTCEntityEvents::GetEvent(TEvent** out, const TFilter& filter) const [TEvent = NEntityEvents::CEventHitBy, TFilter = CFilter_Bind2<NEntityEvents::CIsEventCreatedAfter, NEntityEvents::CIsHitEventHitByExplosion>] -- the newest "hit by" event created after the frame that the explosion filter accepts
// Grade: RELOCATION_MATCH (152v152) -- retry12, model from chunk 337: the real STLport reverse_iterator (copy base, decrement the copy, compare base()) keeps `this` in ebp and re-reads the filter from its slot as retail does
// Leaf: GetEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094E530, 152 bytes (+ int3 padding to 160) (`ret 8`, bool in al).
// Callees: GetType (vtable slot 1), CIsHitEventHitByExplosion::operator() 0x00810F60, the count block DeleteFunc,
// operator delete 0x00BFE9BC.
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
    struct const_iterator {
        SNode* _M_node;
        const_iterator(SNode* node) : _M_node(node) {}
        const CCountedPointer<CEventBase>& operator*() const { return _M_node->_M_data; }
        const_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    struct reverse_iterator {
        const_iterator current;
        reverse_iterator(const_iterator it) : current(it) {}
        const CCountedPointer<CEventBase>& operator*() const { const_iterator tmp = current; return *--tmp; }
        reverse_iterator& operator++() { --current; return *this; }
        const_iterator base() const { return current; }
        bool operator!=(const reverse_iterator& o) const { return base() != o.base(); }
    };
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }
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
// CEventHitBase accessors (out of line): the attacker and the knocked-down flag of the hit parameters.
struct CThing;
struct CEventHitByMethods : CEventBase {
    CThing* GetAttacker() const;                      // 0x00810FC0: PHitParams->Attacker.GetPItem()
    bool WasKnockedDown() const;                      // 0x00810F00: PHitParams byte +0x1a
};
class CIsHitEventHitAndKnockedDownBy { public:
    CThing* Attacker;
    bool operator()(const CEventBase& event) const
    {
        const CEventHitByMethods& hit = (const CEventHitByMethods&)event;
        return hit.GetAttacker() == Attacker && hit.WasKnockedDown();
    }
};
class CIsHitEventHitByExplosion { public:
    long Data;
    bool operator()(const CEventBase& event) const;   // 0x00810F60
};
}
namespace NEntityEvents {
class CEventHitBy : public CEventBase { public: enum { EventType = 0x1 }; };
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

template bool CTCEntityEventsMethods::GetEvent<NEntityEvents::CEventHitBy, CFilter_Bind2<NEntityEvents::CIsEventCreatedAfter, NEntityEvents::CIsHitEventHitByExplosion> >(NEntityEvents::CEventHitBy**, const CFilter_Bind2<NEntityEvents::CIsEventCreatedAfter, NEntityEvents::CIsHitEventHitByExplosion>&) const;