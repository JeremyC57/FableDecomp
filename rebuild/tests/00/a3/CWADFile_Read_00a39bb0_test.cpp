#include <cstdio>
#include <cstring>
static unsigned long g_index, g_offset, g_size; static void* g_buffer;
struct CBankFileAsync {
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size)
    { g_index = index; g_offset = offset; g_buffer = buffer; g_size = size; }
};
struct CWADFile { void* vt; CBankFileAsync* Bank; unsigned long Index; long FilePos;
    void Read(void* buffer, unsigned long size, unsigned long)
    {
        Bank->ReadEntryData(Index, FilePos, buffer, size);
        FilePos += size;
    }
};
int main()
{
    int ok = 1;
    CBankFileAsync bank; char buf[4];
    CWADFile f; f.vt = 0; f.Bank = &bank; f.Index = 9; f.FilePos = 100;
    f.Read(buf, 32, 77);
    if (g_index != 9 || g_offset != 100 || g_buffer != buf || g_size != 32 || f.FilePos != 132) ok = 0;
    f.Read(buf, 8, 0);
    if (g_offset != 132 || f.FilePos != 140) ok = 0;
    if (!ok) { std::printf("00a39bb0_TEST FAIL\n"); return 1; }
    std::printf("00a39bb0_TEST PASS\n");
    return 0;
}