// Behaviour test for CBankFileAsync::OpenFileReadOnly (0x009D5820): a failed base open returns false
// untouched; a successful one creates and opens the threaded reader and re-enables async reads.
#include <cstdio>

static int g_threadedOpens = 0; static bool g_lastNonCached = false;
struct CThreadedFile { bool Open(const char*, bool nonCached) { ++g_threadedOpens; g_lastNonCached = nonCached; return true; } };
struct CBankFile {
    bool baseResult;
    virtual ~CBankFile() {}
    virtual bool OpenFileReadOnly(const char*, unsigned int) { return baseResult; }
    const char* GetBankPathname() { return "bank.big"; }
};
struct CBankFileAsync : CBankFile {
    CThreadedFile* ThreadedFile; bool NonCached, DisableAsync;
    virtual bool OpenFileReadOnly(const char* filename, unsigned int flags)
    {
        if (!CBankFile::OpenFileReadOnly(filename, flags)) return false;
        delete ThreadedFile;
        ThreadedFile = new CThreadedFile;
        ThreadedFile->Open(GetBankPathname(), NonCached);
        DisableAsync = false;
        return true;
    }
};

int main()
{
    CBankFileAsync b; b.baseResult = false; b.ThreadedFile = 0; b.NonCached = true; b.DisableAsync = true;
    if (b.OpenFileReadOnly("x", 0) || b.ThreadedFile || !b.DisableAsync) { std::printf("009d5820_TEST FAIL\n"); return 1; }
    b.baseResult = true;
    if (!b.OpenFileReadOnly("x", 0) || !b.ThreadedFile || b.DisableAsync || g_threadedOpens != 1 || !g_lastNonCached) { std::printf("009d5820_TEST FAIL\n"); return 1; }
    delete b.ThreadedFile;
    std::printf("009d5820_TEST PASS\n");
    return 0;
}
