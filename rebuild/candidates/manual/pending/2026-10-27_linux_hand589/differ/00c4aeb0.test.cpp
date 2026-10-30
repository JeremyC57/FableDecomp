// Behaviour test for 0x00C4AEB0 PKCBufferedReader::WaitForSlot: an out-of-range slot logs the trace line; buffers are
// handed back until the slot's owner byte is free; false when any hand-back reported no further result.
#include <cstdio>
#include <cstring>

static char g_lastLog[256]; static int g_breaks;
struct CString {
    char m_buf[256];
    CString() { m_buf[0] = 0; }
    CString& operator<<(const char* t) { std::strcat(m_buf, t); return *this; }
    CString& operator<<(long v) { char b[32]; std::sprintf(b, "%ld", v); std::strcat(m_buf, b); return *this; }
    CString& operator<<(unsigned int v) { char b[32]; std::sprintf(b, "%u", v); std::strcat(m_buf, b); return *this; }
};
static void AssertHook(const char* m) { std::strcpy(g_lastLog, m); }
static void (*g_pfnAssertHook)(const char*) = AssertHook;
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

struct PKCBufferedReader {
    unsigned char* m_slotOwners; unsigned int m_slotCount;
    int m_returnCalls; int m_failAt; unsigned int m_freeOrder[8]; int m_next;
    void ReturnBuffer(bool* more)          // frees the next slot of the ring; reports "no more" on call m_failAt
    {
        ++m_returnCalls;
        m_slotOwners[m_freeOrder[m_next++]] = 0xFF;
        *more = m_returnCalls != m_failAt;
    }
    bool WaitForSlot(unsigned int slot)
    {
        bool ok = true;
        if (!(slot < m_slotCount)) {
            {
                CString line;
                line << ".\\PKCBufferedReader.cpp" << "(" << (long)84 << ") : " << "Error: Request number (" << slot
                     << ") out of scope (" << m_slotCount << ")" << "\n";
                if (g_pfnAssertHook)
                    g_pfnAssertHook(line.m_buf);
            }
            CDebugBreak();
        }
        while (m_slotOwners[slot] != 0xFF) {
            bool more;
            ReturnBuffer(&more);
            if (!more)
                ok = false;
        }
        return ok;
    }
};

int main()
{
    int bad = 0;
    unsigned char owners[8] = { 0, 1, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    PKCBufferedReader r;
    r.m_slotOwners = owners; r.m_slotCount = 4; r.m_returnCalls = 0; r.m_failAt = 0; r.m_next = 0;
    r.m_freeOrder[0] = 0; r.m_freeOrder[1] = 1; r.m_freeOrder[2] = 2;
    if (!r.WaitForSlot(3) || r.m_returnCalls != 0 || g_breaks != 0) bad = 1;          // already free
    if (!r.WaitForSlot(1) || r.m_returnCalls != 2 || owners[0] != 0xFF || owners[1] != 0xFF) bad = 1;
    r.m_failAt = 3;                                                                      // the next hand-back reports none left
    if (r.WaitForSlot(2) || r.m_returnCalls != 3 || owners[2] != 0xFF) bad = 1;
    if (g_breaks != 0 || g_lastLog[0]) bad = 1;
    r.m_slotCount = 2;                                                                   // slot 2 now out of scope: trace + break
    if (!r.WaitForSlot(2) || g_breaks != 1
        || std::strcmp(g_lastLog, ".\\PKCBufferedReader.cpp(84) : Error: Request number (2) out of scope (2)\n")) bad = 1;
    if (bad) { std::printf("00c4aeb0_TEST FAIL\n"); return 1; }
    std::printf("00c4aeb0_TEST PASS\n");
    return 0;
}
