// Grade: DIFFER(299v305) -- only the dead `mov al/cl,[eax+7Ah]` (ExitThread read between the two Get() calls) in both inlined SetDirtyDiscError copies is missing; everything else matches
// CFileInstaller::StartWritingFile(CActiveFile& file, unsigned long outputSize) -- with the installer lock dropped: create the destination's directory, read the disk's sector size, open the destination for overlapped unbuffered writing, stamp the start time and pre-size the file to the sector-rounded output size, flagging a dirty-disc error when the open or the resize fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3660, length 305 bytes (+ int3 padding), `ret 8`.
// Signature from Ego_r.pdb: ?StartWritingFile@CFileInstaller@@IAEXAAVCActiveFile@1@K@Z.
// Callees: CWideString copy ctor @ 0x0099B720 / ~CWideString @ 0x0099B510 / operator const wchar_t* @ 0x0099AF00 (manifest
//   "CCharString::operator char const*" is a propagated byte twin), CreateDirectoryForFile(const CWideString&) @ 0x00999620 and
//   GetDiskSectorSize(const CWideString&) @ 0x00999180 (both __fastcall), GFGetTime @ 0x009E1BC0, the installer singleton
//   CFileInstallerSingleton::Get @ 0x00404440 (__fastcall), CCriticalSectionLock(section, false) @ 0x00A62920 /
//   ~CCriticalSectionLock @ 0x00A629A0; KERNEL32 LeaveCriticalSection (IAT 0x0143FE10), CreateFileW (0x0143FE2C),
//   SetFilePointer (0x0143FDE8), SetEndOfFile (0x0143FDEC), GetFileSize (0x0143FDF0), EnterCriticalSection (0x0143FE0C).
// Global: the dirty-disc error byte @ 0x013CA814 (the landed Pause 0x009C31E0 tests it).
// Layout: retail CFileInstaller (landed Pause): CriticalSection +0x04, ExitThread +0x7a (PDB +0x9a less 0x20);
//   CActiveFile (PDB, the 200-byte installer copy): DstFileHandle +4, OutputSize +0x18, PFileInfo (counted pointer, object
//   first) +0x1c, MinWriteSize +0x24, StartTime +0x30; CFileInfo: DstPath +4.
// Shape: the static `SetDirtyDiscError(bool)` (PDB ?SetDirtyDiscError@CFileInstaller@@KIX_N@Z) is inlined twice with
//   the constant true held in bl: a scoped lock on the singleton's section, the global store, then
//   `ExitThread = ExitThread || error` through two Get() calls (the dead byte load is the `||` operand). The path copy is
//   homed in the dead `file` argument slot; the rounding divides by the re-read sector size.
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const unsigned short* name, unsigned long access, unsigned long share,
                                                           void* security, unsigned long disposition, unsigned long flags,
                                                           void* templateFile);   // IAT 0x0143FE2C
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* file, long distance, long* distanceHigh,
                                                                      unsigned long method);   // IAT 0x0143FDE8
extern "C" __declspec(dllimport) int __stdcall SetEndOfFile(void* file);                        // IAT 0x0143FDEC
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void* file, unsigned long* sizeHigh);   // IAT 0x0143FDF0

struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    operator const unsigned short*() const;             // 0x0099AF00
};
void __fastcall CreateDirectoryForFile(const CWideString& path);   // 0x00999620
unsigned long __fastcall GetDiskSectorSize(const CWideString& path);   // 0x00999180
double __fastcall GFGetTime();                          // 0x009E1BC0

struct SCriticalSection { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
struct CCriticalSectionLock {                           // 8 bytes on the stack
    SCriticalSection* m_pSection;
    bool              m_locked;
    CCriticalSectionLock(SCriticalSection* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                            // 0x00A629A0
};
extern bool g_bDirtyDiscError;                          // 0x013CA814

struct CFileInstaller;
struct CFileInstallerSingleton {
    static CFileInstaller& __fastcall Get();            // 0x00404440
};

struct CFileInstaller {
    struct CFileInfo {
        CWideString SrcPath;                            // +0x00
        CWideString DstPath;                            // +0x04
    };
    struct CActiveFile {
        void*         SrcFileHandle;                    // +0x00
        void*         DstFileHandle;                    // +0x04
        unsigned long CurrentSrcOffset;                 // +0x08
        unsigned long NoBytesRead;                      // +0x0c
        unsigned long NoBytesWritten;                   // +0x10
        unsigned long InputSize;                        // +0x14
        unsigned long OutputSize;                       // +0x18
        CFileInfo*    PFileInfo;                        // +0x1c CCountedPointer<const CFileInfo>: object
        void*         PFileInfoCount;                   // +0x20 .. and count block
        unsigned long MinWriteSize;                     // +0x24
        unsigned long MinReadSize;                      // +0x28
        unsigned long _pad2c;
        double        StartTime;                        // +0x30
    };

    void*            _vptr;                             // +0x00
    SCriticalSection CriticalSection;                   // +0x04
    unsigned char    _pad1c[0x7a - 0x1c];
    bool             ExitThread;                        // +0x7a

    static void SetDirtyDiscError(bool error)
    {
        CCriticalSectionLock lock(&CFileInstallerSingleton::Get().CriticalSection, false);
        g_bDirtyDiscError = error;
        CFileInstallerSingleton::Get().ExitThread = CFileInstallerSingleton::Get().ExitThread || error;
    }
    void StartWritingFile(CActiveFile& file, unsigned long outputSize);
};

void CFileInstaller::StartWritingFile(CActiveFile& file, unsigned long outputSize)
{
    CWideString path(file.PFileInfo->DstPath);
    LeaveCriticalSection(&CriticalSection);
    CreateDirectoryForFile(path);
    file.OutputSize = outputSize;
    file.MinWriteSize = GetDiskSectorSize(path);
    file.DstFileHandle = CreateFileW(path, 0x40000000, 0, 0, 2, 0x68000000, 0);
    file.StartTime = GFGetTime();
    if (file.DstFileHandle == (void*)-1)
        SetDirtyDiscError(true);
    unsigned long size = (outputSize + file.MinWriteSize - 1) / file.MinWriteSize * file.MinWriteSize;
    SetFilePointer(file.DstFileHandle, size, 0, 0);
    SetEndOfFile(file.DstFileHandle);
    if (GetFileSize(file.DstFileHandle, 0) != size)
        SetDirtyDiscError(true);
    EnterCriticalSection(&CriticalSection);
}
