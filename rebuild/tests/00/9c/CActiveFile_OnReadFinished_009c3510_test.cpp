// Behaviour test for 0x009C3510 CActiveFile::OnReadFinished (NF415: linked): links the row object (the method is declared here, not
// defined). A valid handle is closed through KERNEL32!CloseHandle (the import the row calls) and reset to -1; -1 is left alone.
#include <cstdio>

extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void* attributes, int manualReset, int initialState, const char* name);
extern "C" __declspec(dllimport) int __stdcall GetHandleInformation(void* handle, unsigned long* flags);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);
struct CActiveFile
{
    void* handle;   // +0x00
    void OnReadFinished();                              // the row (linked)
};

int main()
{
    void* h = CreateEventA(0, 1, 0, 0);
    unsigned long flags = 0;
    bool ok = h != 0 && GetHandleInformation(h, &flags) != 0;          // open before
    CActiveFile f; f.handle = h;
    f.OnReadFinished();
    ok = ok && f.handle == (void*)-1 && GetHandleInformation(h, &flags) == 0;   // closed by the row, field reset
    f.OnReadFinished();                                                 // -1: nothing to close
    ok = ok && f.handle == (void*)-1;
    if (!ok && h && GetHandleInformation(h, &flags)) CloseHandle(h);
    std::printf(ok ? "ONREADFINISHED_OK\n" : "ONREADFINISHED_FAIL\n");
    return ok ? 0 : 1;
}
