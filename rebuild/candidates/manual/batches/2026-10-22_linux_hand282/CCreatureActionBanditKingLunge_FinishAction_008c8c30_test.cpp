// Behaviour test for 0x008C8C30 CCreatureAction_BanditKingLunge::FinishAction: runs the reconstructed body over stub things,
// components and actions; checks the stuck path (mode 0x53, the stuck hit location on and the normal one off, the boss loop
// count, the obstruction half a facing ahead with the king ignored, the stuck action's arguments, the obstruction killed only
// when the action is refused), the missing-component cases, and the unstuck fallback for a dead target or an unfinished lunge.
#include <cstdio>
#include <cstring>

#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

struct CTCBase { };
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};

static int g_liveStrings = 0;
struct CCharString {
    char m_text[64];
    CCharString(const char* text, long = -1) { std::strcpy(m_text, text); ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry  entries[8];
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};

struct CTCPhysicsBase {
    C3DVector m_facing;
    virtual ~CTCPhysicsBase() { }
    virtual C3DVector GetFacing() const { return m_facing; }
};

struct CThing {
    unsigned long   m_presentBits[9];
    CInterfaceMap   m_interfaces;
    CTCPhysicsBase* PhysicsTC;
    unsigned char   m_flags91;
    C3DVector       m_position;
    int             m_killed;
    bool            m_killFlag;
    CThing() : PhysicsTC(0), m_flags91(0), m_position(0.0f, 0.0f, 0.0f), m_killed(0), m_killFlag(false)
    {
        for (int i = 0; i < 9; ++i)
            m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = m_interfaces.entries;
    }
    void AddInterface(long type, CTCBase* component)   // test helper: keep the map sorted by type
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) {
            *it = *(it - 1);
            --it;
        }
        it->type = type;
        it->component = component;
        ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    const C3DVector& GetPosition() const { return m_position; }
    void Kill(bool flag) { ++m_killed; m_killFlag = flag; }
    CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};

enum EActionKind { ACTION_NONE, ACTION_STUCK, ACTION_UNSTUCK };
struct CCreatureActionBase;
static int  g_setActionCalls = 0;
static bool g_acceptAction = true;
static int  g_lastKind = ACTION_NONE;
static CThing* g_stuckOwner = 0;
static CThing* g_stuckTarget = 0;
static long g_stuckLoops = -1;
static CThing* g_stuckObstruction = 0;
static CThing* g_stuckEffect = (CThing*)1;
static CThing* g_unstuckOwner = 0;
static long g_unstuckCode = 0;
static bool g_unstuckNoTarget = false;
static long g_soundFirst = -1, g_soundSecond = -1;

struct CThingCreatureBase : CThing {
    CThingCreatureBase* m_self;
    bool SetCurrentAction(const CCreatureActionBase& action);
};

struct CTCCreatureModeManager : CTCBase {
    unsigned long m_lastMode;
    int           m_calls;
    bool AddMode(unsigned long mode) { m_lastMode = mode; ++m_calls; return true; }
};
struct CHitLocation {
    int  m_calls;
    bool m_enabled;
    void SetEnabled(bool enabled) { ++m_calls; m_enabled = enabled; }
};
struct CTCHitLocations : CTCBase {
    CHitLocation* m_stuck;
    CHitLocation* m_normal;
    CHitLocation* GetPHitLocationFromName(const CCharString& name)
    {
        if (std::strcmp(name.m_text, "HIT_LOCATION_TWINBLADE_STUCK") == 0) return m_stuck;
        if (std::strcmp(name.m_text, "HIT_LOCATION_TWINBLADE") == 0) return m_normal;
        return 0;
    }
};
struct CBossDef {
    long TwinBladeStuckLoops;
};
struct CTCBoss : CTCBase {
    CBossDef* PBossDef;
};
static CThing*   g_createResult = 0;
static C3DVector g_createStart(0.0f, 0.0f, 0.0f), g_createEnd(0.0f, 0.0f, 0.0f);
static float     g_createRadius = 0.0f;
static int       g_createCalls = 0;
struct CTCDPhysicalObstruction : CTCBase {
    CThing* m_ignored;
    static CThing* __fastcall Create(const C3DVector& start, const C3DVector& end, float radius)
    {
        ++g_createCalls; g_createStart = start; g_createEnd = end; g_createRadius = radius;
        return g_createResult;
    }
    void SetThingToIgnoreObstruction(CThing* thing) { m_ignored = thing; }
};

static int g_ipDestroyed = 0;
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { ++g_ipDestroyed; }
    CThing* GetPItem() const { return m_pItem; }
};
struct CCreaturePointer {
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const { return m_pItem; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CSoundPair {
    long First;
    long Second;
    CSoundPair() : First(0), Second(0) { }
};
static int g_baseFinish = 0;
static int g_baseDestroyed = 0;
struct CCreatureActionBase {
    int m_kind;
    CCreatureActionBase() : m_kind(ACTION_NONE) { }
    virtual ~CCreatureActionBase() { }
};
struct CCombatActionBase : CCreatureActionBase {
    CCreaturePointer        PCreature;
    long                    TotalTime;
    long                    FramesRunning;
    CBaseIntelligentPointer PTarget;
    CCombatActionBase() { }
    CCombatActionBase(CThingCreatureBase* owner, long animation, const CSoundPair& sounds = CSoundPair())
    {
        PCreature.m_pItem = owner;
        g_unstuckCode = animation; g_soundFirst = sounds.First; g_soundSecond = sounds.Second;
    }
    ~CCombatActionBase() { ++g_baseDestroyed; }
    virtual void FinishAction() { ++g_baseFinish; }
};
struct CCreatureAction_BanditKingStuck : CCombatActionBase {
    long                    TimesToLoop;
    CBaseIntelligentPointer PStuckEffect;
    CBaseIntelligentPointer PObstruction;
    CCreatureAction_BanditKingStuck(CThingCreatureBase* owner, CThing* target, long timesToLoop, CThing* obstruction, CThing* stuckEffect)
    {
        m_kind = ACTION_STUCK;
        g_stuckOwner = owner; g_stuckTarget = target; g_stuckLoops = timesToLoop;
        g_stuckObstruction = obstruction; g_stuckEffect = stuckEffect;
        TimesToLoop = timesToLoop;
    }
};
struct CCreatureAction_BanditKingUnstuck : CCombatActionBase {
    bool Knockdown;
    bool NoTarget;
    CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner, bool noTarget)
        : CCombatActionBase(owner, 0x96), NoTarget(noTarget)
    {
        m_kind = ACTION_UNSTUCK;
        g_unstuckOwner = owner; g_unstuckNoTarget = noTarget;
    }
    virtual void FinishAction() { }
};
struct CCreatureAction_BanditKingLunge : CCombatActionBase {
    virtual void FinishAction();
};

bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action)
{
    ++g_setActionCalls;
    g_lastKind = action.m_kind;
    return g_acceptAction;
}

void CCreatureAction_BanditKingLunge::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PTarget) && FramesRunning >= TotalTime) {
        CTCCreatureModeManager* modeManager;
        PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
        modeManager->AddMode(0x53);
        CTCHitLocations* hitLocations;
        PCreature.GetPItem()->GetInterface(&hitLocations, 0x7f);
        CHitLocation* stuck = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE_STUCK");
        CHitLocation* normal = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE");
        if (stuck)
            stuck->SetEnabled(true);
        if (normal)
            normal->SetEnabled(false);
        long stuckLoops = 0;
        CTCBoss* boss;
        if (PCreature.GetPItem()->GetInterface(&boss, 0xad))
            stuckLoops = boss->PBossDef->TwinBladeStuckLoops;
        C3DVector position = PCreature.GetPItem()->GetPosition();
        position += PCreature.GetPItem()->GetPhysics()->GetFacing() * 0.5f;
        CThing* obstruction = CTCDPhysicalObstruction::Create(position, position, 1.5f);
        if (obstruction && !(obstruction->m_flags91 & 1)) {
            CTCDPhysicalObstruction* physicalObstruction;
            obstruction->GetInterface(&physicalObstruction, 0xed);
            physicalObstruction->SetThingToIgnoreObstruction(PCreature.GetPItem());
        }
        CThing* target = PTarget.GetPItem();
        CThingCreatureBase* owner = PCreature.GetPItem();
        CThingCreatureBase* self = PCreature.GetPItem();
        bool refused = !self->SetCurrentAction(CCreatureAction_BanditKingStuck(owner, target, stuckLoops, obstruction, 0));
        if (refused && obstruction && !(obstruction->m_flags91 & 1))
            obstruction->Kill(true);
    }
    else {
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_BanditKingUnstuck(PCreature.GetPItem(), true));
    }
}

int main()
{
    bool ok = true;
    CTCPhysicsBase physics; physics.m_facing = C3DVector(2.0f, -4.0f, 1.0f);
    CTCCreatureModeManager modes; modes.m_lastMode = 0; modes.m_calls = 0;
    CHitLocation stuckLoc; stuckLoc.m_calls = 0; stuckLoc.m_enabled = false;
    CHitLocation normalLoc; normalLoc.m_calls = 0; normalLoc.m_enabled = true;
    CTCHitLocations hits; hits.m_stuck = &stuckLoc; hits.m_normal = &normalLoc;
    CBossDef bossDef; bossDef.TwinBladeStuckLoops = 7;
    CTCBoss boss; boss.PBossDef = &bossDef;

    CThingCreatureBase king;
    king.m_position = C3DVector(10.0f, 20.0f, 30.0f);
    king.PhysicsTC = &physics;
    king.AddInterface(0xad, &boss);
    king.AddInterface(0x31, &modes);
    king.AddInterface(0x7f, &hits);

    CThing hero;
    CTCDPhysicalObstruction obstructionTC; obstructionTC.m_ignored = 0;
    CThing obstruction; obstruction.AddInterface(0xed, &obstructionTC);

    CCreatureAction_BanditKingLunge lunge;
    lunge.PCreature.m_pItem = &king;
    lunge.PTarget.m_pItem = &hero;
    lunge.TotalTime = 30;
    lunge.FramesRunning = 30;

    // full lunge at a living target, action accepted
    g_createResult = &obstruction;
    lunge.FinishAction();
    ok = ok && g_baseFinish == 1 && modes.m_calls == 1 && modes.m_lastMode == 0x53;
    ok = ok && stuckLoc.m_calls == 1 && stuckLoc.m_enabled && normalLoc.m_calls == 1 && !normalLoc.m_enabled;
    ok = ok && g_createCalls == 1 && g_createRadius == 1.5f;
    ok = ok && g_createStart.X == 11.0f && g_createStart.Y == 18.0f && g_createStart.Z == 30.5f;
    ok = ok && g_createEnd.X == 11.0f && g_createEnd.Y == 18.0f && g_createEnd.Z == 30.5f;
    ok = ok && king.m_position.X == 10.0f && obstructionTC.m_ignored == &king;
    ok = ok && g_setActionCalls == 1 && g_lastKind == ACTION_STUCK && g_stuckOwner == &king && g_stuckTarget == &hero;
    ok = ok && g_stuckLoops == 7 && g_stuckObstruction == &obstruction && g_stuckEffect == 0;
    ok = ok && obstruction.m_killed == 0 && g_liveStrings == 0 && g_ipDestroyed == 3 && g_baseDestroyed == 1;

    // refused: the obstruction is killed; a dead obstruction is left alone and gets no ignore entry
    g_acceptAction = false;
    lunge.FinishAction();
    ok = ok && obstruction.m_killed == 1 && obstruction.m_killFlag;
    obstruction.m_flags91 = 1; obstructionTC.m_ignored = 0;
    lunge.FinishAction();
    ok = ok && obstruction.m_killed == 1 && obstructionTC.m_ignored == 0 && g_setActionCalls == 3;

    // no obstruction created, no boss component: loop count 0, nothing to kill
    g_createResult = 0;
    CThingCreatureBase plain;
    plain.m_position = C3DVector(0.0f, 0.0f, 0.0f);
    plain.PhysicsTC = &physics;
    plain.AddInterface(0x31, &modes);
    plain.AddInterface(0x7f, &hits);
    lunge.PCreature.m_pItem = &plain;
    lunge.FinishAction();
    ok = ok && g_stuckLoops == 0 && g_stuckObstruction == 0 && g_stuckOwner == &plain && g_lastKind == ACTION_STUCK;
    ok = ok && g_createStart.X == 1.0f && g_createStart.Y == -2.0f && g_createStart.Z == 0.5f;

    // missing hit locations are skipped
    hits.m_stuck = 0; hits.m_normal = 0; stuckLoc.m_calls = 0; normalLoc.m_calls = 0;
    lunge.FinishAction();
    ok = ok && stuckLoc.m_calls == 0 && normalLoc.m_calls == 0;

    // unfinished lunge: the unstuck action with no target, code 0x96 and an empty sound pair
    int creates = g_createCalls;
    lunge.PCreature.m_pItem = &king;
    lunge.FramesRunning = 29;
    lunge.FinishAction();
    ok = ok && g_lastKind == ACTION_UNSTUCK && g_unstuckOwner == &king && g_unstuckNoTarget && g_unstuckCode == 0x96;
    ok = ok && g_soundFirst == 0 && g_soundSecond == 0 && g_createCalls == creates;

    // dead or missing target: unstuck as well
    lunge.FramesRunning = 40;
    hero.m_flags91 = 1;
    g_lastKind = ACTION_NONE;
    lunge.FinishAction();
    ok = ok && g_lastKind == ACTION_UNSTUCK && g_createCalls == creates;
    lunge.PTarget.m_pItem = 0;
    g_lastKind = ACTION_NONE;
    lunge.FinishAction();
    ok = ok && g_lastKind == ACTION_UNSTUCK && g_baseFinish == 8 && g_liveStrings == 0;

    if (!ok) { std::printf("008c8c30_TEST FAIL\n"); return 1; }
    std::printf("008c8c30_TEST PASS\n");
    return 0;
}
