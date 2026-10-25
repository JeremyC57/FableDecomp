// Behaviour test for CWADFile::CWADFile(bank, name, access, flags) (0x00A39D80): bank kept, index/position zeroed
// before Open runs, and Open receives the three forwarded arguments.
#include <cstdio>

class CBankFileAsync { public: int dummy; };
class CWideString { public: int dummy; };
enum FileAccessType { FILE_ACCESS_READ, FILE_ACCESS_WRITE };

static const CWideString* g_name = 0; static int g_access = -1; static unsigned long g_flags = 0;
static unsigned long g_indexAtOpen = 99; static long g_posAtOpen = 99;

class CBase {
public:
    virtual ~CBase() {}
    CBase() {}
};

class CWADFile : public CBase {
public:
    CBankFileAsync* Bank;
    unsigned long   Index;
    long            FilePos;
    CWADFile(CBankFileAsync* bank, const CWideString& name, FileAccessType access, unsigned long flags);
    void Open(const CWideString& name, FileAccessType access, unsigned long flags)
    {
        g_name = &name; g_access = access; g_flags = flags; g_indexAtOpen = Index; g_posAtOpen = FilePos;
        Index = 7;
    }
};

CWADFile::CWADFile(CBankFileAsync* bank, const CWideString& name, FileAccessType access, unsigned long flags)
    : Bank(bank), Index(0), FilePos(0)
{
    Open(name, access, flags);
}

int main()
{
    CBankFileAsync bank; CWideString name;
    CWADFile f(&bank, name, FILE_ACCESS_WRITE, 0x55);
    if (f.Bank != &bank || f.Index != 7 || f.FilePos != 0 || g_name != &name || g_access != FILE_ACCESS_WRITE
        || g_flags != 0x55 || g_indexAtOpen != 0 || g_posAtOpen != 0)
        { std::printf("00a39d80_TEST FAIL\n"); return 1; }
    std::printf("00a39d80_TEST PASS\n");
    return 0;
}