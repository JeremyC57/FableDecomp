// Behaviour test for pair<const CCharString, pair<long, CDateAndTime>>::pair(const CCharString&, const pair&) (0x0071EFC0): copy-constructs both members.
#include <cstdio>
#include <cstring>
static int g_copies = 0;
class CCharString { public:
    const char* Text;
    explicit CCharString(const char* t) : Text(t) {}
    CCharString(const CCharString& o) : Text(o.Text) { ++g_copies; }
};
class CDateAndTime { public: int Year; unsigned int Month, Day, Hour, Minute, Second, Millisecond; };
struct Inner { int first; CDateAndTime second; };
struct Outer {
    const CCharString first;
    Inner second;
    Outer(const CCharString& a, const Inner& b) : first(a), second(b) {}
};
int main()
{
    CCharString name("Bowerstone");
    Inner in; in.first = 77; in.second.Year = 2004; in.second.Month = 9; in.second.Day = 14; in.second.Hour = 1; in.second.Minute = 2; in.second.Second = 3; in.second.Millisecond = 4;
    Outer o(name, in);
    if (g_copies != 1 || std::strcmp(o.first.Text, "Bowerstone") != 0 || o.second.first != 77 || o.second.second.Year != 2004
        || o.second.second.Month != 9 || o.second.second.Day != 14 || o.second.second.Millisecond != 4) { std::printf("0071efc0_TEST FAIL\n"); return 1; }
    std::printf("0071efc0_TEST PASS\n");
    return 0;
}