// Behaviour test for 00c698a0: CCombFilter::~CCombFilter -- only the base is destroyed
#include <cstdio>
static int g_base = 0;
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { ++g_base; } };
class CCombFilter : public CDspObject { public: CCombFilter() : m_mode(0) { } unsigned char _pad04[0x0c]; long m_mode; };
int main()
{
    CDspObject* c = new CCombFilter;
    delete c;
    { CCombFilter combs[4]; }
    int ok = g_base == 5 && sizeof(CCombFilter) == 0x14;
    std::printf(ok ? "00c698a0_TEST PASS\n" : "00c698a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
