// CAIStateGroup_MinionGossip::OnReplace() -- drop the pending gossip message from this creature, clear the CTCGossip target, remove the gossip mode (0x1B) and remember the frame of the last gossip
// Grade: RELOCATION_MATCH -- two levers together: the B-17 same-TU noinline FindMostRecentMessage stand-in (reads the const-reference filter, nothing escapes) puts the bound filter's first-pointer store before the UID-filter ctor call, and the creature read into a named local ahead of the call chain (`CThing* creature = PBrain->PCreature;`) gives retail's this=esi / message=edi and the late type-word store
// Leaf: OnReplace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090CA60, length 287 bytes (+ int3 padding).
// Callees: CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF2B0, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (x2),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, IsOriginatedByThing>> @ 0x0090DEE0,
// CMessageEventManager::RemoveMessage @ 0x00493B5B, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0xC1 =
// CTCGossip bit 1 of +0x38, 0x31 = CTCModes bit 17 of +0x24), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
// (temporary vtable 0x0123E7E0), CTCModes::RemoveMode @ 0x0073ABA0, CWorld::GetFrame @ 0x0049D870.
// Shape: the landed message-filter chain (`IsOfType(0x2C) && IsOriginatedByThing(creature)` bound by pointers, the
// bound filter passed by const reference); both components used unconditionally (miss arms read the key slot);
// the gossip target cleared through a CIntelligentPointer temporary of null.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
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

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
namespace N_006af2b0 { struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0
}; } using N_006af2b0::CMessageEventFilter_IsOriginatedByThing;
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* FindMostRecentMessage_0090dee0(const CFilter_Bind2<CMessageEventFilter_IsOfType,
        CMessageEventFilter_IsOriginatedByThing>& filter);              // 0x0090DEE0
    void RemoveMessage(const CMessageEvent* message);                   // 0x00493B5B
};
long TmplSink_ca60(const void* self, long seen);   // stand-in: the work FindMostRecentMessage does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindMostRecentMessage<Bind2> instantiation (template bodies are compiled in the caller's TU):
// reads the const-reference argument, lets no argument escape (as the advisor's 0x0090EE20 model).
__declspec(noinline) CMessageEvent* CMessageEventManager::FindMostRecentMessage_0090dee0(const CFilter_Bind2<CMessageEventFilter_IsOfType,
    CMessageEventFilter_IsOriginatedByThing>& filter)
{
    long r = TmplSink_ca60(this, 0 + *(const long*)&filter);
    return (CMessageEvent*)r;
}
#pragma auto_inline(on)
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
public:
    static long __fastcall GetFrame();   // 0x0049D870
};
struct CIntelligentPointerGossip : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase> (vtable 0x0123E7E0)
    virtual void v00();
    CIntelligentPointerGossip(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerGossip& operator=(const CIntelligentPointerGossip& other) { SetPItem(other.GetPItem()); return *this; }
};
class CTCGossip { public:                              // interface 0xC1
    unsigned char _pad[0x0c];
    CIntelligentPointerGossip PGossipTarget;    // +0x0c
};
struct CTCModes {                               // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CAIStateGroup_MinionGossip : CAIStateGroupBase {
    long FrameOfLastGossip;                     // +0x14
    bool Critical;                              // +0x18
    bool NowRelyingOnGossipee;                  // +0x19
    CTCGossip* PTCGossip;                       // +0x1c
    CTCGossip* PTCGossipOther;                  // +0x20
    CWorld* GetPWorld();                        // 0x008FCFC0
    virtual void OnReplace();
};

void CAIStateGroup_MinionGossip::OnReplace()
{
    CThing* creature = PBrain->PCreature;
    CMessageEvent* message = GetPWorld()->GetPMessageManager()->FindMostRecentMessage_0090dee0(
        CMessageEventFilter_IsOfType(0x2c) && CMessageEventFilter_IsOriginatedByThing(creature));
    if (message)
        GetPWorld()->GetPMessageManager()->RemoveMessage(message);
    CTCGossip* gossip;
    PBrain->PCreature->GetInterfaceV(&gossip, 0xc1);
    gossip->PGossipTarget = CIntelligentPointerGossip(0);
    CTCModes* modes;
    PBrain->PCreature->GetInterfaceV(&modes, 0x31);
    modes->RemoveMode(0x1b);
    FrameOfLastGossip = CWorld::GetFrame();
}