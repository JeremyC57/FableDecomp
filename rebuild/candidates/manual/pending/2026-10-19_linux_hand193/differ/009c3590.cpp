// CFileInstaller::StartReadingFile(CActiveFile* file) -- open the file's source for overlapped unbuffered reading
// Grade: DIFFER(ours v retail) -- 194v194: the dead read of the volatile ExitThread lands in cl (retail al) and GetFileSize's out-pointer is formed in edx (retail ecx); the rest is identical
// (with the installer's critical section released around the open); on failure stop the installer thread, else
// record the input size, the disk sector size as the minimum read and the start time; make it the current read file
// Leaf: StartReadingFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3590, length 191 bytes (padded to 208), `ret 4`.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, the wide-string data accessor @ 0x0099AF00,
// ~CWideString @ 0x0099B510, KERNEL32 imports LeaveCriticalSection (IAT 0x0143FE10), CreateFileW (0x0143FE2C),
// GetFileSize (0x0143FDF0), EnterCriticalSection (0x0143FE0C); CTBaseSingleton<CFileInstaller>::Get @ 0x00404440
// (`__fastcall`), CCriticalSectionLock ctor/dtor @ 0x00A62920 / 0x00A629A0, the `__fastcall` GetDiskSectorSize(const
// CWideString&) @ 0x00999180, GFGetTime @ 0x009E1BC0 (double). Global byte 0x013CA814 (set on the open failure).
// Retail layout (Ego_r.pdb names, retail 0x20 below the PDB after its containers): CFileInstaller CriticalSection +4,
// ExitThread +0x7A (volatile, read once before the store), PCurrentReadFile +0x50; CActiveFile SrcFileHandle +0,
// InputSize +0x14, PFileInfo +0x1C (counted pointer data; CFileInfo::SrcPath at +0), MinReadSize +0x28, StartTime
// +0x30.
// Shape: the path copy is a named local in the dead `file` slot; the failure block's lock and the success block's
// high-size word share the frame; the failure block's bare read of the volatile ExitThread survives as `mov al`.

struct CRITICAL_SECTION_ { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_* section);
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const unsigned short* name, unsigned long access, unsigned long share,
                                                             void* security, unsigned long disposition, unsigned long flags, void* templateFile);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void* file, unsigned long* sizeHigh);

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);          // 0x0099B720
    ~CWideString();                                 // 0x0099B510
    operator const unsigned short*() const;         // 0x0099AF00
};

long __fastcall GetDiskSectorSize(const CWideString& path);   // 0x00999180
double __fastcall GFGetTime();                                 // 0x009E1BC0
extern bool g_installerReadFailed;                             // 0x013CA814

class CCriticalSectionLock {
public:
    CRITICAL_SECTION_* Section;
    bool Locked;
    CCriticalSectionLock(CRITICAL_SECTION_* section, bool alreadyLocked);   // 0x00A62920
    ~CCriticalSectionLock();                                                // 0x00A629A0
};

class CFileInstaller {
public:
    struct CFileInfo {
        CWideString SrcPath;                        // +0x00
    };
    struct CActiveFile {
        void* SrcFileHandle;                        // +0x00
        void* DstFileHandle;                        // +0x04
        unsigned long CurrentSrcOffset;             // +0x08
        unsigned long NoBytesRead;                  // +0x0C
        unsigned long NoBytesWritten;               // +0x10
        unsigned long InputSize;                    // +0x14
        unsigned long OutputSize;                   // +0x18
        CFileInfo* PFileInfo;                       // +0x1C (CCountedPointer<const CFileInfo> data)
        void* PFileInfo_Info;                       // +0x20
        unsigned long MinWriteSize;                 // +0x24
        unsigned long MinReadSize;                  // +0x28
        unsigned long _pad2c;
        double StartTime;                           // +0x30
    };

    virtual ~CFileInstaller();
    CRITICAL_SECTION_ CriticalSection;              // +0x04
    unsigned char _pad1c[0x34];
    CActiveFile* PCurrentReadFile;                  // +0x50
    unsigned char _pad54[0x26];
    volatile bool ExitThread;                       // +0x7A

    static CFileInstaller& __fastcall Get();        // 0x00404440 (CTBaseSingleton<CFileInstaller>::Get)
    void StartReadingFile(CActiveFile* file);
};

void CFileInstaller::StartReadingFile(CActiveFile* file)
{
    CWideString path(file->PFileInfo->SrcPath);
    LeaveCriticalSection(&CriticalSection);
    file->SrcFileHandle = CreateFileW(path, 0x80000000, 1, 0, 3, 0x68000001, 0);
    if (file->SrcFileHandle == (void*)-1) {
        CCriticalSectionLock lock(&Get().CriticalSection, false);
        g_installerReadFailed = true;
        (void)Get().ExitThread;
        Get().ExitThread = true;
    } else {
        unsigned long sizeHigh;
        file->InputSize = GetFileSize(file->SrcFileHandle, &sizeHigh);
        file->MinReadSize = GetDiskSectorSize(path);
        file->StartTime = GFGetTime();
    }
    EnterCriticalSection(&CriticalSection);
    PCurrentReadFile = file;
}
