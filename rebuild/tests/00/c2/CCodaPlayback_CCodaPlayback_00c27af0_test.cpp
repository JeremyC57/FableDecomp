// behaviour test for 0x00C27AF0 CCodaPlayback::CCodaPlayback: polymorphic object, two leading words and the source cleared
#include <cstdio>
#include <string.h>
#include <new>

struct CODASource;
static int g_calls = 0;

struct CCodaPlayback {
    virtual void SetSourceProperty20(float) { ++g_calls; }
    int           m_04;
    int           m_08;
    CODASource*   m_pSource;
    int           m_loopsLeft;
    CCodaPlayback();
};

CCodaPlayback::CCodaPlayback()
    : m_04(0), m_08(0), m_pSource(0)
{
}

int main()
{
    void* raw[(sizeof(CCodaPlayback) + sizeof(void*) - 1) / sizeof(void*)];
    memset(raw, 0xCD, sizeof(raw));
    CCodaPlayback* p = new (raw) CCodaPlayback;
    bool ok = p->m_04 == 0 && p->m_08 == 0 && p->m_pSource == 0 && p->m_loopsLeft == (int)0xCDCDCDCD;
    p->SetSourceProperty20(1.0f);
    ok = ok && g_calls == 1;
    if (!ok) { std::printf("00c27af0_TEST FAIL\n"); return 1; }
    std::printf("00c27af0_TEST PASS\n");
    return 0;
}