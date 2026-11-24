#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>

static unsigned failures;
#define CHECK(condition) do { if (!(condition)) { \
    if (failures < 20) std::printf("line %d: %s\n", __LINE__, #condition); ++failures; \
} } while (0)

static bool Same(const C3DVector& a, const C3DVector& b)
{
    return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}
static bool Near(float a, float b)
{
    return std::fabs(a - b) < 0.0001f;
}

static unsigned rotations;
static float observedAngle;
void C3DVector::RotateXY(float angle)
{
    CHECK(Same(*this, C3DVector(0, -60, 60)));
    ++rotations;
    observedAngle = angle;
    const double radians = angle * 6.283185307179586;
    const float oldX = X;
    X = (float)(oldX * std::cos(radians) + Y * std::sin(radians));
    Y = (float)(Y * std::cos(radians) - oldX * std::sin(radians));
}

struct FixtureModeManager;
struct FixtureThing {
    bool dead, physics;
    C3DVector position;
    FixtureModeManager* manager;
    mutable unsigned positionReads, physicsReads, lookups;

    FixtureThing() : dead(false), physics(false), manager(0), positionReads(0), physicsReads(0), lookups(0) {}
    const C3DVector& GetPos() const { ++positionReads; return position; }
    const void* GetPhysicsTC() const { ++physicsReads; return physics ? this : 0; }
    bool GetTC(FixtureModeManager** out)
    {
        ++lookups;
        if (!manager)
            return false;
        *out = manager;
        return true;
    }
};

struct FixturePointer {
    FixtureThing* item;
    FixtureThing* Get() const { return item; }
    FixtureThing* operator->() const { return item; }
};
static bool IS_THING_ALIVE(const FixturePointer& ptr)
{
    return ptr.item && !ptr.item->dead;
}

static bool simulateAllocationFailure;
static unsigned allocations;
namespace NCreatureMode {
class FixtureStrafe {
public:
    FixtureThing& creature;
    C3DVector route[4];
    C3DVector previous, from, to;

    static void* operator new(size_t bytes)
    {
        ++allocations;
        return simulateAllocationFailure ? 0 : std::malloc(bytes);
    }
    static void operator delete(void* p)
    {
        std::free(p);
    }

    FixtureStrafe(FixtureThing& thing, std::vector<C3DVector>& points, const C3DVector& previousPoint,
                  float burnStart, float burnStop, C3DVector& flameStart, C3DVector& flameEnd)
        : creature(thing), previous(previousPoint), from(flameStart), to(flameEnd)
    {
        CHECK(points.size() == 4 && burnStart == 0.15f && burnStop == 0.6f);
        for (unsigned i = 0; i != points.size() && i != 4; ++i)
            route[i] = points[i];
    }
};
}

struct FixtureModeManager {
    unsigned calls, modes;
    C3DVector route[4];
    C3DVector previous, from, to;
    FixtureThing* receiver;

    FixtureModeManager() : calls(0), modes(0), receiver(0) {}
    void AddMode(NCreatureMode::FixtureStrafe* mode)
    {
        ++calls;
        if (mode) {
            ++modes;
            receiver = &mode->creature;
            previous = mode->previous;
            from = mode->from;
            to = mode->to;
            for (unsigned i = 0; i != 4; ++i)
                route[i] = mode->route[i];
            delete mode;
        }
    }
};

struct FixtureDragon {
    FixtureThing* creature;
    FixturePointer PEnemy, PStrafeMarker1, PStrafeMarker2;
    C3DVector PreviousFlightPos, DragonPosIntoStrafeLeft, DragonPosIntoStrafeRight;
    C3DVector DragonPosOutOfStrafeLeft, DragonPosOutOfStrafeRight;
    C3DVector DragonPosStrafeFlameLeft, DragonPosStrafeFlameRight;
    long GoLeft, CurrentState;
    bool stopping;
    unsigned idleCalls, stopCalls, stopQueries, choices;

    FixtureThing& GetCreature() { return *creature; }
    void AIAction_DoNothing() { ++idleCalls; }
    void Terminate() { ++stopCalls; }
    bool IsStopping() { ++stopQueries; return stopping; }
    long ChooseIntoGroundState() { ++choices; return 5; }
    bool IsStillRunnable();
    void UpdateStrafe();
};

#define CAIStateGroup_JackDragon FixtureDragon
#define CThing FixtureThing
#define CTCCreatureModeManager FixtureModeManager
#define CModeDragonStrafeBurninate FixtureStrafe

const float DRAGON_FLY_SPEED = 30.0f;

// Original same-TU body in FableWin 0x02BD9900 and retail 0x0094FDF0.
inline bool CAIStateGroup_JackDragon::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PEnemy))
        return false;
    if (IsStopping())
        return false;
    return true;
}

void CAIStateGroup_JackDragon::UpdateStrafe()
{
    C3DVector offset;
    if (IS_THING_ALIVE(PStrafeMarker1) && IS_THING_ALIVE(PStrafeMarker2) &&
            PStrafeMarker1->GetPhysicsTC() && PStrafeMarker2->GetPhysicsTC()) {
        CThing* pmarker1;
        CThing* pmarker2;
        if (GoLeft) {
            pmarker1 = PStrafeMarker2.Get();
            pmarker2 = PStrafeMarker1.Get();
        }
        else {
            pmarker1 = PStrafeMarker1.Get();
            pmarker2 = PStrafeMarker2.Get();
        }
        C3DVector strafe_vector = pmarker2->GetPos() - pmarker1->GetPos();
        C3DVector target_pos_offset(0.0f, -60.0f, 60.0f);
        target_pos_offset.RotateXY(strafe_vector.GetAngleXY());
        C3DVector start_pos = pmarker1->GetPos() + target_pos_offset;
        C3DVector flame_start_pos;
        C3DVector flame_stop_pos;
        float fly_speed = DRAGON_FLY_SPEED;
        std::vector<C3DVector> movement_points;
        movement_points.push_back(GetCreature().GetPos());
        if (GoLeft) {
            movement_points.push_back(DragonPosIntoStrafeLeft);
            movement_points.push_back(DragonPosIntoStrafeRight);
            movement_points.push_back(DragonPosOutOfStrafeRight);
            flame_start_pos = DragonPosStrafeFlameLeft;
            flame_stop_pos = DragonPosStrafeFlameRight;
        }
        else {
            movement_points.push_back(DragonPosIntoStrafeRight);
            movement_points.push_back(DragonPosIntoStrafeLeft);
            movement_points.push_back(DragonPosOutOfStrafeLeft);
            flame_start_pos = DragonPosStrafeFlameRight;
            flame_stop_pos = DragonPosStrafeFlameLeft;
        }
        CTCCreatureModeManager* ptc_cmm;
        if (GetCreature().GetTC(&ptc_cmm)) {
            ptc_cmm->AddMode(new NCreatureMode::CModeDragonStrafeBurninate(GetCreature(), movement_points,
                PreviousFlightPos, 0.15f, 0.6f, flame_start_pos, flame_stop_pos));
            AIAction_DoNothing();
        }
    }
    if (!IsStillRunnable()) {
        Terminate();
    }
    else {
        if (GoLeft) {
            PreviousFlightPos = DragonPosIntoStrafeRight;
            GoLeft = 0;
        }
        else {
            PreviousFlightPos = DragonPosIntoStrafeLeft;
            GoLeft = 1;
        }
        CurrentState = ChooseIntoGroundState();
    }
}

#undef CAIStateGroup_JackDragon
#undef CThing
#undef CTCCreatureModeManager
#undef CModeDragonStrafeBurninate

static FixtureThing* Marker(FixtureThing& thing, int condition)
{
    thing.dead = condition == 1;
    thing.physics = condition == 3;
    return condition ? &thing : 0;
}

int main()
{
    for (unsigned scenario = 0; scenario != 768; ++scenario) {
        unsigned digits = scenario;
        const int m1 = digits % 4; digits /= 4;
        const int m2 = digits % 4; digits /= 4;
        const int enemyState = digits % 3; digits /= 3;
        const bool left = (digits & 1) != 0;
        const bool stopping = (digits & 2) != 0;
        const bool hasMode = (digits & 4) != 0;
        simulateAllocationFailure = (digits & 8) != 0;
        rotations = allocations = 0;

        FixtureThing marker1, marker2, creature, enemy;
        FixtureModeManager manager;
        FixtureDragon dragon;
        marker1.position = C3DVector(1, 2, 3);
        marker2.position = C3DVector(5, -7, 9);
        creature.position = C3DVector(9, 10, 11);
        creature.manager = hasMode ? &manager : 0;
        dragon.creature = &creature;
        dragon.PStrafeMarker1.item = Marker(marker1, m1);
        dragon.PStrafeMarker2.item = Marker(marker2, m2);
        dragon.PEnemy.item = enemyState ? &enemy : 0;
        enemy.dead = enemyState == 2;
        dragon.stopping = stopping;
        dragon.GoLeft = left ? 1 : 0;
        dragon.CurrentState = 4;
        dragon.idleCalls = dragon.stopCalls = dragon.stopQueries = dragon.choices = 0;
        dragon.PreviousFlightPos = C3DVector(-9, -8, -7);
        dragon.DragonPosIntoStrafeLeft = C3DVector(1, 11, 111);
        dragon.DragonPosIntoStrafeRight = C3DVector(2, 22, 222);
        dragon.DragonPosOutOfStrafeLeft = C3DVector(3, 33, 333);
        dragon.DragonPosOutOfStrafeRight = C3DVector(4, 44, 444);
        dragon.DragonPosStrafeFlameLeft = C3DVector(5, 55, 555);
        dragon.DragonPosStrafeFlameRight = C3DVector(6, 66, 666);

        dragon.UpdateStrafe();

        const bool ready = m1 == 3 && m2 == 3;
        const bool modeAdded = ready && hasMode;
        const bool modeCreated = modeAdded && !simulateAllocationFailure;
        const bool runnable = enemyState == 1 && !stopping;
        CHECK(marker1.physicsReads == (m1 >= 2 && m2 >= 2 ? 1U : 0U));
        CHECK(marker2.physicsReads == (m1 == 3 && m2 >= 2 ? 1U : 0U));
        CHECK(creature.lookups == (ready ? 1U : 0U));
        CHECK(rotations == (ready ? 1U : 0U));
        CHECK(allocations == (modeAdded ? 1U : 0U));
        CHECK(manager.calls == allocations && manager.modes == (modeCreated ? 1U : 0U));
        CHECK(dragon.idleCalls == manager.calls);
        CHECK(dragon.stopQueries == (enemyState == 1 ? 1U : 0U));
        CHECK(dragon.stopCalls == (runnable ? 0U : 1U) && dragon.choices == (runnable ? 1U : 0U));
        CHECK(dragon.GoLeft == (runnable ? !left : left) && dragon.CurrentState == (runnable ? 5 : 4));
        CHECK(Same(dragon.PreviousFlightPos, runnable ?
            (left ? dragon.DragonPosIntoStrafeRight : dragon.DragonPosIntoStrafeLeft) : C3DVector(-9,-8,-7)));

        if (ready) {
            const float sign = left ? -1.0f : 1.0f;
            float angle = (float)(std::atan2(sign * 4.0, sign * -9.0) / 6.283185307179586);
            if (angle < 0.0f)
                angle += 1.0f;
            CHECK(Near(observedAngle, angle));
            CHECK(marker1.positionReads == (left ? 1U : 2U) && marker2.positionReads == (left ? 2U : 1U));
        }
        if (modeCreated) {
            CHECK(manager.receiver == &creature && Same(manager.route[0], creature.position));
            CHECK(Same(manager.route[1], left ? dragon.DragonPosIntoStrafeLeft : dragon.DragonPosIntoStrafeRight));
            CHECK(Same(manager.route[2], left ? dragon.DragonPosIntoStrafeRight : dragon.DragonPosIntoStrafeLeft));
            CHECK(Same(manager.route[3], left ? dragon.DragonPosOutOfStrafeRight : dragon.DragonPosOutOfStrafeLeft));
            CHECK(Same(manager.previous, C3DVector(-9,-8,-7)));
            CHECK(Same(manager.from, left ? dragon.DragonPosStrafeFlameLeft : dragon.DragonPosStrafeFlameRight));
            CHECK(Same(manager.to, left ? dragon.DragonPosStrafeFlameRight : dragon.DragonPosStrafeFlameLeft));
        }
    }
    std::puts(failures ? "00950cf0_TEST FAIL" : "00950cf0_TEST PASS");
    return failures ? 1 : 0;
}