#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>
// Behaviour test for CCreatureAction_BlockRespond::Initialise @ 0x00855500: with a living target the direction from the
// creature to the blow (the hit position when UseHitPos, else the target's physics position) is normalised and compared
// with the creature's facing: within 50 degrees -> BLOCK_FRONT_RESPONSE, within 50 degrees of the back ->
// BLOCK_BACK_RESPONSE, else LEFT/RIGHT by the sign of facing x direction; MovementDirection becomes the normalised
// opposite direction. The creature's possible combat animations for that name are set when it has any, otherwise the
// action falls back to BLOCK_FRONT_RESPONSE facing the target. The block base Initialise always runs and the final
// position is marked preferred. Same arithmetic as the reconstruction (table-driven inverse square root, one Newton step).
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)
static bool Near(float a, float b) { return fabs(a - b) < 0.002f; }

// 32-bit words on both compilers (unsigned long is 8 bytes on the g++ host)
unsigned int g_InvSqrtMantissaTable[128];
static void BuildInvSqrtTable()
{
    // index = exponent parity (bit 6) and the top six mantissa bits; entry = the eight leading mantissa bits of the
    // inverse square root estimate for that bucket
    for (int i = 0; i < 128; ++i) {
        double m = 1.0 + ((i & 63) + 0.5) / 64.0;
        double target = (i & 64) ? 2.0 / sqrt(m) : sqrt(2.0 / m);
        int t = (int)((target - 1.0) * 256.0 + 0.5);
        if (t < 0)
            t = 0;
        if (t > 255)
            t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned int)t;
    }
}

struct CTCBase;
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned int bits = *(unsigned int*)&lengthSquared;
    unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void operator-=(const C3DVector& o) { X -= o.X; Y -= o.Y; Z -= o.Z; }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    __forceinline C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
    __forceinline void Normalise() { float x = X; float y = Y; float z = Z; *this *= FastInvSqrt(x * x + (y * y + z * z)); }
};

struct CCharString {
    void* m_pData;
    CCharString();
    CCharString(const char* text, long length = -1);
    ~CCharString();
    CCharString& operator=(const char* text);
};
static int g_stringsAlive = 0;
CCharString::CCharString() : m_pData(0) { ++g_stringsAlive; }
CCharString::CCharString(const char* text, long) : m_pData((void*)text) { ++g_stringsAlive; }
CCharString::~CCharString() { --g_stringsAlive; }
CCharString& CCharString::operator=(const char* text) { m_pData = (void*)text; return *this; }
static const char* Text(const CCharString& s) { return s.m_pData ? (const char*)s.m_pData : ""; }

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
static int g_lowerBoundCalls = 0;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    ++g_lowerBoundCalls;
    SEntry* lo = first;
    long n = last - first;
    while (n > 0) {
        long half = n >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            n -= half + 1;
        } else {
            n = half;
        }
    }
    return lo;
}
struct CTCPhysicsBase {
    virtual C3DVector GetFacing() { ++FacingCalls; return Facing; }
    unsigned char _pad04[8];
    C3DVector     Position;
    C3DVector     Facing;
    int           FacingCalls;
};
struct CAnimationEntry;
struct CAnimationEntryVector {
    CAnimationEntry** _M_start;
    CAnimationEntry** _M_finish;
    CAnimationEntry** _M_end_of_storage;
    CAnimationEntryVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CAnimationEntryVector() { if (_M_start) free(_M_start); }
};
struct CThing {
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];
    CInterfaceMap   m_interfaces;
    unsigned char   _pad4c[0x91 - 0x4c];
    unsigned char   m_flags91;
    const C3DVector& GetPosition() const;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
static int g_getPositionCalls = 0;
static C3DVector g_anyPosition(1.0f, 2.0f, 3.0f);
const C3DVector& CThing::GetPosition() const { ++g_getPositionCalls; return g_anyPosition; }

struct CThingCreatureBase : CThing {
    bool GetPossibleCombatAnimations(const CCharString& name, CAnimationEntryVector& out);
};
static int g_possibleCalls = 0;
static const char* g_possibleName = 0;
static bool g_haveAnimations = false;
static void* g_animationsStorage = 0;
bool CThingCreatureBase::GetPossibleCombatAnimations(const CCharString& name, CAnimationEntryVector& out)
{
    ++g_possibleCalls;
    g_possibleName = Text(name);
    if (!g_haveAnimations)
        return false;
    CAnimationEntry** storage = (CAnimationEntry**)malloc(2 * sizeof(CAnimationEntry*));
    storage[0] = 0;
    storage[1] = 0;
    out._M_start = storage;
    out._M_finish = storage + 2;
    out._M_end_of_storage = storage + 2;
    g_animationsStorage = storage;
    return true;
}
struct CBaseIntelligentPointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;
};
CThingCreatureBase* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

class CCombatActionBase {
public:
    virtual void Initialise();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;
    unsigned char           _padb0[0xdd - 0xb0];
    bool                    PreferredFinalPosSet;
    unsigned char           _padde[0x114 - 0xde];
    void SetAnimationName(const CCharString& name);
    void SetCombatAnimations(const CAnimationEntryVector& animations);
};
void CCombatActionBase::Initialise() { }
static int g_setNameCalls = 0;
static const char* g_setName = 0;
void CCombatActionBase::SetAnimationName(const CCharString& name) { ++g_setNameCalls; g_setName = Text(name); }
static int g_setAnimationsCalls = 0;
static void* g_setAnimationsStart = 0;
static long g_setAnimationsCount = 0;
void CCombatActionBase::SetCombatAnimations(const CAnimationEntryVector& animations)
{
    ++g_setAnimationsCalls;
    g_setAnimationsStart = animations._M_start;
    g_setAnimationsCount = (long)(animations._M_finish - animations._M_start);
}
class CCreatureAction_BlockBase : public CCombatActionBase {
public:
    virtual void Initialise();
    long      BlowStrength;
    C3DVector MovementDirection;
};
static int g_baseInitialiseCalls = 0;
void CCreatureAction_BlockBase::Initialise() { ++g_baseInitialiseCalls; }
#pragma pack(push, 1)
class CCreatureAction_BlockRespond : public CCreatureAction_BlockBase {
public:
    bool      UseHitPos;
    C3DVector HitPos;
    bool      FaceTargetAtStart;
    virtual void Initialise();
};
#pragma pack(pop)

void CCreatureAction_BlockRespond::Initialise()
{
    PCreature.GetPItem()->GetPosition();
    CCharString animation;
    if (IS_THING_ALIVE(PTarget)) {
        C3DVector direction;
        if (UseHitPos) {
            direction = HitPos;
        } else {
            CTCPhysicsBase* targetPhysics;
            PTarget.GetPItem()->GetInterface(&targetPhysics, 2);
            direction = targetPhysics->Position;
        }
        CTCPhysicsBase* physics;
        PCreature.GetPItem()->GetInterface(&physics, 2);
        direction -= physics->Position;
        direction *= FastInvSqrt(direction.GetLengthSquared());
        C3DVector facing = physics->GetFacing();
        double limit = cos(0.8726646918931635);
        float dot = direction.X * facing.X + direction.Y * facing.Y + direction.Z * facing.Z;
        if (dot > limit)
            animation = "BLOCK_FRONT_RESPONSE";
        else if (-dot > limit)
            animation = "BLOCK_BACK_RESPONSE";
        else if (facing.X * direction.Y - direction.X * facing.Y > 0.0f)
            animation = "BLOCK_LEFT_RESPONSE";
        else
            animation = "BLOCK_RIGHT_RESPONSE";
        MovementDirection = -direction;
        MovementDirection.Normalise();
    }
    CAnimationEntryVector animations;
    if (PCreature.GetPItem()->GetPossibleCombatAnimations(animation, animations)) {
        SetCombatAnimations(animations);
    } else {
        {
            CCharString name("BLOCK_FRONT_RESPONSE");
            SetAnimationName(name);
        }
        FaceTargetAtStart = true;
    }
    CCreatureAction_BlockBase::Initialise();
    PreferredFinalPosSet = true;
}

static CThingCreatureBase g_creature;
static CThingCreatureBase g_target;
static CTCPhysicsBase g_creaturePhysics;
static CTCPhysicsBase g_targetPhysics;
static CTCPhysicsBase g_otherComponent;
static CInterfaceMap::SEntry g_creatureEntries[3];
static CInterfaceMap::SEntry g_targetEntries[2];

static void Reset(CCreatureAction_BlockRespond& action)
{
    g_stringsAlive = 0;
    g_getPositionCalls = 0;
    g_possibleCalls = 0;
    g_possibleName = 0;
    g_setNameCalls = 0;
    g_setName = 0;
    g_setAnimationsCalls = 0;
    g_setAnimationsStart = 0;
    g_setAnimationsCount = 0;
    g_baseInitialiseCalls = 0;
    g_animationsStorage = 0;
    g_creaturePhysics.FacingCalls = 0;
    g_targetPhysics.FacingCalls = 0;
    g_target.m_flags91 = 0;
    action.PCreature.m_pItem = &g_creature;
    action.PTarget.m_pItem = &g_target;
    action.UseHitPos = false;
    action.FaceTargetAtStart = false;
    action.PreferredFinalPosSet = false;
    action.MovementDirection = C3DVector(9.0f, 9.0f, 9.0f);
}
static bool Is(const char* text, const char* expected) { return text && strcmp(text, expected) == 0; }

int main()
{
    BuildInvSqrtTable();
    g_creatureEntries[0].type = 1; g_creatureEntries[0].component = (CTCBase*)&g_otherComponent;
    g_creatureEntries[1].type = 2; g_creatureEntries[1].component = (CTCBase*)&g_creaturePhysics;
    g_creatureEntries[2].type = 5; g_creatureEntries[2].component = (CTCBase*)&g_otherComponent;
    g_creature.m_presentBits[0] = (1UL << 1) | (1UL << 2) | (1UL << 5);
    g_creature.m_interfaces.first = g_creatureEntries;
    g_creature.m_interfaces.last = g_creatureEntries + 3;
    g_targetEntries[0].type = 2; g_targetEntries[0].component = (CTCBase*)&g_targetPhysics;
    g_targetEntries[1].type = 7; g_targetEntries[1].component = (CTCBase*)&g_otherComponent;
    g_target.m_presentBits[0] = (1UL << 2) | (1UL << 7);
    g_target.m_interfaces.first = g_targetEntries;
    g_target.m_interfaces.last = g_targetEntries + 2;
    g_creaturePhysics.Position = C3DVector(10.0f, 20.0f, 0.0f);
    g_creaturePhysics.Facing = C3DVector(0.0f, 1.0f, 0.0f);
    g_targetPhysics.Facing = C3DVector(1.0f, 0.0f, 0.0f);

    CCreatureAction_BlockRespond action;

    // 1. target straight ahead: front response, pushed straight back; the creature has animations for it
    Reset(action);
    g_targetPhysics.Position = C3DVector(10.0f, 25.0f, 0.0f);
    g_haveAnimations = true;
    action.Initialise();
    CHECK(g_getPositionCalls == 1);
    CHECK(g_creaturePhysics.FacingCalls == 1 && g_targetPhysics.FacingCalls == 0);
    CHECK(g_possibleCalls == 1 && Is(g_possibleName, "BLOCK_FRONT_RESPONSE"));
    CHECK(g_setAnimationsCalls == 1 && g_setAnimationsStart == g_animationsStorage && g_setAnimationsCount == 2);
    CHECK(g_setNameCalls == 0 && !action.FaceTargetAtStart);
    CHECK(Near(action.MovementDirection.X, 0.0f) && Near(action.MovementDirection.Y, -1.0f) && Near(action.MovementDirection.Z, 0.0f));
    CHECK(g_baseInitialiseCalls == 1 && action.PreferredFinalPosSet);
    CHECK(g_stringsAlive == 0);

    // 2. target behind: back response, pushed forward; no animations -> front response facing the target
    Reset(action);
    g_targetPhysics.Position = C3DVector(10.0f, 15.0f, 0.0f);
    g_haveAnimations = false;
    action.Initialise();
    CHECK(g_possibleCalls == 1 && Is(g_possibleName, "BLOCK_BACK_RESPONSE"));
    CHECK(g_setAnimationsCalls == 0);
    CHECK(g_setNameCalls == 1 && Is(g_setName, "BLOCK_FRONT_RESPONSE") && action.FaceTargetAtStart);
    CHECK(Near(action.MovementDirection.X, 0.0f) && Near(action.MovementDirection.Y, 1.0f));
    CHECK(g_baseInitialiseCalls == 1 && action.PreferredFinalPosSet);
    CHECK(g_stringsAlive == 0);

    // 3. target to the left (facing +Y, blow from -X): left response, pushed towards +X
    Reset(action);
    g_targetPhysics.Position = C3DVector(7.0f, 20.0f, 0.0f);
    g_haveAnimations = true;
    action.Initialise();
    CHECK(Is(g_possibleName, "BLOCK_LEFT_RESPONSE"));
    CHECK(Near(action.MovementDirection.X, 1.0f) && Near(action.MovementDirection.Y, 0.0f));

    // 4. target to the right: right response
    Reset(action);
    g_targetPhysics.Position = C3DVector(13.0f, 20.0f, 0.0f);
    action.Initialise();
    CHECK(Is(g_possibleName, "BLOCK_RIGHT_RESPONSE"));
    CHECK(Near(action.MovementDirection.X, -1.0f) && Near(action.MovementDirection.Y, 0.0f));

    // 5. 45 degrees off the facing is still the front; 60 degrees is a side (right of +Y for +X)
    Reset(action);
    g_targetPhysics.Position = C3DVector(14.0f, 24.0f, 0.0f);
    action.Initialise();
    CHECK(Is(g_possibleName, "BLOCK_FRONT_RESPONSE"));
    CHECK(Near(action.MovementDirection.X, -0.70710677f) && Near(action.MovementDirection.Y, -0.70710677f));
    Reset(action);
    g_targetPhysics.Position = C3DVector(10.0f + 8.660254f, 25.0f, 0.0f);
    action.Initialise();
    CHECK(Is(g_possibleName, "BLOCK_RIGHT_RESPONSE"));
    CHECK(Near(action.MovementDirection.X, -0.8660254f) && Near(action.MovementDirection.Y, -0.5f));

    // 6. the hit position overrides the target position (target is behind, the hit is ahead-right): front response
    Reset(action);
    g_targetPhysics.Position = C3DVector(10.0f, 0.0f, 0.0f);
    action.UseHitPos = true;
    action.HitPos = C3DVector(13.0f, 24.0f, 0.0f);
    action.Initialise();
    CHECK(Is(g_possibleName, "BLOCK_FRONT_RESPONSE"));
    CHECK(Near(action.MovementDirection.X, -0.6f) && Near(action.MovementDirection.Y, -0.8f) && Near(action.MovementDirection.Z, 0.0f));

    // 7. a 3D blow: facing +X, hit along (1,1,1) -> 54.7 degrees off, left of the facing; pushed along -(1,1,1)/sqrt(3)
    Reset(action);
    g_creaturePhysics.Facing = C3DVector(1.0f, 0.0f, 0.0f);
    action.UseHitPos = true;
    action.HitPos = C3DVector(12.0f, 22.0f, 2.0f);
    action.Initialise();
    CHECK(Is(g_possibleName, "BLOCK_LEFT_RESPONSE"));
    CHECK(Near(action.MovementDirection.X, -0.57735026f) && Near(action.MovementDirection.Y, -0.57735026f) &&
          Near(action.MovementDirection.Z, -0.57735026f));
    g_creaturePhysics.Facing = C3DVector(0.0f, 1.0f, 0.0f);

    // 8. dead target: no response name, direction untouched, fallback to the front response
    Reset(action);
    g_target.m_flags91 = 1;
    g_haveAnimations = false;
    int lowerBoundBefore = g_lowerBoundCalls;
    action.Initialise();
    CHECK(g_lowerBoundCalls == lowerBoundBefore);
    CHECK(g_creaturePhysics.FacingCalls == 0);
    CHECK(g_possibleCalls == 1 && Is(g_possibleName, ""));
    CHECK(action.MovementDirection.X == 9.0f && action.MovementDirection.Y == 9.0f && action.MovementDirection.Z == 9.0f);
    CHECK(g_setNameCalls == 1 && Is(g_setName, "BLOCK_FRONT_RESPONSE") && action.FaceTargetAtStart);
    CHECK(g_baseInitialiseCalls == 1 && action.PreferredFinalPosSet);
    CHECK(g_stringsAlive == 0);

    // 9. no target at all: same as dead, but with animations available for the empty name
    Reset(action);
    action.PTarget.m_pItem = 0;
    g_haveAnimations = true;
    action.Initialise();
    CHECK(g_possibleCalls == 1 && Is(g_possibleName, ""));
    CHECK(g_setAnimationsCalls == 1 && g_setNameCalls == 0 && !action.FaceTargetAtStart);
    CHECK(action.MovementDirection.Y == 9.0f);
    CHECK(g_baseInitialiseCalls == 1 && action.PreferredFinalPosSet);

    if (g_failures == 0)
        printf("00855500_TEST PASS\n");
    return g_failures;
}