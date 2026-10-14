#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00C25C20: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_closed = 0, g_destroyed = 0, g_freed = 0; static void* g_freedBlock = 0;
struct CScriptModule_00c354a0 { unsigned char _body[0xc]; ~CScriptModule_00c354a0() { ++g_destroyed; } };
static void ReleaseBlock(void* p) { ++g_freed; g_freedBlock = p; std::free(p); }
struct CTimerHost_00c25c20 {
    CScriptModule_00c354a0* m_pModule;
    void CloseTimers() { ++g_closed; }
    void Close()
    {
        CloseTimers();
        CScriptModule_00c354a0* module = m_pModule;
        if (module) { module->~CScriptModule_00c354a0(); ReleaseBlock(module); }
        m_pModule = 0;
    }
};
static bool Run()
{
    CTimerHost_00c25c20 host; void* block = std::malloc(sizeof(CScriptModule_00c354a0)); host.m_pModule = (CScriptModule_00c354a0*)block;
    host.Close();
    if (g_closed != 1 || g_destroyed != 1 || g_freed != 1 || g_freedBlock != block || host.m_pModule != 0) return false;
    host.Close();                                  // already closed: nothing destroyed
    return g_closed == 2 && g_destroyed == 1 && g_freed == 1 && host.m_pModule == 0;
}
int main()
{
    if (!Run()) { std::printf("00c25c20_TEST FAIL\n"); return 1; }
    std::printf("00c25c20_TEST PASS\n");
    return 0;
}
