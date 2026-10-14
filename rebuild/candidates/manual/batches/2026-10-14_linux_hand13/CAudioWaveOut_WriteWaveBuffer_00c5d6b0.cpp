// CAudioWaveOut::WriteWaveBuffer(long) -- submits the current wave header to the device, bumps the queued count under
// the critical section and advances the ring index (wrapping at the buffer count). The argument is unused.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D6B0, length 80 bytes.
// Callees: waveOutWrite (IAT 0x01440488, cbwh = sizeof(WAVEHDR) = 0x20), EnterCriticalSection (IAT 0x0143FE0C),
//          LeaveCriticalSection (IAT 0x0143FE10).
// Layout from the reads: Headers +4 (WAVEHDR*), WaveOut +8, BufferCount +0x10, QueuedCount +0x14, WriteIndex +0x18,
// the CRITICAL_SECTION at +0x20 (its address stays in edi for both calls).
struct WAVEHDR_32 { unsigned char _body[0x20]; };
struct CRITICAL_SECTION_18 { unsigned char _body[0x18]; };

extern "C" __declspec(dllimport) unsigned long __stdcall waveOutWrite(void* hwo, WAVEHDR_32* pwh, unsigned long cbwh);   // IAT 0x01440488
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_18* cs);                         // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_18* cs);                         // IAT 0x0143FE10

struct CAudioWaveOut {
    void* __vftable;
    WAVEHDR_32* Headers;                                 // +4
    void* WaveOut;                                       // +8
    unsigned char _pad0[4];
    long BufferCount;                                    // +0x10
    long QueuedCount;                                    // +0x14
    long WriteIndex;                                     // +0x18
    unsigned char _pad1[4];
    CRITICAL_SECTION_18 Lock;                            // +0x20
    void WriteWaveBuffer(long unused);
};

void CAudioWaveOut::WriteWaveBuffer(long unused)
{
    waveOutWrite(WaveOut, &Headers[WriteIndex], sizeof(WAVEHDR_32));
    EnterCriticalSection(&Lock);
    ++QueuedCount;
    LeaveCriticalSection(&Lock);
    ++WriteIndex;
    if (WriteIndex == BufferCount)
        WriteIndex = 0;
}
