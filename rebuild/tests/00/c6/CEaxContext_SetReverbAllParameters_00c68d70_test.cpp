// Behaviour test for 00c68d70: CEaxContext::SetReverbAllParameters -- the 24 reverb setters run in order on the slot,
// the first failure stops the chain, and the reverb-dirty bit is set either way
#include <cstdio>
struct EAXREVERBPROPERTIES {
    unsigned long ulEnvironment; float flEnvironmentSize, flEnvironmentDiffusion; long lRoom, lRoomHF, lRoomLF; float flDecayTime, flDecayHFRatio, flDecayLFRatio;
    long lReflections; float flReflectionsDelay; float vReflectionsPan[3]; long lReverb; float flReverbDelay; float vReverbPan[3];
    float flEchoTime, flEchoDepth, flModulationTime, flModulationDepth, flAirAbsorptionHF, flHFReference, flLFReference, flRoomRolloffFactor; unsigned long ulFlags;
};
static int g_order[24], g_calls = 0, g_failAt = -1;
struct CEaxContext {
    unsigned long m_dirty;
    long Step(int i) { if (g_calls < 24) g_order[g_calls] = i; ++g_calls; return i == g_failAt ? -1 : 0; }
    long SetEnvironment(unsigned long, unsigned long) { return Step(0); } long SetEnvironmentSize(unsigned long, float) { return Step(1); }
    long SetEnvironmentDiffusion(unsigned long, float) { return Step(2); } long SetRoom(unsigned long, long) { return Step(3); }
    long SetRoomHF(unsigned long, long) { return Step(4); } long SetRoomLF(unsigned long, long) { return Step(5); }
    long SetDecayTime(unsigned long, float) { return Step(6); } long SetDecayHFRatio(unsigned long, float) { return Step(7); }
    long SetDecayLFRatio(unsigned long, float) { return Step(8); } long SetReflections(unsigned long, long) { return Step(9); }
    long SetReflectionsDelay(unsigned long, float) { return Step(10); } long SetReflectionsPan(unsigned long, const float*) { return Step(11); }
    long SetReverb(unsigned long, long) { return Step(12); } long SetReverbDelay(unsigned long, float) { return Step(13); }
    long SetReverbPan(unsigned long, const float*) { return Step(14); } long SetEchoTime(unsigned long, float) { return Step(15); }
    long SetEchoDepth(unsigned long, float) { return Step(16); } long SetModulationTime(unsigned long, float) { return Step(17); }
    long SetModulationDepth(unsigned long, float) { return Step(18); } long SetSlotAirAbsorptionHF(unsigned long, float) { return Step(19); }
    long SetSlotHFReference(unsigned long, float) { return Step(20); } long SetSlotLFReference(unsigned long, float) { return Step(21); }
    long SetRoomRolloffFactor(unsigned long, float) { return Step(22); } long SetReverbFlags(unsigned long, unsigned long) { return Step(23); }
    long SetReverbAllParameters(unsigned long slot, const EAXREVERBPROPERTIES* v)
    {
        long result;
        do {
            if ((result = SetEnvironment(slot, v->ulEnvironment)) < 0) break;
            if ((result = SetEnvironmentSize(slot, v->flEnvironmentSize)) < 0) break;
            if ((result = SetEnvironmentDiffusion(slot, v->flEnvironmentDiffusion)) < 0) break;
            if ((result = SetRoom(slot, v->lRoom)) < 0) break;
            if ((result = SetRoomHF(slot, v->lRoomHF)) < 0) break;
            if ((result = SetRoomLF(slot, v->lRoomLF)) < 0) break;
            if ((result = SetDecayTime(slot, v->flDecayTime)) < 0) break;
            if ((result = SetDecayHFRatio(slot, v->flDecayHFRatio)) < 0) break;
            if ((result = SetDecayLFRatio(slot, v->flDecayLFRatio)) < 0) break;
            if ((result = SetReflections(slot, v->lReflections)) < 0) break;
            if ((result = SetReflectionsDelay(slot, v->flReflectionsDelay)) < 0) break;
            if ((result = SetReflectionsPan(slot, v->vReflectionsPan)) < 0) break;
            if ((result = SetReverb(slot, v->lReverb)) < 0) break;
            if ((result = SetReverbDelay(slot, v->flReverbDelay)) < 0) break;
            if ((result = SetReverbPan(slot, v->vReverbPan)) < 0) break;
            if ((result = SetEchoTime(slot, v->flEchoTime)) < 0) break;
            if ((result = SetEchoDepth(slot, v->flEchoDepth)) < 0) break;
            if ((result = SetModulationTime(slot, v->flModulationTime)) < 0) break;
            if ((result = SetModulationDepth(slot, v->flModulationDepth)) < 0) break;
            if ((result = SetSlotAirAbsorptionHF(slot, v->flAirAbsorptionHF)) < 0) break;
            if ((result = SetSlotHFReference(slot, v->flHFReference)) < 0) break;
            if ((result = SetSlotLFReference(slot, v->flLFReference)) < 0) break;
            if ((result = SetRoomRolloffFactor(slot, v->flRoomRolloffFactor)) < 0) break;
            result = SetReverbFlags(slot, v->ulFlags);
        } while (0);
        m_dirty |= 0x1000000;
        return result;
    }
};
int main()
{
    EAXREVERBPROPERTIES p = { 0 }; CEaxContext c; c.m_dirty = 0;
    bool ok = c.SetReverbAllParameters(0, &p) == 0 && g_calls == 24 && c.m_dirty == 0x1000000;
    for (int i = 0; i < 24 && ok; ++i) ok = g_order[i] == i;
    g_calls = 0; g_failAt = 5; c.m_dirty = 0;
    ok = ok && c.SetReverbAllParameters(1, &p) == -1 && g_calls == 6 && c.m_dirty == 0x1000000;
    std::printf(ok ? "00c68d70_TEST PASS\n" : "00c68d70_TEST FAIL\n");
    return ok ? 0 : 1;
}