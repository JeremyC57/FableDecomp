#include <cstdio>
#include <cstdlib>
#include <cstring>
static unsigned long g_size = 0; static int g_value = -1; struct CVec { unsigned char* a; unsigned char* b; unsigned char* c; void resize(unsigned long n, const unsigned char& v) { g_size = n; g_value = v; } };
struct CMemoryFile { unsigned char _pad[0xc]; long Length; CVec m_buffer; void SetLength(long n) { if (n > Length) { Length = n; m_buffer.resize(n, 0); } } };

int main() {
    CMemoryFile f; f.Length = 10; f.SetLength(5); if (g_size != 0 || f.Length != 10) { std::printf("009e9730_TEST FAIL\n"); return 1; } f.SetLength(20); if (g_size != 20 || g_value != 0 || f.Length != 20) { std::printf("009e9730_TEST FAIL\n"); return 1; }
    std::printf("009e9730_TEST PASS\n"); return 0;
}
