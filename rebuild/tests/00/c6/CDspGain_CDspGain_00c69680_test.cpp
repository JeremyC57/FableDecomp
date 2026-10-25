// Behaviour test for 00c69680: CDspGain::~CDspGain -- the base destructor runs after the (empty) body
#include <cstdio>
static int g_base = 0, g_body = 0;
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { ++g_base; } };
class CDspGain : public CDspObject { public: ~CDspGain() { ++g_body; } unsigned char _body[0x14 - 4]; };
int main()
{
    CDspObject* p = new CDspGain;
    delete p;
    int ok = g_body == 1 && g_base == 1 && sizeof(CDspGain) == 0x14;
    std::printf(ok ? "00c69680_TEST PASS\n" : "00c69680_TEST FAIL\n");
    return ok ? 0 : 1;
}
