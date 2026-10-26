// CDiskFileWin32::SetPosition(long) -- move the logical and the Win32 file pointer; seeking past the end reports "Cannot seek past end of file" and throws CBBBFileException(11)
// Leaf: ?SetPosition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099ABF0, length 157 bytes (vtable slot of CDiskFileWin32, `ret 4`).
// Callees: kernel32 SetFilePointer @ 0x0143FDE8 (held in ebx for both calls), FlushFileBuffers @ 0x0143FDA8;
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// CCharString::operator const char*() @ 0x0099E4C0, CCharString::~CCharString() @ 0x0099EAE0 (release + live-string count),
// _CxxThrowException @ 0x00BFEB84 with the throw info @ 0x01369380 (catchable types CBBBFileException, CExceptionBase; 4-byte
// object, no destructor). String literal @ 0x0129A898.
// Layout: engine/CDiskFileWin32.h (PDB, fits): Position +8, Length +0xc, WinFileHandle +0x10.
// Shape: no EH frame (the retail module is built without /EHsc, so the throw needs no unwind and the message's destructor is
// unreachable); the error report is an inline helper taking the message BY VALUE: its copy is built in the dead `position`
// slot, converted to `const char*` (result unused) and destroyed; the exception object (code 11) sits in the frame because
// its constructor takes the message and reports it (the throw temporary is live across the copy). The new position goes
// through a LARGE_INTEGER whose high half is stored (cdq) but never passed (NULL high pointer): the low half is read
// through a pointer to the local, which keeps the local in memory so the dead high-half store survives.
#include "engine/CDiskFileWin32.h"

extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* file, long distance, long* distanceHigh,
                                                                        unsigned long moveMethod);   // 0x0143FDE8
extern "C" __declspec(dllimport) int __stdcall FlushFileBuffers(void* file);                     // 0x0143FDA8

enum { FILE_BEGIN = 0, FILE_CURRENT = 1 };

union LARGE_INTEGER {
    struct {
        unsigned long LowPart;
        long          HighPart;
    };
    __int64 QuadPart;
};

class CCharString { public:
    char* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    operator const char*() const;                  // 0x0099E4C0
};

inline void ReportFileError(CCharString message)
{
    (const char*)message;
}

struct CExceptionBase { };

struct CBBBFileException : CExceptionBase {
    long Error;
    CBBBFileException(long error) : Error(error) { }
    CBBBFileException(long error, const CCharString& message) : Error(error) { ReportFileError(message); }
};

enum { BBB_FILE_ERROR_SEEK_PAST_END = 11 };

struct CDiskFileWin32_Methods : CDiskFileWin32 {
    void SetPosition(long position);
};

void CDiskFileWin32_Methods::SetPosition(long position)
{
    if (Position != position) {
        SetFilePointer(WinFileHandle, 0, 0, FILE_CURRENT);
        if (position > Length) {
            CCharString message("Cannot seek past end of file", -1);
            throw CBBBFileException(BBB_FILE_ERROR_SEEK_PAST_END, message);
        }
        Position = position;
        LARGE_INTEGER distance;
        distance.QuadPart = position;
        FlushFileBuffers(WinFileHandle);
        LARGE_INTEGER* p = &distance;
        SetFilePointer(WinFileHandle, p->LowPart, 0, FILE_BEGIN);
    }
}