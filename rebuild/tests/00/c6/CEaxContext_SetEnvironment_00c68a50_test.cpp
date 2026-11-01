// Behaviour test for CEaxContext::SetEnvironment 0x00C68A50 (libpk): an environment index above 26 is rejected (last error -2,
// result -3); an unchanged environment is a no-op; a changed one is stored, committed to the hardware block when the slot is the
// primary slot with its enable flag, and (for a real preset, not 26 = "user") the preset's parameters are pushed through the
// setters in retail order, stopping at the first setter that fails; the result is 0 either way.
#include <cstdio>
#include <cstring>

struct Preset { long room, roomHF, roomLF; float decayTime; long reflections; float envSize; long decayHFLimit; };
static const Preset presets[27] = { {-1000, -100, 0, 1.49f, -2602, 7.5f, 1}, {-1000, -500, 0, 1.49f, -2602, 1.4f, 1},
    {-1000, -600, 0, 1.49f, -1000, 1.0f, 1} };

struct Context {
    unsigned long stored[4]; bool enabled[4]; unsigned long primary; unsigned long committed; unsigned long dirty; long lastError;
    int calls; const char* trace[64]; int failAt;
    Context() { memset(this, 0, sizeof(*this)); for (int i = 0; i < 4; ++i) stored[i] = 26; failAt = -1; }
    long step(const char* name) { trace[calls] = name; ++calls; return (calls - 1 == failAt) ? -1 : 0; }
    long SetEnvironment(unsigned long slot, unsigned long environment)
    {
        if (environment > 26) { lastError = -2; return -3; }
        unsigned long& s = stored[slot];
        if (s != environment) {
            s = environment;
            if (primary == slot && enabled[slot]) { committed = environment; dirty |= 1; }
            if (environment != 26) {
                const Preset& p = presets[environment];
                static const char* const order[31] = { "Room", "RoomHF", "RoomLF", "DecayTime", "DecayHFRatio", "DecayLFRatio",
                    "Reflections", "ReflectionsDelay", "ReflectionsPan", "Reverb", "ReverbDelay", "ReverbPan", "EnvironmentDiffusion",
                    "EchoTime", "EchoDepth", "ModulationTime", "ModulationDepth", "SlotAirAbsorptionHF", "RoomRolloffFactor",
                    "SlotHFReference", "LFReference", "ReverbFlags", "EnvironmentSize", "DecayTimeScale", "ReflectionsScale",
                    "ReflectionsDelayScale", "ReverbScale", "ReverbDelayScale", "DecayHFLimit", "EchoTimeScale", "ModulationTimeScale" };
                (void)p;
                for (int i = 0; i < 31; ++i)
                    if (step(order[i]) < 0) return 0;
            }
        }
        return 0;
    }
};

int main()
{
    int fails = 0;
    { Context c; if (c.SetEnvironment(0, 27) != -3 || c.lastError != -2 || c.calls != 0) ++fails; }
    { Context c; c.stored[1] = 2; if (c.SetEnvironment(1, 2) != 0 || c.calls != 0 || c.dirty != 0) ++fails; }   // unchanged: no-op
    { Context c; c.primary = 0; c.enabled[0] = true;
      if (c.SetEnvironment(0, 1) != 0 || c.stored[0] != 1 || c.committed != 1 || c.dirty != 1 || c.calls != 31) ++fails;
      if (strcmp(c.trace[0], "Room") != 0 || strcmp(c.trace[3], "DecayTime") != 0 || strcmp(c.trace[21], "ReverbFlags") != 0
          || strcmp(c.trace[30], "ModulationTimeScale") != 0) ++fails; }
    { Context c; c.primary = 2; c.enabled[0] = true;                      // not the primary slot: stored but not committed
      if (c.SetEnvironment(0, 2) != 0 || c.stored[0] != 2 || c.committed != 0 || c.dirty != 0 || c.calls != 31) ++fails; }
    { Context c; c.stored[0] = 3; if (c.SetEnvironment(0, 26) != 0 || c.stored[0] != 26 || c.calls != 0) ++fails; }   // user environment: no preset walk
    { Context c; c.failAt = 8; if (c.SetEnvironment(0, 1) != 0 || c.calls != 9 || strcmp(c.trace[8], "ReflectionsPan") != 0) ++fails; }
    if (fails == 0) printf("00c68a50_TEST PASS\n"); else printf("00c68a50_TEST FAIL %d\n", fails);
    return fails != 0;
}