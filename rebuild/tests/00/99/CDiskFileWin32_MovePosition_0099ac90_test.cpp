// CDiskFileWin32::MovePosition(long delta) -- move the file position by `delta`, throwing a CBBBFileException (type 11) when the result would fall before the start or past the end, then flush and seek the Win32 handle there
// Leaf: MovePosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099AC90, length 227 bytes (+ int3 padding), `ret 4`.
// Callees: SetFilePointer (KERNEL32 import @ 0x0143FDE8, used twice: loaded once into ebx), FlushFileBuffers (KERNEL32
// import @ 0x0143FDA8), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (messages "Cannot move before
// beginning of file" @ 0x0129A8B8, "Cannot seek past end of file" @ 0x0129A898), copy constructor @ 0x0099EC30,
// operator const char* @ 0x0099E4C0, ~CCharString @ 0x0099EAE0, _CxxThrowException @ 0x00BFEB84 with the
// CBBBFileException ThrowInfo @ 0x01369380.
// Layout: engine/CDiskFileWin32.h (Position +8, Length +0xc, WinFileHandle +0x10); the exception model is the one in the
// landed CDiskFileWin32::Write (0x0099AA90): CBBBFileException(CCharString by value, EFileExceptionType) whose base
// takes the text as `const char*` (the by-value copy is converted and destroyed inline), a 4-byte object holding Type.
// Shape: the first SetFilePointer(FILE_CURRENT) result is unused; both range checks test `Position + delta` and the
// store is `Position += delta` (one CSE'd sum: `mov ecx,[esi+8] ; mov edx,[arg] ; lea eax,[ecx+edx] ; test ; jge`; a
// named `position` local gives `add eax,[arg] ; jns`); the new position is stored before the flush; the seek goes
// through a LARGE_INTEGER local whose high half is stored (cdq) but not passed, its low half read through a pointer to
// the local (keeps the dead high-half store).
#include "engine/CDiskFileWin32.h"
#include "fable_string.h"

struct CStringFileArgument : CCharString {
    CStringFileArgument(const char* text, long length) : CCharString(text, length) { }
    CStringFileArgument(const CStringFileArgument&);   // 0x0099EC30 native copy
};
class CExceptionBase {
public:
    CExceptionBase() { }
    CExceptionBase(const char*) { }
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { NullBuffer = 1, WriteFailed = 5, NotOpenForWrite = 8, BadPosition = 11 };
    EFileExceptionType Type;
    CBBBFileException(CStringFileArgument text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};

union LARGE_INTEGER {
    struct { unsigned long LowPart; long HighPart; };
    __int64 QuadPart;
};
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void* file, long distance, long* distanceHigh, unsigned long method);
extern "C" __declspec(dllimport) int __stdcall FlushFileBuffers(void* file);

struct CDiskFileMover : CDiskFileWin32 {
    void MovePosition(long delta);
};

void CDiskFileMover::MovePosition(long delta)
{
    SetFilePointer(WinFileHandle, 0, 0, 1);
    if (Position + delta < 0) {
        CStringFileArgument message("Cannot move before beginning of file", -1);
        throw CBBBFileException(message, CBBBFileException::BadPosition);
    }
    if (Position + delta > Length) {
        CStringFileArgument message("Cannot seek past end of file", -1);
        throw CBBBFileException(message, CBBBFileException::BadPosition);
    }
    Position += delta;
    LARGE_INTEGER seek;
    seek.QuadPart = Position;
    FlushFileBuffers(WinFileHandle);
    LARGE_INTEGER* p = &seek;
    SetFilePointer(WinFileHandle, p->LowPart, 0, 0);
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall CreateFileA(const char* name, unsigned long access, unsigned long share,
    void* security, unsigned long disposition, unsigned long flags, void* templ);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* h);
extern "C" __declspec(dllimport) int __stdcall DeleteFileA(const char* name);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_converted = 0;
static const char* g_lastText = 0;
CCharString::CCharString(const char* text, long) : storage_((CCharStringData*)text) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString::operator const char*() const { ++g_converted; g_lastText = (const char*)storage_; return g_lastText; }
CStringFileArgument::CStringFileArgument(const CStringFileArgument& o) : CCharString((const char*)o, -1) { }

static int expectThrow(CDiskFileMover& f, long delta, const char* text)
{
    g_lastText = 0;
    try {
        f.MovePosition(delta);
    } catch (CBBBFileException& e) {
        return e.Type == CBBBFileException::BadPosition && g_lastText && strcmp(g_lastText, text) == 0;
    }
    return 0;
}

int main()
{
    const char* name = "t0099ac90.tmp";
    void* h = CreateFileA(name, 0xC0000000UL, 0, 0, 2 /*CREATE_ALWAYS*/, 0x80, 0);
    CHECK(h != (void*)-1);
    static CDiskFileMover f;
    f.WinFileHandle = h;
    f.Length = 100;
    f.Position = 10;
    f.MovePosition(15);
    CHECK(f.Position == 25 && SetFilePointer(h, 0, 0, 1) == 25);
    f.MovePosition(-25);
    CHECK(f.Position == 0 && SetFilePointer(h, 0, 0, 1) == 0);
    f.MovePosition(100);
    CHECK(f.Position == 100 && SetFilePointer(h, 0, 0, 1) == 100);
    CHECK(expectThrow(f, 1, "Cannot seek past end of file"));
    CHECK(f.Position == 100);
    CHECK(expectThrow(f, -101, "Cannot move before beginning of file"));
    CHECK(f.Position == 100 && SetFilePointer(h, 0, 0, 1) == 100);
    f.MovePosition(-40);
    CHECK(f.Position == 60 && SetFilePointer(h, 0, 0, 1) == 60);
    CloseHandle(h);
    DeleteFileA(name);
    if (fails == 0) printf("0099ac90_TEST PASS\n");
    return fails;
}