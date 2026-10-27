// behaviour test for 00be8b00 (standalone copy of the model)
#include <cstdio>

struct E4 { unsigned int w; };
template <class T> class CArray { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; unsigned int size() const { return (unsigned int)(_M_finish - _M_start); } };

int main()
{
    E4 buf[5]; CArray<E4> a; a._M_start = buf; a._M_finish = buf + 5; a._M_end_of_storage = buf + 5;
    if (!(a.size() == 5)) { std::printf("00be8b00_TEST FAIL\n"); return 1; }
    a._M_finish = buf;
    if (!(a.size() == 0)) { std::printf("00be8b00_TEST FAIL\n"); return 1; }
    std::printf("00be8b00_TEST PASS\n");
    return 0;
}