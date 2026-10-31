// Behaviour test for CAudioSource::Rewind (0x00C72800): unless keepState, read AL_SOURCE_STATE, rewind, and replay when it was AL_PLAYING.
#include <stdio.h>
static int g_rewinds = 0, g_plays = 0, g_state = 0x1012, g_lastSource = 0;
struct Ctx {
    void GetSourcei(unsigned long s, long p, long* v) { g_lastSource = (int)s; *v = (p == 0x1010) ? g_state : 0; }
    void SourceRewind(unsigned long s) { g_lastSource = (int)s; ++g_rewinds; }
    void PlaySource(unsigned long s) { g_lastSource = (int)s; ++g_plays; }
};
static Ctx g_AudioContext;
struct Src {
    unsigned long SourceId;
    void Rewind(long keepState)
    {
        if (keepState) return;
        Ctx& context = g_AudioContext;
        long state;
        context.GetSourcei(SourceId, 0x1010, &state);
        context.SourceRewind(SourceId);
        if (state == 0x1012) context.PlaySource(SourceId);
    }
};
int main()
{
    Src s; s.SourceId = 7;
    s.Rewind(1);
    if (g_rewinds != 0 || g_plays != 0) { printf("00c72800_TEST FAIL\n"); return 1; }
    s.Rewind(0);
    if (g_rewinds != 1 || g_plays != 1 || g_lastSource != 7) { printf("00c72800_TEST FAIL\n"); return 1; }
    g_state = 0x1013; s.Rewind(0);
    if (g_rewinds != 2 || g_plays != 1) { printf("00c72800_TEST FAIL\n"); return 1; }
    printf("00c72800_TEST PASS\n"); return 0;
}