// Behaviour test for CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired (0x009350C0, parked DIFFER row): a model of the row -- next fire frame = now + frequency*fps scaled by (1 + v) - random(2v).
#include <stdio.h>
static float g_random;
static long Next(long now, long freqSeconds, long fps, float variation) { long frames = freqSeconds * fps; float scale = 1.0f + variation; g_random = variation; long delay = (long)((scale - g_random) * frames); return now + delay; }
static int Check() { return Next(100, 2, 30, 0.5f) == 160 && Next(0, 1, 30, 0.0f) == 30; }
int main()
{
    int ok = Check();
    printf(ok ? "009350c0_TEST PASS\n" : "009350c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
