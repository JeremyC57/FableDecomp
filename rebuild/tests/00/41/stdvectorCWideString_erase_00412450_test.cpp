#include <cstdio>
#include <cstring>
#include <new>

// Model of 0x00412450 vector<CWideString>::erase(first, last): assign the tail down over [first, last), destroy the
// vacated strings at the end, shrink _M_finish, return first. The string is modelled with narrow text + a live count.
static int g_live = 0;
class CWideString {
public:
    char Text[16];
    explicit CWideString(const char* text) { std::strcpy(Text, text); ++g_live; }
    ~CWideString() { --g_live; Text[0] = '#'; }
    CWideString& operator=(const CWideString& other) { std::strcpy(Text, other.Text); return *this; }
};
template <class T> T* RetailCopy(T* first, T* last, T* result)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class T> void RetailDestroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> class CRetailVector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last)
    {
        T* i = RetailCopy(last, _M_finish, first);
        RetailDestroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
};

static double g_storage[64];

static int Fail()
{
    std::printf("00412450_TEST FAIL\n");
    return 1;
}

int main()
{
    const char* names[] = { "a", "b", "c", "d", "e" };
    CWideString* buf = (CWideString*)g_storage;
    for (int i = 0; i < 5; ++i)
        new (buf + i) CWideString(names[i]);
    CRetailVector<CWideString> v;
    v._M_start = buf;
    v._M_finish = v._M_end_of_storage = buf + 5;
    CWideString* r = v.erase(buf + 1, buf + 3);
    if (r != buf + 1 || v._M_finish != buf + 3 || g_live != 3)
        return Fail();
    if (std::strcmp(buf[0].Text, "a") != 0 || std::strcmp(buf[1].Text, "d") != 0 || std::strcmp(buf[2].Text, "e") != 0 ||
        buf[3].Text[0] != '#' || buf[4].Text[0] != '#')
        return Fail();
    if (v.erase(buf + 3, buf + 3) != buf + 3 || v._M_finish != buf + 3 || g_live != 3)   // empty range at the end
        return Fail();
    v.erase(v._M_start, v._M_finish);                                                     // clear
    if (v._M_finish != buf || g_live != 0)
        return Fail();
    std::printf("00412450_TEST PASS\n");
    return 0;
}