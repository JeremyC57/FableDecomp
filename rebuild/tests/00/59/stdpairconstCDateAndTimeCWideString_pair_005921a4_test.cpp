// Behaviour test for 005921a4: std::pair<const CDateAndTime,CWideString>::pair(const T1&, const T2&) -- the date block is
// copied word for word and the string goes through its copy constructor exactly once.
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
};

int main()
{
    CDateAndTime when = { 2004, 9, 14, 23, 59, 30, 999 };
    CWideString text(L"Albion");
    SPair<const CDateAndTime, CWideString> entry(when, text);
    if (entry.first.Year != 2004 || entry.first.Month != 9 || entry.first.Day != 14 || entry.first.Hour != 23 ||
        entry.first.Minute != 59 || entry.first.Second != 30 || entry.first.Millisecond != 999 ||
        entry.second.storage_ != text.storage_ || g_copies != 1) { std::printf("005921a4_TEST FAIL\n"); return 1; }
    std::printf("005921a4_TEST PASS\n");
    return 0;
}