// Behaviour model of CSalFile::~CSalFile (0x00C5EBD0): a valid handle is closed once and reset; INVALID_HANDLE_VALUE (-1) is left alone.
#include <cstdio>
static int g_closed; static void* g_last;
static void CloseStub(void* h) { ++g_closed; g_last = h; }
class CSalFile { public: CSalFile() : m_hDevice((void*)-1) {} virtual ~CSalFile() { Close(); }
    void Close() { if (m_hDevice != (void*)-1) { CloseStub(m_hDevice); m_hDevice = (void*)-1; } } void* m_hDevice; };
int main()
{
    int h; { CSalFile f; f.m_hDevice = &h; }
    if (g_closed != 1 || g_last != &h) { std::printf("00c5ebd0_TEST FAIL\n"); return 1; }
    { CSalFile f; }
    if (g_closed != 1) { std::printf("00c5ebd0_TEST FAIL\n"); return 1; }
    std::printf("00c5ebd0_TEST PASS\n"); return 0;
}
