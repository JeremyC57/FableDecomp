// CWaveOutDevice::Close() -- shut the waveOut stream: reset it, unprepare every 0x20-byte WAVEHDR, close the handle and clear the handle and header count; then free the header array and the sample buffer
// Leaf: Close
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D610, length 121 bytes (+ int3 padding). Catalogued as
// CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures (a propagated label): it is slot 7 of the RTTI-less vtable
// 0x012B8FC4 (an audio output device class of the no-RTTI module), so the class is named by what it does.
// Callees: WINMM waveOutReset @ [0x0144048C], waveOutUnprepareHeader @ [0x01440490] (hoisted into ebp for the loop),
// waveOutClose @ [0x01440494]; the module's free wrapper @ 0x00C3C370 (__cdecl, twice).
// Layout: vptr +0x00, WAVEHDR array +0x04, HWAVEOUT +0x08, sample buffer +0x0c, header count +0x10.
// Shape: the zero lives in edi (re-zeroed after the loop, which borrows it as the index); the loop byte offset runs in
// ebx and both members are re-read around each import call; each free is followed by its own clear.
typedef struct HWAVEOUT__* HWAVEOUT;
struct WAVEHDR { unsigned char _body[0x20]; };

extern "C" __declspec(dllimport) unsigned int __stdcall waveOutReset(HWAVEOUT device);
extern "C" __declspec(dllimport) unsigned int __stdcall waveOutUnprepareHeader(HWAVEOUT device, WAVEHDR* header, unsigned int size);
extern "C" __declspec(dllimport) unsigned int __stdcall waveOutClose(HWAVEOUT device);
void __cdecl FreeAudioBlock(void* block);          // 0x00C3C370

struct CWaveOutDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual void Close();                          // slot 7
    WAVEHDR*      m_pHeaders;                      // +0x04
    HWAVEOUT      m_hDevice;                       // +0x08
    void*         m_pSamples;                      // +0x0c
    unsigned long m_headerCount;                   // +0x10
};

void CWaveOutDevice::Close()
{
    if (m_hDevice != 0) {
        waveOutReset(m_hDevice);
        for (unsigned long i = 0; i < m_headerCount; ++i)
            waveOutUnprepareHeader(m_hDevice, &m_pHeaders[i], sizeof(WAVEHDR));
        waveOutClose(m_hDevice);
        m_hDevice = 0;
        m_headerCount = 0;
    }
    if (m_pHeaders != 0) {
        FreeAudioBlock(m_pHeaders);
        m_pHeaders = 0;
    }
    if (m_pSamples != 0) {
        FreeAudioBlock(m_pSamples);
        m_pSamples = 0;
    }
}
