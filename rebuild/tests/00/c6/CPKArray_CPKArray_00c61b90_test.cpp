// behaviour test for 0x00C61B90 CPKArray::CPKArray: capacity stored, count zero, storage of capacity entries
#include <cstdio>
struct CPKArray {
    unsigned int  m_capacity;
    unsigned int* m_pData;
    unsigned int  m_count;
    CPKArray(unsigned int capacity) { m_capacity = capacity; m_count = 0; m_pData = new unsigned int[capacity]; }
    ~CPKArray() { delete[] m_pData; }
};
int main()
{
    CPKArray a(16);
    if (a.m_capacity != 16 || a.m_count != 0 || !a.m_pData) { std::printf("00c61b90_TEST FAIL\n"); return 1; }
    for (unsigned int i = 0; i < 16; ++i) a.m_pData[i] = i * 3;   // the whole capacity is writable
    if (a.m_pData[15] != 45) { std::printf("00c61b90_TEST FAIL\n"); return 1; }
    std::printf("00c61b90_TEST PASS\n");
    return 0;
}