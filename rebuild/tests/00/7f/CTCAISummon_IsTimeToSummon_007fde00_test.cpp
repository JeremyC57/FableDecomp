// Behaviour test for CTCAISummon::IsTimeToSummon @ 0x007FDE00: true before the first summon, true for a
// def with no interval, otherwise true only once more than interval * frame-rate frames have passed.
#include <cstdio>

static long g_FramesPerSecond = 15;
static long g_worldFrame = 0;
static long GetWorldFrame() { return g_worldFrame; }

class CSummonDef { public: float TimeBetweenSummons; };

class CTCAISummon { public:
    const CSummonDef* PSummonDef;
    long              LastSummonTick;
    bool IsTimeToSummon()
    {
        if (LastSummonTick == 0)
            return true;
        if (PSummonDef->TimeBetweenSummons == 0.0f)
            return true;
        long framesPerSecond = g_FramesPerSecond;
        return GetWorldFrame() - LastSummonTick > (long)(framesPerSecond * PSummonDef->TimeBetweenSummons) ? true : false;
    }
};

int main()
{
    CSummonDef def; def.TimeBetweenSummons = 2.0f;          // 30 frames
    CTCAISummon s; s.PSummonDef = &def; s.LastSummonTick = 0;
    bool ok = true;
    g_worldFrame = 5;
    if (!s.IsTimeToSummon()) ok = false;                     // never summoned
    s.LastSummonTick = 100;
    g_worldFrame = 130;
    if (s.IsTimeToSummon()) ok = false;                      // exactly 30 frames: not yet
    g_worldFrame = 131;
    if (!s.IsTimeToSummon()) ok = false;                     // 31 frames
    def.TimeBetweenSummons = 0.0f;
    g_worldFrame = 101;
    if (!s.IsTimeToSummon()) ok = false;                     // no interval
    def.TimeBetweenSummons = 0.5f;                           // 7.5 -> 7 frames
    g_worldFrame = 107;
    if (s.IsTimeToSummon()) ok = false;
    g_worldFrame = 108;
    if (!s.IsTimeToSummon()) ok = false;
    if (!ok) { std::printf("007fde00_TEST FAIL\n"); return 1; }
    std::printf("007fde00_TEST PASS\n");
    return 0;
}