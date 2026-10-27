// Behaviour test for 0x00C57380 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct IBuf { int played; int hr; int flags; virtual int Play(unsigned int r, unsigned int p, unsigned int f) { played++; flags = f; return hr; } };
struct CDSoundStreamBuffer {
    IBuf* m_pBuffer; bool createOk; unsigned int ca, cb; int cc;
    bool CreateBuffer(unsigned int a, unsigned int b, bool c) { ca = a; cb = b; cc = c ? 1 : 0; return createOk; }
    bool CreateAndPlay(unsigned int a, unsigned int b)
    {
        if (!CreateBuffer(a, b, false))
            return false;
        if (m_pBuffer->Play(0, 0, 1) != 0)
            return false;
        return true;
    }
};
int main()
{
    IBuf buf; buf.played = 0; buf.hr = 0; buf.flags = 0;
    CDSoundStreamBuffer s; s.m_pBuffer = &buf; s.createOk = false;
    if (s.CreateAndPlay(2, 4096) || buf.played != 0 || s.ca != 2 || s.cb != 4096 || s.cc != 0) { std::printf("00c57380_TEST FAIL\n"); return 1; }
    s.createOk = true;
    if (!s.CreateAndPlay(1, 8) || buf.played != 1 || buf.flags != 1) { std::printf("00c57380_TEST FAIL\n"); return 1; }
    buf.hr = -1;
    if (s.CreateAndPlay(1, 8)) { std::printf("00c57380_TEST FAIL\n"); return 1; }
    std::printf("00c57380_TEST PASS\n");
    return 0;
}