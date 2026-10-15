#include <cstdio>
#include <string.h>

static int g_baseBuilt = 0;

struct CBaseWithLock {
    long m_count; unsigned char m_flag;
    CBaseWithLock() : m_count(0), m_flag(0) { ++g_baseBuilt; }
    virtual ~CBaseWithLock() { }
};

struct SRecord { long m_words[6]; SRecord() { memset(m_words, 0, sizeof(m_words)); } };
struct SPlainRecord {
    long m_words0, m_words1, m_words2, m_words3, m_words4, m_words5;
    SPlainRecord() : m_words0(0), m_words1(0), m_words2(0), m_words3(0), m_words4(0), m_words5(0) { }
};

struct CDerived : CBaseWithLock {
    SRecord m_first; SRecord m_second; SRecord* m_pCurrent; SRecord* m_pPrevious; SPlainRecord m_third;
    CDerived() : m_first(), m_second(), m_pCurrent(&m_first), m_pPrevious(&m_second), m_third() { }
};

int main()
{
    CDerived* d = new CDerived;
    bool ok = g_baseBuilt == 1 && d->m_pCurrent == &d->m_first && d->m_pPrevious == &d->m_second
           && d->m_third.m_words0 == 0 && d->m_third.m_words5 == 0;
    for (int i = 0; i < 6; ++i) ok = ok && d->m_first.m_words[i] == 0 && d->m_second.m_words[i] == 0;
    delete d;
    if (!ok) { std::printf("00c6d9d0_TEST FAIL\n"); return 1; }
    std::printf("00c6d9d0_TEST PASS\n");
    return 0;
}