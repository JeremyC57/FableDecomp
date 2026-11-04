// Behaviour test for CSoundEvent::Play (0x00C21780): the source's logic over stub callees that log each step, run through
// every exit (status 0 / 2, cached buffer with and without a free player, no memory, streamed ok / failed, loaded ok / failed).
#include <cstdio>
#include <cstring>
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_buffers = 0;

struct CSoundEventManager;
struct CStreamBuffer;
struct CPhysicalPlayer { };
static CPhysicalPlayer g_player;
struct CSoundSample {
    unsigned short m_status;
    long streamed, streamResult, loadResult;
    long StartStreaming(CSoundEventManager*, CStreamBuffer*) { Log("stream"); return streamResult; }
    long IsStreamed() { return streamed; }
    long GetSize() { return 100; }
    long LoadData(CSoundEventManager*, CStreamBuffer*) { Log("load"); return loadResult; }
};
struct CSampleRef { CSoundSample* m_pSample; CSoundSample* GetSample() { return m_pSample; } };
struct CStreamBuffer {
    CStreamBuffer() { ++g_buffers; }
    ~CStreamBuffer() { --g_buffers; }
    void Attach(CSoundEventManager*, CSoundSample*, long, long) { Log("attach"); }
    long FinishLoading(CSoundEventManager*, long success) { Log(success ? "finish1" : "finish0"); return success ? 0 : -1; }
    void Release(CSoundEventManager*) { Log("release"); }
};
static CStreamBuffer g_cached;
struct CSoundEventManager {
    bool haveCached, havePlayer; long handle;
    CStreamBuffer* Find(CSoundSample**) { return haveCached ? &g_cached : 0; }
    void* GetPhysicalPlayer(unsigned long) { return havePlayer ? &g_player : 0; }
    long AllocateHandle(CSoundEventManager*, long, long) { return handle; }
};
struct CSoundEvent {
    long m_priority;
    CSampleRef m_sample;
    void CheckState(long) { }
    void WaitForData(CSoundEventManager*) { Log("waitdata"); }
    void WaitForMemory(CSoundEventManager*) { Log("waitmem"); }
    void WaitForPhysicalPlayer(CSoundEventManager*, CStreamBuffer*) { Log("waitplayer"); }
    void PlayPhysical(CSoundEventManager*, CStreamBuffer*, CPhysicalPlayer*) { Log("play"); }
    void Stop(CSoundEventManager*) { Log("stop"); }
    void Play(CSoundEventManager* manager)
    {
        CheckState(1);
        if (m_sample.GetSample()->m_status == 0) { Stop(manager); return; }
        if (m_sample.GetSample()->m_status == 2) { WaitForData(manager); return; }
        CSoundSample* sample = m_sample.GetSample();
        CStreamBuffer* cached = manager->Find(&sample);
        if (cached) {
            CPhysicalPlayer* player = (CPhysicalPlayer*)manager->GetPhysicalPlayer(m_priority);
            if (player) PlayPhysical(manager, cached, player); else WaitForPhysicalPlayer(manager, cached);
            return;
        }
        CSoundSample* toSize = m_sample.GetSample();
        long handle = manager->AllocateHandle(manager, toSize->GetSize(), m_priority);
        if (handle < 0) { WaitForMemory(manager); return; }
        CStreamBuffer* buffer = new CStreamBuffer;
        CSoundSample* toAttach = m_sample.GetSample();
        buffer->Attach(manager, toAttach, handle, m_priority);
        if (m_sample.GetSample()->IsStreamed()) {
            if (m_sample.GetSample()->StartStreaming(manager, buffer) < 0) {
                buffer->FinishLoading(manager, 0);
                buffer->Release(manager); delete buffer; Stop(manager); return;
            }
            m_sample.GetSample()->m_status = 2;
            WaitForData(manager);
            return;
        }
        if (buffer->FinishLoading(manager, m_sample.GetSample()->LoadData(manager, buffer) >= 0) < 0) {
            m_sample.GetSample()->m_status = 0;
            buffer->Release(manager); delete buffer; Stop(manager); return;
        }
        CPhysicalPlayer* player = (CPhysicalPlayer*)manager->GetPhysicalPlayer(m_priority);
        if (player) PlayPhysical(manager, buffer, player); else WaitForPhysicalPlayer(manager, buffer);
    }
};
static const char* Run(unsigned short status, bool cached, bool player, long handle, long streamed, long streamResult, long loadResult, unsigned short* after)
{
    g_log[0] = 0;
    CSoundSample s = { status, streamed, streamResult, loadResult };
    CSoundEventManager m = { cached, player, handle };
    CSoundEvent e; e.m_priority = 3; e.m_sample.m_pSample = &s;
    e.Play(&m);
    if (after) *after = s.m_status;
    return g_log;
}
static void RunTest()
{
    unsigned short st = 0;
    g_buffers = 0;                                               // the static cached buffer counted itself at start-up
    CHECK(std::strcmp(Run(0, false, false, 0, 0, 0, 0, 0), "stop;") == 0);
    CHECK(std::strcmp(Run(2, false, false, 0, 0, 0, 0, 0), "waitdata;") == 0);
    CHECK(std::strcmp(Run(1, true, true, 0, 0, 0, 0, 0), "play;") == 0);
    CHECK(std::strcmp(Run(1, true, false, 0, 0, 0, 0, 0), "waitplayer;") == 0);
    CHECK(std::strcmp(Run(1, false, false, -1, 0, 0, 0, 0), "waitmem;") == 0);
    CHECK(std::strcmp(Run(1, false, false, 5, 1, 0, 0, &st), "attach;stream;waitdata;") == 0 && st == 2 && g_buffers == 0 + 1);
    g_buffers = 0;
    CHECK(std::strcmp(Run(1, false, false, 5, 1, -1, 0, &st), "attach;stream;finish0;release;stop;") == 0 && st == 1 && g_buffers == 0);
    CHECK(std::strcmp(Run(1, false, true, 5, 0, 0, 0, &st), "attach;load;finish1;play;") == 0 && st == 1);
    g_buffers = 0;
    CHECK(std::strcmp(Run(1, false, true, 5, 0, 0, -1, &st), "attach;load;finish0;release;stop;") == 0 && st == 0 && g_buffers == 0);
}
int main()
{
    RunTest();
    if (g_fail) { std::printf("00c21780_TEST FAIL\n"); return 1; }
    std::printf("00c21780_TEST PASS\n");
    return 0;
}