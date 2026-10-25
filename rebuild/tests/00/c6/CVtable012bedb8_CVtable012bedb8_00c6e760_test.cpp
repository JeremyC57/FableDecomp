#include <cstdio>
struct CVtable_012beda4 {                              // vtable 0x012BEDA4, 0x28 bytes
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 lock owner, count, flag
    CVtable_012beda4();                                // 0x00C6E6A0
};
struct SGainRamp {                                     // 4 bytes: a Q15 gain and its per-sample step
    unsigned short m_gain;                             // +0x00
    short          m_step;                             // +0x02
};
struct CVtable_012bedb8 : CVtable_012beda4 {           // vtable 0x012BEDB8
    SGainRamp  m_first;                                // +0x28
    SGainRamp  m_second;                               // +0x2c
    SGainRamp* m_pCurrent;                             // +0x30
    SGainRamp* m_pPrevious;                            // +0x34
    virtual void v00();
    CVtable_012bedb8();
};
CVtable_012bedb8::CVtable_012bedb8()
    : m_pCurrent(&m_first), m_pPrevious(&m_second)
{
}
static int g_baseBuilt;
CVtable_012beda4::CVtable_012beda4() { ++g_baseBuilt; }
void CVtable_012beda4::v00() { }
void CVtable_012bedb8::v00() { }
static int fail() { std::printf("00c6e760_TEST FAIL\n"); return 1; }
int main()
{
    CVtable_012bedb8 e;
    if (g_baseBuilt != 1) return fail();
    if (e.m_pCurrent != &e.m_first || e.m_pPrevious != &e.m_second) return fail();
    if ((void*)&e.m_second <= (void*)&e.m_first) return fail();
    std::printf("00c6e760_TEST PASS\n");
    return 0;
}