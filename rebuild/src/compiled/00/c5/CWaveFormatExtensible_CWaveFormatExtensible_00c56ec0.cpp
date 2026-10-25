// CWaveFormatExtensible::CWaveFormatExtensible(unsigned long rate, unsigned short channels, unsigned short bits) -- fill a WAVE_FORMAT_EXTENSIBLE PCM header: block align, byte rate, the 22-byte extension, valid bits, the speaker mask for 2 / 4 / 6 channels and the PCM sub-format GUID
// Leaf: ??0CWaveFormatExtensible@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C56EC0, length 159 bytes; `ret 0Ch`, returns `this` (a constructor).
// Callees: none. Data: KSDATAFORMAT_SUBTYPE_PCM {00000001-0000-0010-8000-00AA00389B71} @ 0x012B8DC0 (copied as four dwords).
// Caller: the OpenAL streaming source 0x00C57175 builds one on the stack as (44100, channels, 16).
// Layout: the mmreg.h WAVEFORMATEXTENSIBLE (packed: Format 18 bytes, Samples.wValidBitsPerSample +0x12, dwChannelMask
// +0x14, SubFormat +0x18), written flat with the same offsets. The class name is descriptive (no PDB/RTTI for libpk);
// the catalog label "Audio_FormatWaveHeader" is descriptive too.
// Shape: the fields in WAVEFORMATEX fill order (tag, channels, rate, bits, cbSize, then block align and byte rate); VC
// hoists the block-align product (signed `/ 8`: `cdq ; and edx,7`) and its store above the rest. Writing the block align
// first, or the byte rate from a local, moves the +8 store.
struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern const GUID KSDATAFORMAT_SUBTYPE_PCM;        // 0x012B8DC0

struct CWaveFormatExtensible {
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
    CWaveFormatExtensible(unsigned long rate, unsigned short channels, unsigned short bits);
};

CWaveFormatExtensible::CWaveFormatExtensible(unsigned long rate, unsigned short channels, unsigned short bits)
{
    wFormatTag = 0xFFFE;
    nChannels = channels;
    nSamplesPerSec = rate;
    wBitsPerSample = bits;
    cbSize = 22;
    nBlockAlign = channels * bits / 8;
    nAvgBytesPerSec = nBlockAlign * rate;
    wValidBitsPerSample = bits;
    if (channels == 2)
        dwChannelMask = 3;
    else if (channels == 4)
        dwChannelMask = 0x33;
    else if (channels == 6)
        dwChannelMask = 0x3F;
    SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
}