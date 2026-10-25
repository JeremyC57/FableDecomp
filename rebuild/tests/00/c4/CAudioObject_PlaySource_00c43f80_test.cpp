// Behaviour test for 00c43f80: CAudioObject::PlaySource -- refused buffer, failed hardware buffer (error latched), queued buffer
#include <cstdio>
static int g_canUse = 1, g_enqueued = 0; static long g_hr = 0, g_error = 0; static unsigned __int64 g_time = 0;
struct CSoundBuffer { long CreateHardwareBuffer() { return g_hr; } };
struct CAudioSystem { void SetError(long code) { if (!g_error) g_error = code; } };
static CAudioSystem g_audioSystem;
struct CircularQueue { bool Enqueue(CSoundBuffer*, unsigned __int64 t) { ++g_enqueued; g_time = t; return true; } };
struct CAudioObject {
    CircularQueue m_queue;
    bool CanUseBuffer(CSoundBuffer*) { return g_canUse != 0; }
    bool PlaySource(CSoundBuffer* buffer, unsigned __int64 startTime)
    {
        if (CanUseBuffer(buffer)) {
            long hr = buffer->CreateHardwareBuffer();
            if (hr < 0) { g_audioSystem.SetError(hr); return false; }
            return m_queue.Enqueue(buffer, startTime);
        }
        return false;
    }
};
int main()
{
    CAudioObject o; CSoundBuffer b;
    int ok = 1;
    g_canUse = 0; ok &= !o.PlaySource(&b, 5) && g_enqueued == 0 && g_error == 0;
    g_canUse = 1; g_hr = -7; ok &= !o.PlaySource(&b, 5) && g_enqueued == 0 && g_error == -7;
    g_hr = 0; ok &= o.PlaySource(&b, 0x100000002ULL) && g_enqueued == 1 && g_time == 0x100000002ULL;
    std::printf(ok ? "00c43f80_TEST PASS\n" : "00c43f80_TEST FAIL\n");
    return ok ? 0 : 1;
}
