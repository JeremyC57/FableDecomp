// Behaviour model of CSalFile::Close (0x00C5E3D0): a valid handle is closed once and reset to -1; an INVALID_HANDLE_VALUE (-1) handle is
// left alone. The CloseHandle import is a stub here.
#include <cstdio>
static int g_closed; static void* g_last;
static void CloseStub(void* h) { ++g_closed; g_last = h; }
class CSalFile { public: void* m_hDevice; void Close() { if (m_hDevice != (void*)-1) { CloseStub(m_hDevice); m_hDevice = (void*)-1; } } };
int main()
{
    int h; CSalFile f; f.m_hDevice = &h;
    f.Close();
    if (g_closed != 1 || g_last != &h || f.m_hDevice != (void*)-1) { std::printf("CLEARTEX_BAD 1\n"); return 1; }
    f.Close();
    if (g_closed != 1) { std::printf("CLEARTEX_BAD 2\n"); return 1; }
    std::printf("CLEARTEX_OK\n");
    return 0;
}
