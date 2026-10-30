// Behaviour test for CBaseInputPin::Receive (0x00CA8B90): a null sample is E_POINTER; a not-streaming pin returns the
// CheckStreaming code; an IMediaSample2 sample fills the properties through GetProperties; a plain sample fills them field
// by field (discontinuity / preroll / sync point / time / type-change flags, buffer, actual length, size); a type change
// that CheckMediaType refuses flags a run-time error, ends the stream, notifies EC_ERRORABORT and returns
// VFW_E_INVALIDMEDIATYPE.
#include <cstdio>
#include <cstring>

typedef long HRESULT;
#define S_OK 0L
#define S_FALSE 1L
#define E_POINTER ((HRESULT)0x80004003L)
#define E_NOINTERFACE ((HRESULT)0x80004002L)
#define VFW_E_NOT_STOPPED_STREAMING ((HRESULT)0x8004020EL)
#define VFW_E_TYPE_NOT_ACCEPTED ((HRESULT)0x8004022AL)
#define VFW_E_INVALIDMEDIATYPE ((HRESULT)0x80040200L)
#define EC_ERRORABORT 3

struct AM_SAMPLE2_PROPERTIES {
    unsigned long cbData, dwTypeSpecificFlags, dwSampleFlags;
    long lActual;
    double tStart, tStop;
    unsigned long dwStreamId;
    void* pMediaType;
    unsigned char* pbBuffer;
    long cbBuffer;
};

struct Sample {
    bool has2, disc, preroll, sync, time, typechg;
    unsigned char buf[4];
    int released;
};

static int g_endOfStream = 0, g_notify = 0;
static long g_notifyCode = 0, g_notifyP1 = 0;

struct Pin {
    AM_SAMPLE2_PROPERTIES m_SampleProps;
    int m_bRunTimeError;
    HRESULT streaming, accept;
    HRESULT CheckStreaming() { return streaming; }
    HRESULT CheckMediaType(void*) { return accept; }
    void EndOfStream() { ++g_endOfStream; }
    void NotifyEvent(long code, long p1, long) { ++g_notify; g_notifyCode = code; g_notifyP1 = p1; }

    HRESULT Receive(Sample* pSample)
    {
        if (pSample == 0) return E_POINTER;
        HRESULT hr = CheckStreaming();
        if (S_OK != hr) return hr;
        if (pSample->has2) {                       // QueryInterface(IID_IMediaSample2) + GetProperties + Release
            m_SampleProps.cbData = sizeof(m_SampleProps);
            m_SampleProps.dwSampleFlags = 0x110 | 4;
            m_SampleProps.pMediaType = 0;
            ++pSample->released;
        } else {
            m_SampleProps.cbData = sizeof(m_SampleProps);
            m_SampleProps.dwTypeSpecificFlags = 0;
            m_SampleProps.dwStreamId = 0;
            m_SampleProps.dwSampleFlags = 0;
            if (pSample->disc) m_SampleProps.dwSampleFlags |= 4;
            if (pSample->preroll) m_SampleProps.dwSampleFlags |= 2;
            if (pSample->sync) m_SampleProps.dwSampleFlags |= 1;
            if (pSample->time) { m_SampleProps.tStart = 10; m_SampleProps.tStop = 20; m_SampleProps.dwSampleFlags |= 0x110; }
            if (pSample->typechg) { m_SampleProps.pMediaType = pSample; m_SampleProps.dwSampleFlags |= 8; }
            m_SampleProps.pbBuffer = pSample->buf;
            m_SampleProps.lActual = 3;
            m_SampleProps.cbBuffer = 4;
        }
        if (!(m_SampleProps.dwSampleFlags & 8)) return S_OK;
        hr = CheckMediaType(m_SampleProps.pMediaType);
        if (hr == S_OK) return S_OK;
        m_bRunTimeError = 1;
        EndOfStream();
        NotifyEvent(EC_ERRORABORT, VFW_E_TYPE_NOT_ACCEPTED, 0);
        return VFW_E_INVALIDMEDIATYPE;
    }
};

int main()
{
    int ok = 1;
    Pin p; std::memset(&p, 0, sizeof(p));
    Sample s; std::memset(&s, 0, sizeof(s));
    ok &= p.Receive(0) == E_POINTER;
    p.streaming = VFW_E_NOT_STOPPED_STREAMING;
    ok &= p.Receive(&s) == VFW_E_NOT_STOPPED_STREAMING;
    p.streaming = S_OK;
    s.has2 = true;
    ok &= p.Receive(&s) == S_OK && s.released == 1 && p.m_SampleProps.dwSampleFlags == 0x114;
    s.has2 = false; s.disc = s.sync = s.time = true;
    ok &= p.Receive(&s) == S_OK && p.m_SampleProps.dwSampleFlags == (4 | 1 | 0x110) && p.m_SampleProps.pbBuffer == s.buf
        && p.m_SampleProps.lActual == 3 && p.m_SampleProps.cbBuffer == 4 && p.m_SampleProps.cbData == sizeof(AM_SAMPLE2_PROPERTIES);
    s.typechg = true; p.accept = S_OK;
    ok &= p.Receive(&s) == S_OK && g_endOfStream == 0 && p.m_bRunTimeError == 0;
    p.accept = S_FALSE;
    ok &= p.Receive(&s) == VFW_E_INVALIDMEDIATYPE && p.m_bRunTimeError == 1 && g_endOfStream == 1 && g_notify == 1
        && g_notifyCode == EC_ERRORABORT && g_notifyP1 == VFW_E_TYPE_NOT_ACCEPTED;
    if (!ok) { std::printf("00ca8b90_TEST FAIL\n"); return 1; }
    std::printf("00ca8b90_TEST PASS\n");
    return 0;
}