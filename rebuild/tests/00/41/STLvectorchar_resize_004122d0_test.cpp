// Behaviour test for 0x004122D0 _STL::vector<char>::resize(unsigned int new_size, const char& x).
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("004122d0_TEST FAIL\n"); return 1; } } while (0)

// the byte vector: erase is a memmove of the tail (empty here), the fill a memset
class byte_vector {
public:
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    char  storage[64];
    byte_vector() : _M_start(storage), _M_finish(storage), _M_end_of_storage(storage + 64) { }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    static void* copy_trivial(const void* __first, const void* __last, void* __result)
    {
        return (__last == __first) ? __result :
            ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
    }
    char* erase(char* __first, char* __last) { char* __i = (char*)copy_trivial(__last, _M_finish, __first); _M_finish = __i; return __first; }
    void _M_fill_insert(char* __pos, unsigned int __n, const char& __x)
    {
        if (__pos == _M_finish && _M_finish + __n <= _M_end_of_storage) { std::memset(_M_finish, (unsigned char)__x, __n); _M_finish += __n; }
    }
    void resize(unsigned int __new_size, const char& __x)
    {
        if (__new_size < size())
            erase(_M_start + __new_size, _M_finish);
        else
            _M_fill_insert(_M_finish, __new_size - size(), __x);
    }
};

int main()
{
    byte_vector v;
    v.resize(10, 'a');
    CHECK(v.size() == 10 && v._M_start[0] == 'a' && v._M_start[9] == 'a');
    v._M_start[3] = 'z';
    v.resize(4, 'b');
    CHECK(v.size() == 4 && v._M_start[3] == 'z');
    v.resize(7, 'c');
    CHECK(v.size() == 7 && v._M_start[3] == 'z' && v._M_start[4] == 'c' && v._M_start[6] == 'c');
    v.resize(7, 'd');
    CHECK(v.size() == 7 && v._M_start[6] == 'c');
    v.resize(0, 'e');
    CHECK(v.size() == 0);
    std::printf("004122d0_TEST PASS\n");
    return 0;
}