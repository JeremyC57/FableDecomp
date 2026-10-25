// Behaviour test for 00c5d0a0: CAudioRingBuffer::CAudioRingBuffer -- an empty ring
#include <cstdio>
#include <cstring>
#include <new>
class CPKObject { public: virtual ~CPKObject() { } };
class CAudioRingBuffer : public CPKObject {
public:
    CAudioRingBuffer() : m_pData(0), m_capacity(0), m_frameBytes(0), m_channels(0), m_readPos(0), m_writePos(0), m_queued(0) { }
    unsigned char* m_pData; unsigned long m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
};
int main()
{
    unsigned char raw[sizeof(CAudioRingBuffer)]; std::memset(raw, 0x5a, sizeof raw);
    CAudioRingBuffer* r = new (raw) CAudioRingBuffer;
    int ok = r->m_pData == 0 && r->m_capacity == 0 && r->m_frameBytes == 0 && r->m_channels == 0 && r->m_readPos == 0
          && r->m_writePos == 0 && r->m_queued == 0 && sizeof(CAudioRingBuffer) == 0x20;
    std::printf(ok ? "00c5d0a0_TEST PASS\n" : "00c5d0a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
