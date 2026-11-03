// Behaviour test for CAudioRingBuffer::Clear (0x00C5CE40): links the source object; every field after the vptr drops to 0 and
// the vptr is kept.
#include <stdio.h>

class CAudioRingBuffer {
public:
    virtual ~CAudioRingBuffer();
    unsigned char* m_pData;
    unsigned long  m_capacity;
    unsigned long  m_frameBytes;
    unsigned long  m_channels;
    unsigned long  m_readPos;
    unsigned long  m_writePos;
    unsigned long  m_queued;
    void Clear();
};
CAudioRingBuffer::~CAudioRingBuffer() { }

int main()
{
    static unsigned char storage[16];
    CAudioRingBuffer r;
    void* vptr = *(void**)&r;
    r.m_pData = storage; r.m_capacity = 1; r.m_frameBytes = 1; r.m_channels = 1;
    r.m_readPos = 1; r.m_writePos = 1; r.m_queued = 1;
    r.Clear();
    if (*(void**)&r == vptr && r.m_pData == 0 && r.m_capacity == 0 && r.m_frameBytes == 0 && r.m_channels == 0 &&
        r.m_readPos == 0 && r.m_writePos == 0 && r.m_queued == 0)
        printf("00c5ce40_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}
