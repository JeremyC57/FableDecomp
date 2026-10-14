#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SPacketBuffer_009f17a0 { unsigned char m_payload[0x20]; long m_length; long m_a; long m_b; };
struct CEventPacket { unsigned char _pad[5]; unsigned char m_payload[0x20]; unsigned char m_length;
    long CopyToBuffer(SPacketBuffer_009f17a0* buffer) { std::memcpy(buffer->m_payload, m_payload, m_length); buffer->m_length = m_length; buffer->m_a = 0; buffer->m_b = 0; return 0; } };
int main() {
    CEventPacket p; std::memset(&p, 0, sizeof p); p.m_length = 3; p.m_payload[0] = 7; p.m_payload[1] = 8; p.m_payload[2] = 9; p.m_payload[3] = 10; SPacketBuffer_009f17a0 b; std::memset(&b, 0xee, sizeof b);
    if (p.CopyToBuffer(&b) != 0 || b.m_payload[0] != 7 || b.m_payload[2] != 9 || b.m_payload[3] != 0xee || b.m_length != 3 || b.m_a != 0 || b.m_b != 0) { std::printf("009f17a0_TEST FAIL\n"); return 1; }
    std::printf("009f17a0_TEST PASS\n"); return 0;
}
