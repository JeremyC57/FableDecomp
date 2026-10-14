#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SFileData { unsigned char _pad[4]; long m_mode; unsigned char _pad08[8]; unsigned long m_size; unsigned char _pad14[5]; bool m_open; };
struct CLHFile { SFileData* m_pData; void* m_pCount;
    bool GetSize(unsigned long* size) { SFileData* d = m_pData; if (d && d->m_open) { *size = d->m_size; return true; } return false; }
    long GetMode() { SFileData* d = m_pData; if (d && d->m_open) { if (d->m_mode == 0) return 0x200; if (d->m_mode == 1) return 0x800; } return 1; } };

int main()
{
    SFileData d; d.m_size = 1234; d.m_open = true; CLHFile f; f.m_pData = &d; unsigned long s = 0;
    if (!f.GetSize(&s) || s != 1234) { std::printf("00add160_TEST FAIL\n"); return 1; } d.m_open = false; s = 0; if (f.GetSize(&s) || s != 0) { std::printf("00add160_TEST FAIL\n"); return 1; } f.m_pData = 0; if (f.GetSize(&s)) { std::printf("00add160_TEST FAIL\n"); return 1; }
    std::printf("00add160_TEST PASS\n");
    return 0;
}