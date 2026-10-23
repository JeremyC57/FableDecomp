// Behaviour test for 00914460: src/00914460.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_BirdGroundWander::Update()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_BirdGroundWander::Update() -- hop to one to seven nearby wander positions, idling after each, then terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00914460, length 395 bytes.
// Callees: the creature's +0x60 subsystem virtual slot 67 (+0x10C, called with 0), GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890,
//          CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 7 = CTCCreatureNavigation as a bitset::test byte),
//          CThing::GetPosition const @ 0x006A4D00, CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0,
//          CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (repeat 2, re-issued while it reports 4), CCharString(const char*,
//          long) @ 0x0099EBF0 ("ST_IDLE" @ 0x0127138C), CCreatureAction_PlayAnimation(creature, anim, sound, ...) @ 0x008424F0,
//          AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0, ~CCharString @ 0x0099EAE0,
//          Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo" is propagated).
// Shape: the ChickenWander body (0x009118A0) in a counted loop: `hops = Random(7)` and the loop test `i < hops + 1`
//          (VC7.1 hoists hops + 1 into edi and tests it against the zero register `lea edi,[edx+1] ; cmp edi,ebp ; jbe`;
//          a `Random(7) + 1` count is folded to `inc ; je`); the subsystem call goes through an inline creature accessor;
//          every exit breaks to the one Terminate call.
// Layout: CAIStateGroupBase per engine/CAIStateGroupBase.h (PBrain +4); creature bitset +0x20, interface map +0x44.
//          No engine header exists for CAIStateGroup_BirdGroundWander (the PDB header lists it without members).
struct CTCBase;
struct CThing;
struct CThingCreatureBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(const C3DVector& v) { x = v.x; y = v.y; z = v.z; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    struct CThingSubsystem* m_pSubsystem60;     // +0x60 (role not established; virtual slot 67 takes a zero)
    const C3DVector& GetPosition_006a4d00() const;       // 0x006A4D00
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};

struct CThingSubsystem {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41();
    virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53();
    virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61(); virtual void s62(); virtual void s63(); virtual void s64(); virtual void s65();
    virtual void s66();
    virtual void Slot67(long value);            // slot 67 (+0x10c)
};

struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                         // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    CThing* GetCreatureThing() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_BirdGroundWander : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_BirdGroundWander::Update()
{
    GetCreatureThing()->m_pSubsystem60->Slot67(0);
    unsigned long hops = Random(7);
    for (unsigned long i = 0; i < hops + 1; ++i) {
        CThing* creature = PBrain->PCreature;
        CTCCreatureNavigation* navigation;
        creature->GetInterfaceT(&navigation, 7);
        C3DVector target;
        if (!navigation->GetNearbyWanderPosition(creature->GetPosition_006a4d00(), 1.5f, &target, true))
            break;
        bool reached = false;
        long result;
        do {
            result = AIAction_MoveToPos(2, target, &reached, 3, true, true, 0, true, 0.0f);
        } while (result == 4);
        if (result != 2)
            break;
        bool failed;
        {
            CCharString animation("ST_IDLE");
            failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), animation,
                CSoundPair(), false, true, false, 0, 0), false) != 2;
        }
        if (failed)
            break;
    }
    Terminate();
}

// ---- stubs and driver ----
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { CHECK(*type == 7); return first; }
static C3DVector g_pos;
const C3DVector& CThing::GetPosition_006a4d00() const { return g_pos; }
void CThingSubsystem::s00() { } void CThingSubsystem::s01() { } void CThingSubsystem::s02() { } void CThingSubsystem::s03() { } void CThingSubsystem::s04() { } void CThingSubsystem::s05() { } void CThingSubsystem::s06() { } void CThingSubsystem::s07() { } void CThingSubsystem::s08() { } void CThingSubsystem::s09() { } void CThingSubsystem::s10() { } void CThingSubsystem::s11() { } void CThingSubsystem::s12() { } void CThingSubsystem::s13() { } void CThingSubsystem::s14() { } void CThingSubsystem::s15() { } void CThingSubsystem::s16() { } void CThingSubsystem::s17() { } void CThingSubsystem::s18() { } void CThingSubsystem::s19() { } void CThingSubsystem::s20() { } void CThingSubsystem::s21() { } void CThingSubsystem::s22() { } void CThingSubsystem::s23() { } void CThingSubsystem::s24() { } void CThingSubsystem::s25() { } void CThingSubsystem::s26() { } void CThingSubsystem::s27() { } void CThingSubsystem::s28() { } void CThingSubsystem::s29() { } void CThingSubsystem::s30() { } void CThingSubsystem::s31() { } void CThingSubsystem::s32() { } void CThingSubsystem::s33() { } void CThingSubsystem::s34() { } void CThingSubsystem::s35() { } void CThingSubsystem::s36() { } void CThingSubsystem::s37() { } void CThingSubsystem::s38() { } void CThingSubsystem::s39() { } void CThingSubsystem::s40() { } void CThingSubsystem::s41() { } void CThingSubsystem::s42() { } void CThingSubsystem::s43() { } void CThingSubsystem::s44() { } void CThingSubsystem::s45() { } void CThingSubsystem::s46() { } void CThingSubsystem::s47() { } void CThingSubsystem::s48() { } void CThingSubsystem::s49() { } void CThingSubsystem::s50() { } void CThingSubsystem::s51() { } void CThingSubsystem::s52() { } void CThingSubsystem::s53() { } void CThingSubsystem::s54() { } void CThingSubsystem::s55() { } void CThingSubsystem::s56() { } void CThingSubsystem::s57() { } void CThingSubsystem::s58() { } void CThingSubsystem::s59() { } void CThingSubsystem::s60() { } void CThingSubsystem::s61() { } void CThingSubsystem::s62() { } void CThingSubsystem::s63() { } void CThingSubsystem::s64() { } void CThingSubsystem::s65() { } void CThingSubsystem::s66() { }
static long g_slot67Arg = -1; static int g_slot67Calls = 0;
void CThingSubsystem::Slot67(long value) { ++g_slot67Calls; g_slot67Arg = value; }
static CTCCreatureNavigation g_nav; static int g_wanderCalls = 0, g_wanderFailAt = -1;
static const C3DVector* g_wanderCentre; static float g_wanderRadius; static bool g_wanderOnMap;
bool CTCCreatureNavigation::GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap)
{
    CHECK(this == &g_nav);
    ++g_wanderCalls; g_wanderCentre = &centre; g_wanderRadius = radius; g_wanderOnMap = onMap;
    if (g_wanderCalls == g_wanderFailAt) return false;
    result->x = 10.0f * g_wanderCalls; result->y = 1.0f; result->z = 2.0f;
    return true;
}
static int g_strDtors = 0; static const char* g_animName;
CCharString::CCharString(const char* text, long length) { m_pData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtors; }
void CCreatureActionBase::v00() { }
static int g_actionDtors = 0;
CCreatureActionBase::~CCreatureActionBase() { ++g_actionDtors; }
static int g_actionCtors = 0; static CThingCreatureBase* g_actionCreature; static bool g_actionFlagsOk = true;
CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
    const CSoundPair& sound, bool a, bool b, bool c, long d, long e)
{
    ++g_actionCtors; g_actionCreature = creature; g_animName = (const char*)animation.m_pData;
    if (sound.m_first != 0 || sound.m_second != 0 || a || !b || c || d != 0 || e != 0) g_actionFlagsOk = false;
}
static unsigned long g_rorResult = 0;
unsigned long __fastcall GFROR13(unsigned long value) { return g_rorResult; }
CAIStateGroupBase::~CAIStateGroupBase() { }
static unsigned long g_seed = 1;
unsigned long* CAIStateGroupBase::GetPWorldSeed() { return &g_seed; }
static const long* g_moveSeq; static int g_moveCalls = 0; static float g_lastTargetX = 0; static bool g_moveArgsOk = true;
long CAIStateGroupBase::AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
    CThing* avoid, bool c, float speed)
{
    if (repeat != 2 || reached == 0 || *reached != false || movement != 3 || !a || !b || avoid != 0 || !c || speed != 0.0f) g_moveArgsOk = false;
    g_lastTargetX = pos.x;
    return g_moveSeq[g_moveCalls++];
}
static const long* g_actSeq; static int g_actCalls = 0;
long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced)
{
    CHECK(action != 0 && !stopWhenReplaced);
    return g_actSeq[g_actCalls++];
}
static int g_terminates = 0;
void CAIStateGroupBase::Terminate() { ++g_terminates; }
static void Reset(unsigned long ror, const long* moves, const long* acts, int wanderFailAt)
{
    g_rorResult = ror; g_moveSeq = moves; g_actSeq = acts; g_wanderFailAt = wanderFailAt;
    g_moveCalls = g_actCalls = g_wanderCalls = 0; g_terminates = 0; g_slot67Calls = 0; g_slot67Arg = -1;
    g_actionCtors = g_actionDtors = g_strDtors = 0; g_actionFlagsOk = g_moveArgsOk = true; g_animName = 0; g_seed = 1;
}
int main()
{
    static CThing creature;
    static CThingSubsystem sub;
    creature.m_pSubsystem60 = &sub;
    creature.m_presentBits[0] = 1UL << 7;
    g_entries[0].type = 7; g_entries[0].component = (CTCBase*)&g_nav;
    creature.m_interfaces.first = &g_entries[0]; creature.m_interfaces.last = &g_entries[1];
    static CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_BirdGroundWander s; s.PBrain = &brain;
    // 1) random 16 % 7 = 2 -> 3 hops, each: still moving (4) then arrived (2), the idle anim plays (2)
    static const long moves1[6] = { 4, 2, 4, 2, 4, 2 }; static const long acts1[3] = { 2, 2, 2 };
    Reset(16, moves1, acts1, -1); s.Update();
    CHECK(g_slot67Calls == 1 && g_slot67Arg == 0 && g_seed == 16);
    CHECK(g_wanderCalls == 3 && g_moveCalls == 6 && g_actCalls == 3 && g_terminates == 1 && g_moveArgsOk);
    CHECK(g_wanderCentre == &g_pos && g_wanderRadius == 1.5f && g_wanderOnMap && g_lastTargetX == 30.0f);
    CHECK(g_actionCtors == 3 && g_actionDtors == 3 && g_strDtors == 3 && g_actionFlagsOk && g_animName && strcmp(g_animName, "ST_IDLE") == 0);
    CHECK(g_actionCreature == (CThingCreatureBase*)&creature);
    // 2) random 7 % 7 = 0 -> exactly one hop
    static const long moves2[1] = { 2 }; static const long acts2[1] = { 2 };
    Reset(7, moves2, acts2, -1); s.Update();
    CHECK(g_wanderCalls == 1 && g_moveCalls == 1 && g_actCalls == 1 && g_terminates == 1);
    // 3) no wander position on the second hop: break, terminate
    static const long moves3[2] = { 2, 2 }; static const long acts3[2] = { 2, 2 };
    Reset(13, moves3, acts3, 2); s.Update();
    CHECK(g_wanderCalls == 2 && g_moveCalls == 1 && g_actCalls == 1 && g_terminates == 1);
    // 4) the move fails (3): break before the animation
    static const long moves4[2] = { 4, 3 }; static const long acts4[1] = { 2 };
    Reset(13, moves4, acts4, -1); s.Update();
    CHECK(g_wanderCalls == 1 && g_moveCalls == 2 && g_actCalls == 0 && g_actionCtors == 0 && g_terminates == 1);
    // 5) the animation does not finish (1): break after one hop
    static const long moves5[1] = { 2 }; static const long acts5[1] = { 1 };
    Reset(13, moves5, acts5, -1); s.Update();
    CHECK(g_wanderCalls == 1 && g_actCalls == 1 && g_actionDtors == 1 && g_strDtors == 1 && g_terminates == 1);
    if (fails == 0) printf("00914460_TEST PASS\n");
    return fails;
}
