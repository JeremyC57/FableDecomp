#include <cstdio>

// Behaviour model of CHandleBasedMemoryPool::MoveResource (0x00C28220): memmove(destination, source, size).
#include <cstring>
struct Pool { void MoveResource(void*, int, void* source, void* destination, unsigned int size) { std::memmove(destination, source, size); } };

int main()
{

    char buf[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }; Pool p;
    p.MoveResource(0, 0, buf, buf + 2, 5);
    if (buf[0] != 1 || buf[1] != 2 || buf[2] != 1 || buf[6] != 5 || buf[7] != 8) { std::printf("00c28220_TEST FAIL\n"); return 1; }
    std::printf("00c28220_TEST PASS\n");
    return 0;
}