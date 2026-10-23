// CThreadedFile::Open(const CWideString& filename, bool unbuffered) -- open the file read-only for overlapped reads (optionally unbuffered), note whether it lives on drive D:, its length and a sort key
// Leaf: Open
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098E1E0, length 308 bytes (+ int3 padding), `ret 8`.
// Callees: ConvertPathToFullPath @ 0x00999110 and SplitPath @ 0x00997620 (both `__fastcall`, hidden result / first
// arguments in ecx/edx), CWideString::CWideString() @ 0x0099AED0, CWideString::ConvertToCharString() const @ 0x0099B2C0,
// CCharString::ToUpper() const @ 0x0099EE20, CBasicString<char>::operator==(const char*) @ 0x004115A0, ~CCharString
// @ 0x0099EAE0, CWideString::operator const wchar_t* @ 0x0099AF00, CreateFileW (IAT 0x0143FE2C), CWideString::operator=
// @ 0x0099B7D0, GetFileSize (IAT 0x0143FDF0), ~CWideString @ 0x0099B510.
// Globals: 0x013BC9EC the physical sort-key counter; strings 0x0129A15C "D:", 0x0122D70E "".
// Layout: Ego_r.pdb CThreadedFile (= retail, as the landed deleting dtor 0x00991450): DeviceID +0x04, FileHandle +0x08,
// Filename +0x0c, Length +0x10, PhysicalSortKey +0x14, OpenedForWrite +0x18, OpenFlag +0x19.
// Shape: the drive test is the inline CCharString == "D:" written as the two tests (strcmp on the null string, the
// block compare otherwise) on the upper-cased converted drive; the bool lives in bl across both temporaries'
// destructors and becomes the device id through `setne`; the full path and the drive are destroyed on both exits.
#include <string.h>
typedef unsigned short wchar_t;                                // VC7.1 without /Zc:wchar_t
#pragma pack(push, 1)
struct CBasicStringChar {                                      // CBasicString<char>, 13 bytes
    char*         PData;                                       // +0x00
    unsigned long StringLength;                                // +0x04
    unsigned long DataLength : 31;                             // +0x08
    bool          UseFastExtend : 1;                           // +0x0c
    bool operator==(const char* text) const;                   // 0x004115A0
};
struct CCharStringData {                                       // 17 bytes
    CBasicStringChar Data;                                     // +0x00
    long             NoRefs;                                   // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                                            // 0x0099EAE0
    CCharString ToUpper() const;                               // 0x0099EE20
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                             // 0x0099AED0
    ~CWideString();                                            // 0x0099B510
    CWideString& operator=(const CWideString& other);          // 0x0099B7D0
    const wchar_t* GetChars_0099af00() const;                  // 0x0099AF00 (retail's second operator const wchar_t* copy)
    CCharString ConvertToCharString() const;                   // 0x0099B2C0
};
CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* name, CWideString* ext);   // 0x00997620

extern "C" __declspec(dllimport) void* __stdcall CreateFileW(const wchar_t* name, unsigned long access, unsigned long share,
                                                             void* security, unsigned long disposition, unsigned long flags,
                                                             void* templateFile);   // IAT 0x0143FE2C
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void* file, unsigned long* high);   // IAT 0x0143FDF0

// The sort-key counter is a file-scope static of the threaded-file TU: an extern global keeps its store before the drive
// destructor's `lea ecx,[esp+8]` (the callee might read it), retail schedules the lea first.
static unsigned long g_ThreadedFilePhysicalSortKey;            // 0x013BC9EC
// "D:" is the literal 0x0129A15C (VC inlines the null-string strcmp against it with its known length 3).

enum EThreadedFileDeviceID { THREADED_FILE_DEVICE_HD = 0, THREADED_FILE_DEVICE_D = 1 };

class CThreadedFile {
public:
    virtual ~CThreadedFile();
    EThreadedFileDeviceID DeviceID;                            // +0x04
    void*                 FileHandle;                          // +0x08
    CWideString           Filename;                            // +0x0c
    unsigned long         Length;                              // +0x10
    unsigned long         PhysicalSortKey;                     // +0x14
    bool                  OpenedForWrite;                      // +0x18
    bool                  OpenFlag;                            // +0x19
    bool Open(const CWideString& filename, bool unbuffered);
};

bool CThreadedFile::Open(const CWideString& filename, bool unbuffered)
{
    CWideString fullPath = ConvertPathToFullPath(filename);
    CWideString drive;
    SplitPath(fullPath, &drive, 0, 0, 0);
    OpenedForWrite = false;
    bool onDriveD = drive.ConvertToCharString().ToUpper() == "D:";
    DeviceID = (EThreadedFileDeviceID)(onDriveD != 0);
    unsigned long flags = 0x40000001;                          // FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_READONLY
    if (unbuffered)
        flags = 0x60000001;                                    // ... | FILE_FLAG_NO_BUFFERING
    FileHandle = CreateFileW(fullPath.GetChars_0099af00(), 0x80000000, 1, 0, 3, flags, 0);
    if (FileHandle == (void*)-1)
        return false;
    Filename = filename;
    Length = GetFileSize(FileHandle, 0);
    PhysicalSortKey = ++g_ThreadedFilePhysicalSortKey;
    OpenFlag = true;
    return true;
}