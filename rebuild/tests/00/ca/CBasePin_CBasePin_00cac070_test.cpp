// Behaviour test for 0x00CAC070 CBasePin::CBasePin (ANSI object name) -- model copy of the member initialisation and the name copy.
#include <cstdio>
#include <cstring>

typedef unsigned short WCHAR16;
static int Len16(const WCHAR16* s) { int n = 0; while (s[n]) ++n; return n; }
struct CPin {
    WCHAR16* m_pName; void* m_Connected; long m_dir; void* m_pLock; bool a, b, c; void* m_pFilter; void* m_pQSink; long m_TypeVersion;
    long long m_tStart, m_tStop; double m_dRate;
    CPin(void* filter, void* lock, const WCHAR16* name, long dir)
        : m_pName(0), m_Connected(0), m_dir(dir), m_pLock(lock), a(false), b(false), c(false), m_pFilter(filter), m_pQSink(0),
          m_TypeVersion(1), m_tStart(0), m_tStop(0x7fffffffffffffffLL), m_dRate(1.0)
    {
        if (name) {
            int length = Len16(name) + 1;
            m_pName = new WCHAR16[length];
            if (m_pName) std::memcpy(m_pName, name, length * sizeof(WCHAR16));
        }
    }
};

int main()
{
    static const WCHAR16 name[] = { 'I', 'n', 0 };
    int lock, filter;
    CPin p(&filter, &lock, name, 1);
    if (!p.m_pName || p.m_pName[0] != 'I' || p.m_pName[2] != 0 || p.m_dir != 1 || p.m_pLock != &lock || p.m_pFilter != &filter ||
        p.m_TypeVersion != 1 || p.m_tStop != 0x7fffffffffffffffLL || p.m_dRate != 1.0 || p.m_Connected)
    { std::printf("00cac070_TEST FAIL\n"); return 1; }
    CPin q(&filter, &lock, 0, 0);
    if (q.m_pName != 0) { std::printf("00cac070_TEST FAIL\n"); return 1; }
    delete[] p.m_pName;
    std::printf("00cac070_TEST PASS\n");
    return 0;
}