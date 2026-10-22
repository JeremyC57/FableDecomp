// Behaviour test for 00744d00: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CTCPerceiveAlert::FindAlertEvent()
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
#define __cdecl
#define __int64 long long
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPerceiveAlert::FindAlertEvent() -- find the most recent alert message (type 0xC) the owner can see or hear;
// when its sender is alive, an ally of the owner, and the script thing the event names is our tracked thing (+0x0c),
// become directly aware of the perceived thing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00744D00, length 162 bytes (+ int3 padding).
// Label note: catalog "CAIStateGroup_MinionReactToAlert::FindAlertEvent"; the body sits in the CTC region and uses
// +0x04 as the owner thing and +0x0c as an intelligent pointer, and calls SetAsDirectlyAwareOfPerceivedThing
// @ 0x007449B0 on `this` -- treat the class attribution as unverified.
// Callees: the world getter @ 0x00686A80 (`[0x013B8A1C]+24h`, +0x60 = CMessageEventManager),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x00746260,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, AreThingsAllies @ 0x0076D4F0 (__fastcall),
// CGameScriptThing::GetPThing @ 0x006AF520, SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0.
// Shape: the filter chain `IsOfType(0xc) && CanBeSeenOrHeard(owner)` as temporaries (two one-word filters and the Bind2
//   {&first, &second} pair in the 0x10 frame) with the same-TU FindMostRecentMessage stand-in, the manager through an inline
//   accessor (the filter address is pushed after the world call); the sender goes through the landed 0x008D04E0 typed-item
//   test (`GetPItem() != 0 && IsThingOfType<CThing>(GetPItem())`: the rvalue bound to the const reference is the dead frame
//   store, the zero mask the constant `xor al,al ; jne`), then a named alive sender for AreThingsAllies and a named
//   GetPThing() result compared with PTracked (GetPThing is called before PTracked's GetPItem).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    enum { ThingTypeMask = 0 };                 // the base class matches every thing (landed 0x008D04E0 typed-item test)
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x6c - 0x4c];
    unsigned long ThingTypeFlags;               // +0x6c
    unsigned char _pad2[0x91 - 0x70];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
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
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CWorld;
struct CMessageEventFilter_IsOfType { long Type; CMessageEventFilter_IsOfType(long type) : Type(type) { } };
struct CMessageEventFilter_CanBeSeenOrHeard { CThing* PPerceiver; CMessageEventFilter_CanBeSeenOrHeard(CThing* perceiver) : PPerceiver(perceiver) { } };
template<class A, class B> struct CFilter_Bind2T {
    const A* PFirst;
    const B* PSecond;
    CFilter_Bind2T(const A& a, const B& b) : PFirst(&a), PSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2T<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2T<A, B>(a, b); }
typedef CFilter_Bind2T<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CFilter_Bind2;

struct CGameScriptThing {
    CThing* GetPThing();                        // 0x006AF520
};
struct CMessageEvent : CGameScriptThing {
    unsigned char _pad[0x48];
    CBaseIntelligentPointerCtor PSender;        // +0x48
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind2& filter);   // 0x00746260
};
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindMostRecentMessage does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindMostRecentMessage instantiation (template bodies are compiled in the caller's TU): reads the
// const-reference argument, lets no argument escape.
__declspec(noinline) CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CFilter_Bind2& filter)
{
    long r = TmplSink_s7_1(this, 0 + *(const long*)&filter);
    return (CMessageEvent*)r;
}
#pragma auto_inline(on)
struct CWorldEvents {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    CMessageEventManager* GetPMessageEventManager() { return PMessageEventManager; }
};

bool __fastcall AreThingsAllies(CThing* a, CThing* b);   // 0x0076D4F0
// the landed 0x008D04E0 / 0x008941A0 typed-item test: the rvalue GetPItem() binds to the const reference through a frame
// temporary (the dead store) and the zero mask folds the test to a constant (`xor al,al ; jne`)
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

struct CTCPerceiveAlert {
    void* __vftable;                            // +0x00
    CThing* POwner;                             // +0x04
    unsigned char _pad_0x08[0x4];
    CBaseIntelligentPointerCtor PTracked;       // +0x0c
    CWorldEvents* GetPWorld();                  // 0x00686A80
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x007449B0
    void FindAlertEvent();
};

void CTCPerceiveAlert::FindAlertEvent()
{
    CThing* owner = POwner;
    CMessageEvent* ev = GetPWorld()->GetPMessageEventManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0xc) && CMessageEventFilter_CanBeSeenOrHeard(owner));
    if (ev != 0) {
        CBaseIntelligentPointerCtor& sender = ev->PSender;
        if (sender.GetPItem() != 0 && IsThingOfType<CThing>(sender.GetPItem())) {
            CThing* thing = sender.GetPItem();
            if (thing != 0 && !(thing->m_flags91 & 1) && AreThingsAllies(thing, owner)) {
                CThing* perceived = ev->GetPThing();
                if (PTracked.GetPItem() == perceived)
                    SetAsDirectlyAwareOfPerceivedThing();
            }
        }
    }
}

// ---- stubs ----
static char g_log[4096];
static void logf(const char* fmt, long a = 0, long b = 0, long c = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b, c); strcat(g_log, t); strcat(g_log, ";"); }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return first; }
void CBaseIntelligentPointerCtor::v00() { }
CBaseIntelligentPointerCtor::CBaseIntelligentPointerCtor() { m_pItem = 0; }
CBaseIntelligentPointerCtor::~CBaseIntelligentPointerCtor() { }
void CBaseIntelligentPointerCtor::SetPItem(CThing* item) { m_pItem = item; }
CThing* CBaseIntelligentPointerCtor::GetPItem() const { logf("GetPItem(%lx)", (long)(this - (const CBaseIntelligentPointerCtor*)0)); return m_pItem; }
static CThing* g_perceived;
CThing* CGameScriptThing::GetPThing() { logf("GetPThing"); return g_perceived; }
static CWorldEvents g_world; static CMessageEventManager g_manager; static CMessageEvent* g_message;
static const void* g_seenSelf; static const CMessageEventFilter_IsOfType* g_seenFirst;
long TmplSink_s7_1(const void* self, long seen)
{
    g_seenSelf = self; g_seenFirst = (const CMessageEventFilter_IsOfType*)seen;
    logf("FindMostRecentMessage(type=%ld)", g_seenFirst->Type);
    return (long)g_message;
}
static CThing* g_owner; static bool g_allies;
bool __fastcall AreThingsAllies(CThing* a, CThing* b) { logf("AreThingsAllies(%ld,%ld)", (long)(a != 0), (long)(b == g_owner)); return g_allies; }
CWorldEvents* CTCPerceiveAlert::GetPWorld() { return &g_world; }
void CTCPerceiveAlert::SetAsDirectlyAwareOfPerceivedThing() { logf("SetAsDirectlyAware"); }

int main()
{
    static unsigned char ownerBytes[0x100]; memset(ownerBytes, 0, sizeof ownerBytes); g_owner = (CThing*)ownerBytes;
    static unsigned char senderBytes[0x100]; memset(senderBytes, 0, sizeof senderBytes); CThing* sender = (CThing*)senderBytes;
    static unsigned char trackedBytes[0x100]; memset(trackedBytes, 0, sizeof trackedBytes); CThing* tracked = (CThing*)trackedBytes;
    memset(&g_world, 0, sizeof g_world); g_world.PMessageEventManager = &g_manager;
    CMessageEvent* message = (CMessageEvent*)calloc(1, sizeof(CMessageEvent));
    CTCPerceiveAlert* tc = (CTCPerceiveAlert*)calloc(1, sizeof(CTCPerceiveAlert));
    tc->POwner = g_owner; tc->PTracked.m_pItem = tracked;
    char ptr[64]; snprintf(ptr, sizeof ptr, "GetPItem(%lx);", (long)(&message->PSender - (const CBaseIntelligentPointerCtor*)0));
    char trk[64]; snprintf(trk, sizeof trk, "GetPItem(%lx);", (long)(&tc->PTracked - (const CBaseIntelligentPointerCtor*)0));
    char expect[1024];

    // 1. no alert message: only the search (type 0xc filter, world's manager)
    g_message = 0; g_log[0] = 0;
    tc->FindAlertEvent();
    CHECK(strcmp(g_log, "FindMostRecentMessage(type=12);") == 0);
    CHECK(g_seenSelf == &g_manager);

    // 2. message without a sender: three... only the first sender read, nothing else
    g_message = message; message->PSender.m_pItem = 0; g_log[0] = 0;
    tc->FindAlertEvent();
    snprintf(expect, sizeof expect, "FindMostRecentMessage(type=12);%s", ptr);
    CHECK(strcmp(g_log, expect) == 0);

    // 3. dead sender: the typed-item test passes (three GetPItem reads), the alive test fails
    message->PSender.m_pItem = sender; sender->m_flags91 = 1; g_log[0] = 0;
    tc->FindAlertEvent();
    snprintf(expect, sizeof expect, "FindMostRecentMessage(type=12);%s%s%s", ptr, ptr, ptr);
    CHECK(strcmp(g_log, expect) == 0);

    // 4. live sender, not an ally: AreThingsAllies(sender, owner) stops it
    sender->m_flags91 = 0; g_allies = false; g_log[0] = 0;
    tc->FindAlertEvent();
    snprintf(expect, sizeof expect, "FindMostRecentMessage(type=12);%s%s%sAreThingsAllies(1,1);", ptr, ptr, ptr);
    CHECK(strcmp(g_log, expect) == 0);

    // 5. ally, but the event names another thing: GetPThing first, then the tracked pointer, no awareness
    g_allies = true; g_perceived = g_owner; g_log[0] = 0;
    tc->FindAlertEvent();
    snprintf(expect, sizeof expect, "FindMostRecentMessage(type=12);%s%s%sAreThingsAllies(1,1);GetPThing;%s", ptr, ptr, ptr, trk);
    CHECK(strcmp(g_log, expect) == 0);

    // 6. the event names our tracked thing: become directly aware
    g_perceived = tracked; g_log[0] = 0;
    tc->FindAlertEvent();
    snprintf(expect, sizeof expect, "FindMostRecentMessage(type=12);%s%s%sAreThingsAllies(1,1);GetPThing;%sSetAsDirectlyAware;", ptr, ptr, ptr, trk);
    CHECK(strcmp(g_log, expect) == 0);

    if (fails == 0) printf("00744d00_TEST PASS\n"); else printf("00744d00_TEST FAIL\n");
    return fails != 0;
}