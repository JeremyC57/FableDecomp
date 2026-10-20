// CBitstream::WriteBits(unsigned long value, long count) -- libpk bit writer: grow the buffer by 0x100 bytes when fewer than 4 remain, mask `value` to `count` bits, OR the low part into the current byte and store the next up-to-four bytes, then advance the byte/bit position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C90FC0, length 243 bytes (`ret 4`).
// Callees: realloc (msvcr71 import [0x01440054]); data: the bit-mask table g_bitMasks @ 0x0139DDB8.
// Callers: CBitstream::FlushByte @ 0x00C911B0, CCodeTable::WriteCode @ 0x00C96140 (both landed, both call it
// `__fastcall`: ecx = stream, edx = value, the count on the stack).
// Layout (local, no PDB class): +0 byte position, +4 bit position in the current byte, +8 buffer, +0xc
// current byte pointer (buffer + byte position), +0x10 capacity.
// Shape: the count PARAMETER becomes the running bit total (`add edi,ebp` on the stack value: a separate total
// local takes a `lea`); the shifted low byte is formed before the add; the byte advance `count / 8` (signed:
// `cdq ; and edx,7 ; sar 3`) is computed before the bit position store so `count & 7` can reuse edi.
extern "C" __declspec(dllimport) void* __cdecl realloc(void* block, unsigned int size);   // [0x01440054]
extern const unsigned long g_bitMasks[];      // 0x0139DDB8

struct CBitstream {
    long           m_bytePos;                 // +0x00
    long           m_bitPos;                  // +0x04
    unsigned char* m_pBuffer;                 // +0x08
    unsigned char* m_pCurrent;                // +0x0c
    long           m_capacity;                // +0x10
    void __fastcall WriteBits(unsigned long value, long count);
};

void __fastcall CBitstream::WriteBits(unsigned long value, long count)
{
    if (m_bytePos + 4 >= m_capacity) {
        m_pBuffer = (unsigned char*)realloc(m_pBuffer, m_capacity + 0x100);
        m_capacity += 0x100;
        m_pCurrent = m_pBuffer + m_bytePos;
    }
    value &= g_bitMasks[count];
    unsigned char low = (unsigned char)(value << m_bitPos);
    count += m_bitPos;
    *m_pCurrent |= low;
    if (count >= 8) {
        m_pCurrent[1] = (unsigned char)(value >> (8 - m_bitPos));
        if (count >= 16) {
            m_pCurrent[2] = (unsigned char)(value >> (16 - m_bitPos));
            if (count >= 24) {
                m_pCurrent[3] = (unsigned char)(value >> (24 - m_bitPos));
                if (count >= 32) {
                    if (m_bitPos)
                        m_pCurrent[4] = (unsigned char)(value >> (32 - m_bitPos));
                    else
                        m_pCurrent[4] = 0;
                }
            }
        }
    }
    long bytes = count / 8;
    m_bitPos = count & 7;
    m_bytePos += bytes;
    m_pCurrent += bytes;
}