// Behaviour test for 009226b0: src/009226b0.cpp copied verbatim; IsStillValid is true only when no type-0x14 message from the
// creature is pending and the creature's drunkenness component says drunk.
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_Drunk::IsStillValid() const -- still valid while no type-0x14 message is pending from the creature and its drunkenness component (interface 0xC3) still says drunk
// Retry 10b: the landed sibling CAIStateGroup_Drunk::IsRunnable (0x00922600) model copied verbatim -- the filter is
// `IsOfType(0x14) && IsOriginatedByThing(creature)` over a named creature local, and the TU carries the same-TU noinline
// FindMostRecentMessage stand-in reading the bound filter through its const reference; with both, VC7.1 stores the Bind2
// first pointer before the originator constructor call as retail does (the parked model stored it after the call).
// Leaf: IsStillValid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009226B0, length 156 bytes (+ int3 padding).
// Callees: CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF290 (const CThing*: copies the 64-bit UID at +0x18/+0x1c),
// the second world accessor @ 0x008FCFA0 (message manager at +0x60), CMessageEventManager::FindMostRecentMessage
// <CFilter_Bind2<IsOfType, IsOriginatedByThing>> @ 0x008D46D0, CVectorMap::LowerBound @ 0x004365B0 (interface 0xC3).
// Shape: the message pointer is kept in edi across the drunkenness lookup; the lookup's bool is NOT tested (the miss
// path reads the dead key slot `[esp+8]`); `message == 0 && drunk->m_isDrunk` returns a full-register 0/1.
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
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

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
};


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

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

struct CMessageEvent;
struct CMessageEventManager;
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld() const;             // 0x008FCFA0
};

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing> CDrunkFilter;

struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CDrunkFilter& filter);   // 0x008D46D0
};
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindMostRecentMessage does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindMostRecentMessage instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) CMessageEvent* CMessageEventManager::FindMostRecentMessage(const CDrunkFilter& filter)
{
    long r = TmplSink_s7_1(this, 0 + *(const long*)&filter);
    return (CMessageEvent*)r;
}
#pragma auto_inline(on)

struct CTCDrunk {                               // interface 0xc3
    unsigned char _pad[0xc];
    unsigned char m_isDrunk;                    // +0x0c
};

struct CBrainOverride;
struct CAIBrainX : CAIBrain {
    CBrainOverride* GetPOverride();             // 0x0088B7D0
};

struct CAIStateGroup_Drunk : CAIStateGroupBaseW {
    virtual bool IsRunnable();                  // 0x00922600 (landed)
    virtual bool IsStillValid() const;
};

bool CAIStateGroup_Drunk::IsStillValid() const
{
    CThing* creature = PBrain->PCreature;
    CMessageEvent* message = GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(
            CMessageEventFilter_IsOfType(0x14) && CMessageEventFilter_IsOriginatedByThing(creature));
    CTCDrunk* drunk;
    PBrain->PCreature->GetInterface(&drunk, 0xc3);
    return message == 0 && drunk->m_isDrunk;
}
// ---- end source ----
void CBaseIntelligentPointer::v00() {}
void CAIStateGroupBase::v01() {} void CAIStateGroupBase::v02() {} void CAIStateGroupBase::v03() {} void CAIStateGroupBase::v04() {} void CAIStateGroupBase::v05() {} void CAIStateGroupBase::v06() {} void CAIStateGroupBase::v07() {} void CAIStateGroupBase::v08() {} void CAIStateGroupBase::v13() {} void CAIStateGroupBase::v15() {} void CAIStateGroupBase::v16() {} void CAIStateGroupBase::v17() {}

static int g_find = 0, g_orig = 0; static long g_msg = 0; static long g_type = -1; static const CThing* g_originator = 0;
static CWorld g_world; static CMessageEventManager g_mm;
long TmplSink_s7_1(const void* self, long seen)
{   // FindMostRecentMessage: the filter's first word is the address of the IsOfType filter
    ++g_find; CHECK(self == &g_mm); g_type = ((const CMessageEventFilter_IsOfType*)seen)->m_type; return g_msg;
}
CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing* thing) { ++g_orig; g_originator = thing; UID = 0; }
CWorld* CAIStateGroupBaseW::GetPThingWorld() const { return &g_world; }
CBrainOverride* CAIBrainX::GetPOverride() { return 0; }
bool CAIStateGroup_Drunk::IsRunnable() { return false; }
CAIStateGroupBase::~CAIStateGroupBase() {}
void CAIStateGroupBase::Cleanup() {} void CAIStateGroupBase::OnReplace() {} bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() {} long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
int main()
{
    g_world.m_pMessageManager = &g_mm;
    static CThing creature; static CTCDrunk drunk; drunk.m_isDrunk = 1;
    CInterfaceMap::SEntry entries[2] = { { 0x10, 0 }, { 0xc3, (CTCBase*)&drunk } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2; creature.m_presentBits[0xc3 >> 5] = 1UL << (0xc3 & 31);
    static unsigned char brainRaw[0x40]; CAIBrain& brain = *(CAIBrain*)brainRaw; brain.PCreature = &creature;
    static unsigned char raw[sizeof(CAIStateGroup_Drunk)]; CAIStateGroup_Drunk& g = *(CAIStateGroup_Drunk*)raw; g.PBrain = &brain;
    CHECK(g.CAIStateGroup_Drunk::IsStillValid() == true); CHECK(g_find == 1 && g_type == 0x14 && g_orig == 1 && g_originator == &creature);
    g_msg = 0x100; CHECK(g.CAIStateGroup_Drunk::IsStillValid() == false);   // a pending drunk message
    g_msg = 0; drunk.m_isDrunk = 0; CHECK(g.CAIStateGroup_Drunk::IsStillValid() == false);   // sobered up
    if (fails == 0) printf("009226b0_TEST PASS\n"); else printf("009226b0_TEST FAIL\n");
    return fails;
}