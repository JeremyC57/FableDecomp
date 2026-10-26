// behaviour test for 0x00458C9D CMeleeCombatKnockdownEffects::Transfer: the context sees every transferred member once, in the def's order (retail offsets +0x3C, +0x40, +0x44, +0x48, +0x4C, +0x50, +0x51, +0x54, +0x58, +0x59, +0x5A),
// so a binary save / load round trip restores the def
#include <cstdio>
#include <cstring>
struct Context {
    unsigned char buf[1024]; unsigned int pos; int load; const void* order[64]; unsigned int count;
    Context(int l) : pos(0), load(l), count(0) { }
    template <class T> void Transfer(T& v)
    {
        order[count++] = &v;
        if (load) std::memcpy(&v, buf + pos, sizeof(T)); else std::memcpy(buf + pos, &v, sizeof(T));
        pos += (unsigned int)sizeof(T);
    }
};
struct Def {
    long BlendInFrames;
    long BlendOutFrames;
    long AttackerLightingChannel;
    long TargetLightingChannel;
    long EnvironmentTheme;
    bool CameraFOVChange;
    bool CameraPan;
    float AttackerAndTargetSpeedMultiplier;
    bool PauseEntities;
    bool NonAttackingEntities;
    bool AttackerInvulnerable;
};
static void Persist(Def& d, Context& c)                      // the function under test
{
    c.Transfer(d.BlendInFrames);
    c.Transfer(d.BlendOutFrames);
    c.Transfer(d.AttackerLightingChannel);
    c.Transfer(d.TargetLightingChannel);
    c.Transfer(d.EnvironmentTheme);
    c.Transfer(d.CameraFOVChange);
    c.Transfer(d.CameraPan);
    c.Transfer(d.AttackerAndTargetSpeedMultiplier);
    c.Transfer(d.PauseEntities);
    c.Transfer(d.NonAttackingEntities);
    c.Transfer(d.AttackerInvulnerable);
}
int main()
{
    int fail = 0;
    Def d; std::memset(&d, 0, sizeof(d));
    unsigned char* p = (unsigned char*)&d;
    for (unsigned int i = 0; i < (unsigned int)sizeof(d); ++i) p[i] = (unsigned char)(i * 7 + 3);
    d.CameraFOVChange = true;
    d.CameraPan = true;
    d.PauseEntities = true;
    d.NonAttackingEntities = true;
    d.AttackerInvulnerable = true;
    Context save(0); Persist(d, save);
    const void* want[11] = { &d.BlendInFrames, &d.BlendOutFrames, &d.AttackerLightingChannel, &d.TargetLightingChannel, &d.EnvironmentTheme, &d.CameraFOVChange, &d.CameraPan, &d.AttackerAndTargetSpeedMultiplier, &d.PauseEntities, &d.NonAttackingEntities, &d.AttackerInvulnerable };
    if (save.count != 11u) fail |= 1;
    for (unsigned int i = 0; i < save.count && i < 11u; ++i) if (save.order[i] != want[i]) fail |= 2;
    Def e; std::memcpy(&e, &d, sizeof(e));
    std::memset(&e.BlendInFrames, 0, sizeof(e.BlendInFrames));
    std::memset(&e.BlendOutFrames, 0, sizeof(e.BlendOutFrames));
    std::memset(&e.AttackerLightingChannel, 0, sizeof(e.AttackerLightingChannel));
    std::memset(&e.TargetLightingChannel, 0, sizeof(e.TargetLightingChannel));
    std::memset(&e.EnvironmentTheme, 0, sizeof(e.EnvironmentTheme));
    std::memset(&e.CameraFOVChange, 0, sizeof(e.CameraFOVChange));
    std::memset(&e.CameraPan, 0, sizeof(e.CameraPan));
    std::memset(&e.AttackerAndTargetSpeedMultiplier, 0, sizeof(e.AttackerAndTargetSpeedMultiplier));
    std::memset(&e.PauseEntities, 0, sizeof(e.PauseEntities));
    std::memset(&e.NonAttackingEntities, 0, sizeof(e.NonAttackingEntities));
    std::memset(&e.AttackerInvulnerable, 0, sizeof(e.AttackerInvulnerable));
    Context load(1); std::memcpy(load.buf, save.buf, sizeof(save.buf)); Persist(e, load);
    if (load.pos != save.pos) fail |= 4;
    if (std::memcmp(&e, &d, sizeof(d)) != 0) fail |= 8;
    if (fail) { std::printf("00458c9d_TEST FAIL %d\n", fail); return 1; }
    std::printf("00458c9d_TEST PASS\n");
    return 0;
}