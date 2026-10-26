#include <cstdio>
#include <cstring>
// Behaviour test for NThingShot::CTCShotDefinitionBase::Update (0x007C21A0): the body of src/ over stand-ins -- the
// passed-target latch, the null-pointer strip of the hit list, the collision early-out, the sub-shot timer (with kill) and
// the post-update steps.
static char g_log[256];
static void logc(const char* s) { strcat(g_log, s); strcat(g_log, ","); }
class CWorld { public:
    static long s_frame;
    static long GetWorldFrame() { return s_frame; }
};
long CWorld::s_frame = 0;
class CTCPhysicsBase { public:
    bool moving;
    void SetAsMoving(bool m) { moving = m; logc("moving"); }
};
class CThing { public:
    CTCPhysicsBase* PhysicsTC;
    bool killed;
    void Kill(bool removeNow) { killed = removeNow; logc("kill"); }
};
struct CPtr {
    const CThing* PItem;
    CPtr(const CThing* item) : PItem(item) {}
    bool operator==(const CPtr& o) const { return PItem == o.PItem; }
};
static CPtr* remove(CPtr* first, CPtr* last, const CPtr& value)
{
    CPtr* out = first;
    for (; first != last; ++first)
        if (!(*first == value)) *out++ = *first;
    return out;
}
struct CPtrVector {
    CPtr* _M_start;
    CPtr* _M_finish;
    CPtr* begin() { return _M_start; }
    CPtr* end() { return _M_finish; }
    CPtr* erase(CPtr* first, CPtr* last) { _M_finish = first + (_M_finish - last); return first; }
};
class CThingShotDef { public: long NumSubShots; float SubShotDelay; bool KillOnSubShots; };
static long g_framesPerSecond = 30;
struct CTCShotDefinitionBase {
    CThing* ParentThing;
    CPtrVector HitThingsList;
    CThingShotDef* PShotDef;
    long StartFrame;
    bool SubShotsTriggered;
    bool PassedTarget;
    bool passedNow;
    bool collideNow;
    CWorld world;
    void UpdateMovement() { logc("move"); }
    void UpdateOrientation() { logc("orient"); }
    void CheckForCollisions(bool* collided) { *collided = collideNow; logc("check"); }
    void PostUpdate() { logc("post"); }
    void UpdateSound() { logc("sound"); }
    void TriggerSubShots() { logc("sub"); }
    void OnPassedTarget() { logc("passed"); }
    CWorld* GetWorld() { return &world; }
    bool HasPassedTarget() { return passedNow; }
    void OnCollisionProper(bool* collided) { logc(*collided ? "proper1" : "proper0"); }
    void Update();
};

void CTCShotDefinitionBase::Update()
{
    if (!PassedTarget && HasPassedTarget()) {
        OnPassedTarget();
        PassedTarget = true;
    }
    UpdateMovement();
    UpdateOrientation();
    {
        HitThingsList.erase(remove(HitThingsList.begin(), HitThingsList.end(), CPtr(0)), HitThingsList.end());
    }
    {
        bool collided;
        CheckForCollisions(&collided);
        OnCollisionProper(&collided);
        if (collided)
            return;
    }
    long frame = GetWorld()->GetWorldFrame();
    if (PShotDef->NumSubShots > 0 && !SubShotsTriggered
        && (float)(frame - StartFrame) / g_framesPerSecond >= PShotDef->SubShotDelay) {
        TriggerSubShots();
        SubShotsTriggered = true;
        if (PShotDef->KillOnSubShots)
            ParentThing->Kill(true);
    }
    PostUpdate();
    UpdateSound();
    ParentThing->PhysicsTC->SetAsMoving(true);
}

static int fail(int line) { printf("007c21a0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CTCPhysicsBase phys = { false };
    CThing owner = { &phys, false };
    CThing a = { 0, false }, b = { 0, false };
    CPtr items[4] = { CPtr(&a), CPtr(0), CPtr(&b), CPtr(0) };
    CThingShotDef def = { 2, 1.0f, true };
    CTCShotDefinitionBase shot;
    shot.ParentThing = &owner;
    shot.HitThingsList._M_start = items;
    shot.HitThingsList._M_finish = items + 4;
    shot.PShotDef = &def;
    shot.StartFrame = 100;
    shot.SubShotsTriggered = false;
    shot.PassedTarget = false;
    shot.passedNow = true;
    shot.collideNow = true;
    // collision: early return after the strip, no post steps
    g_log[0] = 0;
    shot.Update();
    CHECK(strcmp(g_log, "passed,move,orient,check,proper1,") == 0);
    CHECK(shot.PassedTarget && shot.HitThingsList.end() - shot.HitThingsList.begin() == 2);
    CHECK(items[0].PItem == &a && items[1].PItem == &b);
    // no collision, delay not reached (frame 115 => 0.5 s < 1 s)
    shot.collideNow = false;
    CWorld::s_frame = 115;
    g_log[0] = 0;
    shot.Update();
    CHECK(strcmp(g_log, "move,orient,check,proper0,post,sound,moving,") == 0);
    CHECK(!shot.SubShotsTriggered && phys.moving);
    // delay reached: sub-shots fire once and the shot is killed
    CWorld::s_frame = 130;
    g_log[0] = 0;
    shot.Update();
    CHECK(strcmp(g_log, "move,orient,check,proper0,sub,kill,post,sound,moving,") == 0);
    CHECK(shot.SubShotsTriggered && owner.killed);
    g_log[0] = 0;
    shot.Update();
    CHECK(strcmp(g_log, "move,orient,check,proper0,post,sound,moving,") == 0);
    // no sub-shots defined
    def.NumSubShots = 0;
    shot.SubShotsTriggered = false;
    g_log[0] = 0;
    shot.Update();
    CHECK(!shot.SubShotsTriggered);
    printf("007c21a0_TEST PASS\n");
    return 0;
}