// CSampleHeader::GetFormat(SAudioFormatDesc* out, void* data) -- fills an audio format descriptor from a sample header: data pointer, two cleared words, the format kind mapped from the header's +0xE byte (0/1/2, anything else fails with -1), channels (+4) and sample rate (+6, widened); returns 0
// Leaf: GetFormat
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C356F0, 123 bytes, __fastcall `ret 8`. Descriptive name (no PDB covers
//   libpk): the catalog's `CInventoryItem_CopyProperties` is propagated. Caller: 0x00C268DF, which pre-fills a frame
//   descriptor with kind 0, one channel and 44100 Hz (0xAC44) and tests the result with `jl`.
// Shape: the three header kinds map one-to-one onto the descriptor's kinds through a switch (`sub edx,0 ; je ;
//   dec ; je ; dec ; je`); VC7.1 duplicates the shared channel/rate tail into each case.
struct SAudioFormatDesc {
    void*          m_pData;                 // +0x00
    long           m_reserved4;             // +0x04
    long           m_reserved8;             // +0x08
    unsigned short m_channels;              // +0x0C
    unsigned short m_kind;                  // +0x0E
    unsigned long  m_sampleRate;            // +0x10
};

struct CSampleHeader {
    unsigned char  _pad00[4];
    unsigned short m_channels;              // +0x04
    unsigned short m_sampleRate;            // +0x06
    unsigned char  _pad08[6];
    unsigned char  m_kind;                  // +0x0E
    long GetFormat(SAudioFormatDesc* out, void* data) const;
};

long CSampleHeader::GetFormat(SAudioFormatDesc* out, void* data) const
{
    out->m_pData = data;
    out->m_reserved4 = 0;
    out->m_reserved8 = 0;
    switch (m_kind) {
    case 0: out->m_kind = 0; break;
    case 1: out->m_kind = 1; break;
    case 2: out->m_kind = 2; break;
    default: return -1;
    }
    out->m_channels = m_channels;
    out->m_sampleRate = m_sampleRate;
    return 0;
}