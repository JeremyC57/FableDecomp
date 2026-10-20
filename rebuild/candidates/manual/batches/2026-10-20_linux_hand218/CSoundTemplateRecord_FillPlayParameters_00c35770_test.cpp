// Behaviour test for 0x00C35770 CSoundTemplateRecord::FillPlayParameters (model copy + recording setters).
#include <cstdio>

struct CSoundPlayParameters {
    float volume, pitch, pan, scale; long loopStart, loopEnd, priority, category; short delay;
    unsigned short fadeIn, group; int looped, streamed, positional;
    CSoundPlayParameters() : volume(-1), pitch(-1), pan(-1), scale(-1), loopStart(-1), loopEnd(-1), priority(-1), category(-1),
        delay(-1), fadeIn(0), group(0), looped(-1), streamed(-1), positional(-1) {}
    void SetVolume(float v) { volume = v; }
    void SetPitch(float v) { pitch = v; }
    void SetPan(float v) { pan = v; }
    void SetLoopStart(long v) { loopStart = v; }
    void SetLoopEnd(long v) { loopEnd = v; }
    void SetPriority(long v) { priority = v; }
    void SetDelay(short v) { delay = v; }
    void SetFadeIn(unsigned short v) { fadeIn = v; }
    void SetGroup(unsigned short v) { group = v; }
    void SetCategory(long v) { category = v; }
    void SetLooped(bool v) { looped = v; }
    void SetStreamed(bool v) { streamed = v; }
    void SetPositional(bool v) { positional = v; }
    void SetFrequencyScale(float v) { scale = v; }
};

struct CSoundTemplateRecord {
    unsigned char  _00[8];
    unsigned short Category;
    unsigned short Group;
    unsigned short FadeIn;
    unsigned char  _0e;
    unsigned char  Streamed : 1;
    unsigned char  Positional : 1;
    unsigned char  : 0;
    unsigned char  Looped : 1;
    unsigned char  HasLoopStart : 1;
    unsigned char  HasLoopEnd : 1;
    unsigned char  Volume;
    unsigned char  Pitch;
    unsigned char  Pan;
    long           LoopStart;
    long           LoopEnd;
    long           Priority;
    unsigned long  FrequencyDivisor;
    long FillPlayParameters(CSoundPlayParameters& params) const;
};

long CSoundTemplateRecord::FillPlayParameters(CSoundPlayParameters& params) const
{
    params.SetVolume((float)Volume * (1.0f / 127.0f));
    params.SetPitch((float)Pitch * 0.01f);
    params.SetPan((float)Pan * 0.01f);
    if (HasLoopStart)
        params.SetLoopStart(LoopStart);
    if (HasLoopEnd)
        params.SetLoopEnd(LoopEnd);
    params.SetPriority(Priority);
    float scale;
    if (FrequencyDivisor != 0 && FrequencyDivisor != 0xffffffff)
        scale = 1.0f / (float)FrequencyDivisor;
    else
        scale = 1.0f;
    params.SetFrequencyScale(scale);
    params.SetDelay(0);
    params.SetFadeIn(FadeIn);
    params.SetGroup(Group);
    params.SetCategory(Category);
    if (Streamed)
        params.SetStreamed(true);
    if (Positional)
        params.SetPositional(true);
    params.SetLooped(Looped);
    return 0;
}

static int fail() { std::printf("00c35770_TEST FAIL\n"); return 1; }

int main()
{
    CSoundTemplateRecord r;
    r.Category = 7; r.Group = 3; r.FadeIn = 250; r.Streamed = 1; r.Positional = 0; r.Looped = 1; r.HasLoopStart = 1; r.HasLoopEnd = 0;
    r.Volume = 127; r.Pitch = 50; r.Pan = 100; r.LoopStart = 1000; r.LoopEnd = 2000; r.Priority = 9; r.FrequencyDivisor = 4;
    CSoundPlayParameters p;
    float v = (float)127 * (1.0f / 127.0f), pi = (float)50 * 0.01f, pa = (float)100 * 0.01f;
    if (r.FillPlayParameters(p) != 0) return fail();
    if (p.volume != v || p.pitch != pi || p.pan != pa || p.scale != 0.25f) return fail();
    if (p.loopStart != 1000 || p.loopEnd != -1 || p.priority != 9 || p.delay != 0) return fail();
    if (p.fadeIn != 250 || p.group != 3 || p.category != 7 || p.streamed != 1 || p.positional != -1 || p.looped != 1) return fail();
    CSoundTemplateRecord q = r; q.FrequencyDivisor = 0xffffffff; q.Looped = 0; q.HasLoopStart = 0; q.HasLoopEnd = 1;
    CSoundPlayParameters p2;
    q.FillPlayParameters(p2);
    if (p2.scale != 1.0f || p2.looped != 0 || p2.loopStart != -1 || p2.loopEnd != 2000) return fail();
    q.FrequencyDivisor = 0; CSoundPlayParameters p3; q.FillPlayParameters(p3);
    if (p3.scale != 1.0f) return fail();
    std::printf("00c35770_TEST PASS\n");
    return 0;
}
