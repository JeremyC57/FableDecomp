// Behaviour test for 0x008C2550 CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction: runs the reconstructed body
// against stub creatures, rush counters, battle defs and actions; checks the rush counter bump and its wrap to 1 past
// MaxRushCount, the def reference taken and released (Destroy when the last reference goes), the base FinishAction on both
// paths, no rush when CauseRush is clear or no valid position exists, and the assassin rush: owner = the creature, direction =
// the fast-inverse-sqrt normalised offset to the rush position, distance = its 2D length, no collision, level 0, and a
// "RUSH_OUTOF" looping combat animation as the counted final action (released after SetCurrentAction).
#include <cstdio>
#include <cstring>
#include <cmath>

static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)

static unsigned long g_InvSqrtMantissaTable[128];
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        int e = (i & 64) ? 127 : 128;                 // bit 6 of the index is the exponent's low bit
        double m = 1.0 + ((i & 63) + 0.5) / 64.0;
        double x = std::ldexp(m, e - 127);
        double y = 1.0 / std::sqrt(x);
        int ePrime = (0x17c - e) >> 1;
        double ratio = y / std::ldexp(1.0, ePrime - 127);
        long t = (long)((ratio - 1.0) * 256.0 + 0.5);
        if (t < 0) t = 0;
        if (t > 255) t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)t;
    }
}
static float FastInvSqrt(float lengthSquared)
{
    unsigned int bits;
    std::memcpy(&bits, &lengthSquared, 4);
    unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                            ((unsigned int)g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r;
    std::memcpy(&r, &estimate, 4);
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float Get2DLength() const { return (float)std::sqrt(X * X + Y * Y); }
};
static C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
static C3DVector Normalise(const C3DVector& v) { return v * FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
static C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

static int g_defDestroyed = 0;
struct CJackOfBladesBattleDef {
    long RefCount;
    long MaxRushCount;
    void Destroy() { ++g_defDestroyed; }
};
struct CDefPointer_CJackOfBladesBattleDef {
    CJackOfBladesBattleDef* Data;
    CDefPointer_CJackOfBladesBattleDef() : Data(0) { }
    ~CDefPointer_CJackOfBladesBattleDef()
    {
        if (Data && --Data->RefCount == 0)
            Data->Destroy();
    }
    CJackOfBladesBattleDef* operator->() const { return Data; }
};
struct CTCRushCounter { long RushCount; };

struct CCreatureActionBase;
static CCreatureActionBase* g_current = 0;
struct CThingCreatureBase {
    C3DVector Position;
    CTCRushCounter* Counter;
    CJackOfBladesBattleDef* Def;
    const C3DVector& GetPosition() const { return Position; }
    void GetInterface(CTCRushCounter** out, long type) { if (type == 0xad) *out = Counter; }
    bool GetSubDef(CDefPointer_CJackOfBladesBattleDef* out) const { ++Def->RefCount; out->Data = Def; return true; }
    bool SetCurrentAction(CCreatureActionBase* action);
};

struct CBaseIntelligentPointer {
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const { return m_pItem; }
};

static int g_actionsDestroyed = 0;
struct CCreatureActionBase {
    CBaseIntelligentPointer PCreature;
    virtual ~CCreatureActionBase() { }
    CThingCreatureBase* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCPPointerInfo { long m_count; CCreatureActionBase* m_pItem; };
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* Data;
    CCPPointerInfo*      Info;
    CCountedPointer_CCreatureActionBase(CCreatureActionBase* item) : Data(item), Info(0)
    {
        if (item) { Info = new CCPPointerInfo; Info->m_count = 1; Info->m_pItem = item; }
    }
    CCountedPointer_CCreatureActionBase(const CCountedPointer_CCreatureActionBase& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
    ~CCountedPointer_CCreatureActionBase()
    {
        if (Info) {
            if (--Info->m_count == 0) {
                delete Info->m_pItem;
                ++g_actionsDestroyed;
                delete Info;
            }
        }
    }
};

struct CCreatureAction_PlayCombatAnimation : CCreatureActionBase {
    char Animation[32];
    bool Loop;
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const char* animation, bool loop) : Loop(loop)
    {
        PCreature.m_pItem = creature;
        std::strncpy(Animation, animation, 31);
        Animation[31] = 0;
    }
};

// what SetCurrentAction saw
static int g_rushes = 0;
static CThingCreatureBase* g_rushOwner = 0;
static C3DVector g_rushDir;
static float g_rushDistance = 0.0f;
static bool g_rushCollision = true;
static long g_rushLevel = -1;
static CCreatureAction_PlayCombatAnimation* g_rushFinal = 0;
static long g_finalCountDuring = 0;
static bool g_finalIsRushOut = false;
static bool g_finalLoops = false;
static CThingCreatureBase* g_finalOwner = 0;

struct CCreatureAction_CastAssassinRush : CCreatureActionBase {
    C3DVector Dir;
    float Distance;
    bool UseCollision;
    long Level;
    CCountedPointer_CCreatureActionBase FinalAction;
    CCreatureAction_CastAssassinRush(CThingCreatureBase* owner, const C3DVector& dir, float distance, bool useCollision, long level,
                                     CCountedPointer_CCreatureActionBase finalAction)
        : Dir(dir), Distance(distance), UseCollision(useCollision), Level(level), FinalAction(finalAction)
    {
        PCreature.m_pItem = owner;
    }
};
bool CThingCreatureBase::SetCurrentAction(CCreatureActionBase* action)
{
    CCreatureAction_CastAssassinRush* rush = static_cast<CCreatureAction_CastAssassinRush*>(action);
    ++g_rushes;
    g_rushOwner = rush->PCreature.GetPItem();
    g_rushDir = rush->Dir;
    g_rushDistance = rush->Distance;
    g_rushCollision = rush->UseCollision;
    g_rushLevel = rush->Level;
    g_rushFinal = static_cast<CCreatureAction_PlayCombatAnimation*>(rush->FinalAction.Data);
    g_finalCountDuring = rush->FinalAction.Info ? rush->FinalAction.Info->m_count : 0;
    g_finalIsRushOut = g_rushFinal && std::strcmp(g_rushFinal->Animation, "RUSH_OUTOF") == 0;
    g_finalLoops = g_rushFinal && g_rushFinal->Loop;
    g_finalOwner = g_rushFinal ? g_rushFinal->PCreature.GetPItem() : 0;
    return true;
}

static int g_baseFinish = 0;
struct CCombatAction_GenericStrikeResponseBase : CCreatureActionBase {
    void FinishAction() { ++g_baseFinish; }
};
struct CCombatAction_StrikeResponseJackOfBladesIntoRush : CCombatAction_GenericStrikeResponseBase {
    bool CauseRush;
    bool HasPosition;
    C3DVector Target;
    bool GetValidRushPosition(C3DVector& position) { if (HasPosition) position = Target; return HasPosition; }
    void FinishAction();
};

void CCombatAction_StrikeResponseJackOfBladesIntoRush::FinishAction()
{
    CTCRushCounter* counter;
    PCreature.GetPItem()->GetInterface(&counter, 0xad);
    CDefPointer_CJackOfBladesBattleDef def;
    PCreature.GetPItem()->GetSubDef(&def);
    long count = counter->RushCount + 1;
    if (count > def->MaxRushCount)
        counter->RushCount = 1;
    else
        counter->RushCount = count;
    C3DVector rushPosition;
    if (CauseRush && GetValidRushPosition(rushPosition)) {
        C3DVector delta = rushPosition - PCreature.GetPItem()->GetPosition();
        C3DVector direction = Normalise(delta);
        float distance = delta.Get2DLength();
        CThingCreatureBase* creature = PCreature.GetPItem();
        CCombatAction_GenericStrikeResponseBase::FinishAction();
        CCountedPointer_CCreatureActionBase finalAction(new CCreatureAction_PlayCombatAnimation(creature, "RUSH_OUTOF", true));
        CCreatureAction_CastAssassinRush rush(creature, direction, distance, false, 0, finalAction);
        GetCreature()->SetCurrentAction(&rush);
    } else
        CCombatAction_GenericStrikeResponseBase::FinishAction();
}

static void Reset()
{
    g_rushes = 0; g_rushOwner = 0; g_rushFinal = 0; g_baseFinish = 0; g_defDestroyed = 0; g_actionsDestroyed = 0;
    g_finalCountDuring = 0; g_finalIsRushOut = false; g_finalLoops = false; g_finalOwner = 0; g_rushCollision = true; g_rushLevel = -1; g_rushDistance = 0.0f;
}
static bool Near(float a, float b, float tolerance) { return a - b < tolerance && b - a < tolerance; }

int main()
{
    BuildTable();
    CTCRushCounter counter;
    CJackOfBladesBattleDef def;
    CThingCreatureBase jack;
    jack.Position = C3DVector(1.0f, 2.0f, 3.0f);
    jack.Counter = &counter;
    jack.Def = &def;
    CCombatAction_StrikeResponseJackOfBladesIntoRush action;
    action.PCreature.m_pItem = &jack;

    // no rush flag: counter bumped, base FinishAction, def released
    Reset();
    counter.RushCount = 1; def.MaxRushCount = 3; def.RefCount = 1;
    action.CauseRush = false; action.HasPosition = true; action.Target = C3DVector(4.0f, 6.0f, 3.0f);
    action.FinishAction();
    CHECK(counter.RushCount == 2); CHECK(g_baseFinish == 1); CHECK(g_rushes == 0); CHECK(def.RefCount == 1);
    CHECK(g_defDestroyed == 0);

    // wrap past MaxRushCount; no valid position; last def reference destroys it
    Reset();
    counter.RushCount = 3; def.RefCount = 0;
    action.CauseRush = true; action.HasPosition = false;
    action.FinishAction();
    CHECK(counter.RushCount == 1); CHECK(g_baseFinish == 1); CHECK(g_rushes == 0); CHECK(g_defDestroyed == 1);

    // the rush: offset (3, 4, 12) from Jack -> distance 5, direction (3, 4, 12) / 13
    Reset();
    counter.RushCount = 0; def.RefCount = 1;
    action.CauseRush = true; action.HasPosition = true; action.Target = C3DVector(4.0f, 6.0f, 15.0f);
    action.FinishAction();
    CHECK(counter.RushCount == 1);
    CHECK(g_baseFinish == 1); CHECK(g_rushes == 1); CHECK(g_rushOwner == &jack);
    CHECK(Near(g_rushDistance, 5.0f, 1e-5f));
    CHECK(Near(g_rushDir.X, 3.0f / 13.0f, 2e-3f)); CHECK(Near(g_rushDir.Y, 4.0f / 13.0f, 2e-3f));
    CHECK(Near(g_rushDir.Z, 12.0f / 13.0f, 2e-3f));
    CHECK(!g_rushCollision); CHECK(g_rushLevel == 0);
    CHECK(g_rushFinal != 0); CHECK(g_finalIsRushOut); CHECK(g_finalLoops); CHECK(g_finalOwner == &jack);
    CHECK(g_finalCountDuring == 2);   // the local pointer plus the rush action's copy
    CHECK(g_actionsDestroyed == 1);   // released once both are gone
    CHECK(def.RefCount == 1); CHECK(g_defDestroyed == 0);

    if (g_failures) {
        std::printf("008c2550_TEST FAIL\n");
        return 1;
    }
    std::printf("008c2550_TEST PASS\n");
    return 0;
}
