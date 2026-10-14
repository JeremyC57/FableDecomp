#include <cstdio>
static int g_seek = 0, g_flush = 0, g_close = 0; static void* g_closed = 0; static unsigned long g_method = 99;
static unsigned long __stdcall SetFilePointer(void* file, long distance, long* high, unsigned long method) { ++g_seek; g_method = method; return 0; }
static int __stdcall FlushFileBuffers(void* file) { ++g_flush; return 1; }
static int __stdcall CloseHandle(void* object) { ++g_close; g_closed = object; return 1; }
struct CDiskFileWin32 {
    virtual void v00() { }
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened;
    virtual void Close() { if (Opened) { SetFilePointer(WinFileHandle, 0, 0, 1); FlushFileBuffers(WinFileHandle); CloseHandle(WinFileHandle); } Opened = false; }
};
int main()
{
    CDiskFileWin32 f; f.WinFileHandle = (void*)0x77; f.Opened = false;
    f.Close();
    if (g_seek || g_flush || g_close || f.Opened) { std::printf("0099a920_TEST FAIL\n"); return 1; }
    f.Opened = true; f.Close();
    if (g_seek != 1 || g_method != 1 || g_flush != 1 || g_close != 1 || g_closed != (void*)0x77 || f.Opened) { std::printf("0099a920_TEST FAIL\n"); return 1; }
    std::printf("0099a920_TEST PASS\n"); return 0;
}
