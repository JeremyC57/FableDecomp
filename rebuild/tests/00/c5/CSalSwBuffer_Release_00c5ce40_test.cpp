// Test for 0x00C5CE40 CSalSwBuffer::Release(): links the row and checks every field from +0x04 to +0x1C is zeroed and the vptr
// word kept. NF99 (q11406): written on engine/CSalSwBuffer.h itself (the private members opened with the hand4d b10 test's
// `#define private public`); the object is raw storage, never constructed (no vtable needed). Was the CAudioRingBuffer::Clear test.
#include <stdio.h>
#include <string.h>
#define private public
#define protected public
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#include "engine/CSalSwBuffer.h"
#undef private
#undef protected

int main()
{
    static unsigned char storage[16];
    union { void* align; unsigned char raw[sizeof(CSalSwBuffer)]; } u;
    memset(u.raw, 0x5A, sizeof u.raw);
    unsigned char head[sizeof(void*)];
    memcpy(head, u.raw, sizeof head);
    CSalSwBuffer* r = reinterpret_cast<CSalSwBuffer*>(u.raw);
    r->m_pvBuffer = storage; r->m_nObjects = 1; r->m_cbObjectSize = 1; r->m_nChannels = 1;
    r->m_ReadCursor = 1; r->m_WriteCursor = 1; r->m_nValid = 1;
    r->Release();
    if (memcmp(head, u.raw, sizeof head) == 0 && r->m_pvBuffer == 0 && r->m_nObjects == 0 && r->m_cbObjectSize == 0 &&
        r->m_nChannels == 0 && r->m_ReadCursor == 0 && r->m_WriteCursor == 0 && r->m_nValid == 0)
        printf("00c5ce40_TEST PASS\n");
    else
        printf("FAIL\n");
    return 0;
}
