// Behaviour test for the 0x0071F3D0 record constructor: the leading word is zeroed, the CDateAndTime member is
// default-constructed (seven zero words) and the argument is ignored.
#include <cstdio>
static int g_dateCtors = 0;
class CDateAndTime { public: unsigned long m_words[7]; CDateAndTime() { for (int i = 0; i < 7; ++i) m_words[i] = 0; ++g_dateCtors; } };
struct CRecord { long m_first; CDateAndTime m_second; CRecord(long) : m_first(0), m_second() { } };
int main()
{
    CRecord r(1234);
    bool ok = r.m_first == 0 && g_dateCtors == 1;
    for (int i = 0; i < 7; ++i) ok = ok && r.m_second.m_words[i] == 0;
    if (!ok) { std::printf("0071f3d0_TEST FAIL\n"); return 1; }
    std::printf("0071f3d0_TEST PASS\n");
    return 0;
}