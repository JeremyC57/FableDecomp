// behaviour test for 0x004625FD CMeleeCombatKnockdownEffects::operator=: the base part is assigned first, every member is copied, and the result is *this
#include <cstdio>
static int g_base = 0;
struct Base { long PDefManager; long GlobalIndex; Base& operator=(const Base& o) { ++g_base; PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; return *this; } };
struct Def : Base {
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
    Def& Assign(const Def& o)                              // the function under test
    {
        Base::operator=(o);
        BlendInFrames = o.BlendInFrames;
        BlendOutFrames = o.BlendOutFrames;
        AttackerLightingChannel = o.AttackerLightingChannel;
        TargetLightingChannel = o.TargetLightingChannel;
        EnvironmentTheme = o.EnvironmentTheme;
        CameraFOVChange = o.CameraFOVChange;
        CameraPan = o.CameraPan;
        AttackerAndTargetSpeedMultiplier = o.AttackerAndTargetSpeedMultiplier;
        PauseEntities = o.PauseEntities;
        NonAttackingEntities = o.NonAttackingEntities;
        AttackerInvulnerable = o.AttackerInvulnerable;
        return *this;
    }
};
int main()
{
    int fail = 0;
    Def s, d;
    s.PDefManager = 11; s.GlobalIndex = 12; d.PDefManager = 0; d.GlobalIndex = 0;
    s.BlendInFrames = 100L; d.BlendInFrames = 0L;
    s.BlendOutFrames = 101L; d.BlendOutFrames = 0L;
    s.AttackerLightingChannel = 102L; d.AttackerLightingChannel = 0L;
    s.TargetLightingChannel = 103L; d.TargetLightingChannel = 0L;
    s.EnvironmentTheme = 104L; d.EnvironmentTheme = 0L;
    s.CameraFOVChange = true; d.CameraFOVChange = false;
    s.CameraPan = true; d.CameraPan = false;
    s.AttackerAndTargetSpeedMultiplier = 8.25f; d.AttackerAndTargetSpeedMultiplier = 0.0f;
    s.PauseEntities = true; d.PauseEntities = false;
    s.NonAttackingEntities = true; d.NonAttackingEntities = false;
    s.AttackerInvulnerable = true; d.AttackerInvulnerable = false;
    g_base = 0;
    if (&d.Assign(s) != &d) fail |= 1;
    if (g_base != 1 || d.PDefManager != 11 || d.GlobalIndex != 12) fail |= 2;
    if (d.BlendInFrames != s.BlendInFrames) fail |= 4;
    if (d.BlendOutFrames != s.BlendOutFrames) fail |= 4;
    if (d.AttackerLightingChannel != s.AttackerLightingChannel) fail |= 4;
    if (d.TargetLightingChannel != s.TargetLightingChannel) fail |= 4;
    if (d.EnvironmentTheme != s.EnvironmentTheme) fail |= 4;
    if (d.CameraFOVChange != s.CameraFOVChange) fail |= 4;
    if (d.CameraPan != s.CameraPan) fail |= 4;
    if (d.AttackerAndTargetSpeedMultiplier != s.AttackerAndTargetSpeedMultiplier) fail |= 4;
    if (d.PauseEntities != s.PauseEntities) fail |= 4;
    if (d.NonAttackingEntities != s.NonAttackingEntities) fail |= 4;
    if (d.AttackerInvulnerable != s.AttackerInvulnerable) fail |= 4;
    if (fail) { std::printf("004625fd_TEST FAIL %d\n", fail); return 1; }
    std::printf("004625fd_TEST PASS\n");
    return 0;
}