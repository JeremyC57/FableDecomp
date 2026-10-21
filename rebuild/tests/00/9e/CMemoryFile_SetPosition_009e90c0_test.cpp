// CMemoryFile::SetPosition(long) -- seek; a position past the end builds an error message and throws a CBBBFileException (type 11)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E90C0, 128 bytes (padding included).
// Shape: `position > Length` (+0xc, signed) takes the error path: an explicit CCharString temporary
// (literal 0x0122D70E, 0x0099EBF0; its returned pointer feeds edx) is concatenated with the literal
// @ 0x0129A898 by the __fastcall operator+ (0x0099F600, hidden result in ecx), the temporary dies
// (0x0099EAE0), the message is handed BY VALUE to an inline reporter (copy 0x0099EC30 built in the
// dead `position` slot, its text read through operator const char* 0x0099E4C0, then destroyed) and
// `throw CBBBFileException(11, message)` (_CxxThrowException 0x00BFEB84 with the frame word and ThrowInfo 0x01369380,
// the CBBBFileException throw info -- the same one 0x00A5B020 CCachedFile::SetPosition uses, its type descriptor
// ".?AVCBBBFileException@@"; an earlier note called it the int ThrowInfo). The exception's constructor takes the
// message and reports it, which keeps the throw temporary live across the copy: its own frame word (`sub esp,8`). No
// /EHsc in this module: the message local is never destroyed on the throw path. Otherwise Position
// (+8) = position. Members per the PDB CMemoryFile (retail packs DataBuffer in 12 bytes, so the
// engine header's +0x20/+0x24 sit 4 bytes low in retail).
struct CCharStringData;

struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    operator const char*() const;                   // 0x0099E4C0
};

CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

inline void DebugOutput(const char*) { }            // compiled out in the retail build
inline void ReportError(CCharString message) { DebugOutput(message); }

enum EFileExceptionType {
    FILE_EXCEPTION_SEEK_PAST_END = 11
};
class CBBBFileException {
public:
    EFileExceptionType Type;                        // +0x00
    CBBBFileException(EFileExceptionType type, const CCharString& message) : Type(type) { ReportError(message); }
};

struct CMemoryFile {
    virtual void v00();
    long          AccessType;                       // +0x04
    long          Position;                         // +0x08
    long          Length;                           // +0x0c
    void SetPosition(long position);
};

void CMemoryFile::SetPosition(long position)
{
    if (position > Length) {
        CCharString message = CCharString("", -1) + "CMemoryFile::SetPosition - past end of file";   // literals 0x0122D70E / 0x0129A898 (masked)
        throw CBBBFileException(FILE_EXCEPTION_SEEK_PAST_END, message);
    }
    Position = position;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0, g_converted = 0;
static const char* g_lastConverted = 0;
CCharString::CCharString(const char* text, long) : m_pData((CCharStringData*)text) { ++g_live; }
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString::operator const char*() const { ++g_converted; g_lastConverted = (const char*)m_pData; return g_lastConverted; }
CCharString __fastcall operator+(const CCharString& left, const char* right)
{
    CHECK(strcmp((const char*)left.m_pData, "") == 0);
    return CCharString(right, -1);
}
void CMemoryFile::v00() { }

int main()
{
    static CMemoryFile mf;
    mf.Length = 50;
    mf.Position = 3;
    mf.SetPosition(50);
    CHECK(mf.Position == 50 && g_live == 0);
    mf.SetPosition(-1);
    CHECK(mf.Position == -1);
    int caught = 0;
    try {
        mf.SetPosition(51);
    } catch (CBBBFileException& e) {
        caught = 1;
        CHECK(e.Type == FILE_EXCEPTION_SEEK_PAST_END);
    }
    CHECK(caught == 1 && mf.Position == -1);
    if (fails == 0) printf("009e90c0_TEST PASS\n");
    return fails;
}