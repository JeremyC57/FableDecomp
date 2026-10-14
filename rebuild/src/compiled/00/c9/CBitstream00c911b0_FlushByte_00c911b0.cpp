// CBitstream_00c911b0::FlushByte() -- pad the current byte with zero bits (8 - bitsUsed) when it is partially filled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C911B0, length 24 bytes.
// Callees: WriteBits @ 0x00C90FC0 (`__fastcall`: ecx = this, edx = the value 0, stack: the bit count; manifest "CBitstream::WriteBits"). Shape: the pad
// count is `8 - m_bitsUsed` in eax, compared signed against 8 (`jge` skips when nothing is used), the zero value goes to edx. The chunk label
// "CEditEngine2DWorldMap::RemoveMap" is propagated; the class is named by address.
struct CBitstream_00c911b0;
void __fastcall WriteBits_00c90fc0(CBitstream_00c911b0* stream, long value, long count);   // 0x00C90FC0

struct CBitstream_00c911b0 {
    unsigned char _pad[4];
    long m_bitsUsed;                               // +0x04
    void FlushByte();
};

void CBitstream_00c911b0::FlushByte()
{
    long pad = 8 - m_bitsUsed;
    if (pad < 8)
        WriteBits_00c90fc0(this, 0, pad);
}