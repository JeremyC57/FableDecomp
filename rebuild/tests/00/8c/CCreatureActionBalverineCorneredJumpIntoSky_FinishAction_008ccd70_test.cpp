// Behaviour test for 0x008CCD70 CCreatureAction_BalverineCorneredJumpIntoSky::FinishAction: runs the reconstructed body over
// stub things, components, defs and actions; checks the always-run restore (willable, flying, alpha), the unfinished and
// negative-time early outs, the landing (SetPosition(*EndPos) and a normalised XY facing with Up (0,0,1)), the intangible
// mode's time (battle def or 5.0f) and follow-up action (lunge at a living target, else drop-in), the def release, and the
// particle detach; and the missing-component cases.
#include <cstdio>
#include <cmath>

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
    void Normalise()
    {
        float inv = 1.0f / (float)std::sqrt(X * X + Y * Y + Z * Z);
        X *= inv; Y *= inv; Z *= inv;
    }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
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
    int       m_flyingCalls;
    bool      m_flying;
    int       m_positionCalls;
    C3DVector m_position;
    int       m_rhCalls;
    C3DVector m_up, m_forward;
    CTCPhysicsBase() : m_flyingCalls(0), m_flying(false), m_positionCalls(0), m_rhCalls(0) { }
    virtual ~CTCPhysicsBase() { }
    virtual void SetPosition(const C3DVector& position) { ++m_positionCalls; m_position = position; }
    virtual void SetRHSet(const CRightHandedSet& set) { ++m_rhCalls; m_up = set.Up; m_forward = set.Forward; }
    virtual void SetFlying(bool flying) { ++m_flyingCalls; m_flying = flying; }
};
struct CTCGraphicAppearance {
    int m_alpha;
    CTCGraphicAppearance() : m_alpha(-1) { }
    void SetAlpha(unsigned char alpha) { m_alpha = alpha; }
};
static int g_destroyed = 0;
struct CDefBase {
    long m_count;
    virtual ~CDefBase() { }
    virtual void Destroy() { ++g_destroyed; }
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CBalverineBattleDef : CDefBase {
    float IntangibleTime;
};
template<class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    T* operator->() const { return m_p; }
};
struct CDefClassBase {
    CBalverineBattleDef* m_battleDef;
    bool GetSubDef(CDefPointer<CBalverineBattleDef>& out)
    {
        if (!m_battleDef)
            return false;
        ++m_battleDef->m_count;
        out.m_p = m_battleDef;
        return true;
    }
};
struct CThing {
    unsigned long   m_presentBits[9];
    CInterfaceMap   m_interfaces;
    CTCPhysicsBase* PhysicsTC;
    CTCGraphicAppearance* GraphicAppearanceTC;
    CDefClassBase*  m_pDef;
    unsigned char   m_flags91;
    C3DVector       m_position;
    CThing() : PhysicsTC(0), GraphicAppearanceTC(0), m_pDef(0), m_flags91(0), m_position(0.0f, 0.0f, 0.0f)
    {
        for (int i = 0; i < 9; ++i)
            m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = m_interfaces.entries;
    }
    void AddInterface(long type, CTCBase* component)
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
    CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    CTCGraphicAppearance* GetGraphicAppearance() const { return GraphicAppearanceTC; }
    CDefClassBase* GetDef() const { return m_pDef; }
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
struct CThingCreatureBase : CThing { };
static int     g_willableCalls = 0;
static CThing* g_willableThing = 0;
static bool    g_willableFlag = true;
void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable)
{
    ++g_willableCalls; g_willableThing = thing; g_willableFlag = notWillable;
}

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CCreaturePointer {
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const { return m_pItem; }
    operator CThingCreatureBase*() const { return GetPItem(); }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

enum EKind { KIND_NONE, KIND_DROPIN, KIND_LUNGE };
struct CCreatureActionBase {
    int     m_kind;
    CThing* m_owner;
    CThing* m_target;
    CCreatureActionBase() : m_kind(KIND_NONE), m_owner(0), m_target(0) { }
    virtual ~CCreatureActionBase() { }
};
struct CModeBase {
    virtual ~CModeBase() { }
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    CCountedPointer(T* object) : m_pObject(object) { }
};
struct CModeIntangible : CModeBase {
    CThing*              m_thing;
    float                m_time;
    CCreatureActionBase* m_next;
    CModeIntangible(CThing* thing, float time, CCreatureActionBase* nextAction) : m_thing(thing), m_time(time), m_next(nextAction) { }
};
struct CTCCreatureModeManager : CTCBase {
    int        m_calls;
    CModeBase* m_mode;
    CTCCreatureModeManager() : m_calls(0), m_mode(0) { }
    bool AddMode(CCountedPointer<CModeBase> mode) { ++m_calls; m_mode = mode.m_pObject; return true; }
};
struct CTCAttachedParticles : CTCBase {
    int m_calls;
    CTCAttachedParticles() : m_calls(0) { }
    void DetachParticles() { ++m_calls; }
};

struct CCombatActionBase : CCreatureActionBase {
    CCreaturePointer        PCreature;
    long                    TotalTime;
    long                    FramesRunning;
    CBaseIntelligentPointer PTarget;
    int                     m_baseFinish;
    CCombatActionBase() : TotalTime(0), FramesRunning(0), m_baseFinish(0) { PCreature.m_pItem = 0; PTarget.m_pItem = 0; }
    virtual void FinishAction() { ++m_baseFinish; }
};
struct CCreatureAction_BalverineDropInFromSky : CCombatActionBase {
    CCreatureAction_BalverineDropInFromSky(CThingCreatureBase* owner) { m_kind = KIND_DROPIN; m_owner = owner; }
};
namespace N_008ccc10 { struct CCreatureAction_BalverineLungeAttack : CCreatureAction_BalverineDropInFromSky {
    CCreatureAction_BalverineLungeAttack(CThingCreatureBase* owner, CThing* target)
        : CCreatureAction_BalverineDropInFromSky(owner) { m_kind = KIND_LUNGE; m_target = target; }
}; } using N_008ccc10::CCreatureAction_BalverineLungeAttack;
struct CCreatureAction_BalverineCorneredJumpIntoSky : CCombatActionBase {
    C3DVector* EndPos;
    virtual void FinishAction();
};

void CCreatureAction_BalverineCorneredJumpIntoSky::FinishAction()
{
    CCombatActionBase::FinishAction();
    SetThingAsNotBloodyWillableAtAll(PCreature.GetPItem(), false);
    PCreature.GetPItem()->GetPhysics()->SetFlying(true);
    PCreature.GetPItem()->GetGraphicAppearance()->SetAlpha(0xff);
    if (TotalTime >= 0 && FramesRunning >= TotalTime) {
        if (IS_THING_ALIVE(PTarget)) {
            CTCPhysicsBase* physics = PCreature.GetPItem()->GetPhysics();
            physics->SetPosition(*EndPos);
            CThing* target = PTarget.GetPItem();
            const C3DVector& from = PCreature.GetPItem()->GetPosition();
            const C3DVector& to = target->GetPosition();
            C3DVector forward(to.X - from.X, to.Y - from.Y, 0.0f);
            forward.Normalise();
            physics->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), forward));
        }
        CTCCreatureModeManager* modeManager;
        if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31)) {
            CDefPointer<CBalverineBattleDef> def;
            float time = 5.0f;
            if (PCreature.GetPItem()->GetDef()->GetSubDef(def))
                time = def->IntangibleTime;
            CCreatureActionBase* next;
            if (IS_THING_ALIVE(PTarget))
                next = new CCreatureAction_BalverineLungeAttack(PCreature, PTarget.GetPItem());
            else
                next = new CCreatureAction_BalverineDropInFromSky(PCreature.GetPItem());
            modeManager->AddMode(CCountedPointer<CModeBase>(new CModeIntangible(PCreature.GetPItem(), time, next)));
        }
        CTCAttachedParticles* particles;
        if (PCreature.GetPItem()->GetInterface(&particles, 0x82))
            particles->DetachParticles();
    }
}

static int fail(int line)
{
    std::printf("008ccd70_TEST FAIL (line %d)\n", line);
    return 1;
}
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
static bool near(float a, float b) { return a - b < 0.0001f && b - a < 0.0001f; }

int main()
{
    // 1: unfinished jump -- only the restore runs
    {
        CThingCreatureBase creature; CTCPhysicsBase physics; CTCGraphicAppearance appearance; CTCCreatureModeManager manager;
        CTCAttachedParticles particles;
        creature.PhysicsTC = &physics; creature.GraphicAppearanceTC = &appearance;
        creature.AddInterface(0x31, &manager); creature.AddInterface(0x82, &particles);
        CCreatureAction_BalverineCorneredJumpIntoSky action;
        action.PCreature.m_pItem = &creature; action.TotalTime = 30; action.FramesRunning = 29;
        g_willableCalls = 0; g_willableFlag = true;
        action.FinishAction();
        CHECK(action.m_baseFinish == 1);
        CHECK(g_willableCalls == 1 && g_willableThing == &creature && !g_willableFlag);
        CHECK(physics.m_flyingCalls == 1 && physics.m_flying);
        CHECK(appearance.m_alpha == 255);
        CHECK(physics.m_positionCalls == 0 && manager.m_calls == 0 && particles.m_calls == 0);
        action.TotalTime = -1; action.FramesRunning = 100;
        action.FinishAction();
        CHECK(manager.m_calls == 0 && particles.m_calls == 0 && physics.m_flyingCalls == 2);
    }
    // 2: finished at a living target, battle def present
    {
        CThingCreatureBase creature; CThing target; CTCPhysicsBase physics; CTCGraphicAppearance appearance;
        CTCCreatureModeManager manager; CTCAttachedParticles particles; CDefClassBase defClass; CBalverineBattleDef battleDef;
        battleDef.m_count = 1; battleDef.IntangibleTime = 3.5f; defClass.m_battleDef = &battleDef;
        creature.PhysicsTC = &physics; creature.GraphicAppearanceTC = &appearance; creature.m_pDef = &defClass;
        creature.AddInterface(0x82, &particles); creature.AddInterface(0x31, &manager); creature.AddInterface(9, 0);
        creature.m_position = C3DVector(1.0f, 2.0f, 7.0f);
        target.m_position = C3DVector(4.0f, 6.0f, -3.0f);
        C3DVector endPos(10.0f, 20.0f, 30.0f);
        CCreatureAction_BalverineCorneredJumpIntoSky action;
        action.PCreature.m_pItem = &creature; action.PTarget.m_pItem = &target; action.EndPos = &endPos;
        action.TotalTime = 30; action.FramesRunning = 30;
        g_destroyed = 0;
        action.FinishAction();
        CHECK(physics.m_positionCalls == 1 && physics.m_position.X == 10.0f && physics.m_position.Z == 30.0f);
        CHECK(physics.m_rhCalls == 1 && physics.m_up.X == 0.0f && physics.m_up.Y == 0.0f && physics.m_up.Z == 1.0f);
        CHECK(near(physics.m_forward.X, 0.6f) && near(physics.m_forward.Y, 0.8f) && physics.m_forward.Z == 0.0f);
        CHECK(manager.m_calls == 1);
        CModeIntangible* mode = (CModeIntangible*)manager.m_mode;
        CHECK(mode->m_thing == &creature && mode->m_time == 3.5f);
        CHECK(mode->m_next->m_kind == KIND_LUNGE && mode->m_next->m_owner == &creature && mode->m_next->m_target == &target);
        CHECK(battleDef.m_count == 1 && g_destroyed == 0);
        CHECK(particles.m_calls == 1);
        delete mode->m_next; delete mode;
        // dead target, no battle def, no particles component
        target.m_flags91 = 1; defClass.m_battleDef = 0;
        CThingCreatureBase creature2; CTCCreatureModeManager manager2;
        creature2.PhysicsTC = &physics; creature2.GraphicAppearanceTC = &appearance; creature2.m_pDef = &defClass;
        creature2.AddInterface(0x31, &manager2);
        action.PCreature.m_pItem = &creature2;
        action.FinishAction();
        CHECK(physics.m_positionCalls == 1 && physics.m_rhCalls == 1);
        CHECK(manager2.m_calls == 1);
        mode = (CModeIntangible*)manager2.m_mode;
        CHECK(mode->m_thing == &creature2 && mode->m_time == 5.0f);
        CHECK(mode->m_next->m_kind == KIND_DROPIN && mode->m_next->m_owner == &creature2 && mode->m_next->m_target == 0);
        delete mode->m_next; delete mode;
        // last reference released by the local def pointer
        battleDef.m_count = 0; defClass.m_battleDef = &battleDef;
        action.FinishAction();
        CHECK(g_destroyed == 1 && battleDef.m_count == 0);
        mode = (CModeIntangible*)manager2.m_mode;
        CHECK(mode->m_time == 3.5f);
        delete mode->m_next; delete mode;
    }
    // 3: no mode manager -- no mode, particles still detached; null target skips the landing
    {
        CThingCreatureBase creature; CTCPhysicsBase physics; CTCGraphicAppearance appearance; CTCAttachedParticles particles;
        creature.PhysicsTC = &physics; creature.GraphicAppearanceTC = &appearance;
        creature.AddInterface(0x82, &particles);
        CCreatureAction_BalverineCorneredJumpIntoSky action;
        action.PCreature.m_pItem = &creature; action.TotalTime = 0; action.FramesRunning = 0;
        action.FinishAction();
        CHECK(physics.m_positionCalls == 0 && physics.m_rhCalls == 0 && particles.m_calls == 1);
    }
    std::printf("008ccd70_TEST PASS\n");
    return 0;
}