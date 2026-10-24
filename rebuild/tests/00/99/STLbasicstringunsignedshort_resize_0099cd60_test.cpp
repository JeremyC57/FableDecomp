// Behaviour test for 0099cd60 STLport basic_string<unsigned short>::resize (copy of the source model; append
// is a working stand-in that grows within a fixed capacity and writes the terminator).
#include <cstdio>
#include <cstring>

static int g_appends = 0;

struct WString {
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    typedef unsigned int size_type;
    static unsigned short* move(unsigned short* s1, const unsigned short* s2, size_type n)
    {
        return n == 0 ? s1 : (unsigned short*)memmove(s1, s2, n * sizeof(unsigned short));
    }
    unsigned short* begin() { return _M_start; }
    unsigned short* end()   { return _M_finish; }
    size_type size() const { return _M_finish - _M_start; }
    WString& append(size_type n, unsigned short c)
    {
        ++g_appends;
        for (size_type i = 0; i < n; ++i) *_M_finish++ = c;
        *_M_finish = 0;
        return *this;
    }
    unsigned short* erase(unsigned short* first, unsigned short* last)
    {
        if (first != last) {
            move(first, last, (_M_finish - last) + 1);
            unsigned short* new_finish = _M_finish - (last - first);
            _M_finish = new_finish;
        }
        return first;
    }
    void resize(size_type n, unsigned short c)
    {
        if (n <= size())
            erase(begin() + n, end());
        else
            append(n - size(), c);
    }
};

int main()
{
    unsigned short buf[32];
    for (int i = 0; i < 32; ++i) buf[i] = 0x7777;
    WString s; s._M_start = buf; s._M_finish = buf; s._M_end_of_storage = buf + 31; *buf = 0;
    int ok = 1;
    s.resize(5, 'a');                                       // grow: append 5
    if (s.size() != 5 || g_appends != 1 || buf[0] != 'a' || buf[4] != 'a' || buf[5] != 0) ok = 0;
    buf[3] = 'x'; buf[4] = 'y';
    s.resize(5, 'b');                                       // same size: empty erase, nothing changes
    if (s.size() != 5 || g_appends != 1 || buf[4] != 'y' || buf[5] != 0) ok = 0;
    buf[6] = 0x1234;
    s.resize(3, 'c');                                       // truncate: terminator moves down
    if (s.size() != 3 || g_appends != 1 || buf[2] != 'a' || buf[3] != 0 || buf[6] != 0x1234) ok = 0;
    s.resize(7, 'd');                                       // grow again
    if (s.size() != 7 || g_appends != 2 || buf[3] != 'd' || buf[6] != 'd' || buf[7] != 0) ok = 0;
    s.resize(0, 'e');
    if (s.size() != 0 || buf[0] != 0) ok = 0;
    if (!ok) { std::printf("0099cd60_TEST FAIL\n"); return 1; }
    std::printf("0099cd60_TEST PASS\n");
    return 0;
}