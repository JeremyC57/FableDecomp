// 00c4a1a0 behaviour test: PKCBufferedReader::GetAvailableReadSize -- a read that fits returns its size; one past the
// end returns what is left; each broken expectation (unaligned size, overrun of a whole granule or more, no bytes
// left) logs one assertion line through the sink and fires the trace helper.
#include <cstdio>
#include <cstring>

static int g_logLines = 0;
static int g_traces = 0;
static long g_lastLine = 0;
static void LogSink(long lineNo) { ++g_logLines; g_lastLine = lineNo; }

#define PK_ASSERT_MSG(cond, line_no) if (!(cond)) { LogSink(line_no); ++g_traces; }

struct PKCBufferedReader {
    unsigned long m_blockSize;
    unsigned long m_size;
    long GetAvailableReadSize(unsigned long position, long size)
    {
        PK_ASSERT_MSG(size % m_blockSize == 0, 97)
        long remaining = (long)(m_size - position - size);
        if (remaining >= 0)
            return size;
        long overrun = -remaining;
        PK_ASSERT_MSG(overrun < (long)m_blockSize, 101)
        PK_ASSERT_MSG(size > overrun, 102)
        return remaining + size;
    }
};

static int Fail(const char* why) { std::printf("00c4a1a0_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    PKCBufferedReader r; r.m_blockSize = 0x800; r.m_size = 0x2500;
    if (r.GetAvailableReadSize(0, 0x1000) != 0x1000 || g_logLines != 0) return Fail("fits");
    if (r.GetAvailableReadSize(0x2000, 0x800) != 0x500 || g_logLines != 0) return Fail("tail");
    if (r.GetAvailableReadSize(0x1800, 0x1000) != 0xd00 || g_logLines != 0) return Fail("primed within a granule");
    if (r.GetAvailableReadSize(0x2000, 0x1000) != 0x500 || g_logLines != 1 || g_lastLine != 101) return Fail("overrun");
    g_logLines = 0;
    if (r.GetAvailableReadSize(0, 0x100) != 0x100 || g_logLines != 1 || g_lastLine != 97) return Fail("unaligned");
    g_logLines = 0;
    r.m_size = 0x2000;
    if (r.GetAvailableReadSize(0x2800, 0x800) != -0x800 || g_logLines != 2 || g_lastLine != 102) return Fail("negative");
    if (g_traces != 4) return Fail("trace count");
    std::printf("00c4a1a0_TEST PASS\n");
    return 0;
}