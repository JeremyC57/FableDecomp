// Behaviour test for CAudioWaveOut::Initialize (0x00C5D7F0): model and body verbatim, the waveOut imports and the audio
// allocator stubbed; covers the happy path (format, 4 zeroed buffers, 4 headers prepared and queued) and every failure
// exit (too many channels, format query, open, buffer alloc, header alloc, prepare, write) with the cleanup each one does.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define __declspec(x)
#ifndef _MSC_VER
#define __stdcall
#define __cdecl
#define __forceinline inline
#endif
// CAudioWaveOut::Initialize(unsigned long channels, unsigned long bufferBytes) -- open a 44.1 kHz 16-bit PCM waveOut device (after a format query), allocate bufferBytes/128 zeroed 256*channels-byte buffers and their WAVEHDRs, then prepare and queue every header; any failure frees what was allocated and returns false
//   channels in ebp); a top-tested do-while over i with the buffer offset as `i * blockSize` (VC7.1 strength-reduces it into the
//   dead channels argument slot, initialised in the preheader), i homed in the dead bufferBytes slot.
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D7F0, length 590 bytes (ret 8, /GS cookie 0x0139C8A8 / check 0x00BFE9F9).
// Callees: waveOutOpen (IAT 0x014404B8), waveOutPrepareHeader (IAT 0x01440484), waveOutWrite (IAT 0x01440488), the audio
//          allocator / free @ 0x00C3C360 / 0x00C3C370 (manifest "CTCHeroStats::InformOfEvent" labels are propagated; the
//          landed audio Create 0x00C56860 calls 0x00C3C360 its class operator new); the device callback 0x00C5D720.
// Layout: as the landed CAudioWaveOut::WriteWaveBuffer 0x00C5D6B0: Headers +4, WaveOut +8, Buffer +0xc, BufferCount +0x10,
//         QueuedCount +0x14, WriteIndex +0x18.
#ifdef _MSC_VER
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)
#endif

struct WAVEFORMATEX_18 {
    unsigned short wFormatTag;
    unsigned short nChannels;
    unsigned long  nSamplesPerSec;
    unsigned long  nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
    unsigned short cbSize;
    unsigned char  extra[2];                    // GS: a byte array in the frame object
};
struct WAVEHDR_32 {
    char*          lpData;
    unsigned long  dwBufferLength;
    unsigned long  dwBytesRecorded;
    unsigned long  dwUser;
    unsigned long  dwFlags;
    unsigned long  dwLoops;
    WAVEHDR_32*    lpNext;
    unsigned long  reserved;
};
extern "C" __declspec(dllimport) unsigned long __stdcall waveOutOpen(void** phwo, unsigned int uDeviceID, const WAVEFORMATEX_18* pwfx,
                                                                    unsigned long dwCallback, unsigned long dwInstance, unsigned long fdwOpen);   // IAT 0x014404B8
extern "C" __declspec(dllimport) unsigned long __stdcall waveOutPrepareHeader(void* hwo, WAVEHDR_32* pwh, unsigned long cbwh);   // IAT 0x01440484
extern "C" __declspec(dllimport) unsigned long __stdcall waveOutWrite(void* hwo, WAVEHDR_32* pwh, unsigned long cbwh);           // IAT 0x01440488
void* __cdecl AudioAlloc(unsigned long size);   // 0x00C3C360
void __cdecl AudioFree(void* block);            // 0x00C3C370
void __stdcall WaveOutProc(void* hwo, unsigned int msg, unsigned long instance, unsigned long p1, unsigned long p2);   // 0x00C5D720

struct CAudioWaveOut {
    void* __vftable;
    WAVEHDR_32* Headers;                        // +4
    void* WaveOut;                              // +8
    char* Buffer;                               // +0xc
    unsigned long BufferCount;                  // +0x10
    unsigned long QueuedCount;                  // +0x14
    unsigned long WriteIndex;                   // +0x18
    bool Initialize(unsigned long channels, unsigned long bufferBytes);
};

bool CAudioWaveOut::Initialize(unsigned long channels, unsigned long bufferBytes)
{
    BufferCount = bufferBytes >> 7;
    WAVEFORMATEX_18 format;
    format.wFormatTag = 1;
    const unsigned long& count = channels;
    format.nChannels = (unsigned short)count;
    format.nSamplesPerSec = 44100;
    format.wBitsPerSample = 16;
    format.cbSize = 0;
    format.nBlockAlign = (unsigned short)((format.nChannels * format.wBitsPerSample) / 8);
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    if (count > 2 || waveOutOpen(0, (unsigned int)-1, &format, 0, 0, 9) != 0)
        return false;
    if (waveOutOpen(&WaveOut, (unsigned int)-1, &format, (unsigned long)WaveOutProc, (unsigned long)this, 0x30000) != 0)
        return false;
    QueuedCount = BufferCount;
    WriteIndex = 0;
    unsigned long blockSize = count << 8;                            // bytes per buffer: 256 samples of 16-bit stereo/mono
    Buffer = (char*)AudioAlloc(BufferCount * blockSize);
    if (Buffer == 0) {
        WaveOut = 0;
        return false;
    }
    memset(Buffer, 0, BufferCount * blockSize);
    Headers = (WAVEHDR_32*)AudioAlloc(BufferCount * sizeof(WAVEHDR_32));
    if (Headers == 0) {
        WaveOut = 0;
        AudioFree(Buffer);
        Buffer = 0;
        return false;
    }
    unsigned long i = 0;
    if (i < BufferCount) {
        do {
            Headers[i].lpData = Buffer + i * blockSize;
            Headers[i].dwBufferLength = blockSize;
            Headers[i].dwBytesRecorded = 0;
            Headers[i].dwUser = i;
            Headers[i].dwFlags = 0;
            Headers[i].dwLoops = 0;
            Headers[i].lpNext = 0;
            Headers[i].reserved = 0;
            if (waveOutPrepareHeader(WaveOut, &Headers[i], sizeof(WAVEHDR_32)) != 0) {
                WaveOut = 0;
                AudioFree(Buffer);
                Buffer = 0;
                AudioFree(Headers);
                Headers = 0;
                return false;
            }
            if (waveOutWrite(WaveOut, &Headers[i], sizeof(WAVEHDR_32)) != 0) {
                WaveOut = 0;
                AudioFree(Buffer);
                Buffer = 0;
                AudioFree(Headers);
                Headers = 0;
                return false;
            }
            ++i;
        } while (i < BufferCount);
    }
    return true;
}

// ---- stubs and harness ----
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static int g_openCalls, g_prepCalls, g_writeCalls, g_allocCalls, g_freeCalls;
static int g_failOpenAt = 0, g_failAllocAt = 0, g_failPrepAt = -1, g_failWriteAt = -1;
static WAVEFORMATEX_18 g_lastFormat; static void* g_handle = (void*)0x5151; static CAudioWaveOut* g_lastInstance;
static void* g_freed[4]; static void* g_allocated[4];
static void reset() { g_openCalls = g_prepCalls = g_writeCalls = g_allocCalls = g_freeCalls = 0; g_failOpenAt = g_failAllocAt = 0; g_failPrepAt = g_failWriteAt = -1; memset(g_freed, 0, sizeof(g_freed)); }
extern "C" unsigned long __stdcall waveOutOpen(void** phwo, unsigned int dev, const WAVEFORMATEX_18* fmt, unsigned long cb, unsigned long inst, unsigned long flags)
{
    ++g_openCalls; g_lastFormat = *fmt; CHECK(dev == (unsigned int)-1);
    if (g_openCalls == 1) { CHECK(phwo == 0 && cb == 0 && inst == 0 && flags == 9); }
    else { CHECK(cb == (unsigned long)WaveOutProc && flags == 0x30000); g_lastInstance = (CAudioWaveOut*)inst; if (g_openCalls != g_failOpenAt) *phwo = g_handle; }
    return g_openCalls == g_failOpenAt ? 7 : 0;
}
extern "C" unsigned long __stdcall waveOutPrepareHeader(void* hwo, WAVEHDR_32* h, unsigned long cb) { CHECK(hwo == g_handle && cb == sizeof(WAVEHDR_32)); CHECK(h->dwUser == (unsigned long)g_prepCalls); return g_prepCalls++ == g_failPrepAt ? 3 : 0; }
extern "C" unsigned long __stdcall waveOutWrite(void* hwo, WAVEHDR_32* h, unsigned long cb) { CHECK(hwo == g_handle && cb == sizeof(WAVEHDR_32)); CHECK(h->dwUser == (unsigned long)g_writeCalls); return g_writeCalls++ == g_failWriteAt ? 3 : 0; }
void* __cdecl AudioAlloc(unsigned long size) { ++g_allocCalls; if (g_allocCalls == g_failAllocAt) return 0; void* p = malloc(size); memset(p, 0xAB, size); g_allocated[g_allocCalls] = p; return p; }
void __cdecl AudioFree(void* block) { g_freed[g_freeCalls++] = block; }
void __stdcall WaveOutProc(void*, unsigned int, unsigned long, unsigned long, unsigned long) { }
static CAudioWaveOut* fresh() { CAudioWaveOut* w = new CAudioWaveOut; memset(w, 0, sizeof(*w)); w->WaveOut = (void*)0x77; return w; }
int main()
{
    // happy path: 2 channels, 512 bytes -> 4 buffers of 512 bytes
    reset(); CAudioWaveOut* w = fresh();
    CHECK(w->Initialize(2, 512));
    CHECK(g_openCalls == 2 && g_lastInstance == w && w->WaveOut == g_handle && w->BufferCount == 4 && w->QueuedCount == 4 && w->WriteIndex == 0);
    CHECK(g_lastFormat.wFormatTag == 1 && g_lastFormat.nChannels == 2 && g_lastFormat.nSamplesPerSec == 44100 && g_lastFormat.wBitsPerSample == 16 && g_lastFormat.cbSize == 0 && g_lastFormat.nBlockAlign == 4 && g_lastFormat.nAvgBytesPerSec == 176400);
    CHECK(g_allocCalls == 2 && w->Buffer == g_allocated[1] && w->Headers == g_allocated[2] && g_prepCalls == 4 && g_writeCalls == 4 && g_freeCalls == 0);
    { int zero = 1; for (int k = 0; k < 2048; ++k) if (w->Buffer[k]) zero = 0; CHECK(zero); }
    for (unsigned long i = 0; i < 4; ++i) {
        WAVEHDR_32& h = w->Headers[i];
        CHECK(h.lpData == w->Buffer + i * 512 && h.dwBufferLength == 512 && h.dwBytesRecorded == 0 && h.dwUser == i && h.dwFlags == 0 && h.dwLoops == 0 && h.lpNext == 0 && h.reserved == 0);
    }
    // mono: block align 2, 256-byte buffers
    reset(); w = fresh(); CHECK(w->Initialize(1, 256)); CHECK(g_lastFormat.nBlockAlign == 2 && g_lastFormat.nAvgBytesPerSec == 88200 && w->BufferCount == 2 && w->Headers[1].lpData == w->Buffer + 256 && w->Headers[1].dwBufferLength == 256);
    // too many channels: no open at all
    reset(); w = fresh(); CHECK(!w->Initialize(3, 512)); CHECK(g_openCalls == 0 && w->WaveOut == (void*)0x77);
    // format query fails
    reset(); g_failOpenAt = 1; w = fresh(); CHECK(!w->Initialize(2, 512)); CHECK(g_openCalls == 1 && g_allocCalls == 0);
    // open fails
    reset(); g_failOpenAt = 2; w = fresh(); CHECK(!w->Initialize(2, 512)); CHECK(g_openCalls == 2 && g_allocCalls == 0 && w->QueuedCount == 0);
    // buffer alloc fails
    reset(); g_failAllocAt = 1; w = fresh(); CHECK(!w->Initialize(2, 512)); CHECK(w->WaveOut == 0 && w->Buffer == 0 && g_freeCalls == 0 && w->QueuedCount == 4);
    // header alloc fails: buffer freed
    reset(); g_failAllocAt = 2; w = fresh(); CHECK(!w->Initialize(2, 512)); CHECK(w->WaveOut == 0 && w->Buffer == 0 && w->Headers == 0 && g_freeCalls == 1 && g_freed[0] == g_allocated[1]);
    // prepare fails on the second header: both freed, one write done
    reset(); g_failPrepAt = 1; w = fresh(); CHECK(!w->Initialize(2, 512)); CHECK(w->WaveOut == 0 && w->Buffer == 0 && w->Headers == 0 && g_freeCalls == 2 && g_freed[0] == g_allocated[1] && g_freed[1] == g_allocated[2] && g_prepCalls == 2 && g_writeCalls == 1);
    // write fails on the first header
    reset(); g_failWriteAt = 0; w = fresh(); CHECK(!w->Initialize(2, 512)); CHECK(w->WaveOut == 0 && w->Buffer == 0 && w->Headers == 0 && g_freeCalls == 2 && g_prepCalls == 1 && g_writeCalls == 1);
    // zero buffers: nothing prepared, success
    reset(); w = fresh(); CHECK(w->Initialize(2, 64)); CHECK(w->BufferCount == 0 && g_prepCalls == 0 && g_writeCalls == 0 && g_allocCalls == 2);
    printf("00c5d7f0_TEST %s\n", g_fail ? "FAIL" : "PASS");
    return g_fail;
}