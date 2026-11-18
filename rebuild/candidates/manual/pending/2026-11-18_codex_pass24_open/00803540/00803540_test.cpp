#include <cassert>
#include <cmath>
#include <cstdio>

const float DELTA_NOTIONAL_ZERO = 0.0001f;
const double TWO_PI = 6.283185307179586;

struct C2DVector
{
    float X, Y;
    C2DVector(float x=0, float y=0): X(x), Y(y) {}
    C2DVector& operator+=(const C2DVector& other)
    { X += other.X; Y += other.Y; return *this; }
};

struct C3DVector
{
    float X, Y, Z;
    C3DVector(float x=0, float y=0, float z=0): X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X-v.X,Y-v.Y,Z-v.Z); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X+v.X,Y+v.Y,Z+v.Z); }
    C3DVector operator*(float scale) const { return C3DVector(X*scale,Y*scale,Z*scale); }
    C3DVector operator^(const C3DVector& v) const
    { return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X); }
    C3DVector& operator+=(const C3DVector& other) { *this = *this+other; return *this; }
    C3DVector& operator/=(float scale) { X /= scale; Y /= scale; Z /= scale; return *this; }
    float GetMagnitude() const { return static_cast<float>(std::sqrt(X*X+Y*Y+Z*Z)); }
    void AccurateNormalise() { *this /= GetMagnitude(); }
};

struct CTCHeroRangedAttacks
{
    unsigned BuildReads, AccuracyReads;
    CTCHeroRangedAttacks(): BuildReads(0), AccuracyReads(0) {}
    float GetPullBackAmount() { ++BuildReads; return 2.0f; }
    float GetShotAccuracyFraction() { ++AccuracyReads; return 0.8f; }
};

struct CTCHeroExperience
{
    float Weight;
};

struct CHeroAbilityStatWeight
{
    mutable unsigned Queries;
    CHeroAbilityStatWeight(): Queries(0) {}
    float GetParameter(const CTCHeroExperience& experience) const
    { ++Queries; return experience.Weight; }
};

struct CHeroCombatDef
{
    struct CFloatRange { float LowValue, HighValue; };
    CFloatRange RangedAccuracyShakeyCursor;
    CHeroAbilityStatWeight RangedAccuracyShakeyCursorWeighting;
    float RangedAccuracyShakeFractionOfRadius;
    CHeroCombatDef(): RangedAccuracyShakeFractionOfRadius(0.2f)
    {
        RangedAccuracyShakeyCursor.LowValue = 0.5f;
        RangedAccuracyShakeyCursor.HighValue = 1.5f;
    }
};

struct CDefPointer
{
    const CHeroCombatDef* Ptr;
    bool IsNull() const { return !Ptr; }
    const CHeroCombatDef& operator*() const { return *Ptr; }
};

struct CThingCreatureBase
{
    bool HasRanged, Creature, HasExperience, HasCombatDef;
    unsigned ExperienceChecks, DefinitionReads;
    CTCHeroRangedAttacks Ranged;
    CTCHeroExperience Experience;
    CHeroCombatDef Definition;
    CThingCreatureBase(): HasRanged(false), Creature(false), HasExperience(false),
        HasCombatDef(false), ExperienceChecks(0), DefinitionReads(0)
    { Experience.Weight = 0.25f; }
    bool GetTC(CTCHeroRangedAttacks** out)
    { *out = HasRanged ? &Ranged : 0; return HasRanged; }
    bool PeekTC(const CTCHeroExperience** out)
    {
        ++ExperienceChecks;
        *out = HasExperience ? &Experience : 0;
        return HasExperience;
    }
    unsigned GetTypeFlags() const { return Creature ? 8 : 0; }
    CDefPointer GetPHeroCombatDef()
    {
        ++DefinitionReads;
        CDefPointer result = { HasCombatDef ? &Definition : 0 };
        return result;
    }
};

struct CCamera
{
    C3DVector Position;
    const C3DVector& PeekWorldPos() const { return Position; }
};

struct CGameCameraManager
{
    CCamera Camera;
    const CCamera& GetCamera() const { return Camera; }
};

struct CAIGameCameraBase
{
    CGameCameraManager Manager;
    unsigned Reads;
    CAIGameCameraBase(): Reads(0) {}
    void GetCameraManager(const CGameCameraManager** out)
    { ++Reads; *out = &Manager; }
};

struct CWorld
{
    CAIGameCameraBase GameCamera;
    unsigned SeedReads;
    unsigned long Seed;
    CWorld(): SeedReads(0), Seed(73) {}
    CAIGameCameraBase* PeekGameCamera() { return &GameCamera; }
    unsigned long* GetWorldSeed() { ++SeedReads; return &Seed; }
};

static unsigned long* expected_seed;
static unsigned random_calls;
static float random_angle, expected_radius;

float GFFloatRandom(float range, unsigned long* seed)
{
    assert(seed == expected_seed);
    assert(*seed == 73 + random_calls);
    ++*seed;
    ++random_calls;
    if (random_calls == 1) {
        assert(range == 1.0f);
        return random_angle;
    }
    assert(random_calls == 2 && std::fabs(range-expected_radius) < 0.00001f);
    return range*0.5f;
}

struct CTCTargetingPlayer
{
    CThingCreatureBase Parent;
    CWorld World;
    C3DVector TargetedPos, TargetedPosOffset3D;
    C2DVector TargetedPosOffsetSecondOrder, TargetedPosOffset, DeltaTargetedPosOffset;
    CThingCreatureBase& GetParentThing() { return Parent; }
    CWorld& GetWorld() { return World; }
    void UpdateTargetedPositionRandomWalk();
};
void CTCTargetingPlayer::UpdateTargetedPositionRandomWalk()
{
    CTCHeroRangedAttacks* ptc_ranged;
    if (GetParentThing().GetTC(&ptc_ranged)) {
        float build_up = ptc_ranged->GetPullBackAmount();
        float accuracy = ptc_ranged->GetShotAccuracyFraction();
        float shake_radius = build_up;
        const CGameCameraManager* pcam_man;
        GetWorld().PeekGameCamera()->GetCameraManager(&pcam_man);
        C3DVector line_to_target_pos = TargetedPos - pcam_man->GetCamera().PeekWorldPos();
        float dist_to_target = line_to_target_pos.GetMagnitude();
        if (!(dist_to_target < DELTA_NOTIONAL_ZERO)) {
            line_to_target_pos /= dist_to_target;
            float main_radius = dist_to_target * (1.0f / 7.0f);
            float second_order_radius = main_radius * 0.1f;
            const CTCHeroExperience* ptc_exp;
            if ((GetParentThing().GetTypeFlags() & 8) && GetParentThing().PeekTC(&ptc_exp)) {
                CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(GetParentThing());
                if (!creature.GetPHeroCombatDef().IsNull()) {
                    const CHeroCombatDef& hero_combat_def = *creature.GetPHeroCombatDef();
                    float weight = hero_combat_def.RangedAccuracyShakeyCursorWeighting.GetParameter(*ptc_exp);
                    shake_radius = ((hero_combat_def.RangedAccuracyShakeyCursor.HighValue - hero_combat_def.RangedAccuracyShakeyCursor.LowValue) * weight
                                    + hero_combat_def.RangedAccuracyShakeyCursor.LowValue) * build_up;
                    second_order_radius = main_radius * hero_combat_def.RangedAccuracyShakeFractionOfRadius;
                }
            }
            C3DVector x_axis(0.0f, 0.0f, 0.0f);
            if (fabs(line_to_target_pos.X) > fabs(line_to_target_pos.Y)) {
                x_axis.Y = 1.0f;
                x_axis.X = line_to_target_pos.Y / line_to_target_pos.X * -1.0f;
            } else {
                x_axis.X = 1.0f;
                x_axis.Y = line_to_target_pos.X / line_to_target_pos.Y * -1.0f;
            }
            x_axis.AccurateNormalise();
            C3DVector y_axis = x_axis ^ line_to_target_pos;
            y_axis.AccurateNormalise();
            float angle = GFFloatRandom(1.0f, GetWorld().GetWorldSeed());
            float random_walk_mag = GFFloatRandom(second_order_radius, GetWorld().GetWorldSeed());
            TargetedPosOffsetSecondOrder = C2DVector((float)sin(angle * TWO_PI), (float)cos(angle * TWO_PI));
            TargetedPosOffset += DeltaTargetedPosOffset;
            TargetedPosOffset3D = (x_axis * TargetedPosOffset.X + y_axis * TargetedPosOffset.Y) * main_radius * build_up * 0.0f;
            TargetedPosOffset3D += y_axis * TargetedPosOffsetSecondOrder.Y * random_walk_mag * shake_radius;
        }
    }
}

int main()
{
    const float turns[] = { 0.0f, 0.125f, 0.5f };
    for (unsigned mask = 0; mask < 64; ++mask) {
        for (unsigned angle = 0; angle < 3; ++angle) {
            CTCTargetingPlayer targeting;
            CThingCreatureBase& thing = targeting.Parent;
            thing.HasRanged = (mask & 1) != 0;
            const bool zero_distance = (mask & 2) != 0;
            const bool x_dominates = (mask & 4) != 0;
            thing.Creature = (mask & 8) != 0;
            thing.HasExperience = (mask & 16) != 0;
            thing.HasCombatDef = (mask & 32) != 0;
            const bool will_update = thing.HasRanged && !zero_distance;
            const bool read_definition = will_update && thing.Creature && thing.HasExperience;
            const bool will_weight = read_definition && thing.HasCombatDef;
            CCamera& camera = targeting.World.GameCamera.Manager.Camera;
            camera.Position = C3DVector(4,-5,6);
            targeting.TargetedPos = camera.Position;
            if (!zero_distance) {
                targeting.TargetedPos.X += x_dominates ? 28 : 21;
                targeting.TargetedPos.Y += x_dominates ? 21 : 28;
            }
            targeting.TargetedPosOffset3D = C3DVector(7,9,11);
            targeting.TargetedPosOffset = C2DVector(1,1);
            targeting.DeltaTargetedPosOffset = C2DVector(0.5f,0.25f);
            targeting.TargetedPosOffsetSecondOrder = C2DVector(12,13);
            expected_seed = &targeting.World.Seed;
            expected_radius = will_weight ? 1.0f : 0.5f;
            random_calls = 0;
            random_angle = turns[angle];
            targeting.UpdateTargetedPositionRandomWalk();

            assert(thing.Ranged.BuildReads == thing.HasRanged && thing.Ranged.AccuracyReads == thing.HasRanged);
            assert(targeting.World.GameCamera.Reads == thing.HasRanged);
            assert(thing.ExperienceChecks == (will_update && thing.Creature));
            assert(thing.DefinitionReads == unsigned(!read_definition ? 0 : will_weight ? 2 : 1));
            assert(thing.Definition.RangedAccuracyShakeyCursorWeighting.Queries == will_weight);
            assert(random_calls == (will_update ? 2U : 0U));
            assert(targeting.World.SeedReads == random_calls && targeting.World.Seed == 73 + random_calls);
            if (!will_update) {
                assert(targeting.TargetedPosOffset3D.X == 7 && targeting.TargetedPosOffset3D.Y == 9);
                assert(targeting.TargetedPosOffset3D.Z == 11);
                assert(targeting.TargetedPosOffset.X == 1 && targeting.TargetedPosOffset.Y == 1);
                assert(targeting.TargetedPosOffsetSecondOrder.X == 12 && targeting.TargetedPosOffsetSecondOrder.Y == 13);
            } else {
                const float sine = static_cast<float>(std::sin(turns[angle]*TWO_PI));
                const float cosine = static_cast<float>(std::cos(turns[angle]*TWO_PI));
                const float z = (x_dominates ? -1.0f : 1.0f)*cosine*(will_weight ? 0.75f : 0.5f);
                assert(targeting.TargetedPosOffset.X == 1.5f && targeting.TargetedPosOffset.Y == 1.25f);
                assert(std::fabs(targeting.TargetedPosOffsetSecondOrder.X-sine) < 0.00001f);
                assert(std::fabs(targeting.TargetedPosOffsetSecondOrder.Y-cosine) < 0.00001f);
                assert(targeting.TargetedPosOffset3D.X == 0 && targeting.TargetedPosOffset3D.Y == 0);
                assert(std::fabs(targeting.TargetedPosOffset3D.Z-z) < 0.00001f);
            }
        }
    }
    std::puts("00803540_TEST PASS");
    return 0;
}
