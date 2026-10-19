#include <cstdio>
#include <string.h>
static char g_last[64];
struct CCharString {
    const char* p;
    CCharString(const char* text, long) : p(text) { }
    CCharString(const CCharString& o) : p(o.p) { }
    ~CCharString() { }
    operator const char*() const { return p; }
};
CCharString operator+(const CCharString& a, const char* b) { (void)a; return CCharString(b, -1); }
class CExceptionBase { public: CExceptionBase(const char* t) { strcpy(g_last, t); } };
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(CCharString text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};
struct CMemoryFile {
    long Position, Length;
    void MovePosition(long delta);
};
void CMemoryFile::MovePosition(long delta)
{
    long position = Position + delta;
    if (position < 0) {
        CCharString message = CCharString("", -1) + "Cannot move before beginning of file";
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    if (position > Length) {
        CCharString message = CCharString("", -1) + "Cannot seek past end of file";
        throw CBBBFileException(message, CBBBFileException::SeekOutOfRange);
    }
    Position = position;
}
static int fail() { std::printf("009e9140_TEST FAIL\n"); return 1; }
int main()
{
    CMemoryFile f; f.Position = 10; f.Length = 100;
    f.MovePosition(-10); if (f.Position != 0) return fail();
    f.MovePosition(100); if (f.Position != 100) return fail();
    int caught = 0;
    try { f.MovePosition(1); } catch (CBBBFileException& e) { if (e.Type == 11 && strcmp(g_last, "Cannot seek past end of file") == 0) ++caught; }
    try { f.MovePosition(-101); } catch (CBBBFileException& e) { if (e.Type == 11 && strcmp(g_last, "Cannot move before beginning of file") == 0) ++caught; }
    if (caught != 2 || f.Position != 100) return fail();
    std::printf("009e9140_TEST PASS\n");
    return 0;
}