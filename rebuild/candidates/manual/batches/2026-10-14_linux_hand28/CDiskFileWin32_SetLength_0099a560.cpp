// CDiskFileWin32::SetLength(long) -- grow the file: move the Win32 file pointer to the new length, then back to the current position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A560, length 64 bytes.
// Callees: KERNEL32 SetFilePointer (IAT 0x0143FDE8), used twice so it is loaded into edi once (`__declspec(dllimport)`).
// Virtual (vtable 0x0122D06C). PDB CDiskFileWin32: Position +8, Length +0xc, WinFileHandle +0x10.
// Shape: `cmp eax,[esi+0Ch] ; jle` skips when the new length is not larger; the Length store is scheduled between the
// first call's pushes.
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* file, long distance, long* distanceHigh, unsigned long moveMethod);   // IAT 0x0143FDE8

struct CDiskFileWin32 {
    virtual void v00();
    long  AccessType;                  // +0x04 FileAccessType
    long  Position;                    // +0x08
    long  Length;                      // +0x0c
    void* WinFileHandle;               // +0x10
    bool  Opened;                      // +0x14
    virtual void SetLength(long length);
};

void CDiskFileWin32::SetLength(long length)
{
    if (length > Length) {
        Length = length;
        SetFilePointer(WinFileHandle, length, 0, 0);
        SetFilePointer(WinFileHandle, Position, 0, 0);
    }
}
