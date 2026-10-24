// Behaviour test for 0x00C38CD0 CSlotRequest_00c38cd0::Setup
#include <stdio.h>
static long g_log[16];
static int g_n = 0;
struct CRange { long lo, hi; void Set(long l, long h) { lo = l; hi = h; g_log[g_n++] = 100 + l; } };
struct CRequestPair {
    void StartFirst(long a, long b, long c, void* t) { g_log[g_n++] = a + b + c; (void)t; }
    void StartSecond(long a, long b, long c, void* t) { g_log[g_n++] = 1000 + a + b + c; (void)t; }
};
struct CSlotRequest_00c38cd0 {
    unsigned char m_secondTarget[0x6C];
    unsigned char m_firstTarget[0x4C];
    long          m_active;
    CRequestPair  m_requests;
    CRange        m_range;
    long          m_slotA;
    long          m_slotB;
    long          m_slotC;
    void Setup(long owner0, long owner1, long a, long b, long c, long owner, long rangeLo, long rangeHi);
};
void CSlotRequest_00c38cd0::Setup(long owner0, long owner1, long a, long b, long c, long owner, long rangeLo, long rangeHi)
{
    m_range.Set(0, 0);
    m_range.Set(rangeLo, rangeHi);
    m_requests.StartFirst(owner0, owner1, owner, m_firstTarget);
    m_requests.StartSecond(a, b, c, m_secondTarget);
    m_slotA = m_slotC = m_slotB = -1;
    m_active = 1;
}
int main()
{
    int ok = 1;
    CSlotRequest_00c38cd0 r;
    r.m_slotA = r.m_slotB = r.m_slotC = 5; r.m_active = 0;
    r.Setup(1, 2, 3, 4, 5, 6, 7, 8);
    if (r.m_slotA != -1 || r.m_slotB != -1 || r.m_slotC != -1 || r.m_active != 1) ok = 0;
    if (r.m_range.lo != 7 || r.m_range.hi != 8) ok = 0;
    if (g_n != 4 || g_log[0] != 100 || g_log[1] != 107 || g_log[2] != 9 || g_log[3] != 1012) ok = 0;
    printf(ok ? "00c38cd0_TEST PASS\n" : "00c38cd0_TEST FAIL\n");
    return ok ? 0 : 1;
}