// behaviour test for 00bca080 (standalone copy of the model)
#include <cstdio>

struct E16 { unsigned int w[4]; };
template <class T> class CArray { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; T& operator[](unsigned int i) { return _M_start[i]; } };

int main()
{
    E16 buf[3]; CArray<E16> a; a._M_start = buf; a._M_finish = buf + 3; a._M_end_of_storage = buf + 3;
    if (!(&a[0] == buf && &a[2] == buf + 2 && sizeof(E16) == 16)) { std::printf("00bca080_TEST FAIL\n"); return 1; }
    std::printf("00bca080_TEST PASS\n");
    return 0;
}