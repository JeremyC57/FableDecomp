// Behaviour test for 0090fdb0: final.cpp copied verbatim; every callee is stubbed and scripted so the school window,
// the staggered end time, the walk/run choice, the hand-raising and the random leave test are observable.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define _snprintf snprintf
#endif
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
#define CHECKLOG(e) do { if (strcmp(g_log, e) != 0) { ++fails; printf("FAIL %d: log=%s\n  expected=%s\n", __LINE__, g_log, e); } } while (0)
static char g_log[8192];
static void note(const char* s) { if (strlen(g_log) + strlen(s) + 2 < sizeof(g_log)) { strcat(g_log, s); strcat(g_log, ";"); } }
// ---- source (verbatim) ----
// CAIStateGroup_MinionSitAtSchool::Update() -- read the village school times, pick a randomly staggered end time, walk (seat within 10 units) or run to the seat and sit; then idle while the time of day stays in the school window (leaving at a 1-in-5-seconds random once it is over), raising a hand at random intervals while the village has a lesson on; finally get up and Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FDB0, length 848 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (GetInterface 0x23 = CTCVillageMember, bit 3 of +0x24, unchecked),
// CTCVillageMember::GetPVillage @ 0x007EE540, CTCVillage::GetSchoolTimes @ 0x00658ACB, GetPThingWorld @ 0x008FCFA0,
// GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CThing::GetPosition @ 0x004C73D0, CAIStateGroup_UsesSeat::MoveToSeat @ 0x009385F0 / SitInSeat @ 0x00938380 / the get-up
// @ 0x00938A10, CWorld::GetWorldFrame @ 0x0049D870, AIAction_DoNothing @ 0x0097E200, CCharString(const char*, long) @ 0x0099EBF0
// / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayAnimation ctor @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0,
// AIAction_PerformActionAndWait @ 0x0097DB70, Terminate @ 0x008FCF40.
// Data: 2^32 @ 0x0122DCB4, 1.0f @ 0x0122DED8, 10.0f @ 0x01237F0C, g_framesPerSecond @ 0x01375550,
// "ST_SEATED_CHAIR_RAISE_HAND" @ 0x0128F5D4.
// Layout: local model (PBrain +4; brain PCreature +0x18; PSeat +0x14; thing world +0x1c time of day: now +0x08, stagger
// step +0x10; village lesson flag +0xBC; CCreatureAction_PlayAnimation 0xb4 bytes).
// Shape: the world-seed random is inline (seed*0x24A1+0x24DF, GFROR13, `range ? r % range : 0`) through a __forceinline
// RandomFrames(seconds) helper that copies g_framesPerSecond into a local BEFORE fetching the seed and forms the range
// before the seed update: retail loads fps into a callee-saved register ahead of GetPWorldSeed and multiplies (x40/x5)
// between the seed arithmetic and GFROR13; the leave test's fps load stays inside the `over` branch this way.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    __forceinline float GetMagnitude() const;
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
__forceinline float C3DVector::GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: };
struct CVillage {
    unsigned char _pad[0xbc];
    bool LessonOn;                              // +0xBC
    bool GetSchoolTimes(float* start, float* end);   // 0x00658ACB
};
class CTCVillageMember { public:
    CVillage* GetPVillage();                    // 0x007EE540
};
struct CTimeOfDay {
    unsigned char _pad[0x08];
    float Now;                                  // +0x08
    unsigned char _pad0c[4];
    float StaggerStep;                          // +0x10
};
struct CThingWorld {
    unsigned char _pad[0x1c];
    CTimeOfDay* PTimeOfDay;                     // +0x1c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CThingWorld* GetPThingWorld();              // 0x008FCFA0
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    __forceinline unsigned long RandomFrames(unsigned long seconds)   // random frame count below seconds * fps
    {
        long fps = g_framesPerSecond;
        unsigned long* seed = GetPWorldSeed();
        unsigned long range = fps * seconds;
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CBaseIntelligentPointer PSeat;              // +0x14
    bool MoveToSeat(long movementType);         // 0x009385F0
    bool SitInSeat();                           // 0x00938380
    void GetUpFromSeat();                       // 0x00938A10
};
struct CAIStateGroup_MinionSitAtSchool : CAIStateGroup_UsesSeat {
    virtual void Update();
};

void CAIStateGroup_MinionSitAtSchool::Update()
{
    CTCVillageMember* member;
    GetCreature()->GetInterface(&member, 0x23);
    float startTime;
    float endTime;
    member->GetPVillage()->GetSchoolTimes(&startTime, &endTime);
    float now = GetPThingWorld()->PTimeOfDay->Now;
    unsigned long* seed = GetPWorldSeed();
    *seed = *seed * 0x24A1U + 0x24DFU;
    *seed = GFROR13(*seed);
    unsigned long stagger = *seed & 3;
    endTime = (float)stagger * GetPThingWorld()->PTimeOfDay->StaggerStep + endTime;
    if (endTime > 1.0f)
        endTime = endTime - 1.0f;
    long movement = 5;
    CThing* seat = PSeat.GetPItem();
    if (seat && !(seat->m_flags91 & 1)) {
        const CThing* creature = GetCreature();
        const CThing* item = PSeat.GetPItem();
        if ((item->GetPosition() - creature->GetPosition()).GetMagnitude() < 10.0f)
            movement = 3;
    }
    if (MoveToSeat(movement) && SitInSeat()) {
        long nextHand = CWorld::GetFrame() + RandomFrames(40);
        while (AIAction_DoNothing() == 4) {
            float time = GetPThingWorld()->PTimeOfDay->Now;
            bool over;
            if (endTime > now)
                over = time > endTime || time < now;
            else
                over = time > endTime && time < now;
            if (over && RandomFrames(5) == 0)
                break;
            if (CWorld::GetFrame() > nextHand && member->GetPVillage()->LessonOn) {
                AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_SEATED_CHAIR_RAISE_HAND", CSoundPair(),
                    false, true, false, 0, 0), false);
                nextHand = CWorld::GetFrame() + RandomFrames(40);
            }
        }
        GetUpFromSeat();
    }
    Terminate();
}

// ---- stubs and script ----
long g_framesPerSecond = 30;
static char g_buf[256];
static unsigned long g_seed = 12345;
static float g_start = 0.3f, g_end = 0.5f;
static CVillage g_village;
static CTCVillageMember g_member;
static CTimeOfDay g_tod;
static CThingWorld g_world;
static CThing* g_creature;
static CThing* g_seat;
static C3DVector g_creaturePos(0, 0, 0), g_seatPos(3, 4, 0);
static bool g_moveOk = true, g_sitOk = true;
static long g_doNothing[16]; static float g_times[16]; static int g_doNothingCount = 0, g_doNothingIdx = 0;
static long g_frames[16]; static int g_frameIdx = 0;
static CInterfaceMap::SEntry g_entries[2];

CAIStateGroupBase::~CAIStateGroupBase() { }
void CBaseIntelligentPointer::v00() { }
void CCreatureActionBase::v00() { }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    _snprintf(g_buf, sizeof(g_buf), "LowerBound(%ld)", *type); note(g_buf);
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
bool CVillage::GetSchoolTimes(float* start, float* end) { note("GetSchoolTimes"); *start = g_start; *end = g_end; return true; }
CVillage* CTCVillageMember::GetPVillage() { note("GetPVillage"); return &g_village; }
CThingWorld* CAIStateGroupBase::GetPThingWorld() { note("GetPThingWorld"); return &g_world; }
unsigned long* CAIStateGroupBase::GetPWorldSeed() const { note("GetPWorldSeed"); return &g_seed; }
unsigned long __fastcall GFROR13(unsigned long value) { note("GFROR13"); return (value >> 13) | (value << 19); }
CThing* CBaseIntelligentPointer::GetPItem() const { note("GetPItem"); return m_pItem; }
const C3DVector& CThing::GetPosition() const { note(this == g_seat ? "GetPosition(seat)" : "GetPosition(creature)"); return this == g_seat ? g_seatPos : g_creaturePos; }
bool CAIStateGroup_UsesSeat::MoveToSeat(long movementType) { _snprintf(g_buf, sizeof(g_buf), "MoveToSeat(%ld)", movementType); note(g_buf); return g_moveOk; }
bool CAIStateGroup_UsesSeat::SitInSeat() { note("SitInSeat"); return g_sitOk; }
void CAIStateGroup_UsesSeat::GetUpFromSeat() { note("GetUpFromSeat"); }
void CAIStateGroupBase::Terminate() { note("Terminate"); }
long CAIStateGroupBase::AIAction_DoNothing()
{
    int i = g_doNothingIdx < g_doNothingCount ? g_doNothingIdx : g_doNothingCount - 1;
    ++g_doNothingIdx;
    g_tod.Now = g_times[i];
    _snprintf(g_buf, sizeof(g_buf), "DoNothing->%ld@%g", g_doNothing[i], g_times[i]); note(g_buf);
    return g_doNothing[i];
}
long CWorld::GetFrame() { long f = g_frames[g_frameIdx < 15 ? g_frameIdx : 15]; ++g_frameIdx; _snprintf(g_buf, sizeof(g_buf), "Frame=%ld", f); note(g_buf); return f; }
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; }
CCharString::~CCharString() { }
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
    const CSoundPair& sound, bool a, bool b, bool c, long d, long e)
{
    _snprintf(g_buf, sizeof(g_buf), "PlayAnimation(%s,%d,%d,%d,%ld,%ld,%s)", (const char*)animation.m_pData, (int)a, (int)b, (int)c, d, e,
        creature == (CThingCreatureBase*)g_creature ? "creature" : "?"); note(g_buf);
    CHECK(sound.m_first == 0 && sound.m_second == 0);
}
CCreatureActionBase::~CCreatureActionBase() { note("~Action"); }
long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait) { _snprintf(g_buf, sizeof(g_buf), "PerformActionAndWait(wait=%d)", (int)wait); note(g_buf); return 0; }

// reference random: the same world-seed step, kept apart from the body under test
static unsigned long RefStep(unsigned long& s) { s = s * 0x24A1U + 0x24DFU; s = (s >> 13) | (s << 19); return s; }
static unsigned long RefRandom(unsigned long& s, unsigned long range) { unsigned long r = RefStep(s); return range ? r % range : 0; }

struct CTestGroup : CAIStateGroup_MinionSitAtSchool { };
static CAIBrain g_brain;
static CTestGroup* MakeGroup(CThing* seat)
{
    CTestGroup* g = new CTestGroup();
    g->PBrain = &g_brain;
    g->PSeat.m_pItem = seat;
    return g;
}
static CThing* MakeThing(bool dead)
{
    CThing* t = (CThing*)calloc(1, sizeof(CThing));
    t->m_flags91 = dead ? 1 : 0;
    t->m_presentBits[1] |= 1u << 3;                     // interface 0x23 present
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0x23; g_entries[1].component = (CTCBase*)&g_member;
    t->m_interfaces.first = g_entries; t->m_interfaces.last = g_entries + 2;
    return t;
}
static void Reset(unsigned long seed)
{
    g_log[0] = 0; g_seed = seed; g_doNothingIdx = 0; g_frameIdx = 0; g_tod.Now = 0.4f; g_tod.StaggerStep = 0.01f;
    g_world.PTimeOfDay = &g_tod; g_village.LessonOn = false; g_moveOk = true; g_sitOk = true;
}

int main()
{
    g_creature = MakeThing(false);
    g_brain.PCreature = (CThingCreatureBase*)g_creature;
    g_seat = MakeThing(false);

    // 1. no seat: run (5) to the seat; MoveToSeat fails -> Terminate only
    Reset(1); g_moveOk = false;
    CTestGroup* g = MakeGroup(0);
    g->Update();
    CHECKLOG("LowerBound(35);GetPVillage;GetSchoolTimes;GetPThingWorld;GetPWorldSeed;GFROR13;GetPThingWorld;GetPItem;MoveToSeat(5);Terminate;");
    { unsigned long s = 1; RefStep(s); CHECK(g_seed == s); }

    // 2. dead seat: still 5; seated but DoNothing ends at once (not 4): get up, terminate, no random leave test
    Reset(2); g_seat->m_flags91 = 1;
    g_doNothingCount = 1; g_doNothing[0] = 0; g_times[0] = 0.45f; g_frames[0] = 100;
    g = MakeGroup(g_seat);
    g->Update();
    CHECKLOG("LowerBound(35);GetPVillage;GetSchoolTimes;GetPThingWorld;GetPWorldSeed;GFROR13;GetPThingWorld;GetPItem;MoveToSeat(5);SitInSeat;Frame=100;GetPWorldSeed;GFROR13;DoNothing->0@0.45;GetUpFromSeat;Terminate;");
    g_seat->m_flags91 = 0;

    // 3. live seat 5 units away: walk (3); the hand is raised when the frame passes nextHand with a lesson on;
    //    the random leave test fires only once the time is outside the window
    unsigned long seed = 777;
    Reset(seed); g_village.LessonOn = true;
    unsigned long ref = seed;
    unsigned long stagger = RefStep(ref) & 3;
    float endTime = (float)stagger * 0.01f + 0.5f;        // < 1, no wrap
    long nextHand = 10 + (long)RefRandom(ref, 30 * 40);
    g_frames[0] = 10;                                     // first nextHand
    g_frames[1] = nextHand + 1;                           // iteration 1: raise a hand
    g_frames[2] = 10;                                     // second nextHand base
    g_frames[3] = 0;                                      // iteration 2: no hand
    g_frames[4] = 0; g_frames[5] = 0;
    g_doNothingCount = 4;
    g_doNothing[0] = 4; g_times[0] = 0.45f;               // inside the window, not over
    g_doNothing[1] = 4; g_times[1] = 0.9f;                // over: leave test rolls
    g_doNothing[2] = 4; g_times[2] = 0.9f;                // over again
    g_doNothing[3] = 0; g_times[3] = 0.9f;                // the action ends anyway
    g = MakeGroup(g_seat);
    g->Update();
    long nextHand2 = 10 + (long)RefRandom(ref, 30 * 40);  // after the hand raise
    char expect[2048];
    strcpy(expect, "LowerBound(35);GetPVillage;GetSchoolTimes;GetPThingWorld;GetPWorldSeed;GFROR13;GetPThingWorld;GetPItem;GetPItem;GetPosition(creature);GetPosition(seat);MoveToSeat(3);SitInSeat;Frame=10;GetPWorldSeed;GFROR13;");
    strcat(expect, "DoNothing->4@0.45;GetPThingWorld;");
    _snprintf(g_buf, sizeof(g_buf), "Frame=%ld;GetPVillage;PlayAnimation(ST_SEATED_CHAIR_RAISE_HAND,0,1,0,0,0,creature);PerformActionAndWait(wait=0);~Action;Frame=10;GetPWorldSeed;GFROR13;", nextHand + 1);
    strcat(expect, g_buf);
    // iteration 2: over -> random leave roll; break on zero, else frame test (0 > nextHand2 false)
    strcat(expect, "DoNothing->4@0.9;GetPThingWorld;GetPWorldSeed;GFROR13;");
    bool leave = RefRandom(ref, 30 * 5) == 0;
    if (!leave) {
        strcat(expect, "Frame=0;DoNothing->4@0.9;GetPThingWorld;GetPWorldSeed;GFROR13;");
        leave = RefRandom(ref, 30 * 5) == 0;
        if (!leave) strcat(expect, "Frame=0;DoNothing->0@0.9;");
    }
    strcat(expect, "GetUpFromSeat;Terminate;");
    CHECKLOG(expect);
    CHECK(g_seed == ref);
    CHECK(nextHand2 >= 10 && nextHand2 < 10 + 1200);
    CHECK(endTime > 0.49f && endTime < 0.54f);

    // 4. window near midnight (endTime > 1 wraps when the stagger pushes it over): now = 0.98, end = 0.995 + stagger;
    //    wrapped -> over means time > end AND time < now, so 0.99 (>= now) is inside; unwrapped -> 0.985 is inside
    g_end = 0.995f; Reset(5); g_tod.Now = 0.98f;
    ref = 5; stagger = RefStep(ref) & 3;
    float wrapped = (float)stagger * 0.01f + 0.995f; if (wrapped > 1.0f) wrapped = wrapped - 1.0f;
    g_frames[0] = 10; g_frames[1] = 0; g_frames[2] = 0; g_frames[3] = 0;
    g_doNothingCount = 2;
    g_doNothing[0] = 4; g_times[0] = (wrapped > 0.98f) ? 0.985f : 0.99f;  // inside the window either way (not over)
    g_doNothing[1] = 0; g_times[1] = 0.5f;
    g = MakeGroup(0);
    g->Update();
    // with no seat: run (5); the first DoNothing is inside the window so no leave roll, the second ends the loop
    _snprintf(expect, sizeof(expect), "LowerBound(35);GetPVillage;GetSchoolTimes;GetPThingWorld;GetPWorldSeed;GFROR13;GetPThingWorld;GetPItem;MoveToSeat(5);SitInSeat;Frame=10;GetPWorldSeed;GFROR13;DoNothing->4@%g;GetPThingWorld;Frame=0;DoNothing->0@0.5;GetUpFromSeat;Terminate;", g_times[0]);
    CHECKLOG(expect);

    if (fails == 0) printf("0090fdb0_TEST PASS\n");
    else printf("0090fdb0_TEST FAIL (%d)\n", fails);
    return fails ? 1 : 0;
}