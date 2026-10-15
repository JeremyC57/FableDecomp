#include <cstdio>
#include <cstring>
struct CFakeCom {
    long m_refs; long m_stopResult;
    virtual unsigned long Release() { return --m_refs; }
    virtual long Stop() { return m_stopResult; }
};
struct CSoundStream {
    CFakeCom* m_pBuffer; CFakeCom* m_pNotify;
    long ReleaseBuffers()
    {
        if (m_pBuffer) {
            long hr = m_pBuffer->Stop();
            if (hr < 0) return hr;
            m_pBuffer->Release(); m_pBuffer = 0;
        }
        if (m_pNotify) { m_pNotify->Release(); m_pNotify = 0; }
        return 0;
    }
};
int main()
{
    int ok = 1;
    CFakeCom buf; buf.m_refs = 1; buf.m_stopResult = -7;
    CFakeCom note; note.m_refs = 1; note.m_stopResult = 0;
    CSoundStream s; s.m_pBuffer = &buf; s.m_pNotify = &note;
    if (s.ReleaseBuffers() != -7 || s.m_pBuffer != &buf || s.m_pNotify != &note || buf.m_refs != 1) ok = 0;
    buf.m_stopResult = 0;
    if (s.ReleaseBuffers() != 0 || s.m_pBuffer || s.m_pNotify || buf.m_refs != 0 || note.m_refs != 0) ok = 0;
    if (!ok) { std::printf("00ca8660_TEST FAIL\n"); return 1; }
    std::printf("00ca8660_TEST PASS\n");
    return 0;
}