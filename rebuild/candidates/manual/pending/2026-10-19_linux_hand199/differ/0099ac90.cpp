// CDiskFileWin32::MovePosition(long offset) -- move the file position by a signed offset: throw a CBBBFileException (type 11) when the target lies before the start or past the end, else store it, flush the handle and seek there
// Leaf: MovePosition
// Grade: DIFFER(215v227) -- retail keeps a 0xC frame with a dead store of the seek target's sign-extended high half (`cdq ; mov [esp+18h],edx`) and the new position in eax; ours drops the LARGE_INTEGER (8-byte frame, position in edi). Leaf: MovePosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099AC90, length 240 bytes (ret 4).
// Callees: KERNEL32 SetFilePointer (IAT 0x0143FDE8, held in ebx: a dropped FILE_CURRENT query first, then the FILE_BEGIN seek),
// KERNEL32 FlushFileBuffers (IAT 0x0143FDA8); CCharString(const char*, long) 0x0099EBF0, the narrow copy ctor 0x0099EC30,
// operator const char* 0x0099E4C0 and ~CCharString 0x0099EAE0 (the exception's by-value text argument);
// _CxxThrowException 0x00BFEB84 with ThrowInfo 0x01369380 (CBBBFileException).
// Layout: engine/CDiskFileWin32.h (Position +8, Length +0xC, WinFileHandle +0x10).
// Shape: the landed CDiskFileWriter::Write (0x0099AA90) throw shape: a named message string copied into the exception
// constructor's by-value argument (built in the dead `offset` slot), converted to const char* for the base and destroyed,
// the 4-byte exception object holding only its type; the seek target's high half is a dead LARGE_INTEGER store.
#include "engine/CDiskFileWin32.h"
#include "fable_string.h"

extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* file, long distance, long* distanceHigh,
                                                                       unsigned long method);   // IAT 0x0143FDE8
extern "C" __declspec(dllimport) int __stdcall FlushFileBuffers(void* file);                     // IAT 0x0143FDA8
enum { FILE_BEGIN = 0, FILE_CURRENT = 1 };
union LARGE_INTEGER {
    struct { unsigned long LowPart; long HighPart; };
    __int64 QuadPart;
};

struct CStringFileArgument : CCharString {
    CStringFileArgument(const char* text, long length) : CCharString(text, length) { }
    CStringFileArgument(const CStringFileArgument&);   // 0x0099EC30 narrow copy
};
class CExceptionBase {
public:
    CExceptionBase() { }
    CExceptionBase(const char*) { }
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { NullBuffer = 1, WriteFailed = 5, NotOpenForWrite = 8, SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(CStringFileArgument text, EFileExceptionType type)
        : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};

struct CDiskFileMover : CDiskFileWin32 {
    void MovePosition(long offset);
    void SeekTo(LARGE_INTEGER position)
    {
        FlushFileBuffers(WinFileHandle);
        SetFilePointer(WinFileHandle, position.LowPart, 0, FILE_BEGIN);
    }
};

void CDiskFileMover::MovePosition(long offset)
{
    SetFilePointer(WinFileHandle, 0, 0, FILE_CURRENT);
    long newPosition = Position + offset;
    if (newPosition < 0) {
        CStringFileArgument message("Cannot move before beginning of file", -1);
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    if (newPosition > Length) {
        CStringFileArgument message("Cannot seek past end of file", -1);
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    Position = newPosition;
    LARGE_INTEGER target;
    target.QuadPart = newPosition;
    SeekTo(target);
}
