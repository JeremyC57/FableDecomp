// Behaviour test for LHNetworkLib::StartsWith (0x0083EE10): case-insensitive prefix test -- the lower-cased text cut to
// the prefix length equals the lower-cased prefix (an empty prefix always matches; a prefix longer than the text never).
#include <cstdio>
#include <string>
static std::string toLowerCase(const std::string& text)
{
    std::string lower(text);
    for (unsigned int i = 0; i < lower.size(); ++i)
        if (lower[i] >= 'A' && lower[i] <= 'Z')
            lower[i] = (char)(lower[i] - 'A' + 'a');
    return lower;
}
static bool StartsWith(const std::string& text, const std::string& prefix)
{
    return toLowerCase(text).substr(0, prefix.size()) == toLowerCase(prefix);
}
int main()
{
    int ok = StartsWith(std::string("HTTP/1.1 200 OK"), std::string("http/"))
          && StartsWith(std::string("Content-Length: 5"), std::string("CONTENT-LENGTH:"))
          && StartsWith(std::string("abc"), std::string(""))
          && !StartsWith(std::string("ab"), std::string("abc"))
          && !StartsWith(std::string("Location: x"), std::string("Host:"))
          && StartsWith(std::string("same"), std::string("SAME"));
    if (!ok) {
        std::printf("0083ee10_TEST FAIL\n");
        return 1;
    }
    std::printf("0083ee10_TEST PASS\n");
    return 0;
}