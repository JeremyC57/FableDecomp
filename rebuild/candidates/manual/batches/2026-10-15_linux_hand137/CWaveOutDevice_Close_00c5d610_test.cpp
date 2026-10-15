// Behaviour test for 0x00C5D610 CWaveOutDevice::Close: reset, unprepare each header, close, clear; free both blocks.
#include <cstdio>

struct WAVEHDR { unsigned char _body[0x20]; };
typedef int* HWAVEOUT;
static int g_reset = 0, g_unprepared = 0, g_closed = 0, g_freed = 0; static WAVEHDR* g_lastHeader = 0;
static unsigned int waveOutReset(HWAVEOUT) { ++g_reset; return 0; }
static unsigned int waveOutUnprepareHeader(HWAVEOUT, WAVEHDR* h, unsigned int size) { if (size == 0x20) ++g_unprepared; g_lastHeader = h; return 0; }
static unsigned int waveOutClose(HWAVEOUT) { ++g_closed; return 0; }
static void FreeAudioBlock(void*) { ++g_freed; }

struct CWaveOutDevice {
    WAVEHDR* m_pHeaders; HWAVEOUT m_hDevice; void* m_pSamples; unsigned int m_headerCount;
    virtual void Close()
    {
        if (m_hDevice != 0) {
            waveOutReset(m_hDevice);
            for (unsigned int i = 0; i < m_headerCount; ++i)
                waveOutUnprepareHeader(m_hDevice, &m_pHeaders[i], sizeof(WAVEHDR));
            waveOutClose(m_hDevice);
            m_hDevice = 0;
            m_headerCount = 0;
        }
        if (m_pHeaders != 0) { FreeAudioBlock(m_pHeaders); m_pHeaders = 0; }
        if (m_pSamples != 0) { FreeAudioBlock(m_pSamples); m_pSamples = 0; }
    }
    virtual ~CWaveOutDevice() {}
};

int main()
{
    WAVEHDR headers[3]; int handle; char samples[4];
    CWaveOutDevice d; d.m_pHeaders = headers; d.m_hDevice = &handle; d.m_pSamples = samples; d.m_headerCount = 3;
    d.Close();
    if (g_reset != 1 || g_unprepared != 3 || g_lastHeader != &headers[2] || g_closed != 1 || g_freed != 2
        || d.m_hDevice || d.m_headerCount || d.m_pHeaders || d.m_pSamples) { std::printf("00c5d610_TEST FAIL\n"); return 1; }
    d.Close();
    if (g_reset != 1 || g_freed != 2) { std::printf("00c5d610_TEST FAIL\n"); return 1; }
    std::printf("00c5d610_TEST PASS\n");
    return 0;
}
