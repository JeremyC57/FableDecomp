#include <cstdio>
// Behaviour test for SStringPairRecord_0055de90::SStringPairRecord_0055de90 @ 0x0055DE90
static int g_order = 0;
class CCharString {
public:
    CCharString() : m_pStringData(0), m_order(++g_order) {}
    void* m_pStringData;
    int m_order;
};
struct SStringPairRecord_0055de90 {
    CCharString m_first;
    long m_index;
    CCharString m_second;
    SStringPairRecord_0055de90() : m_index(-1) {}
};
int main()
{
    SStringPairRecord_0055de90 r;
    if (r.m_index != -1 || r.m_first.m_order != 1 || r.m_second.m_order != 2 || r.m_first.m_pStringData != 0) { std::printf("0055de90_TEST FAIL\n"); return 1; }
    std::printf("0055de90_TEST PASS\n");
    return 0;
}