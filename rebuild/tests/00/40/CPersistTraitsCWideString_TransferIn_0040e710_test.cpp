// behaviour test for CPersistTraits<CWideString>::TransferIn 0x0040E710: a quoted string is read up to the closing quote and
// stored; a missing opening or closing quote reports "Unable to find closing quote"
#include <cstdio>
#include <string>
#include <vector>
class CStringParser { public:
    std::vector<std::string> tokens; size_t pos;
    std::string ReadAsSymbol() { return pos < tokens.size() ? tokens[pos++] : std::string(); }
    std::string ReadAsStringUntilString(const std::string&) { return pos < tokens.size() ? tokens[pos++] : std::string(); }
};
class CPersistContext { public: int errors; std::string last; void Error(const CStringParser&, const std::string& m) { ++errors; last = m; } };
static void TransferIn(CPersistContext& context, CStringParser& parser, std::wstring& value)
{
    if (parser.ReadAsSymbol() != "\"") {
        std::string message("Unable to find closing quote");
        context.Error(parser, message);
    } else {
        {
            std::string quote("\"");
            std::string s = parser.ReadAsStringUntilString(quote);
            value.erase(); for (size_t i = 0; i < s.size(); ++i) value += (wchar_t)s[i];
        }
        if (parser.ReadAsSymbol() != "\"") {
            std::string message("Unable to find closing quote");
            context.Error(parser, message);
        }
    }
}
int main()
{
    CPersistContext c; c.errors = 0; std::wstring v = L"old";
    CStringParser p1; p1.tokens.push_back("\""); p1.tokens.push_back("Hello"); p1.tokens.push_back("\""); p1.pos = 0;
    TransferIn(c, p1, v);
    if (c.errors || v != L"Hello") { std::printf("0040e710_TEST FAIL\n"); return 1; }
    CStringParser p2; p2.tokens.push_back("x"); p2.pos = 0; v = L"keep";
    TransferIn(c, p2, v);
    if (c.errors != 1 || v != L"keep" || c.last != "Unable to find closing quote") { std::printf("0040e710_TEST FAIL\n"); return 1; }
    CStringParser p3; p3.tokens.push_back("\""); p3.tokens.push_back("Abc"); p3.tokens.push_back(";"); p3.pos = 0;
    TransferIn(c, p3, v);
    if (c.errors != 2 || v != L"Abc") { std::printf("0040e710_TEST FAIL\n"); return 1; }
    std::printf("0040e710_TEST PASS\n");
    return 0;
}