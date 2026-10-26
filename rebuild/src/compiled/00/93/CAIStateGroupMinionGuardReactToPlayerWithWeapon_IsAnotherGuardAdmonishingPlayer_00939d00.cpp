// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsAnotherGuardAdmonishingPlayer() -- is there a current
// "guard warning" message (type 0x1b) that did NOT originate from this creature; on a hit adopt its id and source.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939D00, length 104 bytes (+ int3 padding).
// Callees: the state group's second world accessor @ 0x008FCFA0 (message manager at +0x60),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, (empty filter)>> @ 0x00939C60 taking
// the bound filter by pointer.
// Shape: the filter is the expression chain `IsOfType(0x1b) && HasExtraData()` passed by const reference:
// 0x10-byte frame with the empty one-byte filter in the high byte of the first slot (`lea ecx,[esp+7]`), the
// type at +8 and the Bind2 pair of pointers at +0xc..+0x10; the manager is reached through an inline accessor
// so its load follows the filter stores. The originator test is the 64-bit UID compare `event->OriginatorUID
// != creature->UID` (low word `jne` to the hit, high word `je` to the miss); then MessageID (+0x40) and the
// source (+0x3c) are copied into +0x1c / +0x14 and `mov al,1`; shared `xor al,al` exit. Layout local (no
// engine header; the sibling 0x00939D70 is landed with these names).
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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
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
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData> CWarningFilter;

class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage(const CWarningFilter& filter);   // 0x00939C60
};

struct CThingUID {                              // CThing's 64-bit UID at +0x18 (PDB)
    unsigned char _pad[0x18];
    unsigned __int64 UID;
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();               // 0x008FCFA0
};

struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBaseW {
    void* m_pWarningSource;                 // +0x14
    long m_frameWarned;                     // +0x18
    long m_lastWarningId;                   // +0x1c
    bool IsAnotherGuardAdmonishingPlayer();
};

bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsAnotherGuardAdmonishingPlayer()
{
    CMessageEvent* event = GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0x1b) && CMessageEventFilter_HasExtraData());
    if (event) {
        CThingUID* creature = (CThingUID*)PBrain->PCreature;
        if (event->OriginatorUID != creature->UID) {
            m_lastWarningId = event->MessageID;
            m_pWarningSource = event->PSource;
            return true;
        }
    }
    return false;
}