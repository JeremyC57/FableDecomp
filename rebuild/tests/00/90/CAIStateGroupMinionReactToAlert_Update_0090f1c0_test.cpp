// behaviour test for 0x0090F1C0 CAIStateGroup_MinionReactToAlert::Update (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
// CAIStateGroup_MinionReactToAlert::Update() -- follow the current alert: terminate when there is none, else remember its id and, when the alerted-about thing is what the creature's perceive component is tracking, mark the creature as indirectly aware of it.
// Grade: RELOCATION_MATCH -- the typed read of the event's subject (`xor dl,dl ; mov [esp+0Ch],eax ; jne` between the second and third GetPItem) is the landed 0x008D04E0 typed-item test: `if (p.GetPItem() != 0 && IsThingOfType<CThing>(p.GetPItem())) subject = p.GetPItem();` -- the rvalue bound to `const CThing* const&` is homed in a dead slot and CThing's zero ThingTypeMask makes the test a materialised constant
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F1C0, length 149 bytes (+ int3 padding). Vtable slot 12.
// Callees: FindAlertEvent @ 0x0090F110, CAIStateGroupBase::Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo"
// is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on the event's pointer at +0x50:
// the typed read, see IsThingOfType), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x34 =
// CTCPerceiveThing, bit 20 of the bitset dword at +0x24), CTCPerceiveThing::GetPerceivedThing @ 0x007446A0
// and SetAsIndirectlyAwareOfPerceivedThing @ 0x007449E0.
// Shape: `if (!event) { Terminate(); return; }` as the out-of-line tail; the target local starts null (`xor
// ebx,ebx`) and takes the typed read; the perceive component is an uninitialised local filled by the inlined
// lookup; the compare is `GetPerceivedThing() == target`. m_lastAlertId is +0x14 (descriptive, no engine header).
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
    unsigned char _pad1[0x20];
    unsigned long ThingTypeFlags;               // +0x6c
    unsigned char _pad2[0x21];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    enum { ThingTypeMask = 0 };                 // the base class matches every thing (landed 0x008D04E0 typed-item test)
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

// The landed 0x008D04E0 / 0x008941A0 typed-item test: the rvalue GetPItem() binds to the const reference through a
// frame temporary (the dead store) and the zero mask folds the test to a constant (`xor dl,dl ; jne`).
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}

struct CMessageEvent {
    unsigned char _pad0[0x40];
    long MessageID;                             // +0x40
    unsigned char _pad1[4];
    CBaseIntelligentPointer POriginator;        // +0x48
    CBaseIntelligentPointer PSubject;           // +0x50 the thing the alert is about
};

struct CTCPerceiveThing {                       // interface type 0x34
    CThing* GetPerceivedThing();                // 0x007446A0
    void SetAsIndirectlyAwareOfPerceivedThing();    // 0x007449E0
};

struct CAIStateGroupBaseT : CAIStateGroupBase {
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_MinionReactToAlert : CAIStateGroupBaseT {
    long m_lastAlertId;                         // +0x14
    CMessageEvent* FindAlertEvent();            // 0x0090F110
    virtual void Update();
};

void CAIStateGroup_MinionReactToAlert::Update()
{
    CMessageEvent* event = FindAlertEvent();
    if (event != 0) {
        m_lastAlertId = event->MessageID;
        CThing* subject = 0;
        if (event->PSubject.GetPItem() != 0 && IsThingOfType<CThing>(event->PSubject.GetPItem()))
            subject = event->PSubject.GetPItem();
        CTCPerceiveThing* pPerceive;
        if (PBrain->PCreature->GetInterface(&pPerceive, 0x34)) {
            if (pPerceive->GetPerceivedThing() == subject)
                pPerceive->SetAsIndirectlyAwareOfPerceivedThing();
        }
    } else {
        Terminate();
    }
}


// ---- stubs ----
static long g_lbKey; static int g_lbOffset; static int g_lbCalls;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lbKey = *type; g_lbCalls++; return first + g_lbOffset; }
static int g_getCalls;
CThing* CBaseIntelligentPointer::GetPItem() const { g_getCalls++; return m_pItem; }
void CBaseIntelligentPointer::v00() { }
static CMessageEvent* g_event;
CMessageEvent* CAIStateGroup_MinionReactToAlert::FindAlertEvent() { return g_event; }
static int g_terminated;
void CAIStateGroupBaseT::Terminate() { g_terminated++; }
static CThing* g_perceived; static int g_aware; static CTCPerceiveThing* g_awareThis;
CThing* CTCPerceiveThing::GetPerceivedThing() { return g_perceived; }
void CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing() { g_aware++; g_awareThis = this; }
CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::v01() { } void CAIStateGroupBase::v02() { } void CAIStateGroupBase::v03() { } void CAIStateGroupBase::v04() { }
void CAIStateGroupBase::v05() { } void CAIStateGroupBase::v06() { } void CAIStateGroupBase::v07() { } void CAIStateGroupBase::v08() { }
void CAIStateGroupBase::Cleanup() { } void CAIStateGroupBase::OnReplace() { } bool CAIStateGroupBase::IsRunnable() { return false; }
void CAIStateGroupBase::Update() { } void CAIStateGroupBase::v13() { } long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
void CAIStateGroupBase::v15() { } void CAIStateGroupBase::v16() { } void CAIStateGroupBase::v17() { }

static int fails;
static void check(int c, const char* w) { if (!c) { fails++; printf("FAIL %s\n", w); } }

static double g_creatureBuf[0x20], g_subjectBuf[0x20], g_otherBuf[0x20], g_eventBuf[0x10];
static double g_pcBuf[1], g_endPcBuf[1];

int main()
{
    CThing* creature = (CThing*)g_creatureBuf;
    CThing* subject = (CThing*)g_subjectBuf;
    CThing* other = (CThing*)g_otherBuf;
    CTCPerceiveThing* pc = (CTCPerceiveThing*)g_pcBuf;
    CTCPerceiveThing* endPc = (CTCPerceiveThing*)g_endPcBuf;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x34; entries[0].component = (CTCBase*)pc;
    entries[1].type = 0; entries[1].component = (CTCBase*)endPc;
    creature->m_interfaces.first = entries; creature->m_interfaces.last = entries + 1;
    creature->m_presentBits[0x34 >> 5] = 1UL << (0x34 & 31);
    subject->ThingTypeFlags = 0x12345678;
    static CAIBrain brain; brain.PCreature = creature;
    CAIStateGroup_MinionReactToAlert* g = new CAIStateGroup_MinionReactToAlert;
    g->PBrain = &brain; g->m_lastAlertId = -1;
    CMessageEvent* ev = (CMessageEvent*)g_eventBuf;

    // 1: no alert -> terminate
    g_event = 0;
    g->Update();
    check(g_terminated == 1 && g->m_lastAlertId == -1 && g_lbCalls == 0, "no alert");
    // 2: alert about the perceived thing -> aware
    g_event = ev; ev->MessageID = 77; ev->PSubject.m_pItem = subject; g_perceived = subject;
    g->Update();
    check(g_terminated == 1 && g->m_lastAlertId == 77 && g_lbKey == 0x34 && g_aware == 1 && g_awareThis == pc && g_getCalls == 3, "aware");
    // 3: perceive tracks another thing -> nothing
    g_perceived = other; ev->MessageID = 78;
    g->Update();
    check(g_aware == 1 && g->m_lastAlertId == 78, "other perceived");
    // 4: no subject -> compared with null: aware when perceive tracks nothing
    ev->PSubject.m_pItem = 0; g_perceived = 0; g_getCalls = 0;
    g->Update();
    check(g_aware == 2 && g_getCalls == 1, "null subject");
    g_perceived = subject;
    g->Update();
    check(g_aware == 2, "null subject vs tracked");
    // 5: lookup end / greater / hit
    ev->PSubject.m_pItem = subject;
    g_lbOffset = 1; g->Update();
    check(g_aware == 3 && g_awareThis == endPc, "end pick");
    g_lbOffset = 0; entries[0].type = 0x35; g->Update();
    check(g_aware == 4 && g_awareThis == endPc, "greater-key pick");
    entries[0].type = 0x34; g->Update();
    check(g_aware == 5 && g_awareThis == pc, "hit pick");
    // 6: no perceive component -> nothing
    creature->m_presentBits[0x34 >> 5] = ~(1UL << (0x34 & 31)); int calls = g_lbCalls;
    g->Update();
    check(g_aware == 5 && g_lbCalls == calls && g_terminated == 1, "no component");
    if (!fails) printf("0090f1c0_TEST PASS\n");
    return fails;
}