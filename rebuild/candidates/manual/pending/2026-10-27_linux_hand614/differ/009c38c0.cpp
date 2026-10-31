// CFileInstaller::CActiveFile::OnWriteFinished() -- close the temp file, trim it to the written size, copy the original's write time, and verify a backup marker file can be created (flagging a dirty-disc error when it cannot)
// Grade: DIFFER(406v409) -- `static __stdcall` GetMarkerFileName gives retail's result pointer in ESI and the callee-pop `ret 4`; left: the inlined SetDirtyDiscError(true) drops retail's dead `mov al,[eax+7Ah]` (3 bytes) and a GetFileSize/GetFileTime/SetFileTime argument register rotation (eax->edx, ecx->eax, edx->ecx); tried: `||` (inert), `|` / `|=` / ternary / plain if (359, worse); the same-length alternative is the nested-class 409v409 shape without the stdcall static
// Leaf: ?OnWriteFinished@CActiveFile@CFileInstaller@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C33A0, 315 bytes, `ret 4` (path), result in esi. Caller: a CFileInstaller
//   method at 0x009C39C2 (path = CFileInfo +4, the destination path). No Ego_r.pdb name (a file-static helper).
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString(const wchar_t*) @ 0x0099B6B0, copy @ 0x0099B720,
//   operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510, SplitPath @ 0x00997620 (__fastcall), ReplaceSubstring @ 0x0099C1E0,
//   operator+(const CWideString&, const CWideString&) @ 0x0099BE70, operator+(const CWideString&, const wchar_t*) @ 0x0099BF30.
// Strings: L"_" 0x0122DE7C, L"." 0x0124C4A8, L".mkf" 0x0129B404.
// Shape: five locals built in the order drive, dir, name, ext, result (destroyed in reverse); the L"." temporary reuses
//   the dead `path` argument slot.
typedef unsigned short wchar_t;                    // VC7.1 default /Zc:wchar_t-
struct CWideStringData;
class CWideString {
public:
    CWideStringData* m_pStringData;
    CWideString();                                  // 0x0099AED0
    CWideString(const wchar_t* text);               // 0x0099B6B0
    CWideString(const CWideString& other);          // 0x0099B720
    ~CWideString();                                 // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    void ReplaceSubstring(const CWideString& find, const CWideString& replace);   // 0x0099C1E0
    operator const wchar_t*() const;                // 0x0099AF00
};

CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);       // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
                          CWideString* extension);  // 0x00997620

static __declspec(noinline) CWideString __stdcall GetMarkerFileName(const CWideString& path)
{
    CWideString drive;
    CWideString directory;
    CWideString name;
    CWideString extension;
    CWideString result;
    SplitPath(path, &drive, &directory, &name, &extension);
    extension.ReplaceSubstring(L".", L"_");
    result = drive + directory + name + extension + L".mkf";
    return result;
}

// ---- the row: CFileInstaller::CActiveFile::OnWriteFinished ----
typedef void* HANDLE;
struct _FILETIME { unsigned long dwLowDateTime, dwHighDateTime; };
extern "C" {
__declspec(dllimport) int __stdcall CloseHandle(HANDLE);
__declspec(dllimport) HANDLE __stdcall CreateFileW(const wchar_t*, unsigned long, unsigned long, void*, unsigned long, unsigned long, HANDLE);
__declspec(dllimport) unsigned long __stdcall SetFilePointer(HANDLE, long, long*, unsigned long);
__declspec(dllimport) int __stdcall SetEndOfFile(HANDLE);
__declspec(dllimport) unsigned long __stdcall GetFileSize(HANDLE, unsigned long*);
__declspec(dllimport) int __stdcall GetFileTime(HANDLE, _FILETIME*, _FILETIME*, _FILETIME*);
__declspec(dllimport) int __stdcall SetFileTime(HANDLE, const _FILETIME*, const _FILETIME*, const _FILETIME*);
}
struct _RTL_CRITICAL_SECTION { unsigned char _body[0x18]; };
class CCriticalSectionLock {
public:
    CCriticalSectionLock(_RTL_CRITICAL_SECTION& section, bool tryLock);   // 0x00A62920
    ~CCriticalSectionLock();                                              // 0x00A629A0
    _RTL_CRITICAL_SECTION* CriticalSection;
    bool Locked;
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                 // CFileInstaller: 0x00404440
};
double GetTimeSinceStart();                     // 0x009E1BC0
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
    T* operator->() const { return Data; }
};

class CFileInstaller {
public:
    class CFileInfo {
    public:
        CWideString SrcPath;                    // +0x00
        CWideString DstPath;                    // +0x04
        long        InputSize;                  // +0x08
        bool        Started;                    // +0x0c
        bool        Decompress;                 // +0x0d
        bool        CreateMarkerFile;           // +0x0e
    };
    class CActiveFile {
    public:
        HANDLE        SrcFileHandle;            // +0x00
        HANDLE        DstFileHandle;            // +0x04
        unsigned long CurrentSrcOffset;         // +0x08
        unsigned long NoBytesRead;              // +0x0c
        unsigned long NoBytesWritten;           // +0x10
        unsigned long InputSize;                // +0x14
        unsigned long OutputSize;               // +0x18
        CCountedPointer<const CFileInfo> PFileInfo;   // +0x1c
        void OnWriteFinished();
    };
    void*                 _padding_;            // +0x00 vptr
    _RTL_CRITICAL_SECTION CriticalSection;      // +0x04
    unsigned char         _pad1c[0x7a - 0x1c];
    bool                  ExitThread;           // +0x7A
    static bool           DirtyDiscError;       // 0x013CA814
protected:
    static void __fastcall SetDirtyDiscError(bool error)   // out of line at 0x009C3530 (landed); inlined here
    {
        CCriticalSectionLock lock(CTBaseSingleton<CFileInstaller>::Get().CriticalSection, false);
        DirtyDiscError = error;
        if (CTBaseSingleton<CFileInstaller>::Get().ExitThread || error)
            error = true;
        CTBaseSingleton<CFileInstaller>::Get().ExitThread = error;
    }
};

void CFileInstaller::CActiveFile::OnWriteFinished()
{
    if (DstFileHandle == (HANDLE)-1)
        return;
    CloseHandle(DstFileHandle);
    DstFileHandle = (HANDLE)-1;
    CWideString dstPath(PFileInfo->DstPath);
    DstFileHandle = CreateFileW(dstPath, 0x40000000, 0, 0, 4, 0x8000000, 0);
    SetFilePointer(DstFileHandle, OutputSize, 0, 0);
    SetEndOfFile(DstFileHandle);
    GetFileSize(DstFileHandle, 0);
    SrcFileHandle = CreateFileW(PFileInfo->SrcPath, 0x80000000, 1, 0, 3, 0x8000000, 0);
    _FILETIME writeTime;
    GetFileTime(SrcFileHandle, 0, 0, &writeTime);
    SetFileTime(DstFileHandle, 0, 0, &writeTime);
    _FILETIME check;
    GetFileTime(DstFileHandle, 0, 0, &check);
    CloseHandle(SrcFileHandle);
    SrcFileHandle = (HANDLE)-1;
    CloseHandle(DstFileHandle);
    DstFileHandle = (HANDLE)-1;
    if (PFileInfo->CreateMarkerFile) {
        CWideString markerName = GetMarkerFileName(PFileInfo->DstPath);
        HANDLE marker = CreateFileW(markerName, 0x40000000, 1, 0, 2, 0x8000000, 0);
        if (marker == (HANDLE)-1) {
            SetDirtyDiscError(true);
            return;
        }
        CloseHandle(marker);
    }
    GetTimeSinceStart();
}
