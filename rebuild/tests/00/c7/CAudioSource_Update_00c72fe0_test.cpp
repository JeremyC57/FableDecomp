#include <cstdio>
// CAudioSource::Update: first tick restarts + takes the start time; properties re-applied each tick; 1 while waiting
// for the start time; then QueueNextBuffer(force): 0 when it returned nonzero, 2 when it returned 0.
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct Sound { i64 firstTime; int queueResult; int queueCalls; };
struct Source {
    int m_started; i64 m_startTime; int restarts; int applies;
    void Restart() { ++restarts; }
    void BeginSound(Sound* s) { m_startTime = s->firstTime; m_started = 1; }
    void ApplySound(Sound* s, i64 now) { ++applies; }
    int QueueNextBuffer(Sound* s, i64 now, bool force) { ++s->queueCalls; return force ? s->queueResult : -99; }
    int Update(Sound* sound, i64 now)
    {
        if (!m_started) { Restart(); BeginSound(sound); }
        ApplySound(sound, now);
        if (m_startTime > now) return 1;
        return QueueNextBuffer(sound, now, true) ? 0 : 2;
    }
};
int main()
{
    Sound snd; snd.firstTime = (i64)3 << 32; snd.queueResult = 0; snd.queueCalls = 0;
    Source src; src.m_started = 0; src.m_startTime = 0; src.restarts = 0; src.applies = 0;
    if (src.Update(&snd, 5) != 1 || src.restarts != 1 || src.applies != 1 || snd.queueCalls != 0) { std::printf("00c72fe0_TEST FAIL\n"); return 1; }
    if (src.Update(&snd, ((i64)3 << 32)) != 2 || src.restarts != 1 || snd.queueCalls != 1) { std::printf("00c72fe0_TEST FAIL\n"); return 1; }
    snd.queueResult = 0x80000001;
    if (src.Update(&snd, ((i64)4 << 32)) != 0 || src.applies != 3) { std::printf("00c72fe0_TEST FAIL\n"); return 1; }
    std::printf("00c72fe0_TEST PASS\n");
    return 0;
}