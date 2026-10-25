// Behaviour test for AsyncThreadProc @ 0x00C2A210 (run the launcher until it reports completion, then release its semaphore).
#include <cstdio>

class CSemaphoreHandle {
public:
    int releases;
    void Release() { ++releases; }
};

class CAsyncThreadLauncher {
public:
    int calls, finishAfter;
    CSemaphoreHandle m_finished;
    virtual ~CAsyncThreadLauncher() { }
    virtual int Process() { ++calls; return calls >= finishAfter ? 1 : 0; }
};

unsigned long AsyncThreadProc(void* parameter)
{
    CAsyncThreadLauncher* launcher = (CAsyncThreadLauncher*)parameter;
    while (!launcher->Process())
        ;
    launcher->m_finished.Release();
    return 0;
}

int main()
{
    CAsyncThreadLauncher l;
    l.calls = 0; l.finishAfter = 5; l.m_finished.releases = 0;
    bool ok = AsyncThreadProc(&l) == 0 && l.calls == 5 && l.m_finished.releases == 1;
    CAsyncThreadLauncher m;
    m.calls = 0; m.finishAfter = 1; m.m_finished.releases = 0;
    ok = ok && AsyncThreadProc(&m) == 0 && m.calls == 1 && m.m_finished.releases == 1;
    if (!ok) { std::printf("00c2a210_TEST FAIL\n"); return 1; }
    std::printf("00c2a210_TEST PASS\n");
    return 0;
}