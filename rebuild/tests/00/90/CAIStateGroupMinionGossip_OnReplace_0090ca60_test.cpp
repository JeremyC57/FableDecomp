// behaviour test for 0x0090CA60 CAIStateGroup_MinionGossip::OnReplace (body verbatim, callees stubbed)
#include <stdio.h>
// CAIStateGroup_MinionGossip::OnReplace() -- drop the pending gossip message from this creature, clear the CTCGossip target, remove the gossip mode (0x1B) and remember the frame of the last gossip
// Grade: RELOCATION_MATCH -- two levers together: the B-17 same-TU noinline FindMostRecentMessage stand-in (reads the const-reference filter, nothing escapes) puts the bound filter's first-pointer store before the UID-filter ctor call, and the creature read into a named local ahead of the call chain (`CThing* creature = PBrain->PCreature;`) gives retail's this=esi / message=edi and the late type-word store
// Leaf: OnReplace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090CA60, length 287 bytes (+ int3 padding).
// Callees: CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF2B0, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (x2),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, IsOriginatedByThing>> @ 0x0090DEE0,
// CMessageEventManager::RemoveMessage @ 0x00493B5B, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0xC1 =
// CTCGossip bit 1 of +0x38, 0x31 = CTCModes bit 17 of +0x24), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
// (temporary vtable 0x0123E7E0), CTCModes::RemoveMode @ 0x0073ABA0, GFGetCurrentFrame @ 0x0049D870.
// Shape: the landed message-filter chain (`IsOfType(0x2C) && IsOriginatedByThing(creature)` bound by pointers, the
// bound filter passed by const reference); both components used unconditionally (miss arms read the key slot);
// the gossip target cleared through a CIntelligentPointer temporary of null.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

struct CThing {
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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CAIStateGroupBase {
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
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
namespace N_006af2b0 { struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0
}; } using N_006af2b0::CMessageEventFilter_IsOriginatedByThing;
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
struct CMessageEvent;
struct CMessageEventManager {
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
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};
struct CIntelligentPointerGossip : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase> (vtable 0x0123E7E0)
    virtual void v00();
    CIntelligentPointerGossip(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerGossip& operator=(const CIntelligentPointerGossip& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CTCGossip {                              // interface 0xC1
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
    FrameOfLastGossip = GFGetCurrentFrame();
}

// ---- stubs ----
static CInterfaceMap::SEntry* g_lbQueue[4]; static int g_lbN, g_lbI;
static long g_lbKeys[4];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lbKeys[g_lbI] = *type; return g_lbQueue[g_lbI++]; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = (CThing*)0x1234; }
static int g_ipDtors;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { g_ipDtors++; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
void CBaseIntelligentPointer::v00() { }
void CIntelligentPointerGossip::v00() { }
void CIntelligentPointerThing::v00() { }
static unsigned long g_removedMode; static int g_removeModeCalls;
void CTCModes::RemoveMode(unsigned long mode) { g_removedMode = mode; g_removeModeCalls++; }
long __fastcall GFGetCurrentFrame() { return 4321; }
unsigned long __fastcall GFROR13(unsigned long v) { return v; }
CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing* thing) { UID = (unsigned __int64)(unsigned long)thing; }
static CMessageEvent* g_found; static long g_seenType; static unsigned __int64 g_seenUID;
long TmplSink_ca60(const void* self, long seen)
{
    const CMessageEventFilter_IsOfType* first = (const CMessageEventFilter_IsOfType*)seen;
    g_seenType = first->m_type;
    return (long)g_found;
}
static const CMessageEvent* g_removed; static int g_removeCalls;
void CMessageEventManager::RemoveMessage(const CMessageEvent* message) { g_removed = message; g_removeCalls++; }
static CMessageEventManager g_mgr;
static CWorld g_world;
static int g_worldCalls;
CWorld* CAIStateGroup_MinionGossip::GetPWorld() { g_worldCalls++; return &g_world; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return 0; }
CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::v01() { } void CAIStateGroupBase::v02() { } void CAIStateGroupBase::v03() { } void CAIStateGroupBase::v04() { }
void CAIStateGroupBase::v05() { } void CAIStateGroupBase::v06() { } void CAIStateGroupBase::v07() { } void CAIStateGroupBase::v08() { }
void CAIStateGroupBase::Cleanup() { } void CAIStateGroupBase::OnReplace() { } bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() { } void CAIStateGroupBase::v13() { } long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
void CAIStateGroupBase::v15() { } void CAIStateGroupBase::v16() { } void CAIStateGroupBase::v17() { }

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

int main()
{
    static CThing creature; static CAIBrain brain; static CTCModes modes;
    static double gbuf[2][4]; CTCGossip& gossip = *(CTCGossip*)gbuf[0]; CTCGossip& gossipSentinel = *(CTCGossip*)gbuf[1];
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x31; entries[0].component = (CTCBase*)&modes;
    entries[1].type = 0xc1; entries[1].component = (CTCBase*)&gossip;
    entries[2].type = 0;    entries[2].component = (CTCBase*)&gossipSentinel;   // the end slot the miss arm reads
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    creature.m_presentBits[0xc1 >> 5] |= 1UL << (0xc1 & 31);
    creature.m_presentBits[0x31 >> 5] |= 1UL << (0x31 & 31);
    brain.PCreature = &creature;
    g_world.m_pMessageManager = &g_mgr;
    CAIStateGroup_MinionGossip* group = new CAIStateGroup_MinionGossip;
    group->PBrain = &brain; group->FrameOfLastGossip = 0;
    gossip.PGossipTarget.m_pItem = &creature; gossipSentinel.PGossipTarget.m_pItem = &creature;
    // 1: a pending message -> removed; lookups hit
    static char msg[0x50]; g_found = (CMessageEvent*)msg;
    g_lbQueue[0] = &entries[1]; g_lbQueue[1] = &entries[0]; g_lbI = 0;
    group->OnReplace();
    check(g_seenType == 0x2c && g_removeCalls == 1 && g_removed == (CMessageEvent*)msg && g_worldCalls == 2, "message removed");
    check(g_lbKeys[0] == 0xc1 && g_lbKeys[1] == 0x31 && gossip.PGossipTarget.m_pItem == 0, "gossip target cleared");
    check(g_removeModeCalls == 1 && g_removedMode == 0x1b && group->FrameOfLastGossip == 4321, "mode removed, frame");
    // 2: no message; the gossip lookup lands on end (miss pick) and the modes lookup on a greater key (miss pick)
    g_found = 0; g_worldCalls = 0; g_removeCalls = 0; gossip.PGossipTarget.m_pItem = &creature; gossipSentinel.PGossipTarget.m_pItem = &creature;
    entries[2].component = (CTCBase*)&gossipSentinel;
    g_lbQueue[0] = &entries[2]; g_lbQueue[1] = &entries[1]; g_lbI = 0; group->FrameOfLastGossip = 0;
    group->OnReplace();
    check(g_removeCalls == 0 && g_worldCalls == 1, "no message");
    check(gossipSentinel.PGossipTarget.m_pItem == 0 && gossip.PGossipTarget.m_pItem == &creature, "miss pick end slot");
    check(g_removeModeCalls == 2 && group->FrameOfLastGossip == 4321, "second pass");
    // 3: key below the found entry's type -> end slot; key equal -> the entry
    entries[2].component = (CTCBase*)&modes;
    g_lbQueue[0] = &entries[1]; g_lbQueue[1] = &entries[1]; g_lbI = 0; gossip.PGossipTarget.m_pItem = &creature;
    group->OnReplace();
    check(gossip.PGossipTarget.m_pItem == 0 && g_removeModeCalls == 3, "greater-key pick");
    if (!fails) printf("0090ca60_TEST PASS\n");
    return fails;
}