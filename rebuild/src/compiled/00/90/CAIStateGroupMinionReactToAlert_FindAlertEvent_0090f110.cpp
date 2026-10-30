// CAIStateGroup_MinionReactToAlert::FindAlertEvent() -- the most recent alert message (type 12) addressed to this
// Grade: RELOCATION_MATCH (was DIFFER 146v154 nd=21) -- lever: the originator read written in the landed 0x00744D00
//   shape `POriginator.GetPItem() != 0 && IsThingOfType<CThing>(POriginator.GetPItem())` then the item fetched again
//   (CThing::ThingTypeMask 0, ThingTypeFlags +0x6c): gives retail's three GetPItem calls, the dead frame store and xor
//   dl,dl; jne.
// creature whose originator is alive and an ally of the creature.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F110, length 154 bytes (+ int3 padding).
// Callees: the second world accessor @ 0x008FCFA0 (message manager at +0x60), CMessageEventManager::
// FindMostRecentMessage<CFilter_Bind2<IsOfType, IsForThing>> @ 0x00746260, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (three times on the event's POriginator at +0x48: the typed read, see GetTypedThing below),
// GFAreThingsAllies @ 0x0076D4F0 (__fastcall: originator, creature).
// Shape: filter chain `IsOfType(12) && IsForThing(creature)` by const reference (0x10-byte frame: type at +8,
// creature at +0xc, Bind2 pointers at +0x10..+0x14); the event is kept in edi, the originator pointer address
// in ebx; `xor eax,eax` null result shared by every failed test.
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
    enum { ThingTypeMask = 0 };                 // the base class matches every thing (landed 0x00744D00 / 0x008D04E0)
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x6c - 0x4c];
    unsigned long ThingTypeFlags;               // +0x6c
    unsigned char _pad2[0x91 - 0x70];
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

struct CThingUpcast;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
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
    CWorld& GetWorld() const;                   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0 (Ego_r.pdb ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ)
    const CWorld& PeekWorld() const;            // 0x008FCFA0 (Ego_r.pdb ?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ)
public:
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

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
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (out of line, `ret 4`)
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

// CMessageEvent (PDB): Type +8, OriginatorUID +0x20, TimeStamp +0x34, Lifespan +0x38, MessageID +0x40, POriginator +0x48
class CMessageEvent { public:
    unsigned char _pad0[0x20];
    unsigned __int64 OriginatorUID;             // +0x20
    unsigned char _pad1[0x14];
    void* PSource;                              // +0x3c
    long MessageID;                             // +0x40
    unsigned char _pad2[4];
    CBaseIntelligentPointer POriginator;        // +0x48 CIntelligentPointer<CThing>
};

class CMessageEventManager;
class CWorld {
public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() const { return m_pMessageManager; }
};

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsForThing> CAlertFilter;

class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CAlertFilter& filter);   // 0x00746260
};

bool __fastcall GFAreThingsAllies(CThing* a, CThing* b);    // 0x0076D4F0
struct CThingUpcast : CThing { };

// The typed read of an intelligent pointer as retail inlines it in this module: a null test, the item bound to
// the const-reference parameter of a (release-empty) type check whose constant-false result is still
// materialised (`xor dl,dl ; mov [slot],eax ; jne`: the referenced temporary is homed in a dead slot), then the
// item fetched once more as the result.
template <class T> inline bool IsThingOfType(const CThing* const& thing)   // landed 0x00744D00 shape
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

struct CAIStateGroup_MinionReactToAlert : CAIStateGroupBase {
    CMessageEvent* FindAlertEvent();
};

CMessageEvent* CAIStateGroup_MinionReactToAlert::FindAlertEvent()
{
    CMessageEvent* event = PeekWorld().GetPMessageManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(12) && CMessageEventFilter_IsForThing(PBrain->PCreature));
    if (event) {
        CThing* originator = 0;
        if (event->POriginator.GetPItem() != 0 && IsThingOfType<CThing>(event->POriginator.GetPItem()))
            originator = event->POriginator.GetPItem();
        if (originator != 0 && !(originator->m_flags91 & 1)) {
            if (GFAreThingsAllies(originator, PBrain->PCreature))
                return event;
        }
    }
    return 0;
}