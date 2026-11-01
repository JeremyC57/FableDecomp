// Test for CVtable_012b8a64::Process(const short*, long* const*) -- the scalar frame of the multi-tap delay: copy the 256-byte input
// block into the input buffer, swap the current/previous tap sets, and carry the last 64 bytes of the aligned buffer to its history head.
#include <cstdio>
#include <cstring>
struct CDelay {
    unsigned char storage[0x40 + 0x100];
    unsigned char* m_pAlignedBuffer;      // history head at -0x40
    int* m_pCurrent; int* m_pPrevious;
    int tapsA[4], tapsB[4];
    CDelay() { std::memset(storage, 0, sizeof(storage)); m_pAlignedBuffer = storage + 0x40; m_pCurrent = tapsA; m_pPrevious = tapsB; }
    void Process(const short* input)
    {
        std::memcpy(m_pAlignedBuffer, input, 0x100);
        int* current = m_pCurrent; m_pCurrent = m_pPrevious; m_pPrevious = current;
        std::memcpy(m_pAlignedBuffer - 0x40, m_pAlignedBuffer + 0xc0, 0x40);
    }
};
int main()
{
    short in[128];
    for (int i = 0; i < 128; ++i) in[i] = (short)(i * 3);
    CDelay d;
    d.Process(in);
    const short* hist = (const short*)(d.storage);
    if (d.m_pCurrent != d.tapsB || d.m_pPrevious != d.tapsA || hist[0] != in[96] || hist[31] != in[127])
    { std::printf("00c6d630_TEST FAIL\n"); return 1; }
    std::printf("00c6d630_TEST PASS\n");
    return 0;
}