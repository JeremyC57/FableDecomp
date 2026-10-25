// Behaviour test for 00c29fb0: PKCSemaphore::~PKCSemaphore -- closes the handle word
#include <cstdio>
static int g_closed = 0; static void* g_last = 0;
static int CloseHandle(void* handle) { ++g_closed; g_last = handle; return 1; }
class PKCSemaphore { public: PKCSemaphore(void* h) : m_hSemaphore(h) { } ~PKCSemaphore() { CloseHandle(m_hSemaphore); } private: void* m_hSemaphore; };
static bool Check() { { PKCSemaphore s((void*)0x1234); } return g_closed == 1 && g_last == (void*)0x1234 && sizeof(PKCSemaphore) == 4; }
int main()
{
    if (!Check()) { std::printf("00c29fb0_TEST FAIL\n"); return 1; }
    std::printf("00c29fb0_TEST PASS\n");
    return 0;
}
