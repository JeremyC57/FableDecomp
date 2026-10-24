// Behaviour test for 0x009E98C0 back_insert_iterator<vector<unsigned char>>::operator=.
#include <cstdio>
#include <cstdlib>
#include <new>

static int gOverflows = 0;
struct vector {
    typedef unsigned char value_type;
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
};

int main()
{
    vector v = { 0, 0, 0 };
    back_insert_iterator it(v);
    const char* text = "FABLE";
    bool ok = true;
    for (int i = 0; text[i]; ++i) { unsigned char c = (unsigned char)text[i]; ok = ok && &(it = c) == &it; }
    ok = ok && v._M_finish - v._M_start == 5 && v._M_start[0] == 'F' && v._M_start[4] == 'E' && gOverflows == 4;
    std::free(v._M_start);
    if (!ok) { std::printf("009e98c0_TEST FAIL\n"); return 1; }
    std::printf("009e98c0_TEST PASS\n");
    return 0;
}