// Behaviour test for CAudioRingBuffer::Flush (0x00C5CEA0): links the source object; the cursors and the queued count drop to 0,
// the storage description (+0x04..+0x10) is kept.
#include <cstdio>

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
    void Flush();
};
CAudioRingBuffer::~CAudioRingBuffer() { }

int main()
{
    static unsigned char storage[16];
    CAudioRingBuffer r;
    r.m_pData = storage; r.m_capacity = 64; r.m_frameBytes = 4; r.m_channels = 2;
    r.m_readPos = 7; r.m_writePos = 9; r.m_queued = 2;
    r.Flush();
    if (r.m_readPos == 0 && r.m_writePos == 0 && r.m_queued == 0 &&
        r.m_pData == storage && r.m_capacity == 64 && r.m_frameBytes == 4 && r.m_channels == 2)
        printf("INIT_B_OK\n");
    else
        printf("INIT_B_FAIL\n");
    return 0;
}
