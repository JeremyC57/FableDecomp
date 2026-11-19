// Test for 0x00C5CE40 CSalSwBuffer::Release(): links the row and checks every field from +0x04 to +0x1C is zeroed and the vptr kept.
// A test-local mirror of engine/CSalSwBuffer.h's layout with the members public (the header keeps them private); the
// CCodaObjectBase base is folded into the vptr (NF95 part 2, q11406; was the CAudioRingBuffer::Clear test).
#include <stdio.h>

class CSalSwBuffer {
public:
    virtual ~CSalSwBuffer();
    void Release();
    void*        m_pvBuffer;       // +0x04
    unsigned int m_nObjects;       // +0x08
    unsigned int m_cbObjectSize;   // +0x0c
    unsigned int m_nChannels;      // +0x10
    unsigned int m_ReadCursor;     // +0x14
    unsigned int m_WriteCursor;    // +0x18
    unsigned int m_nValid;         // +0x1c
};
CSalSwBuffer::~CSalSwBuffer() { }

int main()
{
    static unsigned char storage[16];
    CSalSwBuffer r;
    void* vptr = *(void**)&r;
    r.m_pvBuffer = storage; r.m_nObjects = 1; r.m_cbObjectSize = 1; r.m_nChannels = 1;
    r.m_ReadCursor = 1; r.m_WriteCursor = 1; r.m_nValid = 1;
    r.Release();
    if (*(void**)&r == vptr && r.m_pvBuffer == 0 && r.m_nObjects == 0 && r.m_cbObjectSize == 0 && r.m_nChannels == 0 &&
        r.m_ReadCursor == 0 && r.m_WriteCursor == 0 && r.m_nValid == 0)
        printf("00c5ce40_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}
