// Behaviour test for 00c69880: CCombFilter::CCombFilter -- the DSP base runs once, the body is left alone
#include <cstdio>
#include <cstring>
#include <new>
static int g_base = 0;
class CDspObject { public: CDspObject() { ++g_base; } virtual ~CDspObject() { } };
class CCombFilter : public CDspObject { public: CCombFilter() { } unsigned char _body[0x14 - 4]; };
int main()
{
    unsigned char raw[sizeof(CCombFilter)]; std::memset(raw, 0x3c, sizeof raw);
    CCombFilter* p = new (raw) CCombFilter;
    int ok = g_base == 1 && sizeof(CCombFilter) == 0x14 && p->_body[3] == 0x3c;
    std::printf(ok ? "00c69880_TEST PASS\n" : "00c69880_TEST FAIL\n");
    return ok ? 0 : 1;
}
