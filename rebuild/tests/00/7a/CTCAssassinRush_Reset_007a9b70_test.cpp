// Behaviour test for CTCAssassinRush::Reset (0x007A9B70).
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
struct CSpecialAbilitiesAssassinRushDef { float TrailDelaySecs; float RushDelay; float* DashDistance; };
template <class T> struct CDefPointer { T* Object; T* operator->() const { return Object; } };
class CTCAssassinRush {
public:
    CThing* ParentThing;
    CDefPointer<const CSpecialAbilitiesAssassinRushDef> PDef;
    CIntelligentPointer<CThing> AssassinTrail, AssassinDisplacementTrail, CollidedThing;
    long TrailDelay; bool StartTrailDelay; C3DVector PositionStartedFrom; bool StartedMoving, JustReleased, DirectionOverride;
    float DashDistance; long SpellLevel; bool IsZTargetting; long RushDelayFrameCount;
    CTCAssassinRush() : AssassinTrail(0), AssassinDisplacementTrail(0), CollidedThing(0) {}
    void Reset()
    {
        StartTrailDelay = false;
        PositionStartedFrom = ParentThing->GetPosition();
        CollidedThing = CIntelligentPointer<CThing>(0);
        StartedMoving = false;
        JustReleased = true;
        DirectionOverride = false;
        IsZTargetting = false;
        TrailDelay = (long)(g_framesPerSecond * PDef->TrailDelaySecs);
        RushDelayFrameCount = (long)(g_framesPerSecond * PDef->RushDelay);
        DashDistance = PDef->DashDistance[SpellLevel];
        if (IS_THING_ALIVE(AssassinTrail))
            AssassinTrail.GetPItem()->Kill(true);
        if (IS_THING_ALIVE(AssassinDisplacementTrail))
            AssassinDisplacementTrail.GetPItem()->Kill(true);
    }
};

int main()
{
    int ok = 1;
    float dash[3] = { 5.0f, 7.5f, 10.0f };
    CSpecialAbilitiesAssassinRushDef def; def.TrailDelaySecs = 0.5f; def.RushDelay = 2.0f; def.DashDistance = dash;
    CThing hero; hero.m_stateFlags = 0; hero.pos.X = 1.0f; hero.pos.Y = 2.0f; hero.pos.Z = 3.0f;
    CThing trail; trail.m_stateFlags = 0; CThing deadTrail; deadTrail.m_stateFlags = 1; CThing victim; victim.m_stateFlags = 0;
    CTCAssassinRush r; r.ParentThing = &hero; r.PDef.Object = &def; r.SpellLevel = 1;
    r.AssassinTrail.SetPItem(&trail); r.AssassinDisplacementTrail.SetPItem(&deadTrail); r.CollidedThing.SetPItem(&victim);
    r.StartTrailDelay = true; r.StartedMoving = true; r.JustReleased = false; r.DirectionOverride = true; r.IsZTargetting = true;
    r.Reset();
    if (r.StartTrailDelay || r.StartedMoving || !r.JustReleased || r.DirectionOverride || r.IsZTargetting) ok = 0;
    if (r.PositionStartedFrom.Z != 3.0f || r.CollidedThing.GetPItem() != 0) ok = 0;
    if (r.TrailDelay != 15 || r.RushDelayFrameCount != 60 || r.DashDistance != 7.5f) ok = 0;
    if (g_kills != 1 || !(trail.m_stateFlags & 1)) ok = 0;
    if (!ok) { std::printf("007a9b70_TEST FAIL\n"); return 1; }
    std::printf("007a9b70_TEST PASS\n");
    return 0;
}