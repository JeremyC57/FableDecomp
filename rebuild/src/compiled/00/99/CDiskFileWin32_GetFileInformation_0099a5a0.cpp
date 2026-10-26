// CDiskFileWin32::GetFileInformation() -- return the open file's creation / last-access / last-write times and read-only flag (all zero when the handle query fails)
// Leaf: ?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A5A0, length 222 bytes, `ret 4` (hidden result).
// Callees: CDateAndTime::CDateAndTime() @ 0x009E9AB0, CDateAndTime::Initialise(const _FILETIME*) @ 0x009E9CB0,
// GetFileInformationByHandle (KERNEL32 import @ 0x0143FDAC).
// Layout: engine/CDiskFileWin32.h (WinFileHandle +0x10); CFileInformation (Ego_r.pdb, 0x58 bytes): CreationTime +0,
// LastAccessTime +0x1c, LastWriteTime +0x38 (CDateAndTime, 7 dwords each), ReadOnly +0x54. BY_HANDLE_FILE_INFORMATION is
// the Win32 record (attributes +0, the three FILETIMEs +4/+0xc/+0x14), declared here without <windows.h>.
// Shape: the local CFileInformation default-constructs its three times; on success each is Initialise()d in place from
// the FILETIMEs and ReadOnly takes the attribute bit (`and dl,1`); on failure each is assigned a default-constructed
// temporary (implicit `operator=` = `rep movsd` of 7 dwords) and ReadOnly cleared; the result is copied out as one
// 0x16-dword block (implicit copy constructor, no NRVO).
struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
struct BY_HANDLE_FILE_INFORMATION {
    unsigned long dwFileAttributes;                // +0x00
    _FILETIME ftCreationTime;                      // +0x04
    _FILETIME ftLastAccessTime;                    // +0x0c
    _FILETIME ftLastWriteTime;                     // +0x14
    unsigned long dwVolumeSerialNumber;
    unsigned long nFileSizeHigh;
    unsigned long nFileSizeLow;
    unsigned long nNumberOfLinks;
    unsigned long nFileIndexHigh;
    unsigned long nFileIndexLow;
};
extern "C" __declspec(dllimport) int __stdcall GetFileInformationByHandle(void* file, BY_HANDLE_FILE_INFORMATION* info);

class CDateAndTime { public:
    long Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime();                                // 0x009E9AB0
    void Initialise(const _FILETIME* time);        // 0x009E9CB0
};
class CFileInformation { public:
    CDateAndTime CreationTime;                     // +0x00
    CDateAndTime LastAccessTime;                   // +0x1c
    CDateAndTime LastWriteTime;                    // +0x38
    bool ReadOnly;                                 // +0x54
};

class CDiskFileWin32 { public:                            // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    void*       Pathname;                          // +0x18 CWideString
    virtual CFileInformation GetFileInformation();
};

CFileInformation CDiskFileWin32::GetFileInformation()
{
    CFileInformation info;
    BY_HANDLE_FILE_INFORMATION data;
    if (GetFileInformationByHandle(WinFileHandle, &data)) {
        info.CreationTime.Initialise(&data.ftCreationTime);
        info.LastAccessTime.Initialise(&data.ftLastAccessTime);
        info.LastWriteTime.Initialise(&data.ftLastWriteTime);
        info.ReadOnly = (data.dwFileAttributes & 1) != 0;
    } else {
        info.CreationTime = CDateAndTime();
        info.LastAccessTime = CDateAndTime();
        info.LastWriteTime = CDateAndTime();
        info.ReadOnly = false;
    }
    return info;
}