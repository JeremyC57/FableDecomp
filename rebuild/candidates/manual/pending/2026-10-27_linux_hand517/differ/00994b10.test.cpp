// Test: CMemoryDataOutputStream::SetPosition -- positions up to the data size are accepted, past the end throws SeekOutOfRange (11)
#include <cstdio>
struct CBBBFileException { long Type; };
struct CStream {
    unsigned long size; unsigned long CurrentPosition;
    void SetPosition(unsigned long position)
    {
        if (position <= size) { CurrentPosition = position; return; }
        CBBBFileException e = { 11 };
        throw e;
    }
};
int main()
{
    CStream s = { 10, 0 };
    s.SetPosition(10);
    bool threw = false;
    try { s.SetPosition(11); } catch (const CBBBFileException& e) { threw = (e.Type == 11); }
    if (s.CurrentPosition != 10 || !threw) { std::printf("00994b10_TEST FAIL\n"); return 1; }
    std::printf("00994b10_TEST PASS\n");
    return 0;
}
