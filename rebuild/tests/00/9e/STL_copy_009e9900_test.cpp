// Behaviour test for 0x009E9900 STLport __copy(const unsigned char*, ..., back_insert_iterator<vector<unsigned char>>).
#include <cstdio>
#include <cstdlib>
#include <new>

static int gOverflows = 0;
struct vector {
    unsigned char* _M_start; unsigned char* _M_finish; unsigned char* _M_end_of_storage;
    void _M_insert_overflow(unsigned char* position, const unsigned char& x, unsigned int fill_len, bool atend)
    {
        ++gOverflows;
        unsigned int n = (unsigned int)(_M_finish - _M_start), len = n + (n > fill_len ? n : fill_len);
        unsigned char* p = (unsigned char*)std::malloc(len);
        for (unsigned int i = 0; i < n; ++i) p[i] = _M_start[i];
        for (unsigned int i = 0; i < fill_len; ++i) p[n + i] = x;
        std::free(_M_start);
        _M_start = p; _M_finish = p + n + fill_len; _M_end_of_storage = p + len;
        (void)position; (void)atend;
    }
    void push_back(const unsigned char& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) unsigned char(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, 1, true);
    }
};
struct back_insert_iterator {
    vector* container;
    explicit back_insert_iterator(vector& v) : container(&v) {}
    back_insert_iterator& operator=(const unsigned char& value) { container->push_back(value); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};

static back_insert_iterator __copy(const unsigned char* first, const unsigned char* last, back_insert_iterator result, int*)
{
    for (int n = (int)(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    vector v = { 0, 0, 0 };
    const unsigned char data[6] = { 1, 2, 3, 4, 5, 6 };
    back_insert_iterator r = __copy(data, data + 6, back_insert_iterator(v), (int*)0);
    bool ok = r.container == &v && v._M_finish - v._M_start == 6 && v._M_start[0] == 1 && v._M_start[5] == 6;
    r = __copy(data, data, back_insert_iterator(v), (int*)0);
    ok = ok && r.container == &v && v._M_finish - v._M_start == 6;
    r = __copy(data + 4, data + 6, r, (int*)0);
    ok = ok && v._M_finish - v._M_start == 8 && v._M_start[6] == 5 && v._M_start[7] == 6;
    std::free(v._M_start);
    if (!ok) { std::printf("009e9900_TEST FAIL\n"); return 1; }
    std::printf("009e9900_TEST PASS\n");
    return 0;
}