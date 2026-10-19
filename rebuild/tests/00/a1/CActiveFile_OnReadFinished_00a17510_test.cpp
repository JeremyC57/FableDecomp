#include <cstdio>
#include <new>

struct CActiveFile {
    void* m_data;
    void Finish();
    void OnReadFinished();
};

static int g_finishCalled = 0;
static void* g_lastFreed = (void*)0;

// definition for the decl-only helper referenced by the source
void CActiveFile::Finish() { g_finishCalled++; }

// observe the free (retail calls the MSVCR71 free thunk 0x00BFEA14): recorded only while armed
// around the call under test; any other free (CRT start-up/exit) is a harmless leak
static int g_armed = 0;
extern "C" void __cdecl free(void* p) { if (g_armed) g_lastFreed = p; }

int main()
{
    // case 1: non-null m_data -> Finish runs, pointer freed
    CActiveFile a;
    static char block[32];
    void* blk = block;
    a.m_data = blk;
    g_finishCalled = 0; g_lastFreed = (void*)0;
    g_armed = 1; a.OnReadFinished(); g_armed = 0;

    bool ok1 = (g_finishCalled == 1) && (g_lastFreed == blk);

    // case 2: null m_data -> Finish runs, no free
    CActiveFile b;
    b.m_data = (void*)0;
    g_finishCalled = 0; g_lastFreed = (void*)0;
    g_armed = 1; b.OnReadFinished(); g_armed = 0;

    bool ok2 = (g_finishCalled == 1) && (g_lastFreed == (void*)0);

    if (ok1 && ok2) {
        std::printf("ONREADFINISHED_OK\n");
        return 0;
    }
    std::printf("FAIL c1=%d c2=%d\n", (int)ok1, (int)ok2);
    return 1;
}