// Behaviour test for CCodaVoice::Reset (0x00C59860): a self-contained copy of the body over the same layout (the voice class is
// abstract in the source model): positions +0x18..+0x24 and the request word +0x2C to 0, the flags word +0x28 to 1.
#include <cstdio>
#include <cstring>

struct CCodaVoiceModel {
    void*         vptr;                                 // +0x00
    unsigned char _pad04[0x18 - 4];
    unsigned long m_positionB[2];                       // +0x18
    unsigned long m_positionC[2];                       // +0x20
    unsigned long m_flags;                              // +0x28
    unsigned long m_requests;                           // +0x2c
    void Reset() { m_positionB[0] = 0; m_positionB[1] = 0; m_positionC[0] = 0; m_positionC[1] = 0; m_requests = 0; m_flags = 1; }
};

int main()
{
    CCodaVoiceModel v;
    memset(&v, 0x5A, sizeof(v));
    v.Reset();
    if (sizeof(v) == 0x30 && v.m_positionB[0] == 0 && v.m_positionB[1] == 0 && v.m_positionC[0] == 0 && v.m_positionC[1] == 0 && v.m_requests == 0 && v.m_flags == 1 &&
        v._pad04[0] == 0x5A && v._pad04[0x13] == 0x5A) {
        printf("CLOSE_OK\n");
        return 0;
    }
    printf("CLOSE_BAD\n");
    return 1;
}
