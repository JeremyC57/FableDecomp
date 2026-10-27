#include <cstdio>

// Behaviour model of CHandleBasedMemoryPool::ValidateLargestFreeArea (0x00C280E0): the walk finds the highest non-empty
// bucket and its largest area (the retail check against the cached size is compiled out; the model returns it).
struct Header { int Size; Header* NextAvailable; };
struct Pool {
    Header* FreeLists[256];
    int ValidateLargestFreeArea()
    {
        for (int index = 255; index >= 0; --index) {
            if (FreeLists[index] != 0) {
                int largest = 0;
                for (Header* area = FreeLists[index]; area; area = area->NextAvailable)
                    if (largest < area->Size) largest = area->Size;
                return largest;
            }
        }
        return -1;
    }
};

int main()
{

    static Pool p; for (int i = 0; i < 256; ++i) p.FreeLists[i] = 0;
    if (p.ValidateLargestFreeArea() != -1) { std::printf("00c280e0_TEST FAIL\n"); return 1; }
    Header a; a.Size = 300; Header b; b.Size = 900; Header c; c.Size = 500;
    a.NextAvailable = &b; b.NextAvailable = &c; c.NextAvailable = 0;
    p.FreeLists[3] = &a;
    Header lo; lo.Size = 5000; lo.NextAvailable = 0; p.FreeLists[1] = &lo;
    if (p.ValidateLargestFreeArea() != 900) { std::printf("00c280e0_TEST FAIL\n"); return 1; }
    std::printf("00c280e0_TEST PASS\n");
    return 0;
}