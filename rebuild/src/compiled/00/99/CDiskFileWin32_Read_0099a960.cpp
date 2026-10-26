// CDiskFileWin32::Read(void* destination, long size, CASuspendableProcess*) -- read size bytes at the current position through Win32 ReadFile and advance by the count actually read; a null destination, a read at end of file or a failed ReadFile report and throw CBBBFileException(1/3/4)
// Leaf: ?Read@ (Ego_r.pdb ?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z, virtual; modelled through CDiskFileWin32_Methods)
// Retail: Fable.exe @ 0x0099A960, 290 bytes, `ret 0Ch`.
// Body: null destination -> "Null dst parameter" (0x0129A850), code 1; size 0 -> no-op; Position == Length -> "Cannot read
// past end of file" (0x0129A830), code 3; ReadFile (kernel32 import 0x0143FED0) != TRUE -> "Cannot read file"
// (0x0129A81C), code 4; Position += bytes read. The suspendable-process argument is unused.
// Callees: CCharString(const char*, long) 0x0099EBF0, copy ctor 0x0099EC30, operator const char* 0x0099E4C0, ~CCharString
// 0x0099EAE0, _CxxThrowException 0x00BFEB84 with throw info 0x01369380 (CBBBFileException, 4-byte object).
// Layout: engine/CDiskFileWin32.h (Position +8, Length +0xc, WinFileHandle +0x10).
// Shape: the CBBBFileException / by-value ReportFileError model of the landed CDiskFileWin32::SetPosition 0x0099ABF0
// (no EH frame: the message dtor after the throw is unreachable; its copy lands in a dead argument slot); the
// exception ctor and ReportFileError are __forceinline or the third throw calls the ctor out of line.
// Opt: /O2 (16-aligned entry).
#include "engine/CDiskFileWin32.h"

extern "C" __declspec(dllimport) int __stdcall ReadFile(void* file, void* buffer, unsigned long toRead,
                                                         unsigned long* read, void* overlapped);   // 0x0143FED0

class CCharString { public:
    char* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    operator const char*() const;                  // 0x0099E4C0
};

__forceinline void ReportFileError(CCharString message)
{
    (const char*)message;
}

struct CExceptionBase { };

struct CBBBFileException : CExceptionBase {
    long Error;
    CBBBFileException(long error) : Error(error) { }
    __forceinline CBBBFileException(long error, const CCharString& message) : Error(error) { ReportFileError(message); }
};

enum {
    BBB_FILE_ERROR_NULL_DESTINATION = 1,
    BBB_FILE_ERROR_READ_PAST_END    = 3,
    BBB_FILE_ERROR_READ_FAILED      = 4
};

class CASuspendableProcess;

struct CDiskFileWin32_Methods : CDiskFileWin32 {
    void Read(void* destination, long size, CASuspendableProcess* process);
};

void CDiskFileWin32_Methods::Read(void* destination, long size, CASuspendableProcess* process)
{
    if (destination == 0) {
        CCharString message("Null dst parameter", -1);
        throw CBBBFileException(BBB_FILE_ERROR_NULL_DESTINATION, message);
    }
    if (size != 0) {
        if (Position == Length) {
            CCharString message("Cannot read past end of file", -1);
            throw CBBBFileException(BBB_FILE_ERROR_READ_PAST_END, message);
        }
        unsigned long bytesRead;
        if (ReadFile(WinFileHandle, destination, size, &bytesRead, 0) != 1) {
            CCharString message("Cannot read file", -1);
            throw CBBBFileException(BBB_FILE_ERROR_READ_FAILED, message);
        }
        Position += bytesRead;
    }
}