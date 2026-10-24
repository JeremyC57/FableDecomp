// Behaviour test for _STL::vector<unsigned char>::vector(n, val, alloc) -- fill constructor (0x009D2A40).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct ByteVector
{
    unsigned char* Start;
    unsigned char* Finish;
    unsigned char* EndOfStorage;
    ByteVector(unsigned int n, const unsigned char& val)
        : Start(0), Finish(0), EndOfStorage(0)
    {
        Start = n != 0 ? (unsigned char*)std::malloc(n) : 0;
        Finish = Start;
        EndOfStorage = Start + n;
        std::memset(Start, val, n);
        Finish = Start + n;
    }
};

static int Check(unsigned int n, unsigned char val)
{
    ByteVector v(n, val);
    if ((unsigned int)(v.Finish - v.Start) != n || v.EndOfStorage != v.Finish) return 0;
    if (n == 0 && v.Start != 0) return 0;
    for (unsigned int i = 0; i < n; ++i)
        if (v.Start[i] != val) return 0;
    std::free(v.Start);
    return 1;
}

int main()
{
    if (!Check(0u, 7) || !Check(1u, 0xAB) || !Check(3u, 0) || !Check(17u, 0xFF) || !Check(64u, 0x5A))
    {
        std::printf("009d2a40_TEST FAIL\n");
        return 1;
    }
    std::printf("009d2a40_TEST PASS\n");
    return 0;
}