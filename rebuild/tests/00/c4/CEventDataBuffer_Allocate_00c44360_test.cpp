#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour model of CEventDataBuffer::Allocate (0x00C44360): allocates count*12 bytes, zeroes them, points the
// secondary area `count` dwords in, sets the capacity and clears the cursors; on failure sets error -5, returns false.
static long g_error;
static bool g_fail;
static void* Alloc(unsigned int size) { return g_fail ? 0 : std::malloc(size); }
struct Buffer {
    unsigned int* m_data; unsigned int* m_secondary; long m_capacity, m_used, m_read, m_write;
    bool Allocate(long count)
    {
        unsigned int size = (unsigned int)count * 12;
        m_data = (unsigned int*)Alloc(size);
        if (m_data == 0) { g_error = -5; return false; }
        m_secondary = m_data + count;
        std::memset(m_data, 0, size);
        m_capacity = count; m_used = 0; m_read = 0; m_write = 0;
        return true;
    }
};
int main()
{
    Buffer b; b.m_used = b.m_read = b.m_write = 9;
    if (!b.Allocate(5) || b.m_secondary != b.m_data + 5 || b.m_capacity != 5 || b.m_used || b.m_read || b.m_write) { std::printf("00c44360_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 15; ++i) if (b.m_data[i] != 0) { std::printf("00c44360_TEST FAIL\n"); return 1; }
    std::free(b.m_data);
    g_fail = true;
    if (b.Allocate(3) || g_error != -5 || b.m_data != 0) { std::printf("00c44360_TEST FAIL\n"); return 1; }
    std::printf("00c44360_TEST PASS\n");
    return 0;
}