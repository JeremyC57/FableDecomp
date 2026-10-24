// Behaviour test for 0x00566EA0 std::operator!=(const basic_string&, const char*).
#include <cstdio>
#include <cstring>

namespace stl {
struct char_traits_char {
    static unsigned int length(const char* __s) { return (unsigned int)std::strlen(__s); }
    static int compare(const char* __s1, const char* __s2, unsigned int __n) { return std::memcmp(__s1, __s2, __n); }
};
class basic_string {
public:
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const char* data() const { return _M_start; }
};
inline bool operator==(const basic_string& __s, const char* __s2)
{
    unsigned int __n = char_traits_char::length(__s2);
    return __s.size() == __n && char_traits_char::compare(__s.data(), __s2, __n) == 0;
}
bool operator!=(const basic_string& __s, const char* __s2) { return !(__s == __s2); }
}

static stl::basic_string Make(char* buf)
{
    stl::basic_string s;
    s._M_start = buf;
    s._M_finish = buf + std::strlen(buf);
    s._M_end_of_storage = s._M_finish + 1;
    return s;
}

int main()
{
    char hello[] = "hello";
    char empty[] = "";
    stl::basic_string a = Make(hello), e = Make(empty);
    int ok = 1;
    if (a != "hello") ok = 0;               // equal -> false
    if (!(a != "hellO")) ok = 0;            // same length, different byte -> true
    if (!(a != "hell")) ok = 0;             // shorter -> true
    if (!(a != "hello!")) ok = 0;           // longer -> true
    if (e != "") ok = 0;                    // both empty -> false
    if (!(e != "x")) ok = 0;
    if (!ok) { std::printf("00566ea0_TEST FAIL\n"); return 1; }
    std::printf("00566ea0_TEST PASS\n");
    return 0;
}