// CEventPacket::CopyToBuffer(SPacketBuffer* buffer) -- copy the packet's payload (the byte count at +0x25, the bytes from +5) into the buffer, store the length at +0x20 and clear the two words at +0x24 / +0x28; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F17A0, length 51 bytes.
// Callees: none (the inline memcpy intrinsic: `rep movsd` of the count shifted by 2, then `rep movsb` of the low two bits). Shape: the byte count is `movzx`ed
// twice (once for the copy, once for the length store), the zero register clears the two words; `ret 4`.
#include <string.h>

struct SPacketBuffer_009f17a0 {
    unsigned char m_payload[0x20];                 // +0x00
    long          m_length;                        // +0x20
    long          m_a;                             // +0x24
    long          m_b;                             // +0x28
};

struct CEventPacket {
    unsigned char _pad[5];
    unsigned char m_payload[0x20];                 // +0x05
    unsigned char m_length;                        // +0x25
    long CopyToBuffer(SPacketBuffer_009f17a0* buffer);
};

long CEventPacket::CopyToBuffer(SPacketBuffer_009f17a0* buffer)
{
    memcpy(buffer->m_payload, m_payload, m_length);
    buffer->m_length = m_length;
    buffer->m_a = 0;
    buffer->m_b = 0;
    return 0;
}
