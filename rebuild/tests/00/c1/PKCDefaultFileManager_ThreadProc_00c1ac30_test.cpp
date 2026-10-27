#include <cstdio>

// Behaviour model of PKCDefaultFileManager::ThreadProc (0x00C1AC30): run the manager's read loop, exit code 0.
static int g_runs = 0;
class PKCDefaultFileManager {
public:
    int m_id;
    void AsyncReadThread() { g_runs += m_id; }
    static unsigned int ThreadProc(void* manager) { ((PKCDefaultFileManager*)manager)->AsyncReadThread(); return 0; }
};

int main()
{

    PKCDefaultFileManager m; m.m_id = 5;
    if (PKCDefaultFileManager::ThreadProc(&m) != 0) { std::printf("00c1ac30_TEST FAIL\n"); return 1; }
    if (g_runs != 5) { std::printf("00c1ac30_TEST FAIL\n"); return 1; }
    std::printf("00c1ac30_TEST PASS\n");
    return 0;
}