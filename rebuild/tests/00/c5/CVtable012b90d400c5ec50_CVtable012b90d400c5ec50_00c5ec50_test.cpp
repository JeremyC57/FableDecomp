// Behaviour model of CSalKsPin::~CSalKsPin (0x00C5EC50): the empty derived destructor runs the CSalFile base destructor, which closes
// a valid handle once and leaves -1 alone.
#include <cstdio>
static int g_closed; static void* g_last;
static void CloseStub(void* h) { ++g_closed; g_last = h; }
class CSalFile { public: CSalFile() : m_hDevice((void*)-1) {} virtual ~CSalFile() { Close(); }
    void Close() { if (m_hDevice != (void*)-1) { CloseStub(m_hDevice); m_hDevice = (void*)-1; } } void* m_hDevice; };
class CSalKsPin : public CSalFile { public: virtual ~CSalKsPin() {} };
int main()
{
    int h; { CSalKsPin p; p.m_hDevice = &h; }
    if (g_closed != 1 || g_last != &h) { std::printf("00c5ec50_TEST FAIL\n"); return 1; }
    { CSalKsPin p; }
    if (g_closed != 1) { std::printf("00c5ec50_TEST FAIL\n"); return 1; }
    std::printf("00c5ec50_TEST PASS\n"); return 0;
}
