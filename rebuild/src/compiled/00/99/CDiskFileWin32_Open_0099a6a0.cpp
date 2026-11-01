// CDiskFileWin32::Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags) -- remember the pathname, expand it to a full path (an empty result throws CBBBFileException(1) "Bad filename ..."), open it through CreateFileW for the access type (read / read-write existing / read-write open-always / create-always under a created directory; flag bit 2 asks for FILE_FLAG_SEQUENTIAL_SCAN), throw CBBBFileException(2) "Cannot open file '...' (access type N) - <last error>" on INVALID_HANDLE_VALUE, then take the file size, rewind and mark the file opened
// Leaf: ?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099A6A0, 640 bytes, vtable 0x0122D06C slot 1 (`ret 0Ch`); jump table 0x0099A910.
// Identity: FableWin.pdb / Ego_r.pdb ?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z; the landed copy
//   constructor 0x0099ADC0 calls it with (other.Pathname, 1, 2). The class layout is Ego_r.pdb CDiskFileWin32 (0x1c:
//   AccessType +4, Position +8, Length +0xc, WinFileHandle +0x10, Opened +0x14, Pathname +0x18); the generated
//   engine/CDiskFileWin32.h flattens Pathname to its data pointer, so the typed local model of the landed 0x0099ADC0 is used.
// Callees: CWideString() 0x0099AED0, CWideString::operator=(const CWideString&) 0x0099B7D0, CAFile::ConvertPathToFullPath
//   0x00999140 (?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z), CWideString::GetLength 0x0099B220,
//   CWideString::ToCharString 0x0099B2C0 (?ToCharString@CWideString@@QBE?AVCCharString@@XZ; the landed file spells it
//   ConvertToCharString), operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*)
//   0x0099F600, operator+(const CCharString&, const CCharString&) 0x0099F570, GFIntToCharString 0x0099F830,
//   GFGetLastWindowsError 0x00A5ED60, CCharString copy 0x0099EC30 / operator const char* 0x0099E4C0 / ~CCharString 0x0099EAE0,
//   CAFile::ValidatePathnameIsOnWriteableDrive 0x00999970 (?ValidatePathnameIsOnWriteableDrive@CAFile@@SIXABVCWideString@@@Z),
//   CAFile::CreateDirectoryForFile 0x00999620, CWideString::operator const wchar_t* 0x0099AF00, ~CWideString 0x0099B510,
//   kernel32 CreateFileW [0x0143FE2C], GetFileSize [0x0143FDF0], SetFilePointer [0x0143FDE8] (held in ebp for both calls),
//   _CxxThrowException 0x00BFEB84 with throw info 0x01369380 (CBBBFileException, 4-byte object: the Read 0x0099A960 model).
// Strings: 0x0129A80C "Bad filename ", 0x0129A7E0 "Cannot open file '", 0x0129A7F4 "' (access type ", 0x0129A804 ") - ".
// Shape: `attributes = 0` is declared before the pathname assignment (its zero register also clears WinFileHandle) and
//   raised by an `if`, not a ternary (VC7.1 turns `(flags & 2) ? 0x08000000 : 0` into `and/shl`); the switch's
//   default leaves a null handle that the INVALID_HANDLE_VALUE test folds away; the error text keeps the last-error
//   string in its own named local (its destructor and the message's are unreachable after the throw: no EH frame),
//   the by-value message copy of the inline exception constructor lands in the dead pathname slot.
typedef unsigned short wchar16;

extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const wchar16* name, unsigned long access, unsigned long share, void* security, unsigned long disposition, unsigned long attributes, void* templateFile);   // 0x0143FE2C
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void* file, unsigned long* high);                                   // 0x0143FDF0
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* file, long distance, long* distanceHigh, unsigned long method);   // 0x0143FDE8

class CCharString { public:
    char* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    operator const char*() const;                    // 0x0099E4C0
};
class CWideString { public:
    void* PStringData;
    CWideString();                                   // 0x0099AED0
    ~CWideString();                                  // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    long GetLength() const;                          // 0x0099B220
    CCharString ToCharString() const;                // 0x0099B2C0 ?ToCharString@CWideString@@QBE?AVCCharString@@XZ
    operator const wchar16*() const;                 // 0x0099AF00
};
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
CCharString __fastcall GFGetLastWindowsError();                                        // 0x00A5ED60

class CAFile { public:
    static bool __fastcall ConvertPathToFullPath(const CWideString& path, CWideString& fullPath);   // 0x00999140
    static void __fastcall ValidatePathnameIsOnWriteableDrive(const CWideString& path);           // 0x00999970
    static void __fastcall CreateDirectoryForFile(const CWideString& path);                       // 0x00999620
};

__forceinline void ReportFileError(CCharString message)
{
    (const char*)message;
}
enum EFileExceptionType { FILE_EXCEPTION_BAD_FILENAME = 1, FILE_EXCEPTION_CANNOT_OPEN = 2 };
struct CExceptionBase { };
struct CBBBFileException : CExceptionBase {                 // throw info 0x01369380, 4-byte object
    EFileExceptionType Type;
    __forceinline CBBBFileException(const CCharString& message, EFileExceptionType type) : Type(type) { ReportFileError(message); }
};

enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_READ_WRITE = 2, FILE_ACCESS_READ_WRITE_OPEN_ALWAYS = 3, FILE_ACCESS_CREATE = 4 };

class CBaseClassNonCopyable { protected: virtual ~CBaseClassNonCopyable(); };
class CDiskFileWin32 : public CBaseClassNonCopyable { public:        // vtable 0x0122D06C (PDB layout, 0x1c bytes)
    long        AccessType;            // +0x04 FileAccessType
    long        Position;              // +0x08
    long        Length;                // +0x0c
    void*       WinFileHandle;         // +0x10
    bool        Opened;                // +0x14
    CWideString Pathname;              // +0x18
    virtual void Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags);   // 0x0099A6A0
};

void CDiskFileWin32::Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    CWideString fullPath;
    unsigned long attributes = 0;
    Pathname = pathname;
    WinFileHandle = 0;
    if (flags & 2)
        attributes = 0x08000000;    // FILE_FLAG_SEQUENTIAL_SCAN
    CAFile::ConvertPathToFullPath(pathname, fullPath);
    if (fullPath.GetLength() <= 0) {
        CCharString message = "Bad filename " + fullPath.ToCharString();
        throw CBBBFileException(message, FILE_EXCEPTION_BAD_FILENAME);
    }
    void* handle;
    switch (accessType) {
    case FILE_ACCESS_READ:
        handle = CreateFileW(fullPath, 0x80000000, 1, 0, 3, attributes, 0);
        AccessType = accessType;
        break;
    case FILE_ACCESS_READ_WRITE:
        CAFile::ValidatePathnameIsOnWriteableDrive(fullPath);
        handle = CreateFileW(fullPath, 0xC0000000, 1, 0, 3, attributes, 0);
        AccessType = accessType;
        break;
    case FILE_ACCESS_READ_WRITE_OPEN_ALWAYS:
        CAFile::ValidatePathnameIsOnWriteableDrive(fullPath);
        handle = CreateFileW(fullPath, 0xC0000000, 0, 0, 4, attributes, 0);
        AccessType = FILE_ACCESS_READ_WRITE;
        break;
    case FILE_ACCESS_CREATE:
        CAFile::ValidatePathnameIsOnWriteableDrive(fullPath);
        CAFile::CreateDirectoryForFile(fullPath);
        handle = CreateFileW(fullPath, 0xC0000000, 0, 0, 2, attributes, 0);
        AccessType = FILE_ACCESS_READ_WRITE;
        break;
    default:
        handle = 0;
        break;
    }
    if (handle == (void*)-1) {
        CCharString error = GFGetLastWindowsError();
        CCharString message = "Cannot open file '" + fullPath.ToCharString() + "' (access type " + GFIntToCharString(accessType) + ") - " + error;
        throw CBBBFileException(message, FILE_EXCEPTION_CANNOT_OPEN);
    }
    Length = GetFileSize(handle, 0);
    SetFilePointer(handle, 0, 0, 0);
    WinFileHandle = handle;
    Position = 0;
    Opened = true;
    SetFilePointer(handle, 0, 0, 1);
}