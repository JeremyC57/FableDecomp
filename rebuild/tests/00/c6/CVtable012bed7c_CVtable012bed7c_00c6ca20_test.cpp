// Behaviour test for 0x00c6ca20 CVtable_012bed7c constructor
#include <cstdio>
#include <string.h>
struct CBase { unsigned char _body[0x24]; CBase() { memset(_body, 0x5a, sizeof(_body)); } virtual ~CBase() {} };
struct SRecord { long m_words[12]; SRecord() { memset(m_words, 0, sizeof(m_words)); } };
struct CObject : CBase {
    unsigned char  m_state[0x40];
    unsigned char  m_buffer[0x110];
    unsigned char* m_pAlignedBuffer;
    SRecord  m_first, m_second;
    SRecord* m_pCurrent; SRecord* m_pPrevious;
    long m_valuesA[6], m_valuesB[6];
    CObject()
        : m_pAlignedBuffer(reinterpret_cast<unsigned char*>((reinterpret_cast<unsigned long>(m_buffer) + 15) & ~15UL)),
          m_first(), m_second(), m_pCurrent(&m_first), m_pPrevious(&m_second)
    {
        for (long i = 0; i < 6; ++i) { m_valuesA[i] = 0; m_valuesB[i] = 0; }
    }
};
int main()
{
    CObject* o = new CObject;
    bool ok = (reinterpret_cast<unsigned long>(o->m_pAlignedBuffer) & 15) == 0 && o->m_pAlignedBuffer >= o->m_buffer
           && o->m_pAlignedBuffer < o->m_buffer + 16 && o->m_pCurrent == &o->m_first && o->m_pPrevious == &o->m_second;
    for (int i = 0; i < 12; ++i) ok = ok && o->m_first.m_words[i] == 0 && o->m_second.m_words[i] == 0;
    for (int i = 0; i < 6; ++i) ok = ok && o->m_valuesA[i] == 0 && o->m_valuesB[i] == 0;
    delete o;
    if (!ok) { std::printf("00c6ca20_TEST FAIL\n"); return 1; }
    std::printf("00c6ca20_TEST PASS\n");
    return 0;
}