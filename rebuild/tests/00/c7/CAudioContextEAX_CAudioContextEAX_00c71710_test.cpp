// Behaviour test for 00c71710: CAudioContextEAX::CAudioContextEAX -- the base sets up the aligned mix block
#include <cstdio>
static int g_base = 0;
class CAudioContextBase {
public:
    CAudioContextBase() { ++g_base; m_pMix = (long*)(((unsigned long)this + 0x1b) & ~15UL); }
    virtual ~CAudioContextBase() { }
    unsigned char _pad004[0x218]; long* m_pMix;
};
class CAudioContextEAX : public CAudioContextBase { public: CAudioContextEAX() { } };
int main()
{
    CAudioContextEAX* c = new CAudioContextEAX;
    unsigned long p = (unsigned long)c->m_pMix, t = (unsigned long)c;
    int ok = g_base == 1 && (p & 15) == 0 && p >= t + 0x0c && p <= t + 0x1b && sizeof(CAudioContextBase) == 0x220;
    delete c;
    std::printf(ok ? "00c71710_TEST PASS\n" : "00c71710_TEST FAIL\n");
    return ok ? 0 : 1;
}
