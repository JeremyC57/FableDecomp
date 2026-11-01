// behaviour test for 00c16700 (standalone copy of the model)
#include <cstdio>
#include <cstring>

#include <cstdlib>
struct A { virtual ~A() {} virtual void* Allocate(unsigned int n) { return std::malloc(n); } virtual void Free(void* p) { std::free(p); } };

int main()
{
    A al; char* p = (char*)al.Allocate(32);
    if (!(p != 0)) { std::printf("00c16700_TEST FAIL\n"); return 1; }
    std::memset(p, 7, 32);
    if (!(p[31] == 7)) { std::printf("00c16700_TEST FAIL\n"); return 1; }
    al.Free(p);
    std::printf("00c16700_TEST PASS\n");
    return 0;
}