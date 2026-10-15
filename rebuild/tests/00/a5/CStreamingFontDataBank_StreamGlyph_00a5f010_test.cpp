// Behaviour test for 0x00A5F010: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model: the bank file's blocking and asynchronous reads and the streaming switch.
struct Data { int id; };
struct CP { Data* d; void* info; CP() : d(0), info(0) {} };
static Data g_req = { 77 };
struct BankFile { int syncReads, asyncReads; unsigned int lastOffset, lastSize, lastBufSize; int lastPriority; void* lastBuf;
    CP ReadAsync(unsigned int off, void* buf, unsigned int size, unsigned int bufSize, int prio)
    { ++asyncReads; lastOffset = off; lastBuf = buf; lastSize = size; lastBufSize = bufSize; lastPriority = prio; CP p; p.d = &g_req; return p; }
    void Read(unsigned int off, void* buf, unsigned int size) { ++syncReads; lastOffset = off; lastBuf = buf; lastSize = size; } };
static bool g_async = true;
struct Bank { BankFile* file;
    CP StreamGlyph(void* buf, unsigned int off, unsigned int size, bool sync)
    {
        if (!sync && g_async) return file->ReadAsync(off, buf, size, size, 4);
        file->Read(off, buf, size);
        return CP();
    } };
static void Run()
{
    BankFile f; f.syncReads = 0; f.asyncReads = 0; Bank b; b.file = &f; char buf[4];
    CP a = b.StreamGlyph(buf, 100, 32, false);
    CHECK(a.d == &g_req && f.asyncReads == 1 && f.lastOffset == 100 && f.lastSize == 32 && f.lastBufSize == 32 && f.lastPriority == 4 && f.lastBuf == buf);
    CP s = b.StreamGlyph(buf, 200, 16, true);
    CHECK(s.d == 0 && s.info == 0 && f.syncReads == 1 && f.lastOffset == 200);
    g_async = false; CP t = b.StreamGlyph(buf, 300, 8, false); CHECK(t.d == 0 && f.syncReads == 2 && f.asyncReads == 1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a5f010_TEST FAIL\n"); return 1; }
    std::printf("00a5f010_TEST PASS\n");
    return 0;
}