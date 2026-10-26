// Behaviour test for 005927f4: std::pair<const CDateAndTime,CWideString>::pair(const pair&) -- memberwise copy: the date
// block word for word, the string through its copy constructor exactly once.
#include <cstdio>

static int g_copies = 0;
class CDateAndTime { public:
    long          Year;
    unsigned long Month;
    unsigned long Day;
    unsigned long Hour;
    unsigned long Minute;
    unsigned long Second;
    unsigned long Millisecond;
};
class CWideString {
public:
    const wchar_t* storage_;
    CWideString(const wchar_t* s) : storage_(s) { }
    CWideString(const CWideString& other) : storage_(other.storage_) { ++g_copies; }
};
template <class T1, class T2> struct SPair {
    T1 first;
    T2 second;
    SPair(const T1& a, const T2& b) : first(a), second(b) { }
    SPair(const SPair& other) : first(other.first), second(other.second) { }
};

int main()
{
    CDateAndTime when = { -1, 1, 2, 3, 4, 5, 6 };
    CWideString text(L"Oakvale");
    SPair<const CDateAndTime, CWideString> a(when, text);
    g_copies = 0;
    SPair<const CDateAndTime, CWideString> b(a);
    if (b.first.Year != -1 || b.first.Month != 1 || b.first.Day != 2 || b.first.Hour != 3 || b.first.Minute != 4 ||
        b.first.Second != 5 || b.first.Millisecond != 6 || b.second.storage_ != text.storage_ || g_copies != 1) {
        std::printf("005927f4_TEST FAIL\n");
        return 1;
    }
    std::printf("005927f4_TEST PASS\n");
    return 0;
}