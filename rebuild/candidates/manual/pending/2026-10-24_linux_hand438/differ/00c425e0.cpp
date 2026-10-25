// _global::DecodeImaAdpcm(const void* pSource, unsigned long sourceBytes, int channels, void* pDest, unsigned long destCapacity) -- decode whole 36-byte-per-channel IMA ADPCM blocks (64 samples each) at 44100 Hz into 16-bit PCM through a stack CImaAdpcmCodec; returns the decoded byte count, or -1 when the destination is too small or the codec fails
// Grade: DIFFER(221v217) -- register colouring only: retail keeps channels/blocks/size in esi/edi/ebx (edi pushed after the div, `cmp ebx,[esp+60h]` straight from the frame), ours edi/ebx/esi with the capacity loaded into eax first. 4 grades: `channels * 128 * blocks` and `blocks * (channels * 128)` reassociate to imul-then-shl; int channels the same; `(channels << 7) * blocks` (this file) restores retail's shl-then-imul. Next: a named `bytesPerBlock` local, or the decode size from a same-TU noinline helper.
// Leaf: ?DecodeImaAdpcm@@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C425E0, 217 bytes (+ int3 padding), __fastcall (source in ecx, byte count
//   in edx) `ret 0xc`. Built under /EHsc (the codec's out-of-line destructor 0x00C5EFB0 is the state-0 unwind action).
// Library: libpk audio (no PDB/RTTI). The codec is the DirectX SDK's CImaAdpcmCodec (imaadpcm.cpp): empty
//   constructor 0x00C5EFA0 / destructor 0x00C5EFB0, `__fastcall`-called static CreateImaAdpcmFormat(channels,
//   samplesPerSec, samplesPerBlock, pwfx) 0x00C5F070 (format tag 0x69), Initialize(pwfx, fEncoder) 0x00C5F9E0,
//   Convert(src, dst, cBlocks) 0x00C5FA50. Called once, from 0x00C3465E (a sound buffer's decode; `jg` on the
//   result). The manifest's "Hero_ProcessExperienceBlockParry" is propagated.
struct IMAADPCMWAVEFORMAT_00c425e0 {                    // 20 bytes
    unsigned short wFormatTag;
    unsigned short nChannels;
    unsigned long nSamplesPerSec;
    unsigned long nAvgBytesPerSec;
    unsigned short nBlockAlign;
    unsigned short wBitsPerSample;
    unsigned short cbSize;
    unsigned short wSamplesPerBlock;
};

class CImaAdpcmCodec_00c425e0 {                         // 0x24 bytes
public:
    CImaAdpcmCodec_00c425e0();                          // 0x00C5EFA0
    ~CImaAdpcmCodec_00c425e0();                         // 0x00C5EFB0
    static int __fastcall CreateImaAdpcmFormat(unsigned short nChannels, unsigned long nSamplesPerSec,
                                               unsigned short nSamplesPerBlock,
                                               IMAADPCMWAVEFORMAT_00c425e0* pwfx);   // 0x00C5F070
    int Initialize(const IMAADPCMWAVEFORMAT_00c425e0* pwfx, int fEncoder);          // 0x00C5F9E0
    int Convert(const void* pvSrc, void* pvDst, unsigned long cBlocks);              // 0x00C5FA50
private:
    IMAADPCMWAVEFORMAT_00c425e0 m_wfx;                  // +0
    int m_fEncoder;                                     // +0x14
    long m_nStepIndexL;                                 // +0x18
    long m_nStepIndexR;                                 // +0x1c
    void* m_pfnConvert;                                 // +0x20
};

long __fastcall DecodeImaAdpcm(const void* pSource, unsigned long sourceBytes, int channels, void* pDest,
                               unsigned long destCapacity)
{
    unsigned long blocks = sourceBytes / (channels * 36);
    unsigned long decodedBytes = (channels << 7) * blocks;
    if (decodedBytes > destCapacity)
        return -1;
    CImaAdpcmCodec_00c425e0 codec;
    IMAADPCMWAVEFORMAT_00c425e0 format;
    CImaAdpcmCodec_00c425e0::CreateImaAdpcmFormat((unsigned short)channels, 44100, 64, &format);
    if (!codec.Initialize(&format, 0))
        return -1;
    if (!codec.Convert(pSource, pDest, blocks))
        return -1;
    return decodedBytes;
}
