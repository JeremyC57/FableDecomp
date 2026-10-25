// 00c4ac70 behaviour test: PKCBufferedReader::ReturnBuffer -- the finished request id is taken from the mailbox, every
// consecutive ring slot from that id carrying it is freed (0xFF), the walk stops at the first foreign slot, the
// outstanding count drops, and the optional out flag reports whether more results are queued; each broken expectation
// logs one assertion line.
#include <cstdio>

static int g_logLines = 0;
static long g_lastLine = 0;
#define PK_ASSERT_MSG(cond, line_no) if (!(cond)) { ++g_logLines; g_lastLine = line_no; }

struct PKCMailbox {
    unsigned long results[8];
    long count;
    long GetCount() { return count; }
    unsigned long GetResult()
    {
        unsigned long r = results[0];
        for (long i = 1; i < count; ++i) results[i - 1] = results[i];
        --count;
        return r;
    }
};

struct PKCBufferedReader {
    unsigned char* m_slotOwners;
    unsigned long  m_slotCount;
    PKCMailbox     m_mailbox;
    long           m_outstanding;
    void ReturnBuffer(bool* pMoreResults)
    {
        PK_ASSERT_MSG(m_outstanding != 0, 56)
        if (pMoreResults)
            *pMoreResults = m_mailbox.GetCount() != 0;
        unsigned long id = m_mailbox.GetResult();
        PK_ASSERT_MSG(id < m_slotCount, 62)
        PK_ASSERT_MSG(m_slotOwners[id] == id, 63)
        for (unsigned long i = 0; i < m_slotCount; ++i) {
            unsigned long slot = (i + id) % m_slotCount;
            if (m_slotOwners[slot] == id)
                m_slotOwners[slot] = 0xff;
            else
                i = m_slotCount;
        }
        --m_outstanding;
    }
};

static int Fail(const char* why) { std::printf("00c4ac70_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    // slots: 0..5; request 4 owns slots 4,5 and wraps onto 0; request 1 owns slots 1,2; slot 3 free
    unsigned char owners[6] = { 4, 1, 1, 0xff, 4, 4 };
    PKCBufferedReader r;
    r.m_slotOwners = owners; r.m_slotCount = 6; r.m_outstanding = 2;
    r.m_mailbox.count = 2; r.m_mailbox.results[0] = 4; r.m_mailbox.results[1] = 1;
    bool more = false;
    r.ReturnBuffer(&more);
    if (!more || r.m_outstanding != 1 || g_logLines != 0) return Fail("first return");
    if (owners[4] != 0xff || owners[5] != 0xff || owners[0] != 0xff || owners[1] != 1 || owners[2] != 1) return Fail("ring walk");
    r.ReturnBuffer(0);
    if (r.m_outstanding != 0 || owners[1] != 0xff || owners[2] != 0xff || owners[3] != 0xff || g_logLines != 0) return Fail("second return");
    // out-of-sync id with nothing outstanding: lines 56 and 63
    r.m_mailbox.count = 1; r.m_mailbox.results[0] = 2;
    more = false;
    r.ReturnBuffer(&more);
    if (!more || g_logLines != 2 || g_lastLine != 63 || r.m_outstanding != -1) return Fail("asserts");
    std::printf("00c4ac70_TEST PASS\n");
    return 0;
}