// Behaviour test for the CCreatureAction_PlayAnimationWithLookTurning constructor at 0x00842E40: arguments forwarded to the right base overload.
#include <cstdio>
class CThingCreatureBase { public: int id; };
class CAnimationEntry { public: int id; };
class CCharString { public: const char* text; };
class CSoundPair { public: long a, b; };
static int g_which = 0;
static const void* g_owner; static const void* g_anim; static const void* g_sound;
static bool g_stay, g_loop, g_move, g_phys, g_cam; static long g_count, g_c, g_d;
class CCreatureAction_PlayAnimation {
public:
    int kind;
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CCharString& animName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d)
    { kind = 0; g_which = 1; g_owner = &owner; g_anim = &animName; g_sound = &sound; g_move = useAnimationMovement; g_phys = usePhysics; g_cam = cameraUpdate; g_c = c; g_d = d; }
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CAnimationEntry& anim, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d)
    { kind = 0; g_which = 2; g_owner = &owner; g_anim = &anim; g_sound = &sound; g_move = useAnimationMovement; g_phys = usePhysics; g_cam = cameraUpdate; g_c = c; g_d = d; }
    CCreatureAction_PlayAnimation(CThingCreatureBase& owner, const CAnimationEntry& anim, bool stayOnLastFrame, bool looping,
                                  long numberOfTimesToLoop, const CSoundPair& sound, bool useAnimationMovement, bool usePhysics,
                                  bool cameraUpdate, long c, long d)
    { kind = 0; g_which = 3; g_owner = &owner; g_anim = &anim; g_stay = stayOnLastFrame; g_loop = looping; g_count = numberOfTimesToLoop; g_sound = &sound; g_move = useAnimationMovement; g_phys = usePhysics; g_cam = cameraUpdate; g_c = c; g_d = d; }
};
class CCreatureAction_PlayAnimationWithLookTurning : public CCreatureAction_PlayAnimation {
public:
    CCreatureAction_PlayAnimationWithLookTurning(CThingCreatureBase& owner, const CAnimationEntry& anim, const CSoundPair& sound,
                                                 bool useAnimationMovement, bool usePhysics, long c, long d)
        : CCreatureAction_PlayAnimation(owner, anim, sound, useAnimationMovement, usePhysics, false, c, d) { kind = 7; }
};
int main()
{
    CThingCreatureBase o; CAnimationEntry n; CSoundPair s;
    CCreatureAction_PlayAnimationWithLookTurning a(o, n, s, true, true, 5, 6);
    if (g_which != 2 || g_owner != &o || g_anim != &n || g_sound != &s || !g_move || !g_phys || g_cam || g_c != 5 || g_d != 6 || a.kind != 7) { std::printf("00842e40_TEST FAIL\n"); return 1; }
    std::printf("00842e40_TEST PASS\n");
    return 0;
}