#include <cstdio>
#include <cstring>
// Behaviour test for STLport vector<unsigned char>::erase(iterator) (0x0053c7ef): the tail shifts down, the size drops by one,
// the returned position is the erased slot; erasing the last element only drops the finish pointer.
static void* copy_trivial(const void* first, const void* last, void* result)
{
    unsigned long n = (unsigned long)((const char*)last - (const char*)first);
    std::memmove(result, first, n);
    return (char*)result + n;
}
typedef unsigned char T;
struct vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* end() { return _M_finish; }
    T* erase(T* pos)
    {
        if (pos + 1 != end())
            copy_trivial(pos + 1, _M_finish, pos);
        --_M_finish;
        return pos;
    }
};
int main()
{
    T data[4];
    for (int i = 0; i < 4; ++i) data[i] = (T)(i + 10);
    vec v; v._M_start = data; v._M_finish = data + 4; v._M_end_of_storage = data + 4;
    T* r = v.erase(data + 1);
    if (r != data + 1 || v._M_finish != data + 3 || data[0] != (T)10 || data[1] != (T)12 || data[2] != (T)13) { std::printf("0053c7ef_TEST FAIL\n"); return 1; }
    r = v.erase(data + 2);
    if (r != data + 2 || v._M_finish != data + 2 || data[1] != (T)12) { std::printf("0053c7ef_TEST FAIL\n"); return 1; }
    std::printf("0053c7ef_TEST PASS\n");
    return 0;
}