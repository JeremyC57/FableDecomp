// Behaviour test for 00c61fb0: CPKVoiceHandle::Release -- a live handle is released through the table and invalidated
#include <cstdio>
static long g_released = 0; static int g_calls = 0; static void* g_pEmitterSeen = 0;
class CPKEmitter { public: int tag; };
class CPKVoiceTable { public: virtual void Release(long handle) { ++g_calls; g_released = handle; } };
static CPKVoiceTable g_table;
class CPKVoiceHandle {
public:
    long m_handle;
    CPKVoiceTable* GetTable(CPKEmitter* emitter) { g_pEmitterSeen = emitter; return &g_table; }
    void Release(CPKEmitter* emitter) { long handle = m_handle; if (handle >= 0) { GetTable(emitter)->Release(handle); m_handle = -1; } }
};
int main()
{
    CPKEmitter e; CPKVoiceHandle h; h.m_handle = 7; h.Release(&e);
    int ok = g_calls == 1 && g_released == 7 && g_pEmitterSeen == &e && h.m_handle == -1;
    h.Release(&e); ok = ok && g_calls == 1;
    std::printf(ok ? "00c61fb0_TEST PASS\n" : "00c61fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
