// behaviour test for 0x0087CA30 CCreatureAction_GeneralInterruptedAttack::Initialise(): a player character rumbles its
// joystick (type 3, {1.0, 0.5, 0.0, 0.2, not cumulative}, true); then the combat Initialise runs, then a dust effect is
// made with the negated facing vector and no hit list
#include <cstdio>
struct Vec { float X, Y, Z; };
struct InitRumble { float MaxLevel, Smoothness, AttackTime, DecayTime; bool Cumulative; };
struct Joystick { int count; long type; InitRumble r; bool replace;
    void AddRumble(long t, const InitRumble& ir, bool rep) { ++count; type = t; r = ir; replace = rep; } };
static Joystick gJoysticks[4];
static int gOrder = 0, gInitAt = 0, gDustAt = 0;
static Vec gDustDir; static const void* gDustHits = (const void*)1;
struct Creature { bool player; long joy; Vec facing;
    bool IsPlayerCharacter() const { return player; } long GetJoystickDeviceNumber() const { return joy; }
    Vec GetFacingVector() const { return facing; } };
static void CreateDustEffectForCreature(Creature&, const Vec& dir, const void* hits) { gDustAt = ++gOrder; gDustDir = dir; gDustHits = hits; }
struct Action {
    Creature* PCreature;
    void BaseInitialise() { gInitAt = ++gOrder; }
    void Initialise()
    {
        if (PCreature->IsPlayerCharacter()) {
            InitRumble r = { 1.0f, 0.5f, 0.0f, 0.2f, false };
            gJoysticks[PCreature->GetJoystickDeviceNumber()].AddRumble(3, r, true);
        }
        BaseInitialise();
        Vec f = PCreature->GetFacingVector();
        Vec direction = { -f.X, -f.Y, -f.Z };
        CreateDustEffectForCreature(*PCreature, direction, 0);
    }
};
int main()
{
    int fails = 0;
    Creature hero = { true, 2, { 0.0f, 1.0f, -0.5f } };
    Action a = { &hero };
    a.Initialise();
    Joystick& j = gJoysticks[2];
    if (j.count != 1 || j.type != 3 || !j.replace || j.r.MaxLevel != 1.0f || j.r.Smoothness != 0.5f || j.r.AttackTime != 0.0f
        || j.r.DecayTime != 0.2f || j.r.Cumulative) fails |= 1;
    if (gInitAt != 1 || gDustAt != 2 || gDustDir.X != 0.0f || gDustDir.Y != -1.0f || gDustDir.Z != 0.5f || gDustHits) fails |= 2;
    Creature villager = { false, 1, { 1.0f, 0.0f, 0.0f } };
    a.PCreature = &villager; a.Initialise();
    if (gJoysticks[1].count || j.count != 1 || gDustDir.X != -1.0f || gDustAt != 4) fails |= 4;
    if (fails) { std::printf("0087ca30_TEST FAIL %d\n", fails); return 1; }
    std::printf("0087ca30_TEST PASS\n");
    return 0;
}