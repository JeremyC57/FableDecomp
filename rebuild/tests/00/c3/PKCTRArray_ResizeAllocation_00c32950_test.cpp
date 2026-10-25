// 00c32950 behaviour test: PKCTRArray::ResizeAllocation -- the entry block grows by growBy, the old entries are copied
// to the front of the new block and the old block is freed; a zero growth or an empty-capacity array with no block
// logs, but the resize still happens.
#include <cstdio>
#include <cstring>

static int g_logLines = 0;
static long g_lastLine = 0;
static int g_frees = 0;
#define PK_ASSERT_MSG(cond, line_no) if (!(cond)) { ++g_logLines; g_lastLine = line_no; }

struct PKCTRArray {
    void**        m_pEntries;
    unsigned long m_capacity;
    unsigned long m_count;
    void ResizeAllocation(unsigned long growBy)
    {
        PK_ASSERT_MSG(growBy > 0, 29)
        void** entries = new void*[m_capacity + growBy];
        PK_ASSERT_MSG(entries != 0, 31)
        if (m_capacity) {
            PK_ASSERT_MSG(m_pEntries != 0, 34)
            std::memcpy(entries, m_pEntries, m_capacity * sizeof(void*));
            delete[] m_pEntries;
            ++g_frees;
        }
        m_pEntries = entries;
        m_capacity += growBy;
    }
};

static int Fail(const char* why) { std::printf("00c32950_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    static int a, b, c;
    PKCTRArray arr; arr.m_pEntries = 0; arr.m_capacity = 0; arr.m_count = 0;
    arr.ResizeAllocation(2);
    if (arr.m_capacity != 2 || arr.m_pEntries == 0 || g_frees != 0 || g_logLines != 0) return Fail("first");
    arr.m_pEntries[0] = &a; arr.m_pEntries[1] = &b;
    void** old = arr.m_pEntries;
    arr.ResizeAllocation(3);
    if (arr.m_capacity != 5 || arr.m_pEntries == old || g_frees != 1) return Fail("grow");
    if (arr.m_pEntries[0] != &a || arr.m_pEntries[1] != &b) return Fail("copy");
    arr.m_pEntries[2] = &c;
    arr.ResizeAllocation(0);
    if (g_logLines != 1 || g_lastLine != 29 || arr.m_capacity != 5 || arr.m_pEntries[2] != &c) return Fail("zero growth");
    delete[] arr.m_pEntries;
    std::printf("00c32950_TEST PASS\n");
    return 0;
}