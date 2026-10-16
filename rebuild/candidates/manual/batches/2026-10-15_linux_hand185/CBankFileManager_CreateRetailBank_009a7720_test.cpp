// Behaviour test for 009a7720: CBankFileManager::CreateRetailBank -- open for write, then magic + version + 0 + 0, close on scope exit.
#include <cstdio>
#include <cstring>
static unsigned char g_out[64]; static int g_len = 0, g_opens = 0, g_closes = 0, g_access = 0;
static const unsigned char g_magic[4] = { 'B', 'B', 'B', 'B' };
struct TFile { TFile() {} ~TFile() { ++g_closes; } void Open(const char*, int access, unsigned long) { ++g_opens; g_access = access; } };
struct TStream {
    TFile* f; TStream(TFile* file) : f(file) {}
    void Write(const void* d, long n) { std::memcpy(g_out + g_len, d, n); g_len += n; }
    void WriteULong(unsigned long v) { unsigned char b[4] = { (unsigned char)v, (unsigned char)(v >> 8), (unsigned char)(v >> 16), (unsigned char)(v >> 24) }; Write(b, 4); }
};
struct TManager { unsigned long FileVersion;
    void CreateRetailBank(const char* path) { TFile file; file.Open(path, 4, 2); TStream s(&file); s.Write(g_magic, 4); s.WriteULong(FileVersion); s.WriteULong(0); s.WriteULong(0); } };
int main()
{
    TManager m; m.FileVersion = 0x105; m.CreateRetailBank("x.big");
    if (g_len != 16 || std::memcmp(g_out, "BBBB", 4) || g_out[4] != 5 || g_out[5] != 1 || g_out[8] || g_out[12] || g_opens != 1 || g_closes != 1 || g_access != 4)
    { std::printf("009a7720_TEST FAIL\n"); return 1; }
    std::printf("009a7720_TEST PASS\n");
    return 0;
}
