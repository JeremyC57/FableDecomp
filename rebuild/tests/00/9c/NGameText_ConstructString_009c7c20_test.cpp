#include <cstdio>

// test stand-in for CWideString: a fixed-capacity UTF-16 buffer with the engine operations the rows call
class CWideString {
public:
    enum { CAP = 128 };
    CWideString() { Len = 0; Data[0] = 0; }
    CWideString(const unsigned short* text) { Assign(text); }
    CWideString(const CWideString& other) { Assign(other.Data); }
    ~CWideString() { ++Destroyed; }
    CWideString& operator=(const CWideString& other) { Assign(other.Data); return *this; }
    bool Contains(const CWideString& text) const { return Find(text) >= 0; }
    void Replace(const CWideString& search, const CWideString& replacement)
    {
        int at;
        while ((at = Find(search)) >= 0) {
            unsigned short out[CAP];
            int n = 0, i;
            for (i = 0; i < at; ++i) out[n++] = Data[i];
            for (i = 0; i < replacement.Len; ++i) out[n++] = replacement.Data[i];
            for (i = at + search.Len; i < Len; ++i) out[n++] = Data[i];
            out[n] = 0;
            Assign(out);
        }
    }
    void Format(const unsigned short* fmt, int value)     // "%d"-only stand-in
    {
        (void)fmt;
        char digits[16];
        int n = std::sprintf(digits, "%d", value);
        for (int i = 0; i <= n; ++i) Data[i] = (unsigned short)(unsigned char)digits[i];
        Len = n;
    }
    bool Equals(const char* ascii) const
    {
        int i = 0;
        for (; ascii[i]; ++i) if (i >= Len || Data[i] != (unsigned short)(unsigned char)ascii[i]) return false;
        return i == Len;
    }
    int Find(const CWideString& s) const
    {
        if (s.Len == 0) return -1;
        for (int i = 0; i + s.Len <= Len; ++i) {
            int j = 0;
            while (j < s.Len && Data[i + j] == s.Data[j]) ++j;
            if (j == s.Len) return i;
        }
        return -1;
    }
    void Assign(const unsigned short* text)
    {
        Len = 0;
        while (text[Len] && Len < CAP - 1) { Data[Len] = text[Len]; ++Len; }
        Data[Len] = 0;
    }
    static int Destroyed;
    unsigned short Data[CAP];
    int Len;
};
int CWideString::Destroyed = 0;

static CWideString W(const char* ascii)
{
    unsigned short buf[CWideString::CAP];
    int i = 0;
    for (; ascii[i] && i < CWideString::CAP - 1; ++i) buf[i] = (unsigned short)(unsigned char)ascii[i];
    buf[i] = 0;
    return CWideString(buf);
}

static CWideString operator+(const unsigned short* left, const CWideString& right)
{
    CWideString r(left);
    unsigned short buf[CWideString::CAP];
    int n = 0, i;
    for (i = 0; i < r.Len; ++i) buf[n++] = r.Data[i];
    for (i = 0; i < right.Len; ++i) buf[n++] = right.Data[i];
    buf[n] = 0;
    return CWideString(buf);
}

namespace _STL {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    size_type size() const { return size_type(end() - begin()); }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}

namespace NGameText {
const unsigned short g_PlaceholderPrefix[] = { '%', 0 };
const unsigned short g_NumberFormat[] = { '%', 'd', 0 };

inline CWideString FormatIndex(int index)
{
    CWideString number;
    number.Format(g_NumberFormat, index);
    return number;
}

bool ConstructString(CWideString& output, const _STL::vector<CWideString>& replacements)
{
    bool failed = false;
    for (unsigned int i = 0; i != replacements.size(); ++i) {
        CWideString key = g_PlaceholderPrefix + FormatIndex(i + 1);
        if (output.Contains(key))
            output.Replace(key, replacements[i]);
        else
            failed = true;
    }
    if (failed)
        return false;
    return true;
}
}

int main()
{
    CWideString items[3];
    items[0] = W("Hero"); items[1] = W("sword"); items[2] = W("Oakvale");
    _STL::vector<CWideString> v;
    v._M_start = items; v._M_finish = items + 3; v._M_end_of_storage = items + 3;

    CWideString out = W("%1 took the %2 to %3.");
    if (!NGameText::ConstructString(out, v) || !out.Equals("Hero took the sword to Oakvale.")) { std::printf("009c7c20_TEST FAIL\n"); return 1; }

    CWideString partial = W("%1 and %3 only");
    if (NGameText::ConstructString(partial, v) || !partial.Equals("Hero and Oakvale only")) { std::printf("009c7c20_TEST FAIL\n"); return 1; }

    _STL::vector<CWideString> empty;
    empty._M_start = empty._M_finish = empty._M_end_of_storage = items;
    CWideString untouched = W("%1");
    if (!NGameText::ConstructString(untouched, empty) || !untouched.Equals("%1")) { std::printf("009c7c20_TEST FAIL\n"); return 1; }
    std::printf("009c7c20_TEST PASS\n");
    return 0;
}