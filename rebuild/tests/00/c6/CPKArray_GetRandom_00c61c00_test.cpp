// behaviour test for 0x00C61C00 CPKArray::GetRandom: index = rand * count / 32767, the top value clamped to the last entry
#include <cstdio>
static int g_rand = 0;
static int FakeRand() { return g_rand; }
struct CPKArray {
    unsigned int  m_capacity;
    unsigned int* m_pData;
    unsigned int  m_count;
    unsigned int GetRandom()
    {
        unsigned int count = m_count;
        unsigned int index = (unsigned int)((float)FakeRand() * (float)count * (1.0f / 32767.0f));
        if (index == count)
            index--;
        return m_pData[index];
    }
};
int main()
{
    unsigned int data[5] = { 10, 11, 12, 13, 14 };
    CPKArray a = { 5, data, 5 };
    g_rand = 0;     if (a.GetRandom() != 10) { std::printf("00c61c00_TEST FAIL\n"); return 1; }
    g_rand = 32767; if (a.GetRandom() != 14) { std::printf("00c61c00_TEST FAIL\n"); return 1; }   // clamped
    g_rand = 16384; if (a.GetRandom() != 12) { std::printf("00c61c00_TEST FAIL\n"); return 1; }
    g_rand = 6553;  if (a.GetRandom() != 10) { std::printf("00c61c00_TEST FAIL\n"); return 1; }
    g_rand = 6554;  if (a.GetRandom() != 11) { std::printf("00c61c00_TEST FAIL\n"); return 1; }
    std::printf("00c61c00_TEST PASS\n");
    return 0;
}