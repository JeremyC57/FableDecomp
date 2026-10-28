// behaviour test for 0x008b8310 CCreatureAction_ProjectileCrouch::CCreatureAction_ProjectileCrouch(CThingCreatureBase& creature, CThingCreatureBase& target, const C3DVector& facePos): always turns, to the given position;
// the base gets the creature's "SWORD_SWING" sound pair and priority 0x96 (the name string dies before the body), the
// combat animation is "CROUCH"
#include <cstdio>
#include <cstring>
static int gStrings = 0;
struct Str { const char* s; Str(const char* t) : s(t) { ++gStrings; } ~Str() { --gStrings; } };
struct Vec { float X, Y, Z; };
struct Thing { Vec pos; const Vec& GetPos() const { return pos; } };
struct SoundPair { const char* name; };
static SoundPair GetSoundPair(const Thing&, const Str& name) { SoundPair p; p.name = name.s; return p; }
struct CombatBase {
    Thing* creature; Thing* target; long priority; const char* sound; char anim[16]; int liveStrings;
    CombatBase(Thing& c, Thing& t, long p, const SoundPair& s) : creature(&c), target(&t), priority(p), sound(s.name), liveStrings(gStrings) { anim[0] = 0; }
    void SetCombatAnimation(const Str& name) { std::strcpy(anim, name.s); }
};
struct Crouch : CombatBase {
    bool TurnToFaceTarget; Vec FacePos;
    Crouch(Thing& c, Thing& t, bool turn) : CombatBase(c, t, 0x96, GetSoundPair(c, "SWORD_SWING")), TurnToFaceTarget(turn)
    {
        FacePos.X = FacePos.Y = FacePos.Z = -99.0f;   // uninitialised in retail; a marker here
        SetCombatAnimation("CROUCH");
        if (turn) FacePos = t.GetPos();
    }
    Crouch(Thing& c, Thing& t, const Vec& p) : CombatBase(c, t, 0x96, GetSoundPair(c, "SWORD_SWING")), TurnToFaceTarget(true), FacePos(p)
    {
        SetCombatAnimation("CROUCH");
    }
};
int main()
{
    int fails = 0;
    Thing hero = { { 0.0f, 0.0f, 0.0f } }, enemy = { { 5.0f, 6.0f, 7.0f } };
    Vec p = { 1.0f, 2.0f, 3.0f };
    Crouch c(hero, enemy, p);
    if (!c.TurnToFaceTarget || c.FacePos.X != 1.0f || c.FacePos.Y != 2.0f || c.FacePos.Z != 3.0f) fails |= 1;
    if (c.creature != &hero || c.target != &enemy || c.priority != 0x96 || std::strcmp(c.sound, "SWORD_SWING") != 0) fails |= 4;
    if (std::strcmp(c.anim, "CROUCH") != 0 || c.liveStrings != 1 || gStrings != 0) fails |= 8;
    if (fails) { std::printf("008b8310_TEST FAIL %d\n", fails); return 1; }
    std::printf("008b8310_TEST PASS\n");
    return 0;
}