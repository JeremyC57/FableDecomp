// CDiskFileWin32::Close() -- when open: rewind the Win32 file pointer, flush and close the handle; always clear Opened
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A920, length 64 bytes.
// Callees: KERNEL32 SetFilePointer (IAT 0x0143FDE8), FlushFileBuffers (IAT 0x0143FDA8), CloseHandle (IAT 0x0143FE30), each
// through its import slot (`call dword ptr [__imp]`). Virtual (vtable 0x0122D06C). PDB CDiskFileWin32: WinFileHandle +0x10,
// bool Opened +0x14.
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* file, long distance, long* distanceHigh, unsigned long moveMethod);   // IAT 0x0143FDE8
extern "C" __declspec(dllimport) int __stdcall FlushFileBuffers(void* file);   // IAT 0x0143FDA8
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* object);      // IAT 0x0143FE30

struct CDiskFileWin32 {
    virtual void v00();
    long  AccessType;                  // +0x04 FileAccessType
    long  Position;                    // +0x08
    long  Length;                      // +0x0c
    void* WinFileHandle;               // +0x10
    bool  Opened;                      // +0x14
    virtual void Close();
};

void CDiskFileWin32::Close()
{
    if (Opened) {
        SetFilePointer(WinFileHandle, 0, 0, 1);
        FlushFileBuffers(WinFileHandle);
        CloseHandle(WinFileHandle);
    }
    Opened = false;
}
