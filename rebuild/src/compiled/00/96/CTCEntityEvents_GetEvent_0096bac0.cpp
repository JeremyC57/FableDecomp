// CTCEntityEvents::GetEvent(TEvent** out, const TFilter& filter) const [TEvent = NEntityEvents::CEventHitBy, TFilter = CFilter_Bind3<NEntityEvents::CIsHitEventHitByShot, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsEventCreatedAfter>] -- the newest CEventHitBy event (type 0x1) that was a shot (0x00810F70), that was not blocked (0x00810F50 negated), created after the frame
// Leaf: GetEvent
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096BAC0, 172 bytes (+ int3 padding to 176) (`ret 8`, bool in al).
// Callees: GetType (vtable slot 1), the filter callees named below, the count block DeleteFunc, operator delete 0x00BFE9BC.
// Label: the catalog says CIsHitEventHitByMelee, but the first filter calls 0x00810F70, the shot test
// (0x00810F80 is the melee one, see the twin 0x0096BB70).
// Model: `this` is engine/CTCEntityEvents.h (Events +0x0c, opaque there: read through the local STLport list model);
// the counted pointer, the event classes and the filters are local models (CEventBase / CEventHitBase / filter
// sizes from Ego_r.pdb: the empty filters are 1 byte, the CFilter_BindN hold their filters by reference).
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

// Retail CThing (local model: retail sits 0x10 below Ego_r.pdb from ThingTypeFlags on):
// DefGlobalIndex +0x8c [PDB +0x9c], the state bit byte +0x91 [PDB +0xa1] (Dead = 0x01).
struct CThing {
    unsigned char  _pad_0x00[0x8c];
    unsigned short DefGlobalIndex;                    // +0x8c
    unsigned char  _pad_0x8e[0x91 - 0x8e];
    unsigned char  StateBits;                         // +0x91 Dead 0x01
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
struct CHitParameters;
// CEventHitBase (Ego_r.pdb): PHitParams CCountedPointer<CHitParameters> at +8; IsFlourish is slot 5.
// The accessors read retail CHitParameters (STLport map/vector 12 bytes: PThingHitting +0x50, PWeapon +0x58).
struct CEventHitBase : CEventBase {
    virtual bool IsFlourish() const;                  // slot 5: PHitParams->Flourish (+0x1d)
    CCountedPointer<CHitParameters> PHitParams;       // +0x08
    CThing* GetPThingHitting() const;                 // 0x00810FC0: PHitParams->PThingHitting.GetPItem()
    CThing* GetPWeapon() const;                       // 0x00810FD0: PHitParams->PWeapon.GetPItem()
    bool IsKnockDown() const;                         // 0x00810F00: PHitParams->KnockDown (+0x1a)
    bool IsDecapitate() const;                        // 0x00810F10: PHitParams->Decapitate (+0x1b)
};

struct CEventHitBy : CEventHitBase { enum { EventType = 0x1 }; };              // GetType 0x0083AF50
struct CIsEventCreatedAfter {
    long WorldFrame;
    bool operator()(const CEventBase* event) const { return event->WorldFrame > WorldFrame; }
};
struct CIsHitEventHitByShot {
    bool operator()(const CEventHitBase* event) const;   // 0x00810F70 (PHitParams damage-map shot test)
};
struct CIsHitEventBlocked {
    bool operator()(const CEventHitBase* event) const;   // 0x00810F50 (PHitParams->Blocked, +0x26)
};
}
namespace std {
template <class P> class unary_negate {               // STLport: !pred(x)
protected:
    P _M_pred;
public:
    template <class T> bool operator()(T x) const { return !_M_pred(x); }
};
}
template <class A, class B, class C> struct CFilter_Bind3 {     // every filter, held by reference
    const A& First;
    const B& Second;
    const C& Third;
    template <class T> bool operator()(T event) const { return First(event) && Second(event) && Third(event); }
};
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
    template <class TEvent, class TFilter> bool GetEvent(TEvent** out, const TFilter& filter) const;
};

// The newest event of TEvent's type that passes the filter: stored to *out, true; false when none.
template <class TEvent, class TFilter>
bool CTCEntityEventsMethods::GetEvent(TEvent** out, const TFilter& filter) const
{
    for (CEventList::reverse_iterator it = GetEvents().rbegin(); it != GetEvents().rend(); ++it)
    {
        CCountedPointer<NEntityEvents::CEventBase> event = *it;
        if (event->GetType() == TEvent::EventType && filter((TEvent*)event.Data))
        {
            *out = (TEvent*)event.Data;
            return true;
        }
    }
    return false;
}

template bool CTCEntityEventsMethods::GetEvent<NEntityEvents::CEventHitBy, CFilter_Bind3<NEntityEvents::CIsHitEventHitByShot, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsEventCreatedAfter > >(NEntityEvents::CEventHitBy**, const CFilter_Bind3<NEntityEvents::CIsHitEventHitByShot, std::unary_negate<NEntityEvents::CIsHitEventBlocked>, NEntityEvents::CIsEventCreatedAfter >&) const;