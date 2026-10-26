// Behaviour test for 00922750: src/00922750.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_Drunk::TalkToSelfUpdate()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_Drunk::TalkToSelfUpdate() -- every so often a drunk mutters to himself: play the talk-to-self speech while idling, then schedule the next one 10..30 seconds later
// Retail: Fable.exe (TLC, VC7.1) @ 0x00922750, 264 bytes (`ret`).
// Callees: the brain world accessor 0x008FCFC0 + the static frame counter 0x0049D870, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0xf), CTCTalk::IsTalking 0x006E28A0, CCharString(const char*, long) 0x0099EBF0
// ("TEXT_AI_OPINION_ACT_DRUNK_TALK_TO_SELF" 0x01290014) / ~CCharString 0x0099EAE0, CTCTalk::PlayAISpeech(const
// CCharString&, bool, EDialogueLayer, bool, bool) 0x006E3B30, AIAction_DoNothing 0x0097E200, Terminate 0x008FCF40,
// GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, __ftol2 0x00BFEA70; 10.0f pool 0x01290004, fps global 0x01375550.
// Shape: the landed AI state-group model (0x00950A30); a `for (;;)` loop with a named string in its own block per speech
// attempt; `== 2` (still playing) idles with AIAction_DoNothing and `continue`s while it keeps going, else Terminate()
// and return; result 0 re-arms the timer with `frame + (rand(20) + 10) * fps` (fimul of the frame-rate global, VC copies
// it to the frame before the random call); a final `return` closes the loop body. That return stays its own epilogue that
// the re-arm store falls into -- the earlier `do { if (result != 2) { ...; return; } } while (...); Terminate();` form
// tail-merged the result != 0 exit into the Terminate epilogue and scheduled the store between the pops.
// Layout: Ego_r.pdb CAIStateGroup_Drunk FrameToNextTalkToSelf +0x14.
// Leaf: TalkToSelfUpdate@CAIStateGroup_Drunk
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
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
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
};


class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

class CTCTalk { public:                                    // the interface-0xf component
    bool IsTalking();                               // 0x006E28A0
    long PlayAISpeech(const CCharString& text, bool allowNonPure, long layer, bool a, bool b);   // 0x006E3B30 (ESpeechPlaybackResult)
};

struct CWorldFrame {
    static long GetWorldFrame();                    // 0x0049D870 (static: GetPWorld() is evaluated for nothing)
};
extern long g_framesPerSecond;                      // 0x01375550
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CAIStateGroup_Drunk : CAIStateGroupBase {
    long FrameToNextTalkToSelf;                     // +0x14
    unsigned long* GetPWorldSeed();                 // 0x008FCFD0
    long AIAction_DoNothing();                      // 0x0097E200 (EAIActionReturn)
    void Terminate();                               // 0x008FCF40
    void TalkToSelfUpdate();
};

void CAIStateGroup_Drunk::TalkToSelfUpdate()
{
    if (((CWorldFrame*)GetPWorld())->GetWorldFrame() <= FrameToNextTalkToSelf)
        return;
    CTCTalk* talk;
    PBrain->PCreature->GetInterface(&talk, 0xf);
    if (talk->IsTalking())
        return;
    for (;;) {
        long result;
        {
            CCharString text("TEXT_AI_OPINION_ACT_DRUNK_TALK_TO_SELF", -1);
            result = talk->PlayAISpeech(text, false, 2, false, true);
        }
        if (result == 2) {
            if (AIAction_DoNothing() == 4)
                continue;
            Terminate();
            return;
        }
        if (result == 0) {
            CWorldFrame* world = (CWorldFrame*)GetPWorld();
            long framesPerSecond = g_framesPerSecond;
            FrameToNextTalkToSelf = (long)(world->GetWorldFrame() +
                (GFFloatRandom(20.0f, GetPWorldSeed()) + 10.0f) * framesPerSecond);
        }
        return;
    }
}

// ---- stubs and driver ----
CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::v01() { } void CAIStateGroupBase::v02() { } void CAIStateGroupBase::v03() { } void CAIStateGroupBase::v04() { }
void CAIStateGroupBase::v05() { } void CAIStateGroupBase::v06() { } void CAIStateGroupBase::v07() { } void CAIStateGroupBase::v08() { }
void CAIStateGroupBase::Cleanup() { } void CAIStateGroupBase::OnReplace() { } bool CAIStateGroupBase::IsRunnable() { return true; }
void CAIStateGroupBase::Update() { } void CAIStateGroupBase::v13() { } long CAIStateGroupBase::GetTimeToInitiate() { return 0; }
void CAIStateGroupBase::v15() { } void CAIStateGroupBase::v16() { } void CAIStateGroupBase::v17() { }
static int g_worldCalls = 0;
CWorld* CAIStateGroupBase::GetPWorld() { ++g_worldCalls; return (CWorld*)0x1234; }
static long g_frame = 0; static int g_frameCalls = 0;
long CWorldFrame::GetWorldFrame() { ++g_frameCalls; return g_frame; }
static CInterfaceMap::SEntry g_entries[2];
static long g_lbType = -1;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { g_lbType = *type; return first; }
static CTCTalk g_talk; static bool g_talking = false; static int g_isTalkingCalls = 0;
bool CTCTalk::IsTalking() { CHECK(this == &g_talk); ++g_isTalkingCalls; return g_talking; }
static const char* g_lastText; static int g_strDtors = 0;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtors; }
static const long* g_speechSeq; static int g_speechCalls = 0; static bool g_argsOk = true;
long CTCTalk::PlayAISpeech(const CCharString& text, bool allowNonPure, long layer, bool a, bool b)
{
    g_lastText = (const char*)text.m_pData;
    if (allowNonPure || layer != 2 || a || !b) g_argsOk = false;
    return g_speechSeq[g_speechCalls++];
}
long g_framesPerSecond = 30;
static unsigned long g_seed;
unsigned long* CAIStateGroup_Drunk::GetPWorldSeed() { return &g_seed; }
static float g_randRange = -1.0f; static unsigned long* g_randSeed;
float __fastcall GFFloatRandom(float range, unsigned long* seed) { g_randRange = range; g_randSeed = seed; return 5.0f; }
static const long* g_idleSeq; static int g_idleCalls = 0;
long CAIStateGroup_Drunk::AIAction_DoNothing() { return g_idleSeq[g_idleCalls++]; }
static int g_terminates = 0;
void CAIStateGroup_Drunk::Terminate() { ++g_terminates; }
static void Reset(CAIStateGroup_Drunk& s, long frame, long next, const long* speech, const long* idle)
{
    g_frame = frame; s.FrameToNextTalkToSelf = next; g_speechSeq = speech; g_idleSeq = idle;
    g_speechCalls = g_idleCalls = g_terminates = 0; g_strDtors = 0; g_lastText = 0; g_argsOk = true;
    g_randRange = -1.0f; g_randSeed = 0; g_isTalkingCalls = 0; g_talking = false; g_lbType = -1;
}
int main()
{
    static CThing creature;
    creature.m_presentBits[0] = 1UL << 0xf;
    g_entries[0].type = 0xf; g_entries[0].component = (CTCBase*)&g_talk;
    creature.m_interfaces.first = &g_entries[0]; creature.m_interfaces.last = &g_entries[1];
    static CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_Drunk s; s.PBrain = &brain;
    // 1) not yet time: nothing happens
    static const long none[1] = { 0 };
    Reset(s, 100, 100, none, none); s.TalkToSelfUpdate();
    CHECK(g_isTalkingCalls == 0 && g_speechCalls == 0 && s.FrameToNextTalkToSelf == 100);
    // 2) already talking: no speech
    Reset(s, 101, 100, none, none); g_talking = true; s.TalkToSelfUpdate();
    CHECK(g_lbType == 0xf && g_isTalkingCalls == 1 && g_speechCalls == 0 && s.FrameToNextTalkToSelf == 100);
    // 3) speech started (0): re-arm at frame + (5 + 10) * 30
    static const long ok[1] = { 0 };
    Reset(s, 200, 100, ok, none); s.TalkToSelfUpdate();
    CHECK(g_speechCalls == 1 && g_argsOk && g_lastText && strcmp(g_lastText, "TEXT_AI_OPINION_ACT_DRUNK_TALK_TO_SELF") == 0 && g_strDtors == 1);
    CHECK(g_randRange == 20.0f && g_randSeed == &g_seed && s.FrameToNextTalkToSelf == 200 + 15 * 30 && g_terminates == 0 && g_idleCalls == 0);
    // 4) busy (2) twice while idling keeps going (4), then started: two idles, three attempts, re-armed
    static const long busy2[3] = { 2, 2, 0 }; static const long go2[2] = { 4, 4 };
    Reset(s, 300, 100, busy2, go2); s.TalkToSelfUpdate();
    CHECK(g_speechCalls == 3 && g_idleCalls == 2 && g_strDtors == 3 && g_terminates == 0 && s.FrameToNextTalkToSelf == 300 + 450);
    // 5) busy and the idle stops (not 4): terminate, timer untouched
    static const long busy1[1] = { 2 }; static const long stop[1] = { 1 };
    Reset(s, 400, 100, busy1, stop); s.TalkToSelfUpdate();
    CHECK(g_speechCalls == 1 && g_idleCalls == 1 && g_terminates == 1 && s.FrameToNextTalkToSelf == 100 && g_strDtors == 1);
    // 6) failed (1): plain return, no re-arm, no terminate
    static const long fail[1] = { 1 };
    Reset(s, 500, 100, fail, none); s.TalkToSelfUpdate();
    CHECK(g_speechCalls == 1 && g_terminates == 0 && g_idleCalls == 0 && s.FrameToNextTalkToSelf == 100 && g_randRange == -1.0f);
    if (fails == 0) printf("00922750_TEST PASS\n");
    return fails;
}
