// Behaviour test for 0x006D4AA0 CPersistContext_TransferableVector<pair<CCharString,long> >::Resize: the traits default
// ("", 0) is built, STLport resize(n, x) erases the tail or fill-inserts copies of the default, and the traits string is
// destroyed on both paths. Working stand-ins for erase / _M_fill_insert over a fixed buffer.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif

static int g_strings = 0;
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { ++g_strings; }
    CCharString(const CCharString& other) : Text(other.Text) { ++g_strings; }
    ~CCharString() { --g_strings; }
    const CCharString& operator=(const CCharString& other) { Text = other.Text; return *this; }
};

template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
typedef pair<CCharString, long> SNamedLong;

static int g_erases = 0, g_inserts = 0;
struct SNamedLongVector {
    SNamedLong* _M_start;
    SNamedLong* _M_finish;
    SNamedLong* _M_end_of_storage;
    SNamedLong* begin() { return _M_start; }
    SNamedLong* end() { return _M_finish; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    SNamedLong* erase(SNamedLong* first, SNamedLong* last)
    {
        ++g_erases;
        SNamedLong* out = first;
        for (SNamedLong* p = last; p != _M_finish; ++p) *out++ = *p;
        _M_finish = out;
        return first;
    }
    void _M_fill_insert(SNamedLong* position, unsigned long n, const SNamedLong& x)
    {
        ++g_inserts;
        if (position != _M_finish) return;
        for (unsigned long i = 0; i < n; ++i) *_M_finish++ = x;
    }
    void insert(SNamedLong* position, unsigned long n, const SNamedLong& x) { _M_fill_insert(position, n, x); }
    __forceinline void resize(unsigned long newSize, const SNamedLong& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
};

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<SNamedLong> {
    SNamedLong DefaultVal;
    CPersistTraits() : DefaultVal("", 0) { }
};

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(unsigned long count);
    SNamedLongVector* TransVector;
};

void CPersistContext_TransferableVector::Resize(unsigned long count)
{
    CPersistTraits<SNamedLong> traits;
    TransVector->resize(count, traits.DefaultVal);
}
long CPersistContext_TransferableVector::GetCount() { return (long)TransVector->size(); }

int main()
{
    static const char* names[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
    SNamedLong s0(names[0], 1), s1(names[1], 2), s2(names[2], 3), s3(names[3], 4), s4(names[4], 5), s5(names[5], 6), s6(names[6], 7), s7(names[7], 8);
    SNamedLong arr[8] = { s0, s1, s2, s3, s4, s5, s6, s7 };
    int live = g_strings;
    SNamedLongVector vec; vec._M_start = arr; vec._M_finish = arr + 4; vec._M_end_of_storage = arr + 8;
    CPersistContext_TransferableVector wrapper; wrapper.TransVector = &vec;
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->Resize(2);
    int ok = g_erases == 1 && hook->GetCount() == 2 && g_strings == live && arr[1].second == 2;
    hook->Resize(5);
    ok = ok && g_inserts == 1 && hook->GetCount() == 5 && arr[2].first.Text[0] == 0 && arr[4].second == 0 && arr[5].second == 6 && g_strings == live;
    if (!ok) { std::printf("006d4aa0_TEST FAIL\n"); return 1; }
    std::printf("006d4aa0_TEST PASS\n");
    return 0;
}