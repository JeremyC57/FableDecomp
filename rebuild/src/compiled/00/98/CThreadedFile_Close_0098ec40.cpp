// CThreadedFile::Close() -- if the file handle is valid, makes the threaded-file manager drop requests still pending for this file, closes the handle, resets it to INVALID_HANDLE_VALUE and clears the open flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098EC40, length 47 bytes.
// Callees: the threaded-file manager's pending-request purge @ 0x0098E7D0 (manifest
// "CThreadedFileManager_ProcessPendingRequests") on the global manager g_ThreadedFileManager @ 0x013BC870,
// CloseHandle (IAT 0x0143FE30).
// Layout: PDB CThreadedFile (28 bytes): DeviceID +0x04, FileHandle +0x08, Filename (CWideString) +0x0c,
// Length +0x10, PhysicalSortKey +0x14, OpenedForWrite +0x18, OpenFlag +0x19 (no engine header).
// Shape: void member; the two flag clears share one `xor al,al` zero register (OpenFlag stored first,
// so it is assigned first in the source); the same body is inlined into the deleting destructor 0x00991450.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);   // IAT 0x0143FE30

struct CThreadedFile;

struct CThreadedFileManager {
    void RemovePendingRequests(CThreadedFile* file);   // 0x0098E7D0
};
extern CThreadedFileManager g_ThreadedFileManager;    // 0x013BC870

struct CWideString {
    void* m_pData;
};

struct CThreadedFile {
    void*         m_vtable;                        // +0x00
    long          DeviceID;                        // +0x04 EThreadedFileDeviceID
    void*         FileHandle;                      // +0x08
    CWideString   Filename;                        // +0x0c
    unsigned long Length;                          // +0x10
    unsigned long PhysicalSortKey;                 // +0x14
    bool          OpenedForWrite;                  // +0x18
    bool          OpenFlag;                        // +0x19
    void Close();
};

void CThreadedFile::Close()
{
    if (FileHandle != (void*)-1) {
        g_ThreadedFileManager.RemovePendingRequests(this);
        CloseHandle(FileHandle);
        FileHandle = (void*)-1;
        OpenFlag = false;
        OpenedForWrite = false;
    }
}