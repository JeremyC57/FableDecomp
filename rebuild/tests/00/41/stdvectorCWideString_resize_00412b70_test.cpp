#include <cstdio>
#include <cstring>
#include <new>

// Model of 0x00412B70 vector<CWideString>::resize(n): resize(n, CWideString()) -- erase the excess tail
// (0x00412450) or fill-insert copies of an empty string at the end (0x004126E0; capacity is preallocated here).
static int g_live = 0;
class CWideString {
public:
    char Text[16];
    CWideString() { Text[0] = '\0'; ++g_live; }
    explicit CWideString(const char* text) { std::strcpy(Text, text); ++g_live; }
    CWideString(const CWideString& other) { std::strcpy(Text, other.Text); ++g_live; }
    ~CWideString() { --g_live; }
    CWideString& operator=(const CWideString& other) { std::strcpy(Text, other.Text); return *this; }
};
template <class T> class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T* erase(T* first, T* last)
    {
        T* out = first;
        for (T* in = last; in != _M_finish; ++in, ++out)
            *out = *in;
        for (T* p = out; p != _M_finish; ++p)
            p->~T();
        _M_finish = out;
        return first;
    }
    void insert(T* position, unsigned int n, const T& value)   // only ever called at end() here
    {
        if (position != _M_finish)
            return;
        for (; n > 0; --n, ++_M_finish)
            new (_M_finish) T(value);
    }
    void resize(unsigned int newSize, const T& value)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), value);
    }
    void resize(unsigned int newSize) { resize(newSize, T()); }
};

static double g_storage[128];

static int Fail()
{
    std::printf("00412b70_TEST FAIL\n");
    return 1;
}

int main()
{
    CWideString* buf = (CWideString*)g_storage;
    CRetailVector<CWideString> v;
    v._M_start = v._M_finish = buf;
    v._M_end_of_storage = buf + 8;
    new (v._M_finish++) CWideString("x");
    new (v._M_finish++) CWideString("y");
    v.resize(5);
    if (v.size() != 5 || g_live != 5 || std::strcmp(buf[0].Text, "x") != 0 || std::strcmp(buf[1].Text, "y") != 0 ||
        buf[2].Text[0] != '\0' || buf[4].Text[0] != '\0')
        return Fail();
    v.resize(1);
    if (v.size() != 1 || g_live != 1 || std::strcmp(buf[0].Text, "x") != 0)
        return Fail();
    v.resize(1);
    if (v.size() != 1 || g_live != 1)
        return Fail();
    v.resize(0);
    if (v.size() != 0 || g_live != 0)
        return Fail();
    std::printf("00412b70_TEST PASS\n");
    return 0;
}