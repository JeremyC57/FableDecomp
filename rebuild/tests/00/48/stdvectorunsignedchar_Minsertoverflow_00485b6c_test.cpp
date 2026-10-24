// Behaviour test for 0x00485B6C std::vector<unsigned char>::_M_insert_overflow (model copy).
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void* copy_trivial(const void* first, const void* last, void* result)
{
    unsigned int n = (unsigned int)((const char*)last - (const char*)first);
    std::memmove(result, first, n);
    return (char*)result + n;
}

struct ByteVectorModel {
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(unsigned char* position, const unsigned char& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        unsigned char* new_start = len != 0 ? (unsigned char*)std::malloc(len) : 0;
        unsigned char* new_finish = (unsigned char*)copy_trivial(_M_start, position, new_start);
        std::memset(new_finish, x, fill_len);
        new_finish += fill_len;
        if (!atend)
            new_finish = (unsigned char*)copy_trivial(position, _M_finish, new_finish);
        if (_M_start) std::free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};

int main()
{
    ByteVectorModel v;
    v._M_start = (unsigned char*)std::malloc(3);
    v._M_start[0] = 'a'; v._M_start[1] = 'b'; v._M_start[2] = 'c';
    v._M_finish = v._M_start + 3;
    v._M_end_of_storage = v._M_finish;
    unsigned char x = 'X';
    v._M_insert_overflow(v._M_start + 1, x, 2, false);       // a X X b c, capacity 6
    int ok = v.size() == 5 && std::memcmp(v._M_start, "aXXbc", 5) == 0 && v._M_end_of_storage == v._M_start + 6;
    v._M_insert_overflow(v._M_finish, x, 8, true);           // append 8 X, capacity 5 + 8
    if (v.size() != 13 || v._M_start[12] != 'X' || v._M_start[4] != 'c' || v._M_end_of_storage != v._M_start + 13) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("00485b6c_TEST FAIL\n"); return 1; }
    std::printf("00485b6c_TEST PASS\n");
    return 0;
}