// CAudioWaveOut::Initialize(unsigned long channels, unsigned long bufferBytes) -- open a 44.1 kHz 16-bit PCM waveOut device (after a format query), allocate bufferBytes/128 zeroed 256*channels-byte buffers and their WAVEHDRs, then prepare and queue every header; any failure frees what was allocated and returns false
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D7F0, length 590 bytes (ret 8, /GS cookie 0x0139C8A8 / check 0x00BFE9F9).
// Callees: waveOutOpen (IAT 0x014404B8), waveOutPrepareHeader (IAT 0x01440484), waveOutWrite (IAT 0x01440488), the audio
//          allocator / free @ 0x00C3C360 / 0x00C3C370 (manifest "CTCHeroStats::InformOfEvent" labels are propagated; the
//          landed audio Create 0x00C56860 calls 0x00C3C360 its class operator new); the device callback 0x00C5D720.
// Layout: as the landed CAudioWaveOut::WriteWaveBuffer 0x00C5D6B0: Headers +4, WaveOut +8, Buffer +0xc, BufferCount +0x10,
//         QueuedCount +0x14, WriteIndex +0x18.
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

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
    format.nChannels = (unsigned short)channels;
    format.nSamplesPerSec = 44100;
    format.wBitsPerSample = 16;
    format.cbSize = 0;
    format.nBlockAlign = (unsigned short)((format.nChannels * format.wBitsPerSample) / 8);
    format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
    if (channels > 2 || waveOutOpen(0, (unsigned int)-1, &format, 0, 0, 9) != 0)
        return false;
    if (waveOutOpen(&WaveOut, (unsigned int)-1, &format, (unsigned long)WaveOutProc, (unsigned long)this, 0x30000) != 0)
        return false;
    QueuedCount = BufferCount;
    WriteIndex = 0;
    unsigned long& blockSize = channels;
    blockSize <<= 8;                            // bytes per buffer: 256 samples of 16-bit stereo/mono
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
    unsigned long offset = 0;
    for (unsigned long i = 0; i < BufferCount; ++i) {
        Headers[i].lpData = Buffer + offset;
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
        offset += blockSize;
    }
    return true;
}
