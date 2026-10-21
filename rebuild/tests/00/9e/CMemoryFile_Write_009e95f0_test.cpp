// CMemoryFile::Write(const void*, long, CASuspendableProcess*) -- copy bytes into the memory file at the current position, growing it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E95F0, 316 bytes, `ret 0Ch`.
// Shape: the new end is written as `Position + size` at each use (the __int64 range check, the Length compare/store) with
// SetSize(Length): VC CSEs one sum as `mov edx,[esi+8] ; lea ecx,[edx+ebx]`, which also fixes the register rotation in the
// not-writable throw; a named `long end` local gives `mov ecx,[esi+8] ; add ecx,ebx`.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    friend CCharString __fastcall operator+(const CCharString& a, const char* b);   // 0x0099F600
};
class CExceptionBase {
public:
    CExceptionBase(const char*) { }
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { NullParameter = 1, NotOpenForWriting = 8, ReadPastEnd = 3, SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(CCharString text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};
class CASuspendableProcess;
template <class T> struct CArray {                     // CArray<T>: the element block first
    T*   Data;
    long Count;
    long Capacity;
    long Grow;
    void SetSize(long size);                           // 0x004897F0
    T& operator[](long index) { return Data[index]; }
};
struct CMemoryFile {
    // vptr at +0x00
    long                   AccessType;                  // +0x04 FileAccessType (1: read only)
    long                   Position;                    // +0x08
    long                   Length;                      // +0x0c
    CArray<unsigned char>  DataBuffer;                  // +0x10
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);
    virtual void Write(const void* buffer, long size, CASuspendableProcess* process);
};

void CMemoryFile::Write(const void* buffer, long size, CASuspendableProcess* process)
{
    if (!buffer) {
        CCharString message = CCharString("", -1) + "Null src parameter";
        throw CBBBFileException(message, CBBBFileException::NullParameter);
    }
    if (size) {
        __int64 checked = Position + size;
        if (checked < 0 || checked > 0x7FFFFFFF)
            return;
        if (AccessType == 1) {
            CCharString message = CCharString("", -1) + "File is not open for writing";
            throw CBBBFileException(message, CBBBFileException::NotOpenForWriting);
        }
        if (Position + size > Length) {
            Length = Position + size;
            DataBuffer.SetSize(Length);
        }
        for (; size > 0; --size) {
            DataBuffer[Position++] = *(const unsigned char*)buffer;
            buffer = (const unsigned char*)buffer + 1;
        }
    }
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0;
static const char* g_lastText = 0;
static unsigned char g_store[64];
static long g_setSize = -1;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { ++g_live; }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString::operator const char*() const { g_lastText = (const char*)PStringData; return g_lastText; }
CCharString __fastcall operator+(const CCharString& a, const char* b) { return CCharString(b, -1); }
template <class T> void CArray<T>::SetSize(long size) { g_setSize = size; Count = size; }
void CMemoryFile::Read(void*, long, CASuspendableProcess*) { }

static int expectThrow(CMemoryFile& f, const void* buf, long size, int type, const char* text)
{
    g_lastText = 0;
    try {
        f.Write(buf, size, 0);
    } catch (CBBBFileException& e) {
        return e.Type == type && g_lastText && strcmp(g_lastText, text) == 0;
    }
    return 0;
}

int main()
{
    static CMemoryFile f;
    f.DataBuffer.Data = g_store;
    f.AccessType = 2; f.Position = 2; f.Length = 4;
    const unsigned char src[5] = { 1, 2, 3, 4, 5 };
    f.Write(src, 5, 0);
    CHECK(f.Position == 7 && f.Length == 7 && g_setSize == 7);
    CHECK(g_store[2] == 1 && g_store[6] == 5);
    g_setSize = -1;
    f.Position = 0;
    f.Write(src, 3, 0);
    CHECK(f.Position == 3 && f.Length == 7 && g_setSize == -1 && g_store[0] == 1 && g_store[2] == 3);
    f.Write(src, 0, 0);
    CHECK(f.Position == 3);
    CHECK(expectThrow(f, 0, 4, CBBBFileException::NullParameter, "Null src parameter"));
    f.AccessType = 1;
    CHECK(expectThrow(f, src, 2, CBBBFileException::NotOpenForWriting, "File is not open for writing"));
    CHECK(f.Position == 3);
    f.AccessType = 2;
    f.Position = 0x7FFFFFF0;
    f.Write(src, 0x20, 0);                       // wraps negative: silently ignored
    CHECK(f.Position == 0x7FFFFFF0 && f.Length == 7);
    // no /EHsc in this module: the message locals are never destroyed on the throwing paths (g_live stays > 0)
    if (fails == 0) printf("009e95f0_TEST PASS\n");
    return fails;
}