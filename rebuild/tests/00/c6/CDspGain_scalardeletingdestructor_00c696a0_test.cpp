// Behaviour test for 00c696a0: CDspGain scalar deleting destructor -- destroy, then free
#include <cstdio>
static int g_dtor = 0, g_base = 0;
class CDspObject { public: CDspObject() { } virtual ~CDspObject() { ++g_base; } };
class CDspGain : public CDspObject { public: CDspGain() { } ~CDspGain() { ++g_dtor; } float m_gain[4]; };
int main()
{
    CDspObject* p = new CDspGain;
    delete p;
    int ok = g_dtor == 1 && g_base == 1;
    std::printf(ok ? "00c696a0_TEST PASS\n" : "00c696a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
