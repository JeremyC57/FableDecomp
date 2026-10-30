// CActiveFile::OnWriteFinished() -- close the temp file, trim it to the written size, copy the original's write time, and verify a backup can be created
// Grade: DIFFER(409v409) (retry1, was 414v409) -- with GetMarkerFileName 0x009C33A0 as a same-TU static the call now passes the result pointer in ESI like retail; left: the GetFileTime/SetFileTime argument scheduling (retail loads TempHandle into ecx after the call, ours pushes [esi+4] through edx first).
// Leaf: ?OnWriteFinished@CActiveFile@@QAEXXZ
// OldGrade: DIFFER(314v315) -- lane gap: custom register convention (LTCG-style): retail takes the hidden CWideString result pointer in esi (caller 0x009C39BE `lea esi,[esp+14h] ; call`), never saves esi and returns it with `ret 4`; VC7.1 without /GL always passes it on the stack
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

static __declspec(noinline) CWideString GetMarkerFileName(const CWideString& path)
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

// ---- the caller (CActiveFile::OnWriteFinished 0x009C38C0, the row parked beside this one) ----
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
        CWideString backupName = GetMarkerFileName(info->TempName);
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
