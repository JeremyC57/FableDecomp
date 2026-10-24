// behaviour test for 009cbee0: the buffer destructor body -- delete[] the array (when present),
// null the pointer and zero the count; an empty buffer only zeroes the count.
#include <cstdio>
static int g_deletes = 0;
struct CArrayBuffer {
    unsigned char _pad[0x4];
    unsigned long m_count;
    unsigned long* m_data;
    void Release()
    {
        if (m_data) {
            ++g_deletes;
            delete[] m_data;
            m_data = 0;
        }
        m_count = 0;
    }
};
int main()
{
    int ok = 1;
    CArrayBuffer b;
    b.m_count = 5;
    b.m_data = new unsigned long[5];
    b.Release();
    if (b.m_data != 0 || b.m_count != 0 || g_deletes != 1) ok = 0;
    b.m_count = 7;
    b.Release();
    if (b.m_data != 0 || b.m_count != 0 || g_deletes != 1) ok = 0;
    if (!ok) { std::printf("009cbee0_TEST FAIL\n"); return 1; }
    std::printf("009cbee0_TEST PASS\n");
    return 0;
}