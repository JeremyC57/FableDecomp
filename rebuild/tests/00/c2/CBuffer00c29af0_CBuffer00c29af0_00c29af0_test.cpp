// behaviour test for 00c29af0: the small owning container's destructor body -- delete the block at +0x4
// when present, then zero the pointer and both counters unconditionally.
#include <cstdio>
static int g_deletes = 0;
struct CBuffer {
    unsigned char _pad[0x4];
    unsigned long* m_data;
    unsigned long m_count;
    unsigned long m_capacity;
    void Release()
    {
        if (m_data) {
            ++g_deletes;
            delete m_data;
        }
        m_data = 0;
        m_count = 0;
        m_capacity = 0;
    }
};
int main()
{
    int ok = 1;
    CBuffer b;
    b.m_data = new unsigned long(3);
    b.m_count = 1;
    b.m_capacity = 4;
    b.Release();
    if (b.m_data != 0 || b.m_count != 0 || b.m_capacity != 0 || g_deletes != 1) ok = 0;
    b.m_count = 9;
    b.m_capacity = 9;
    b.Release();
    if (b.m_count != 0 || b.m_capacity != 0 || g_deletes != 1) ok = 0;
    if (!ok) { std::printf("00c29af0_TEST FAIL\n"); return 1; }
    std::printf("00c29af0_TEST PASS\n");
    return 0;
}