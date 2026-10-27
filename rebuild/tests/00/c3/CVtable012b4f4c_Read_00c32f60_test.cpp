#include <cstdio>

// Behaviour model of CVtable_012b4f4c::Read (0x00C32F60): queue count * unit-size bytes for this handle's key.
struct Manager {
    unsigned int m_key, m_size; void* m_args[4];
    bool QueueRead(unsigned int key, unsigned int size, void* a, void* b, void* c, void* d)
    { m_key = key; m_size = size; m_args[0] = a; m_args[1] = b; m_args[2] = c; m_args[3] = d; return true; }
};
struct FileHandle {
    Manager* m_pManager; unsigned int m_key;
    unsigned int GetUnitSize() { return 16; }
    bool Read(unsigned int count, int, void* a, void* b, void* c, void* d) { return m_pManager->QueueRead(m_key, GetUnitSize() * count, a, b, c, d); }
};

int main()
{

    Manager m; FileHandle h; h.m_pManager = &m; h.m_key = 3; int x[4];
    if (!h.Read(5, 99, &x[0], &x[1], &x[2], &x[3])) { std::printf("00c32f60_TEST FAIL\n"); return 1; }
    if (m.m_key != 3 || m.m_size != 80 || m.m_args[0] != &x[0] || m.m_args[3] != &x[3]) { std::printf("00c32f60_TEST FAIL\n"); return 1; }
    std::printf("00c32f60_TEST PASS\n");
    return 0;
}