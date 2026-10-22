// Behaviour test for 00779ca0: body copied from src/00779ca0.cpp (CTCDExperienceOrb::CTCDExperienceOrb), the base constructor stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CThing;
static CThing* g_baseParent = 0; static int g_baseCalls = 0;
struct CTCDriverBase {
    virtual void v00() {}
    unsigned char _body[0x8];
    CTCDriverBase(CThing* parent) { ++g_baseCalls; g_baseParent = parent; }
};
struct CIndexM1 { long v; CIndexM1() : v(-1) {} };
struct CZeroWord { long v; CZeroWord() : v(0) {} };
struct CTCDExperienceOrb : CTCDriverBase {
    long          m_target;
    CIndexM1      m_index10;
    CIndexM1      m_index14;
    long          LifeTimer;
    bool          m_firstUpdate;
    bool          m_collected;
    virtual void v00() {}
    CTCDExperienceOrb(CThing* parent);
};
CTCDExperienceOrb::CTCDExperienceOrb(CThing* parent)
    : CTCDriverBase(parent)
    , m_target(0)
    , m_firstUpdate(true)
    , m_collected(false)
{
}
int main()
{
    unsigned char raw[sizeof(CTCDExperienceOrb)]; memset(raw, 0xAA, sizeof(raw));
    CThing* parent = (CThing*)0x1234;
    CTCDExperienceOrb* p = new (raw) CTCDExperienceOrb(parent);
    CHECK(g_baseCalls == 1); CHECK(g_baseParent == parent);
    CHECK(p->m_target == 0); CHECK(p->m_index10.v == -1); CHECK(p->m_index14.v == -1);
    CHECK(p->m_firstUpdate == true); CHECK(p->m_collected == false);
    CHECK(p->LifeTimer == (long)0xAAAAAAAA);
    if (fails == 0) printf("00779ca0_TEST PASS\n"); else printf("00779ca0_TEST FAIL\n");
    return fails;
}