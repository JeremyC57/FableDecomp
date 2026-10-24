// Behaviour test for MicroThread::SetStack (0x009D86B0).
#include <cstdio>

namespace MicroThread
{
    unsigned char* g_pStackTop = 0;
    unsigned char* g_pStackEnd = 0;
    unsigned int   g_stackSize = 0;

    void SetStack(unsigned char* stack, unsigned int size)
    {
        unsigned char* top = stack - 4;
        *(unsigned int*)top = 0xCDCDCDCD;
        g_pStackTop = top;
        g_pStackEnd = stack;
        g_stackSize = size;
    }
}

int main()
{
    unsigned int buffer[16];
    for (int i = 0; i < 16; ++i) buffer[i] = 0;
    unsigned char* end = (unsigned char*)&buffer[16];
    MicroThread::SetStack(end, 64);
    if (MicroThread::g_pStackTop != (unsigned char*)&buffer[15]) { std::printf("009d86b0_TEST FAIL\n"); return 1; }
    if (buffer[15] != 0xCDCDCDCD || buffer[14] != 0) { std::printf("009d86b0_TEST FAIL\n"); return 1; }
    if (MicroThread::g_pStackEnd != end || MicroThread::g_stackSize != 64) { std::printf("009d86b0_TEST FAIL\n"); return 1; }
    std::printf("009d86b0_TEST PASS\n");
    return 0;
}