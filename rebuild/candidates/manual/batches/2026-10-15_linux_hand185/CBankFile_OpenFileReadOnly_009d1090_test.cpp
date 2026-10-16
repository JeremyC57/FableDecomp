// Behaviour test for 009d1090: CBankFile::OpenFileReadOnly -- missing file fails early; header failure closes; success runs the post-open step.
#include <cstdio>
struct TBank {
    bool exists, headerOk; bool lastValidate; int opens, closes, posts; bool ReadOnly, RetailMode; unsigned long OpenFlags, ChangeCount;
    TBank() : exists(true), headerOk(true), lastValidate(false), opens(0), closes(0), posts(0), ReadOnly(false), RetailMode(true), OpenFlags(0), ChangeCount(9) {}
    bool ReadFileInformation(bool v) { lastValidate = v; return headerOk; }
    bool OpenFileReadOnly(unsigned long flags)
    {
        ReadOnly = true; OpenFlags = flags; ChangeCount = 0; RetailMode = false;
        if (!exists) return false;
        ++opens;
        if (!ReadFileInformation(!(flags & 2))) { ++closes; return false; }
        ++posts; return true;
    }
};
int main()
{
    TBank a; a.exists = false;
    if (a.OpenFileReadOnly(5) || a.opens || !a.ReadOnly || a.OpenFlags != 5 || a.ChangeCount || a.RetailMode) { std::printf("009d1090_TEST FAIL\n"); return 1; }
    TBank b; b.headerOk = false;
    if (b.OpenFileReadOnly(2) || b.closes != 1 || b.lastValidate) { std::printf("009d1090_TEST FAIL\n"); return 1; }
    TBank c;
    if (!c.OpenFileReadOnly(0) || c.posts != 1 || !c.lastValidate || c.closes) { std::printf("009d1090_TEST FAIL\n"); return 1; }
    std::printf("009d1090_TEST PASS\n");
    return 0;
}
