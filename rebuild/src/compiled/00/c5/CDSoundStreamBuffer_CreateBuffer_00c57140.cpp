// CDSoundStreamBuffer::CreateBuffer(unsigned long channels, unsigned long samples, bool hardware3D) -- create the stream's DirectSound secondary buffer: 16-bit 44.1 kHz PCM of channels x samples frames (chunk = 256 bytes per channel); a 3D buffer is CTRL3D | LOCHARDWARE over plain PCM, otherwise more than two channels use a WAVE_FORMAT_EXTENSIBLE header; then lock the whole buffer, zero it, unlock, rewind the cursor; false on any failure
// Leaf: ?CreateBuffer@CDSoundStreamBuffer@@QAE_NKK_N@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C57140, 366 bytes, thiscall `ret 0Ch`, /GS cookie frame (0x5c bytes:
//   __security_cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9 on both exits).
// Identity (descriptive, libpk DirectSound stream driver, no PDB / RTTI): the landed binding
//   `?CreateBuffer@CDSoundStreamBuffer@@QAE_NKK_N@Z` (callers CreateAndPlay 0x00C57380 with false, slot 4 0x00C56D70 with
//   (1, x, true)); members as the landed CDSoundStreamBuffer rows: +4 the IDirectSoundBuffer (OpenDevice / LockChunk),
//   +8 the IDirectSound device (OpenDevice), +0xC cursor, +0x10 buffer bytes, +0x14 chunk bytes. The catalog label
//   CSound_AllocateBuffers is descriptive.
// Callees: CWaveFormatExtensible::CWaveFormatExtensible 0x00C56EC0 (44100, channels, 16); IDirectSound::CreateSoundBuffer
//   (slot 3); IDirectSoundBuffer Lock (slot 11, DSBLOCK_ENTIREBUFFER) / Unlock (slot 19, +0x4C); the memset intrinsic.
// Shape: the buffer size is kept in a local (retail stores the register to dwBufferBytes; the memset reloads the
//   member); DSBUFFERDESC is the 0x14-byte DirectX 7 layout (dwSize 0x14, no guid3DAlgorithm), WAVEFORMATEX packed. The
//   Lock outputs live in their own block: only then does VC7.1 home bytes1 / bytes2 in the dead channels / hardware3D
//   argument slots as retail (function-scope locals take the channels / samples slots). Grades: 3.
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
#pragma pack(push, 1)
struct WAVEFORMATEX {                              // mmsystem.h (pack 1), 18 bytes
    unsigned short wFormatTag;
    unsigned short nChannels;
    unsigned long  nSamplesPerSec;
    unsigned long  nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
    unsigned short cbSize;
};
#pragma pack(pop)
struct DSBUFFERDESC {                              // dsound.h, DirectX 7 layout (0x14 bytes)
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwBufferBytes;
    unsigned long dwReserved;
    WAVEFORMATEX* lpwfxFormat;
};
#define DSBCAPS_LOCHARDWARE         0x00000004
#define DSBCAPS_CTRL3D              0x00000010
#define DSBCAPS_GLOBALFOCUS         0x00008000
#define DSBCAPS_GETCURRENTPOSITION2 0x00010000
#define DSBLOCK_ENTIREBUFFER        0x00000002
#define WAVE_FORMAT_PCM             1

struct CWaveFormatExtensible {                     // mmreg.h WAVEFORMATEXTENSIBLE, flat (landed ctor 0x00C56EC0)
    unsigned short wFormatTag;                     // +0x00 (WAVE_FORMAT_EXTENSIBLE 0xFFFE)
    unsigned short nChannels;                      // +0x02
    unsigned long  nSamplesPerSec;                 // +0x04
    unsigned long  nAvgBytesPerSec;                // +0x08
    unsigned short nBlockAlign;                    // +0x0c
    unsigned short wBitsPerSample;                 // +0x0e
    unsigned short cbSize;                         // +0x10 (22)
    unsigned short wValidBitsPerSample;            // +0x12
    unsigned long  dwChannelMask;                  // +0x14
    GUID           SubFormat;                      // +0x18
    CWaveFormatExtensible(unsigned long rate, unsigned short channels, unsigned short bits);   // 0x00C56EC0
};

struct IDirectSoundBuffer {                        // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetCaps(void* caps);                              // slot 3
    virtual long __stdcall GetCurrentPosition(unsigned long* play, unsigned long* write);   // slot 4
    virtual long __stdcall GetFormat(void* format, unsigned long size, unsigned long* written);   // slot 5
    virtual long __stdcall GetVolume(long* volume);                          // slot 6
    virtual long __stdcall GetPan(long* pan);                                // slot 7
    virtual long __stdcall GetFrequency(unsigned long* frequency);           // slot 8
    virtual long __stdcall GetStatus(unsigned long* status);                 // slot 9
    virtual long __stdcall Initialize(void* sound, const void* desc);        // slot 10
    virtual long __stdcall Lock(unsigned long offset, unsigned long bytes, void** ptr1, unsigned long* bytes1,
                                void** ptr2, unsigned long* bytes2, unsigned long flags);   // slot 11 (+0x2C)
    virtual long __stdcall Play(unsigned long reserved, unsigned long priority, unsigned long flags);   // slot 12
    virtual long __stdcall SetCurrentPosition(unsigned long position);       // slot 13
    virtual long __stdcall SetFormat(const WAVEFORMATEX* format);            // slot 14
    virtual long __stdcall SetVolume(long volume);                           // slot 15
    virtual long __stdcall SetPan(long pan);                                 // slot 16
    virtual long __stdcall SetFrequency(unsigned long frequency);            // slot 17
    virtual long __stdcall Stop();                                           // slot 18
    virtual long __stdcall Unlock(void* ptr1, unsigned long bytes1, void* ptr2, unsigned long bytes2);   // slot 19 (+0x4C)
};
struct IDirectSound {                              // COM: __stdcall, `this` pushed
    virtual long __stdcall QueryInterface(const GUID& iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall CreateSoundBuffer(const DSBUFFERDESC* desc, IDirectSoundBuffer** buffer, void* outer);   // slot 3
};

class CDSoundStreamBuffer {                        // vtable 0x012B8D98 (ctor 0x00C56D30)
public:
    virtual void v00();
    IDirectSoundBuffer* m_pBuffer;                 // +0x04
    IDirectSound*       m_pDevice;                 // +0x08
    unsigned long       m_cursor;                  // +0x0c
    unsigned long       m_bufferBytes;             // +0x10
    unsigned long       m_chunkBytes;              // +0x14
    bool CreateBuffer(unsigned long channels, unsigned long samples, bool hardware3D);
};

bool CDSoundStreamBuffer::CreateBuffer(unsigned long channels, unsigned long samples, bool hardware3D)
{
    unsigned long bufferBytes = channels * samples * 2;
    m_bufferBytes = bufferBytes;
    m_chunkBytes = channels * 256;
    CWaveFormatExtensible extensible(44100, (unsigned short)channels, 16);

    WAVEFORMATEX format;
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = (unsigned short)channels;
    format.nSamplesPerSec = 44100;
    format.wBitsPerSample = 16;
    format.cbSize = 0;
    format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
    format.nAvgBytesPerSec = format.nBlockAlign * format.nSamplesPerSec;

    DSBUFFERDESC desc;
    desc.dwSize = sizeof(desc);
    desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
    if (hardware3D) {
        desc.dwFlags |= DSBCAPS_CTRL3D | DSBCAPS_LOCHARDWARE;
        desc.lpwfxFormat = &format;
    } else if (channels <= 2) {
        desc.lpwfxFormat = &format;
    } else {
        desc.lpwfxFormat = (WAVEFORMATEX*)&extensible;
    }
    desc.dwBufferBytes = bufferBytes;
    desc.dwReserved = 0;
    if (m_pDevice->CreateSoundBuffer(&desc, &m_pBuffer, 0) < 0)
        return false;

    {   // clear the new buffer
        void* ptr1;
        unsigned long bytes1;
        void* ptr2;
        unsigned long bytes2;
        if (m_pBuffer->Lock(0, 0, &ptr1, &bytes1, &ptr2, &bytes2, DSBLOCK_ENTIREBUFFER) < 0)
            return false;
        memset(ptr1, 0, m_bufferBytes);
        if (m_pBuffer->Unlock(ptr1, bytes1, ptr2, bytes2) < 0)
            return false;
    }
    m_cursor = 0;
    return true;
}