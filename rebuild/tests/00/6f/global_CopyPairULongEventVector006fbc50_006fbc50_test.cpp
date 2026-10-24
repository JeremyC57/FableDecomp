#include <cstdio>

namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }

static int g_copies = 0;
struct CSmallVector_CEvent_32 {
    NAnimationEvents::CEvent* m_pData;
    unsigned long             m_count;
    void Copy(const CSmallVector_CEvent_32& other)     // stand-in for 0x006FB5C0: reallocate and copy
    {
        delete[] m_pData;
        m_pData = new NAnimationEvents::CEvent[other.m_count];
        m_count = other.m_count;
        for (unsigned long i = 0; i < other.m_count; ++i) m_pData[i] = other.m_pData[i];
        ++g_copies;
    }
    CSmallVector_CEvent_32& operator=(const CSmallVector_CEvent_32& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
};
struct SElem { unsigned long first; CSmallVector_CEvent_32 second; };
struct STag { STag() { } };

static SElem* Copy(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    STag tag;
    SElem buf[4];
    for (int i = 0; i < 4; ++i) {
        buf[i].first = 100 + i; buf[i].second.m_count = i; buf[i].second.m_pData = i ? new NAnimationEvents::CEvent[i] : 0;
        for (int j = 0; j < i; ++j) { buf[i].second.m_pData[j].m_a = i * 10 + j; buf[i].second.m_pData[j].m_b = -j; }
    }
    SElem* r = Copy(buf + 1, buf + 4, buf, tag, 0);       // shift down: 3 real copies
    bool ok = r == buf + 3 && g_copies == 3;
    for (int i = 0; i < 3; ++i) {
        if (buf[i].first != (unsigned long)(101 + i) || buf[i].second.m_count != (unsigned long)(i + 1)) ok = false;
        for (int j = 0; j <= i; ++j) if (buf[i].second.m_pData[j].m_a != (i + 1) * 10 + j) ok = false;
    }
    g_copies = 0;
    Copy(buf, buf + 1, buf, tag, 0);                      // self-assignment: no Copy call
    if (g_copies != 0 || buf[0].first != 101) ok = false;
    if (Copy(buf, buf, buf + 2, tag, 0) != buf + 2) ok = false;
    if (!ok) { std::printf("006fbc50_TEST FAIL\n"); return 1; }
    std::printf("006fbc50_TEST PASS\n");
    return 0;
}