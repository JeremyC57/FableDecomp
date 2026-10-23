// Behaviour test for 0090ee20: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CAIStateGroup_MinionKidCalledToMother::FindCallHomeEvent()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define snprintf _snprintf
#else
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_MinionKidCalledToMother::FindCallHomeEvent() -- the most recent "call home" message (type 1) that
// the kid's living mother sent to this kid.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090EE20, length 134 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on the creature's PMother at +0x180),
// CMessageEventFilter_IsOriginatedByThing::ctor(const CThing*) @ 0x006AF2B0 (out of line, copies the 64-bit
// UID), the second world accessor @ 0x008FCFA0 (message manager at +0x60), CMessageEventManager::
// FindMostRecentMessage<CFilter_Bind3<IsOfType, IsOriginatedByThing, IsForThing>> @ 0x0090EDC0.
// Shape: the mother is a local tested for null and for the dead bit (+0x91 bit 0; retail keeps the constant 1
// in ecx and reuses it as the type-1 store); then the filter chain `IsOfType(1) && IsOriginatedByThing(mother)
// && IsForThing(creature)` by const reference: 0x1c-byte frame with the type at +4, the creature at +8, the
// 8-byte UID filter at +0xc and the Bind3 pointers at +0x14..+0x1c; `xor eax,eax` null result. Layout local
// (the engine header flattens the base; PMother is CThingAICreature's intelligent pointer at +0x180).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

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
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_HasExtraData {
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

namespace N_006af2b0 { struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (out of line, `ret 4`)
}; } using N_006af2b0::CMessageEventFilter_IsOriginatedByThing;

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const A& a, const B& b, const C& c) : m_pFirst(&a), m_pSecond(&b), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(*ab.m_pFirst, *ab.m_pSecond, c); }

// CMessageEvent (PDB): Type +8, OriginatorUID +0x20, TimeStamp +0x34, Lifespan +0x38, MessageID +0x40, POriginator +0x48
struct CMessageEvent {
    unsigned char _pad0[0x20];
    unsigned __int64 OriginatorUID;             // +0x20
    unsigned char _pad1[0x14];
    void* PSource;                              // +0x3c
    long MessageID;                             // +0x40
    unsigned char _pad2[4];
    CBaseIntelligentPointer POriginator;        // +0x48 CIntelligentPointer<CThing>
};

struct CMessageEventManager;
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsForThing> CCallHomeFilter;

struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CCallHomeFilter& filter);   // 0x0090EDC0
};
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindMostRecentMessage does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindMostRecentMessage instantiation (template bodies are compiled in the caller's TU): reads the
// const-reference argument, lets no argument escape.
__declspec(noinline) CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CCallHomeFilter& filter)
{
    long r = TmplSink_s7_1(this, 0 + *(const long*)&filter);
    return (CMessageEvent*)r;
}
#pragma auto_inline(on)

struct CThingAICreatureM {                      // the kid: CThingAICreature's mother pointer
    unsigned char _pad[0x180];
    CBaseIntelligentPointer PMother;            // +0x180 CIntelligentPointer<CThingAICreature>
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

// the kid's mother thing: a named creature parameter keeps the creature in eax and forms the pointer address
// with `lea` (an inline member chain adds 0x180 to the `this` register instead)
__forceinline CThing* MotherOf(CThingAICreatureM* kid)
{
    return kid->PMother.GetPItem();
}

struct CAIStateGroup_MinionKidCalledToMother : CAIStateGroupBaseW {
    CMessageEvent* FindCallHomeEvent();
};

CMessageEvent* CAIStateGroup_MinionKidCalledToMother::FindCallHomeEvent()
{
    CThing* mother = MotherOf((CThingAICreatureM*)PBrain->PCreature);
    if (mother != 0 && !(mother->m_flags91 & 1)) {
        return GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(
            CMessageEventFilter_IsOfType(1)
            && CMessageEventFilter_IsOriginatedByThing(MotherOf((CThingAICreatureM*)PBrain->PCreature))
            && CMessageEventFilter_IsForThing(PBrain->PCreature));
    }
    return 0;
}

// ---- stubs ----
static char g_log[2048];
static void logf(const char* fmt, long a = 0, long b = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b); strcat(g_log, t); strcat(g_log, ";"); }
CAIStateGroupBase::~CAIStateGroupBase() { }
#define V(n) void CAIStateGroupBase::n() { }
V(v01) V(v02) V(v03) V(v04) V(v05) V(v06) V(v07) V(v08) V(Cleanup) V(OnReplace) V(Update) V(v13) V(v15) V(v16) V(v17)
#undef V
bool CAIStateGroupBase::IsRunnable() { return false; }
long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
CWorld* CAIStateGroupBase::GetPWorld() { return 0; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return 0; }
long __fastcall GFGetCurrentFrame() { return 0; }
unsigned long __fastcall GFROR13(unsigned long value) { return value; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return first; }
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { logf("GetPItem(%lx)", (long)(this - (const CBaseIntelligentPointer*)0)); return m_pItem; }
static unsigned __int64 g_motherUID = 0x1122334455667788ULL;
CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing* thing)
{
    logf("IsOriginatedByThing(%ld)", (long)(thing != 0));
    UID = thing ? g_motherUID : 0;
}
static CWorld g_world; static CMessageEventManager g_manager; static CMessageEvent g_message;
static int g_worldCalls;
CWorld* CAIStateGroupBaseW::GetPThingWorld() { ++g_worldCalls; return &g_world; }   // not logged: the compiler may fetch the world before or after the filter arguments
static const CMessageEventFilter_IsOfType* g_seenFirst; static const void* g_seenSelf; static long g_sinkResult;
long TmplSink_s7_1(const void* self, long seen)
{
    g_seenSelf = self; g_seenFirst = (const CMessageEventFilter_IsOfType*)seen;
    logf("FindMostRecentMessage(type=%ld)", g_seenFirst->m_type);
    return g_sinkResult;
}

int main()
{
    // the kid creature: a CThing whose CThingAICreature part holds PMother at +0x180
    static unsigned char kidBytes[0x200]; memset(kidBytes, 0, sizeof kidBytes);
    CThing* kid = (CThing*)kidBytes;
    CBaseIntelligentPointer* pMother = &((CThingAICreatureM*)kidBytes)->PMother;
    static unsigned char motherBytes[0x100]; memset(motherBytes, 0, sizeof motherBytes);
    CThing* mother = (CThing*)motherBytes;
    static CAIBrain brain; memset(&brain, 0, sizeof brain); brain.PCreature = kid;
    memset(&g_world, 0, sizeof g_world); g_world.m_pMessageManager = &g_manager;
    CAIStateGroup_MinionKidCalledToMother* group = (CAIStateGroup_MinionKidCalledToMother*)calloc(1, sizeof(CAIStateGroup_MinionKidCalledToMother));
    new (group) CAIStateGroup_MinionKidCalledToMother();
    group->PBrain = &brain;
    char expect[512];

    // 1. no mother: null, only the one GetPItem
    pMother->m_pItem = 0; g_log[0] = 0;
    CHECK(group->FindCallHomeEvent() == 0);
    snprintf(expect, sizeof expect, "GetPItem(%lx);", (long)(pMother - (CBaseIntelligentPointer*)0));
    CHECK(strcmp(g_log, expect) == 0);

    // 2. dead mother (+0x91 bit 0): null, no filter built
    pMother->m_pItem = mother; mother->m_flags91 = 1; g_log[0] = 0;
    CHECK(group->FindCallHomeEvent() == 0);
    CHECK(strcmp(g_log, expect) == 0);

    // 3. living mother: the type-1 / originated-by-mother / for-kid filter reaches the world's message manager
    mother->m_flags91 = 0; g_sinkResult = (long)&g_message; g_log[0] = 0;
    CHECK(group->FindCallHomeEvent() == &g_message);
    snprintf(expect, sizeof expect, "GetPItem(%lx);GetPItem(%lx);IsOriginatedByThing(1);FindMostRecentMessage(type=1);",
             (long)(pMother - (CBaseIntelligentPointer*)0), (long)(pMother - (CBaseIntelligentPointer*)0));
    CHECK(strcmp(g_log, expect) == 0);
    CHECK(g_seenSelf == &g_manager);
    CHECK(g_worldCalls == 1);

    // 4. the manager's answer is returned as-is (null when nothing matches)
    g_sinkResult = 0; g_log[0] = 0;
    CHECK(group->FindCallHomeEvent() == 0);
    CHECK(strcmp(g_log, expect) == 0);

    // 5. other dead-bits of the mother flag byte do not matter
    mother->m_flags91 = 0xfe; g_sinkResult = (long)&g_message; g_log[0] = 0;
    CHECK(group->FindCallHomeEvent() == &g_message);

    if (fails == 0) printf("0090ee20_TEST PASS\n"); else printf("0090ee20_TEST FAIL\n");
    return fails != 0;
}