// Behaviour test for 0x00C5E2C0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>
struct SKsStreamBlock { unsigned int Size; unsigned char _h[0x24]; void* Data; unsigned int Options; unsigned char Overlapped[0x14]; unsigned int pad; };
static void* g_dev = 0; static unsigned int g_code = 0, g_outSize = 0; static void* g_out = 0; static void* g_ov = 0; static int g_enter = 0, g_leave = 0;
static int DeviceIoControl(void* d, unsigned int code, void* in, unsigned int inSize, void* out, unsigned int outSize, unsigned int* ret, void* ov)
{ g_dev = d; g_code = code; g_out = out; g_outSize = outSize; g_ov = ov; *ret = 0; return 1; }
struct K { void* m_handle; SKsStreamBlock* m_pBlocks; unsigned int m_eventCount; int m_queued; unsigned int m_writeIndex;
    void WriteBuffer(int unused)
    {
        SKsStreamBlock* block = &m_pBlocks[m_writeIndex];
        unsigned int returned;
        DeviceIoControl(m_handle, 0x2f8013, 0, 0, block, block->Size, &returned, block->Overlapped);
        g_enter++; ++m_queued; g_leave++;
        if (++m_writeIndex == m_eventCount)
            m_writeIndex = 0;
    } };
int main()
{
    SKsStreamBlock b[2]; b[0].Size = 0x30; b[1].Size = 0x31;
    K k; k.m_handle = (void*)&k; k.m_pBlocks = b; k.m_eventCount = 2; k.m_queued = 0; k.m_writeIndex = 1;
    k.WriteBuffer(0);
    if (g_out != &b[1] || g_outSize != 0x31 || g_ov != b[1].Overlapped || g_code != 0x2f8013 || g_dev != &k || k.m_queued != 1 || k.m_writeIndex != 0) { std::printf("00c5e2c0_TEST FAIL\n"); return 1; }
    k.WriteBuffer(0);
    if (g_out != &b[0] || k.m_writeIndex != 1 || k.m_queued != 2 || g_enter != 2 || g_leave != 2) { std::printf("00c5e2c0_TEST FAIL\n"); return 1; }
    std::printf("00c5e2c0_TEST PASS\n");
    return 0;
}
