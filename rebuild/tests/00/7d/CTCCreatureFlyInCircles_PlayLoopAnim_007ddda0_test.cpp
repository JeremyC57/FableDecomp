// Behaviour test for 007ddda0: src/007ddda0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCCreatureFlyInCircles::PlayLoopAnim()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureFlyInCircles::PlayLoopAnim() -- point the wind along the creature's flat heading (towards the living hero, else its facing) and start the dragon-flight loop animation for the current fly mode (strafe at half wind, fly loop, or fire-breathing at 1.5 wind early in a swoop), clearing the transition flag once the action is taken
// Leaf: ?PlayLoopAnim@CTCCreatureFlyInCircles@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DDDA0, 496 bytes, plain `ret`.
// Callees: CCharString() 0x0099E4B0 / operator=(const char*) 0x0099EFE0 ("STRAFE_RIGHT" 0x0126FAC8, "FLY_LOOP" 0x0126FAD8,
//   "BREATHE_FIRE_STRAIGHT_LOOP" 0x0126FAE4) / ~CCharString 0x0099EAE0, CTCBase::PeekWorld 0x00686A80 (catalog
//   "CViewBase::GetDisplayEngine"), CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPPlayerCharacter 0x00487DC0,
//   CThing::GetPosition 0x004C73D0, the physics body's facing (vtable +0x118), CTCBase::GetWorld 0x00686A70,
//   CEnvironment::OverrideWindDirection 0x006B2B10, CCreatureAction_DragonFlight ctor 0x008F4CB0 / temporary dtor
//   0x0062E900 (~CCombatActionBase), CThingCreatureBase::SetCurrentAction 0x006644F0.
// Layout: CTCCreatureFlyInCircles Creature +0x0c, Mode +0x34, Transitioning +0x38, SwoopProgress +0x6c; CThing physics
//   body +0x60, dead bit +0x91 bit 0; CWorld environment +0x18; the fast inverse-sqrt mantissa table 0x013CE558.
// Shape: inline Normalise over local copies (the Z = 0 term folds away, Z = r * 0); the SetCurrentAction result is a named
//   bool so it is kept in bl across the temporary's destructor; the world is a named local so GetWorld is called before
//   the OverrideWindDirection argument pushes.
extern unsigned long g_InvSqrtMantissaTable[128];       // 0x013CE558

struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

struct CCharString {
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const char* text);              // 0x0099EFE0
};

struct CPhysicsBody {                                      // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                         // slot 70 (+0x118)
};
struct CCreatureActionBase;
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                             // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char StateBits;                               // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;                  // 0x004C73D0
    bool IsDead() const { return (StateBits & 1) != 0; }
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CCreatureActionBase { };
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _body[0x11c];
    ~CCombatActionBase();                                  // 0x0062E900
};
struct CCreatureAction_DragonFlight : CCombatActionBase {
    CCreatureAction_DragonFlight(CThingCreatureBase* creature, const CCharString& animation);   // 0x008F4CB0
};

struct CPlayer {
    CThing* GetPPlayerCharacter() const;                   // 0x00487DC0
};
struct CEnvironment {
    void OverrideWindDirection(const C3DVector& direction, float speed);   // 0x006B2B10
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* Environment;                             // +0x18
    CPlayer* PeekMyPlayer() const;                         // 0x0049D9D0
};

enum EFlyMode { FLY_MODE_STRAFE, FLY_MODE_LOOP, FLY_MODE_BREATHE_FIRE };

struct CTCCreatureFlyInCircles {
    void*               __vftable;                         // +0x00
    CThing*             ParentThing;                       // +0x04 (CTCBase)
    unsigned char       _pad08[0x0c - 0x08];
    CThingCreatureBase* Creature;                          // +0x0c
    unsigned char       _pad10[0x34 - 0x10];
    EFlyMode            Mode;                              // +0x34
    bool                Transitioning;                     // +0x38
    unsigned char       _pad39[0x6c - 0x39];
    float               SwoopProgress;                     // +0x6c
    CWorld* GetWorld();                                    // 0x00686A70 (CTCBase)
    const CWorld* PeekWorld() const;                       // 0x00686A80 (CTCBase)
    void PlayLoopAnim();
};

void CTCCreatureFlyInCircles::PlayLoopAnim()
{
    CCharString animation;
    float windSpeed = 0.0f;
    C3DVector direction;
    CThing* hero = PeekWorld()->PeekMyPlayer()->GetPPlayerCharacter();
    if (hero && !hero->IsDead())
        direction = hero->GetPosition() - Creature->GetPosition();
    else
        direction = Creature->PhysicsBody->GetFacing();
    direction.Z = 0.0f;
    direction.Normalise();
    switch (Mode) {
    case FLY_MODE_STRAFE:
        animation = "STRAFE_RIGHT";
        windSpeed = 0.5f;
        break;
    case FLY_MODE_LOOP:
        animation = "FLY_LOOP";
        break;
    case FLY_MODE_BREATHE_FIRE:
        animation = "BREATHE_FIRE_STRAIGHT_LOOP";
        if (SwoopProgress < 0.6f)
            windSpeed = 1.5f;
        break;
    }
    CWorld* world = GetWorld();
    world->Environment->OverrideWindDirection(direction, windSpeed);
    bool started = Creature->SetCurrentAction(CCreatureAction_DragonFlight(Creature, animation));
    if (started && Transitioning)
        Transitioning = false;
}

// ---- stubs ----
#undef VF
#define VF(n) void CPhysicsBody::v##n() {}
VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
C3DVector CPhysicsBody::GetFacing() { return C3DVector(0, 0, 0); }
unsigned long g_InvSqrtMantissaTable[128];
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static int g_strings = 0;
CCharString::CCharString() { PStringData = 0; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
CCharString& CCharString::operator=(const char* text) { PStringData = (void*)text; return *this; }

static C3DVector g_facing;
struct CTestBody : CPhysicsBody { virtual C3DVector GetFacing() { Log("GetFacing"); return g_facing; } };
static CThing* g_heroThing = 0;
static C3DVector g_heroPos, g_creaturePos;
const C3DVector& CThing::GetPosition() const { return this == g_heroThing ? g_heroPos : g_creaturePos; }
static int g_setResult = 0; static const CCreatureActionBase* g_action = 0;
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) { Log("SetAction"); g_action = &action; return g_setResult != 0; }
static int g_actions = 0; static const char* g_anim = 0; static CThingCreatureBase* g_actionCreature = 0;
CCreatureAction_DragonFlight::CCreatureAction_DragonFlight(CThingCreatureBase* creature, const CCharString& animation)
{ Log("DragonFlight"); ++g_actions; g_anim = (const char*)animation.PStringData; g_actionCreature = creature; }
CCombatActionBase::~CCombatActionBase() { Log("~Action"); --g_actions; }
static CThing* g_hero = 0;
CThing* CPlayer::GetPPlayerCharacter() const { Log("GetHero"); return g_hero; }
static CPlayer g_player;
CPlayer* CWorld::PeekMyPlayer() const { return &g_player; }
static C3DVector g_wind; static float g_speed = -1;
void CEnvironment::OverrideWindDirection(const C3DVector& direction, float speed) { Log("Wind"); g_wind = direction; g_speed = speed; }
static CEnvironment g_env;
static CWorld g_world;
CWorld* CTCCreatureFlyInCircles::GetWorld() { Log("GetWorld"); return &g_world; }
const CWorld* CTCCreatureFlyInCircles::PeekWorld() const { Log("PeekWorld"); return &g_world; }

static void Reset() { g_log[0] = 0; g_step = 0; g_anim = 0; g_speed = -1; g_wind = C3DVector(9, 9, 9); g_action = 0; }
static int Near(float a, float b) { return fabs(a - b) < 2e-3f; }

int main()
{
    for (int i = 0; i < 128; ++i) {
        double x = ((i >> 6) ? 1.0 : 2.0) * (1.0 + (i & 63) / 64.0 + 1.0 / 128.0);
        long m = (long)floor((2.0 / sqrt(x) - 1.0) * 256.0 + 0.5);
        if (m < 0) m = 0;
        if (m > 255) m = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)m;
    }
    g_world.Environment = &g_env;
    CTestBody body;
    CThingCreatureBase creature; memset(&creature, 0, sizeof creature); creature.PhysicsBody = &body;
    CThing hero; memset(&hero, 0, sizeof hero); g_heroThing = &hero;
    CTCCreatureFlyInCircles fly; memset(&fly, 0xcd, sizeof fly); fly.Creature = &creature;
    g_creaturePos = C3DVector(1, 2, 50); g_heroPos = C3DVector(4, 6, -20); g_facing = C3DVector(0, -2, 7);
    // 1: strafe towards the living hero: flat unit direction (0.6, 0.8, 0), wind 0.5, action taken clears the flag
    {
        Reset(); g_hero = &hero; hero.StateBits = 0; fly.Mode = FLY_MODE_STRAFE; fly.Transitioning = true; g_setResult = 1;
        fly.PlayLoopAnim();
        CHECK(strcmp(g_log, "1:PeekWorld;2:GetHero;3:GetWorld;4:Wind;5:DragonFlight;6:SetAction;7:~Action;") == 0);
        CHECK(Near(g_wind.X, 0.6f) && Near(g_wind.Y, 0.8f) && g_wind.Z == 0.0f && g_speed == 0.5f);
        CHECK(strcmp(g_anim, "STRAFE_RIGHT") == 0 && g_actionCreature == &creature && g_action != 0);
        CHECK(fly.Transitioning == false && g_actions == 0 && g_strings == 0);
    }
    // 2: dead hero: facing used (0, -1, 0), fly loop at no wind; action refused leaves the flag set
    {
        Reset(); hero.StateBits = 1; fly.Mode = FLY_MODE_LOOP; fly.Transitioning = true; g_setResult = 0;
        fly.PlayLoopAnim();
        CHECK(strcmp(g_log, "1:PeekWorld;2:GetHero;3:GetFacing;4:GetWorld;5:Wind;6:DragonFlight;7:SetAction;8:~Action;") == 0);
        CHECK(Near(g_wind.X, 0.0f) && Near(g_wind.Y, -1.0f) && g_wind.Z == 0.0f && g_speed == 0.0f);
        CHECK(strcmp(g_anim, "FLY_LOOP") == 0 && fly.Transitioning == true && g_strings == 0);
    }
    // 3: no hero, breathe fire early in the swoop: wind 1.5
    {
        Reset(); g_hero = 0; fly.Mode = FLY_MODE_BREATHE_FIRE; fly.SwoopProgress = 0.3f; fly.Transitioning = false; g_setResult = 1;
        fly.PlayLoopAnim();
        CHECK(strcmp(g_log, "1:PeekWorld;2:GetHero;3:GetFacing;4:GetWorld;5:Wind;6:DragonFlight;7:SetAction;8:~Action;") == 0);
        CHECK(g_speed == 1.5f && strcmp(g_anim, "BREATHE_FIRE_STRAIGHT_LOOP") == 0 && fly.Transitioning == false);
    }
    // 4: breathe fire late in the swoop: no wind
    {
        Reset(); fly.SwoopProgress = 0.6f;
        fly.PlayLoopAnim();
        CHECK(g_speed == 0.0f && strcmp(g_anim, "BREATHE_FIRE_STRAIGHT_LOOP") == 0);
    }
    // 5: an unknown mode keeps the empty animation name and no wind
    {
        Reset(); fly.Mode = (EFlyMode)7; fly.Transitioning = true; g_setResult = 1;
        fly.PlayLoopAnim();
        CHECK(g_anim == 0 && g_speed == 0.0f && fly.Transitioning == false && g_strings == 0 && g_actions == 0);
    }
    if (fails == 0) printf("007ddda0_TEST PASS\n");
    return fails;
}