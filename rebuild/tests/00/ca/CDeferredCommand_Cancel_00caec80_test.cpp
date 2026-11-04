#include <cstdio>

// HRESULTs are 32-bit: modelled as int so the test also holds on a 64-bit host (long is 64-bit there).
struct CAdviseHolder;
static CAdviseHolder* g_holder = 0; static int g_result = 0;
struct IAdviseSink {
    virtual ~IAdviseSink() { }
    virtual int Unadvise(CAdviseHolder* holder) { g_holder = holder; return g_result; }
};
struct CPrimaryBase { virtual ~CPrimaryBase() { } int m_word04; int m_word08; };
struct IAdviseSource { virtual int Unadvise() = 0; virtual ~IAdviseSource() { } };
struct CAdviseHolder : CPrimaryBase, IAdviseSource {
    IAdviseSink* m_pSink;
    int Unadvise()
    {
        if (!m_pSink)
            return 0x80040234;
        int hr = m_pSink->Unadvise(this);
        if (hr < 0)
            return hr;
        m_pSink = 0;
        return 0;
    }
};

int main()
{
    IAdviseSink sink; CAdviseHolder h; h.m_pSink = 0;
    IAdviseSource* iface = &h;                           // called through the secondary interface, as retail is
    if (iface->Unadvise() != (int)0x80040234) { std::printf("00caec80_TEST FAIL\n"); return 1; }
    h.m_pSink = &sink; g_result = (int)0x80004005;
    if (iface->Unadvise() != (int)0x80004005 || h.m_pSink != &sink || g_holder != &h) { std::printf("00caec80_TEST FAIL\n"); return 1; }
    g_result = 1;                                        // S_FALSE still counts as success: sink dropped, S_OK returned
    if (iface->Unadvise() != 0 || h.m_pSink != 0) { std::printf("00caec80_TEST FAIL\n"); return 1; }
    std::printf("00caec80_TEST PASS\n");
    return 0;
}