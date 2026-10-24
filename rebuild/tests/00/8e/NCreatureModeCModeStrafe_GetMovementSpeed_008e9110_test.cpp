#include <cstdio>
#include <cstring>
// Behaviour test for NCreatureMode::CModeStrafe::GetMovementSpeed (retail 0x008E9110): the strafe animation chosen for the
// direction is looked up by name; its movement distance over its duration, divided by the frame rate, is the speed.
struct C3DVector { float X, Y, Z; };
enum EMovementType { MOVEMENT_WALK, MOVEMENT_RUN };
struct CCharString { char Text[32]; };
CCharString GetStrafeAnimationName(const C3DVector& a, const C3DVector& b, EMovementType type)
{
    CCharString s;
    std::strcpy(s.Text, (a.X * b.X + a.Y * b.Y) >= 0.0f ? "STRAFE_MOVEMENT_FORWARD" : "STRAFE_MOVEMENT_BACK");
    (void)type;
    return s;
}
struct C3DAnimationInfo {
    float Duration;
    float GetDuration() const { return Duration; }
};
struct CMeshDataBank {
    C3DAnimationInfo Infos[4];
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const { return &Infos[index]; }
};
struct CWorld { CMeshDataBank* PMainMeshBank; };
static CWorld g_world;
static long g_FramesPerSecond = 4;
struct CAnimationEntry { long BankIndex; };
struct CThingCreatureBase {
    CAnimationEntry Forward;
    CAnimationEntry Back;
    float Distances[4];
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index)
    {
        (void)index;
        return std::strcmp(name.Text, "STRAFE_MOVEMENT_FORWARD") == 0 ? &Forward : &Back;
    }
    float GetAnimMovementDistance(long animIndex) const { return Distances[animIndex]; }
    static const CWorld& PeekWorld() { return g_world; }
};
struct CModeStrafe {
    CThingCreatureBase* ParentThing;
    float GetMovementSpeed(const C3DVector& a, const C3DVector& b, EMovementType type) const
    {
        CCharString name = GetStrafeAnimationName(a, b, type);
        CMeshDataBank* bank = CThingCreatureBase::PeekWorld().PMainMeshBank;
        CThingCreatureBase* creature = ParentThing;
        long animIndex = creature->GetAnimFromName(name, 0)->BankIndex;
        float duration = bank->PeekAnimation(animIndex)->GetDuration();
        float speed = creature->GetAnimMovementDistance(animIndex) / duration;
        return speed / (float)g_FramesPerSecond;
    }
};

int main()
{
    CMeshDataBank bank;
    bank.Infos[1].Duration = 2.0f;
    bank.Infos[2].Duration = 0.5f;
    g_world.PMainMeshBank = &bank;
    CThingCreatureBase hero;
    hero.Forward.BankIndex = 1;
    hero.Back.BankIndex = 2;
    hero.Distances[1] = 16.0f;
    hero.Distances[2] = 1.0f;
    CModeStrafe m;
    m.ParentThing = &hero;
    C3DVector facing = { 1.0f, 0.0f, 0.0f };
    C3DVector ahead = { 1.0f, 0.0f, 0.0f };
    C3DVector behind = { -1.0f, 0.0f, 0.0f };
    float forward = m.GetMovementSpeed(facing, ahead, MOVEMENT_WALK);
    float back = m.GetMovementSpeed(facing, behind, MOVEMENT_RUN);
    if (forward != 2.0f || back != 0.5f) { std::printf("008e9110_TEST FAIL\n"); return 1; }
    std::printf("008e9110_TEST PASS\n");
    return 0;
}