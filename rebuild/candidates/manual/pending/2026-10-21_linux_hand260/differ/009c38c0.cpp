// Grade: DIFFER(414v409) -- MakeBackupName 0x009C33A0 is an LTCG esi-result callee (lane gap); also GetFileTime/SetFileTime argument scheduling
// CActiveFile::OnWriteFinished() -- close the temp file, trim it to the written size, copy the original's write time, and verify a backup can be created
// Layout: no engine header for CActiveFile: +0 Handle, +4 TempHandle, +0x18 WrittenSize, +0x1C PInfo (CWideString name at +0, temp name at +4, backup flag at +0xE).
typedef void* HANDLE;
struct FILETIME { unsigned long Low, High; };
extern "C" {
__declspec(dllimport) int __stdcall CloseHandle(HANDLE);
__declspec(dllimport) HANDLE __stdcall CreateFileW(const unsigned short*, unsigned long, unsigned long, void*, unsigned long, unsigned long, HANDLE);
__declspec(dllimport) unsigned long __stdcall SetFilePointer(HANDLE, long, long*, unsigned long);
__declspec(dllimport) int __stdcall SetEndOfFile(HANDLE);
__declspec(dllimport) unsigned long __stdcall GetFileSize(HANDLE, unsigned long*);
__declspec(dllimport) int __stdcall GetFileTime(HANDLE, FILETIME*, FILETIME*, FILETIME*);
__declspec(dllimport) int __stdcall SetFileTime(HANDLE, const FILETIME*, const FILETIME*, const FILETIME*);
}
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);       // 0x0099B720
    ~CWideString();                               // 0x0099B510
    operator const unsigned short*() const;              // 0x0099AF00
};
CWideString MakeBackupName(const CWideString& name);   // 0x009C33A0 (retail: LTCG, result pointer in esi)
struct CCriticalSection { };
struct CCriticalSectionLock { char _d[8]; CCriticalSectionLock(CCriticalSection* cs, long flags); ~CCriticalSectionLock(); };   // 0x00A62920 / 0x00A629A0
struct CFileInstaller { long _0; CCriticalSection CS; char _pad[0x7a - 0x5]; bool WriteFailed; static CFileInstaller* Get(); };   // 0x00404440
extern bool g_FileWriteError;                     // 0x013CA814
float GetTimeSinceStart();                        // 0x009E1BC0
struct CFileInfo { CWideString Name; CWideString TempName; char _pad8[0xe - 0x8]; bool MakeBackup; };

class CActiveFile {
public:
    HANDLE Handle;                                // +0x00
    HANDLE TempHandle;                            // +0x04
    char _pad08[0x18 - 0x08];
    long WrittenSize;                             // +0x18
    CFileInfo* PInfo;                             // +0x1C
    void OnWriteFinished();
};

void CActiveFile::OnWriteFinished()
{
    if (TempHandle == (HANDLE)-1)
        return;
    CloseHandle(TempHandle);
    CWideString tempName(PInfo->TempName);
    TempHandle = (HANDLE)-1;
    TempHandle = CreateFileW(tempName, 0x40000000, 0, 0, 4, 0x8000000, 0);
    SetFilePointer(TempHandle, WrittenSize, 0, 0);
    SetEndOfFile(TempHandle);
    GetFileSize(TempHandle, 0);
    Handle = CreateFileW(PInfo->Name, 0x80000000, 1, 0, 3, 0x8000000, 0);
    FILETIME writeTime;
    GetFileTime(Handle, 0, 0, &writeTime);
    SetFileTime(TempHandle, 0, 0, &writeTime);
    FILETIME check;
    GetFileTime(TempHandle, 0, 0, &check);
    CloseHandle(Handle);
    Handle = (HANDLE)-1;
    CloseHandle(TempHandle);
    TempHandle = (HANDLE)-1;
    CFileInfo* info = PInfo;
    if (info->MakeBackup) {
        CWideString backupName = MakeBackupName(info->TempName);
        HANDLE backup = CreateFileW(backupName, 0x40000000, 1, 0, 2, 0x8000000, 0);
        if (backup == (HANDLE)-1) {
            CCriticalSectionLock lock(&CFileInstaller::Get()->CS, 0);
            g_FileWriteError = true;
            bool previous = CFileInstaller::Get()->WriteFailed;
            CFileInstaller::Get()->WriteFailed = true;
            return;
        }
        CloseHandle(backup);
    }
    GetTimeSinceStart();
}
