// Behaviour test for CTCBattleCharge::Reset (0x007A6F00).
#include <cstdio>

static long g_framesPerSecond = 30;
struct C3DVector { float X, Y, Z; };
static int g_kills = 0;
class CThing {
public:
    unsigned char m_stateFlags;
    C3DVector pos;
    const C3DVector& GetPosition() const { return pos; }
    void Kill(bool) { ++g_kills; m_stateFlags |= 1; }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_stateFlags & 1)) return true; }
    return false;
}
struct CSpecialAbilitiesBattleChargeDef { float* DashDistance; float* RadiusOfEffect; float PoseDelaySecs; float ChargeDelay; };
template <class T> struct CDefPointer { T* Object; T* operator->() const { return Object; } };
class CTCBattleCharge {
public:
    CThing* ParentThing;
    CIntelligentPointer<CThing> PTarget;
    CDefPointer<const CSpecialAbilitiesBattleChargeDef> PDef;
    C3DVector PositionStartedFrom;
    CIntelligentPointer<CThing> CollidedThing;
    bool StartedMoving, JustReleased, DirectionOverride;
    float DashDistance; long SpellLevel; float RadiusOfEffect; long PoseDelay; bool StartPoseDelay; bool IsZTargetting; long ChargeDelayFrameCount;
    CTCBattleCharge() : PTarget(0), CollidedThing(0) {}
    void Reset()
    {
        PositionStartedFrom = ParentThing->GetPosition();
        CollidedThing = CIntelligentPointer<CThing>(0);
        StartedMoving = false;
        JustReleased = true;
        DirectionOverride = false;
        DashDistance = PDef->DashDistance[SpellLevel];
        RadiusOfEffect = PDef->RadiusOfEffect[SpellLevel];
        PoseDelay = (long)(g_framesPerSecond * PDef->PoseDelaySecs);
        ChargeDelayFrameCount = (long)(g_framesPerSecond * PDef->ChargeDelay);
        StartPoseDelay = false;
        PTarget = CIntelligentPointer<CThing>(0);
        IsZTargetting = false;
    }
};

int main()
{
    int ok = 1;
    float dash[2] = { 4.0f, 6.0f }, radius[2] = { 1.5f, 2.5f };
    CSpecialAbilitiesBattleChargeDef def; def.DashDistance = dash; def.RadiusOfEffect = radius; def.PoseDelaySecs = 1.0f; def.ChargeDelay = 0.5f;
    CThing hero; hero.m_stateFlags = 0; hero.pos.X = 4.0f; hero.pos.Y = 5.0f; hero.pos.Z = 6.0f;
    CThing enemy; enemy.m_stateFlags = 0;
    CTCBattleCharge c; c.ParentThing = &hero; c.PDef.Object = &def; c.SpellLevel = 1;
    c.PTarget.SetPItem(&enemy); c.CollidedThing.SetPItem(&enemy);
    c.StartedMoving = true; c.JustReleased = false; c.DirectionOverride = true; c.StartPoseDelay = true; c.IsZTargetting = true;
    c.Reset();
    if (c.PTarget.GetPItem() || c.CollidedThing.GetPItem() || c.StartedMoving || !c.JustReleased || c.DirectionOverride) ok = 0;
    if (c.StartPoseDelay || c.IsZTargetting || c.PositionStartedFrom.Y != 5.0f) ok = 0;
    if (c.DashDistance != 6.0f || c.RadiusOfEffect != 2.5f || c.PoseDelay != 30 || c.ChargeDelayFrameCount != 15) ok = 0;
    if (!ok) { std::printf("007a6f00_TEST FAIL\n"); return 1; }
    std::printf("007a6f00_TEST PASS\n");
    return 0;
}
