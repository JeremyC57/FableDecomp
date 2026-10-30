// Behaviour test for 0x0083ECF0 LHNetworkLib::toUpperCase (see src/0083ecf0.cpp): an upper-cased copy of the text,
// toupper on every character pushed into a fresh string; the input is untouched and an empty input gives an empty string.
#include <cstdio>
#include <ctype.h>
#include <string>
static std::string toUpperCase(const std::string& text)
{
    std::string out;
    for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
        out += (char)toupper((unsigned char)*it);
    return out;
}
int main()
{
    std::string in = "Location: http://a1/b";
    bool ok = toUpperCase(in) == "LOCATION: HTTP://A1/B" && in == "Location: http://a1/b";
    ok = ok && toUpperCase("") == "" && toUpperCase("x") == "X" && toUpperCase("ABC") == "ABC";
    std::printf(ok ? "0083ecf0_TEST PASS\n" : "0083ecf0_TEST FAIL\n");
    return ok ? 0 : 1;
}