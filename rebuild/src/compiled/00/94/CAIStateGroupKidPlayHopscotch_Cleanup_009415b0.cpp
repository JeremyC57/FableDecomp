// CAIStateGroup_KidPlayHopscotch::Cleanup() -- while the hopscotch marker is alive: free its action point if the kid
// still holds it and withdraw the kid's own "hopscotch started" message (type 0x18) from the message manager.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009415B0, length 222 bytes (+ int3 padding). Vtable slot 9.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (marker pointer at +0x15, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface, 0x2d = bit 13 of +0x24), CTCUserActionPointBase::IsActionPointFree
// @ 0x007FEBF0, the action point's release(slot) @ 0x007FEF40 (manifest label
// CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera propagated), CAIStateGroupBase::GetPWorld @ 0x008FCFC0
// (twice), CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, IsOriginatedByThing>> @ 0x0090DEE0,
// CMessageEventManager::RemoveMessage @ 0x00493B5B.
// Shape: inlined IS_THING_ALIVE on the unaligned marker pointer (pack(1): the bool at +0x14 precedes it, as in the
// landed OnActivate 0x00941550); the action-point lookup result used unconditionally (esi); the filter chain
// `IsOfType(0x18) && IsOriginatedByThing(creature)` built in the frame with the UID filter's constructor inline
// here (two dword copies); the manager is re-fetched through GetPWorld for the remove.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
public:
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

class CMessageEventFilter_HasExtraData { public:
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing)     // inline in this TU: the thing's UID at +0x18
        : UID(*(const unsigned __int64*)((const unsigned char*)thing + 0x18)) { }
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

class CMessageEvent;
class CMessageEventManager;
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
};

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing> COwnMessageFilter;

class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage_0090dee0(const COwnMessageFilter& filter);   // 0x0090DEE0
public:
    void RemoveMessage(const CMessageEvent*);   // 0x00493B5B
};

struct CTCUserActionPointBase {                 // interface 0x2d
    bool IsActionPointFree();                   // 0x007FEBF0
    void ReleaseSlot(long slot);                // 0x007FEF40
};

#pragma pack(push, 1)
struct CAIStateGroup_KidPlayHopscotch : CAIStateGroupBase {
    bool m_initiated;                           // +0x14
    CIntelligentPointer<CThing> m_pMarker;          // +0x15 CIntelligentPointer<CThing> (unaligned)
    unsigned char _pad1d[3];                    // +0x1d
    long m_slot;                                // +0x20 the held action-point slot
    virtual void Cleanup();
};
#pragma pack(pop)

void CAIStateGroup_KidPlayHopscotch::Cleanup()
{
    if (IS_THING_ALIVE(m_pMarker)) {
        CTCUserActionPointBase* point;
        m_pMarker.GetPItem()->GetInterface(&point, 0x2d);
        if (!point->IsActionPointFree())
            point->ReleaseSlot(m_slot);
        CMessageEvent* event = GetWorld().GetPMessageManager()->FindMostRecentMessage_0090dee0(
            CMessageEventFilter_IsOfType(0x18) && CMessageEventFilter_IsOriginatedByThing(PBrain->PCreature));
        if (event)
            GetWorld().GetPMessageManager()->RemoveMessage(event);
    }
}