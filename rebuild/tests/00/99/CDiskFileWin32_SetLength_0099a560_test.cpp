#include <cstdio>
static int g_calls = 0; static void* g_h[2]; static long g_dist[2]; static unsigned long g_method[2];
static unsigned long __stdcall SetFilePointer(void* file, long distance, long* high, unsigned long method) { if (g_calls < 2) { g_h[g_calls] = file; g_dist[g_calls] = distance; g_method[g_calls] = method; } ++g_calls; return 0; }
struct CDiskFileWin32 {
    virtual void v00() { }
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened;
    virtual void SetLength(long length) { if (length > Length) { Length = length; SetFilePointer(WinFileHandle, length, 0, 0); SetFilePointer(WinFileHandle, Position, 0, 0); } }
};
int main()
{
    CDiskFileWin32 f; f.Position = 10; f.Length = 100; f.WinFileHandle = (void*)0x1234;
    f.SetLength(50);
    if (g_calls != 0 || f.Length != 100) { std::printf("0099a560_TEST FAIL\n"); return 1; }
    f.SetLength(200);
    if (g_calls != 2 || f.Length != 200 || g_h[0] != (void*)0x1234 || g_dist[0] != 200 || g_method[0] != 0 || g_h[1] != (void*)0x1234 || g_dist[1] != 10 || g_method[1] != 0) { std::printf("0099a560_TEST FAIL\n"); return 1; }
    std::printf("0099a560_TEST PASS\n"); return 0;
}