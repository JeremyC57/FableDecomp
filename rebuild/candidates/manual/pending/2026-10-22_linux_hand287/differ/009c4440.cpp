// Grade: DIFFER(625v609) -- the inlined SetDirtyDiscError(true) keeps retail's dead load of DirtyDiscError only in retail; source/dest spill words swapped, &SizeCheckedExtensions not kept in edi across the temporaries' dtors
// CFileInstaller::IsFileCopyNecessary(const CCountedPointer<CFileCopy>& copy) -- true when either file is missing, when a non-ignored extension's sizes differ, or when the modification times differ by more than three seconds; an unreadable source raises the dirty-disc error and returns false
// Leaf: IsFileCopyNecessary
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int n);
#pragma intrinsic(memcmp)
typedef void* HANDLE;
typedef union _LARGE_INTEGER { struct { unsigned long LowPart; long HighPart; } u; __int64 QuadPart; } LARGE_INTEGER;
extern "C" __declspec(dllimport) HANDLE __stdcall CreateFileW(const unsigned short* name, unsigned long access, unsigned long share, void* security, unsigned long disposition, unsigned long flags, HANDLE templateFile);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(HANDLE h);
extern "C" __declspec(dllimport) int __stdcall GetFileSizeEx(HANDLE h, LARGE_INTEGER* size);
#define INVALID_HANDLE_VALUE ((HANDLE)(long)-1)

struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
    CCharString ToLower() const;                       // 0x0099ED10
};
struct CWideString {
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
    operator const unsigned short*() const;            // 0x0099AF00
    CCharString ConvertToCharString() const;           // 0x0099B2C0
};
struct CDateAndTime {
    unsigned __int64 Time;
};
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);   // 0x00999320
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* name, CWideString* ext);   // 0x00997620

struct _RTL_CRITICAL_SECTION;
struct CCriticalSectionLock {
    void* _w[2];
    CCriticalSectionLock(_RTL_CRITICAL_SECTION* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                           // 0x00A629A0
};
struct CCodeSectionManager {
    void*         __vftable;
    unsigned char CriticalSection[0x18];               // +0x04
    unsigned char _pad1c[0x5e];
    bool          DirtyDiscError;                      // +0x7a
};
CCodeSectionManager* __fastcall GetCodeSectionManager(); // 0x00404440
extern bool g_DirtyDiscError;                          // 0x013CA814

struct CFileCopy {
    CWideString Source;                                // +0
    CWideString Dest;                                  // +4
};
template<class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
struct CIgnoredExtensionSet {                          // set<CCharString>
    void* Header;
    void* find(const CCharString& key);                // 0x009AB5D0
    void* end() const { return Header; }
};
struct CFileInstaller {
    unsigned char        _pad[0x81];
    bool                 Cancelled;                    // +0x81
    unsigned char        _pad82[0x12];
    CIgnoredExtensionSet SizeCheckedExtensions;        // +0x94
    bool IsFileCopyNecessary(const CCountedPointer<CFileCopy>& copy);
};

__forceinline void __fastcall SetDirtyDiscError(bool error)   // 0x009C3530
{
    CCriticalSectionLock lock((_RTL_CRITICAL_SECTION*)GetCodeSectionManager()->CriticalSection, false);
    g_DirtyDiscError = error;
    GetCodeSectionManager()->DirtyDiscError = GetCodeSectionManager()->DirtyDiscError || error;
}

bool CFileInstaller::IsFileCopyNecessary(const CCountedPointer<CFileCopy>& copy)
{
    const CWideString& source = copy->Source;
    const CWideString& dest = copy->Dest;
    if (!PathExists(source) || !PathExists(dest))
        return true;
    HANDLE sourceFile = CreateFileW(source, 0x80000000, 1, 0, 3, 0x8000001, 0);
    HANDLE destFile = CreateFileW(dest, 0x80000000, 1, 0, 3, 0x8000001, 0);
    if (sourceFile == INVALID_HANDLE_VALUE) {
        SetDirtyDiscError(true);
        if (destFile != INVALID_HANDLE_VALUE)
            CloseHandle(destFile);
        return false;
    }
    if (destFile == INVALID_HANDLE_VALUE) {
        CloseHandle(sourceFile);
        return true;
    }
    CWideString ext;
    SplitPath(source, 0, 0, 0, &ext);
    void* found = SizeCheckedExtensions.find(ext.ConvertToCharString().ToLower());
    if (found == SizeCheckedExtensions.end()) {
        LARGE_INTEGER sourceSize;
        if (!GetFileSizeEx(sourceFile, &sourceSize)) {
            CloseHandle(sourceFile);
            CloseHandle(destFile);
#pragma inline_depth(0)
            SetDirtyDiscError(true);
#pragma inline_depth()
            return false;
        }
        LARGE_INTEGER destSize;
        if (!GetFileSizeEx(destFile, &destSize) || memcmp(&destSize, &sourceSize, sizeof(LARGE_INTEGER)) != 0) {
            CloseHandle(sourceFile);
            CloseHandle(destFile);
            return true;
        }
    }
    CloseHandle(sourceFile);
    CloseHandle(destFile);
    if (Cancelled)
        return false;
    CDateAndTime sourceTime = GetFileModifiedTime(source);
    CDateAndTime destTime = GetFileModifiedTime(dest);
    if (destTime.Time >= sourceTime.Time && destTime.Time - sourceTime.Time > 30000000)
        return true;
    if (sourceTime.Time > destTime.Time && sourceTime.Time - destTime.Time > 30000000)
        return true;
    return false;
}
