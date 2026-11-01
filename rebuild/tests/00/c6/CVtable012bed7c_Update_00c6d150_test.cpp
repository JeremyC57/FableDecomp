// Behaviour test for CVtable_012bed7c::Update (0x00C6D150): the direction lookup's two delays are copied into the four
// delay slots when they change (or on force), stereo / quad panning gains are derived from the azimuth, the volume is
// taken, and the gain records are rebuilt exactly when something changed.
#include <stdio.h>
#include <math.h>
struct SDirectionFilter { short m_coefficients[25]; unsigned short m_delay; };
static SDirectionFilter g_left = { {0}, 17 }, g_right = { {0}, 23 };
static long g_lookups = 0;
void LookupDirectionFilterPair(long table, float azimuth, float elevation, const SDirectionFilter** first, const SDirectionFilter** second)
{
    (void)table; (void)azimuth; (void)elevation; ++g_lookups; *first = &g_left; *second = &g_right;
}
struct CAudioMaster { unsigned long m_outputChannels; };
static CAudioMaster g_master = { 2 };
static CAudioMaster* g_pAudioMaster = &g_master;
class CVtable_012bed7c {
public:
    float m_volume; float m_gains[6]; long m_delays[6]; int rebuilds; bool lastCrossfade;
    CVtable_012bed7c() : m_volume(0), rebuilds(0), lastCrossfade(false) { for (int i = 0; i < 6; ++i) { m_gains[i] = 0; m_delays[i] = 0; } }
    void RebuildGain(bool crossfade) { ++rebuilds; lastCrossfade = crossfade; }
    void Update(float x, float y, float blend, float volume, const float* gains, bool crossfade, bool force);
};
void CVtable_012bed7c::Update(float x, float y, float blend, float volume, const float* gains, bool crossfade, bool force)
{
    const SDirectionFilter* first;
    const SDirectionFilter* second;
    bool changed = false;
    float gain[6];
    (void)blend; (void)gains;
    LookupDirectionFilterPair(4, x, y, &first, &second);
    if (force || m_delays[0] != first->m_delay || m_delays[1] != second->m_delay) {
        m_delays[0] = first->m_delay;
        m_delays[2] = first->m_delay;
        m_delays[1] = second->m_delay;
        m_delays[3] = second->m_delay;
        changed = true;
    }
    if (g_pAudioMaster->m_outputChannels >= 4) {
        float s = (float)sin(x * 0.017453292f) * 0.4f;
        float c = (float)cos(x * 0.017453292f) * 0.4f;
        gain[0] = (c + 0.6f) * (0.6f - s);
        gain[1] = (c + 0.6f) * (s + 0.6f);
        gain[2] = (0.6f - s) * (0.6f - c);
        gain[3] = (s + 0.6f) * (0.6f - c);
        if (force || m_gains[0] != gain[0] || m_gains[1] != gain[1] || m_gains[2] != gain[2] || m_gains[3] != gain[3]) {
            m_gains[0] = gain[0]; m_gains[1] = gain[1]; m_gains[2] = gain[2]; m_gains[3] = gain[3];
            changed = true;
        }
    } else {
        float s = (float)sin(x * 0.017453292f) * 0.4f;
        gain[0] = 0.6f - s;
        gain[1] = s + 0.6f;
        if (force || m_gains[0] != gain[0] || m_gains[1] != gain[1]) {
            m_gains[0] = gain[0]; m_gains[1] = gain[1];
            changed = true;
        }
    }
    if (force || m_volume != volume) {
        m_volume = volume;
        changed = true;
    }
    if (changed)
        RebuildGain(crossfade);
}
static bool near(float a, float b) { return fabs(a - b) < 1e-4; }
int main()
{
    CVtable_012bed7c fx;
    fx.Update(0.0f, 0.0f, 0.0f, 0.5f, 0, true, false);           // stereo, centre: equal 0.6 gains, delays copied
    if (fx.rebuilds != 1 || !fx.lastCrossfade || fx.m_volume != 0.5f || fx.m_delays[0] != 17 || fx.m_delays[2] != 17 || fx.m_delays[1] != 23 || fx.m_delays[3] != 23 ||
        !near(fx.m_gains[0], 0.6f) || !near(fx.m_gains[1], 0.6f)) { printf("00c6d150_TEST FAIL stereo centre\n"); return 1; }
    fx.Update(0.0f, 0.0f, 0.0f, 0.5f, 0, true, false);           // nothing changed: no rebuild
    if (fx.rebuilds != 1 || g_lookups != 2) { printf("00c6d150_TEST FAIL unchanged\n"); return 1; }
    fx.Update(90.0f, 0.0f, 0.0f, 0.5f, 0, false, false);         // hard right: left 0.2, right 1.0
    if (fx.rebuilds != 2 || fx.lastCrossfade || !near(fx.m_gains[0], 0.2f) || !near(fx.m_gains[1], 1.0f)) { printf("00c6d150_TEST FAIL stereo right %f %f\n", fx.m_gains[0], fx.m_gains[1]); return 1; }
    fx.Update(90.0f, 0.0f, 0.0f, 0.75f, 0, true, false);         // only the volume changed
    if (fx.rebuilds != 3 || fx.m_volume != 0.75f || !fx.lastCrossfade) { printf("00c6d150_TEST FAIL volume\n"); return 1; }
    fx.Update(90.0f, 0.0f, 0.0f, 0.75f, 0, false, true);         // force: rebuild even when nothing changed
    if (fx.rebuilds != 4) { printf("00c6d150_TEST FAIL force\n"); return 1; }
    g_master.m_outputChannels = 4;
    fx.Update(0.0f, 0.0f, 0.0f, 0.75f, 0, false, false);         // quad, centre: front 0.6, rear 0.12
    if (fx.rebuilds != 5 || !near(fx.m_gains[0], 0.6f) || !near(fx.m_gains[1], 0.6f) || !near(fx.m_gains[2], 0.12f) || !near(fx.m_gains[3], 0.12f)) { printf("00c6d150_TEST FAIL quad %f %f %f %f\n", fx.m_gains[0], fx.m_gains[1], fx.m_gains[2], fx.m_gains[3]); return 1; }
    fx.Update(180.0f, 0.0f, 0.0f, 0.75f, 0, false, false);       // behind: front 0.12, rear 0.6
    if (fx.rebuilds != 6 || !near(fx.m_gains[0], 0.12f) || !near(fx.m_gains[3], 0.6f)) { printf("00c6d150_TEST FAIL quad rear\n"); return 1; }
    printf("00c6d150_TEST PASS\n");
    return 0;
}