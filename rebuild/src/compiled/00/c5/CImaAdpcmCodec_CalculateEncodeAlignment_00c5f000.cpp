// CImaAdpcmCodec::CalculateEncodeAlignment(unsigned short nChannels, unsigned short nSamplesPerBlock) -- block alignment of an Xbox ADPCM stream: (samples-1) * 4 bits per channel rounded up to bytes, then up to a multiple of 8, plus the 4-byte header of every channel
// Leaf: CalculateEncodeAlignment
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5F000, 48 bytes (`__fastcall`, nChannels in cx, nSamplesPerBlock in dx).
// Identity: the manifest's "Memory_ComputeAllocationSize" is propagated. The body is the DirectX/Xbox SDK
//   CImaAdpcmCodec::CalculateEncodeAlignment (imaadpcm.cpp) compiled into the audio library: its two callers are
//   CreateImaAdpcmFormat 0x00C5F070 and IsValidImaAdpcmFormat 0x00C5F110, and the codec's Initialize 0x00C5F9E0 indexes
//   the {DecodeM16, DecodeS16} / {EncodeM16, EncodeS16} table at 0x012B9274 (the encoder strings "Step index chosen: %d",
//   "Using per-block encoding" sit next to it).
// Shape: the working value is a signed INT (retail divides with `cdq ; and edx,7 ; add ; sar 3`, never truncating to
//   16 bits between the steps); only the encoded-sample-bit count is a 16-bit constant (`movzx eax,ax` after the
//   `lea eax,[ecx*4]`). The `* 8 + header` tail folds into `lea eax,[ecx+eax*2] ; shl eax,2`.
typedef unsigned short WORD;

class CImaAdpcmCodec {
public:
    static WORD __fastcall CalculateEncodeAlignment(WORD nChannels, WORD nSamplesPerBlock);
};

WORD __fastcall CImaAdpcmCodec::CalculateEncodeAlignment(WORD nChannels, WORD nSamplesPerBlock)
{
    const WORD nEncodedSampleBits = nChannels * 4;    // IMAADPCM_BITS_PER_SAMPLE
    const WORD nHeaderBytes = nChannels * 4;          // IMAADPCM_HEADER_LENGTH
    int nBlockAlign;

    nBlockAlign = nSamplesPerBlock - 1;               // the first sample lives in the header
    nBlockAlign *= nEncodedSampleBits;
    nBlockAlign += 7;
    nBlockAlign /= 8;

    nBlockAlign += 7;                                 // the stereo encoder wants whole DWORD pairs
    nBlockAlign /= 8;
    nBlockAlign *= 8;

    nBlockAlign += nHeaderBytes;
    return (WORD)nBlockAlign;
}