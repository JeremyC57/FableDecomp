#include <cstdio>

// Behaviour model of CHandleBasedMemoryPool_012b4098::GetSizeFromHandle (0x00C283A0): the size of the handle's area header.
struct Header { int Resource; int Handle; int Size; };
struct Pool { Header** Handles; unsigned int GetSizeFromHandle(int h) { return Handles[h]->Size; } };

int main()
{

    Header a; a.Size = 64; Header b; b.Size = 4096; Header* table[2] = { &a, &b };
    Pool p; p.Handles = table;
    if (p.GetSizeFromHandle(0) != 64 || p.GetSizeFromHandle(1) != 4096) { std::printf("00c283a0_TEST FAIL\n"); return 1; }
    std::printf("00c283a0_TEST PASS\n");
    return 0;
}