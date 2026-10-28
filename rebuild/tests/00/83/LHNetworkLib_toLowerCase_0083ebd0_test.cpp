// behaviour test for 0x0083ebd0 LHNetworkLib::toLowerCase(const std::string&):
// every character goes through tolower (one push_back each, the buffer growing past its 8-byte default), the source is
// left untouched and the copy keeps embedded characters and length
#include <cstdio>
#include <cctype>
#include <string>
static std::string toLowerCase(const std::string& text)
{
    std::string result;
    int length = (int)text.size();
    for (int i = 0; i < length; ++i)
        result += (char)std::tolower(text[i]);
    return result;
}
int main()
{
    int fails = 0;
    std::string in("Content-Type: Text/HTML; Charset=UTF-8");
    std::string out = toLowerCase(in);
    if (out != "content-type: text/html; charset=utf-8") fails |= 1;
    if (in != "Content-Type: Text/HTML; Charset=UTF-8") fails |= 2;
    if (toLowerCase("").size() != 0) fails |= 4;
    if (toLowerCase("ABC123xyz") != "abc123xyz") fails |= 8;
    if (fails) { std::printf("0083ebd0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0083ebd0_TEST PASS\n");
    return 0;
}