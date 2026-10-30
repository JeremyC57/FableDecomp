// Behaviour test for CAudioSource::Stop (0x00C72620): stop only a playing/paused source, detach the buffer,
// delete the AL source inside a suspend bracket, process the context again only if it was not suspended before.
#include <cstdio>

typedef void (*PFN_alGetSourcei)(unsigned int, int, int*);
typedef void (*PFN_alSourceStop)(unsigned int);
typedef void (*PFN_alSourcei)(unsigned int, int, int);
typedef void (*PFN_alDeleteSources)(int, const unsigned int*);
typedef void (*PFN_alcContext)(void*);

static int g_state, g_stops, g_bufferCleared, g_deleted, g_suspends, g_processes;
static char g_log[16]; static int g_logLen;
static void GetSourceiImpl(unsigned int, int param, int* v) { if (param == 0x1010) *v = g_state; g_log[g_logLen++] = 'g'; }
static void SourceStopImpl(unsigned int) { ++g_stops; g_log[g_logLen++] = 's'; }
static void SourceiImpl(unsigned int, int param, int value) { if (param == 0x1009 && value == 0) ++g_bufferCleared; g_log[g_logLen++] = 'b'; }
static void DeleteSourcesImpl(int n, const unsigned int* ids) { if (n == 1 && *ids == 77) ++g_deleted; g_log[g_logLen++] = 'd'; }
static void SuspendImpl(void*) { ++g_suspends; g_log[g_logLen++] = 'S'; }
static void ProcessImpl(void*) { ++g_processes; g_log[g_logLen++] = 'P'; }

struct CAudioContext {
    PFN_alGetSourcei pfnGetSourcei; PFN_alSourceStop pfnSourceStop; PFN_alSourcei pfnSourcei;
    PFN_alDeleteSources pfnDeleteSources; PFN_alcContext pfnSuspendContext, pfnProcessContext;
    void* Context; bool Suspended;
    void GetSourcei(unsigned int s, int p, int* v) { pfnGetSourcei(s, p, v); }
    void SourceStop(unsigned int s) { pfnSourceStop(s); }
    void Sourcei(unsigned int s, int p, int v) { pfnSourcei(s, p, v); }
    void DeleteSources(int n, const unsigned int* s) { pfnDeleteSources(n, s); }
    void Suspend() { pfnSuspendContext(Context); Suspended = true; }
    void Process() { pfnProcessContext(Context); Suspended = false; }
};
static CAudioContext g_AudioContext = { GetSourceiImpl, SourceStopImpl, SourceiImpl, DeleteSourcesImpl, SuspendImpl, ProcessImpl, 0, false };

enum { AL_BUFFER = 0x1009, AL_SOURCE_STATE = 0x1010, AL_PLAYING = 0x1012, AL_PAUSED = 0x1013, AL_STOPPED = 0x1014 };
struct CAudioSource {
    unsigned int SourceId;
    void Stop()
    {
        int state;
        CAudioContext& context = g_AudioContext;
        context.GetSourcei(SourceId, AL_SOURCE_STATE, &state);
        if (state == AL_PLAYING || state == AL_PAUSED)
            context.SourceStop(SourceId);
        context.Sourcei(SourceId, AL_BUFFER, 0);
        bool wasSuspended = context.Suspended;
        context.Suspend();
        context.DeleteSources(1, &SourceId);
        if (!wasSuspended)
            context.Process();
    }
};

static int run(int state, bool suspended, const char* expect)
{
    g_state = state; g_AudioContext.Suspended = suspended; g_logLen = 0;
    CAudioSource s; s.SourceId = 77;
    s.Stop();
    g_log[g_logLen] = 0;
    int i;
    for (i = 0; expect[i] || g_log[i]; ++i) if (expect[i] != g_log[i]) return 0;
    return g_AudioContext.Suspended == suspended;
}

int main()
{
    if (!run(AL_PLAYING, false, "gsbSdP")) { std::printf("00c72620_TEST FAIL\n"); return 1; }
    if (!run(AL_PAUSED, false, "gsbSdP")) { std::printf("00c72620_TEST FAIL\n"); return 1; }
    if (!run(AL_STOPPED, false, "gbSdP")) { std::printf("00c72620_TEST FAIL\n"); return 1; }
    if (!run(AL_PLAYING, true, "gsbSd")) { std::printf("00c72620_TEST FAIL\n"); return 1; }   // already suspended: stays suspended
    if (g_deleted != 4 || g_bufferCleared != 4) { std::printf("00c72620_TEST FAIL\n"); return 1; }
    std::printf("00c72620_TEST PASS\n");
    return 0;
}