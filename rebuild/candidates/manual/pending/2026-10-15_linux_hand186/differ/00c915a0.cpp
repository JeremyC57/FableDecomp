// CBitstream::ReadBits(long count) -- reads `count` bits (LSB first) from the byte stream at the current bit offset, returning -1 past the end, and advances
// Grade: DIFFER(190v188) -- the -1 result is a separate `mov edi,-1 ; jmp` block after the second compare; retail sets `or edi,-1` between the two compares and `jg` skips the read | tried: if/else -1, -1 default with ||, offset local, first shift via member | next: an early-return-free goto/flag form or a value initialised to -1 inside the first-compare failure
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C915A0, 188 bytes (`__fastcall`: ecx = this, edx = the bit count; plain `ret`).
// Globals: the low-bit mask table s_BitMasks[] @ 0x0139DDB8 (mask for n bits at index n).
// Shape: the mask is fetched first into the `push ecx` frame word; the count parameter becomes the total `count + m_bitOffset` and drives a
//          ladder of up to five byte reads (the offset re-read from the member; the fifth only when the read straddles and the offset is non-zero).
//          The result defaults to -1 and the read runs unless fewer than four bytes remain AND the bits run past the buffer.
//          The advance is a signed `bits / 8` (cdq ; and edx,7) for both the data pointer and the byte position,
//          and `bits & 7` for the new offset. Same fastcall convention as the landed sibling FlushByte 0x00C911B0.
struct CBitstream {
    long m_bytePosition;                           // +0x00 bytes consumed
    long m_bitOffset;                              // +0x04 bit offset inside the current byte
    long m_unknown08;                              // +0x08
    const unsigned char* m_pData;                  // +0x0c current byte
    long m_size;                                   // +0x10 buffer size in bytes
};

extern unsigned long s_BitMasks[];                 // 0x0139DDB8

unsigned long __fastcall ReadBits_00c915a0(CBitstream* stream, long count)
{
    unsigned long mask = s_BitMasks[count];
    long offset = stream->m_bitOffset;
    count += offset;
    unsigned long value;
    if (stream->m_bytePosition + 4 >= stream->m_size && count + stream->m_bytePosition * 8 > stream->m_size * 8) {
        value = 0xffffffff;
    } else {
        const unsigned char* p = stream->m_pData;
        value = p[0] >> stream->m_bitOffset;
        if (count > 8) {
            value |= p[1] << (8 - offset);
            if (count > 16) {
                value |= p[2] << (16 - offset);
                if (count > 24) {
                    value |= p[3] << (24 - offset);
                    if (count > 32 && offset != 0)
                        value |= p[4] << (32 - offset);
                }
            }
        }
        value &= mask;
    }
    stream->m_pData += count / 8;
    stream->m_bytePosition += count / 8;
    stream->m_bitOffset = count & 7;
    return value;
}
