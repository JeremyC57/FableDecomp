#include <cstdio>
// Behaviour model of CAIStateGroup_Guildmaster::Update (0x00977E00): decision logic over frames, fps, def intervals and helpers.
struct Def { float MinSecondsBetweenTeleport, MinSecondsBetweenWillUse; };
struct Sim {
    long AttackNum, LastWillFrame, LastTeleportFrame, frame, fps, helpers;
    bool shieldUp, runnable1, runnable2, teleportPosOk, terminated;
    int willShieldCasts, cancels, teleports, actions[3], bodyguardCalls;
    Def def;
};
static void Update(Sim& s)
{
    if (!s.shieldUp && s.helpers > 0) ++s.willShieldCasts;
    if (!s.runnable1) { s.terminated = true; return; }
    float since_will = (float)(s.frame - s.LastWillFrame) / s.fps;
    if (s.helpers == 0) {
        if (s.shieldUp) ++s.cancels;
    } else if (since_will > s.def.MinSecondsBetweenWillUse) {
        if ((float)(s.frame - s.LastTeleportFrame) / s.fps > s.def.MinSecondsBetweenTeleport && s.teleportPosOk) {
            ++s.teleports; s.LastTeleportFrame = s.frame;
        } else {
            ++s.bodyguardCalls;
            switch (s.AttackNum % 3) { case 0: ++s.actions[0]; break; case 1: ++s.actions[1]; break; case 2: ++s.actions[2]; break; }
            ++s.AttackNum; s.LastWillFrame = s.frame;
        }
    }
    if (!s.runnable2) { s.terminated = true; return; }
}
int main()
{
    Sim s = {0};
    s.fps = 30; s.helpers = 2; s.runnable1 = s.runnable2 = true; s.def.MinSecondsBetweenTeleport = 10.0f; s.def.MinSecondsBetweenWillUse = 2.0f;
    int fail = 0;
    s.frame = 30; Update(s);                       // 1 s < 2 s: nothing but the will shield cast
    if (s.willShieldCasts != 1 || s.bodyguardCalls != 0) fail = 1;
    s.frame = 90; Update(s);                       // 3 s: no teleport (pos not ok) -> action 0
    if (s.actions[0] != 1 || s.AttackNum != 1 || s.LastWillFrame != 90) fail = 2;
    s.frame = 180; Update(s); s.frame = 270; Update(s);
    if (s.actions[1] != 1 || s.actions[2] != 1 || s.AttackNum != 3) fail = 3;
    s.teleportPosOk = true; s.frame = 400; Update(s);   // 13.3 s since teleport frame 0 > 10 -> teleport
    if (s.teleports != 1 || s.LastTeleportFrame != 400 || s.AttackNum != 3) fail = 4;
    s.helpers = 0; s.shieldUp = true; Update(s);
    if (s.cancels != 1) fail = 5;
    s.runnable2 = false; Update(s);
    if (!s.terminated) fail = 6;
    if (fail) { std::printf("00977e00_TEST FAIL\n"); return 1; }
    std::printf("00977e00_TEST PASS\n");
    return 0;
}