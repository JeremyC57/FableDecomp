// Behaviour test for 0x009BBBF0 CPersistContext::CheckUnknownSymbols(): the row's logic over a tiny whitespace-separated parser
// copy; every item left after the cursor is reported once as `Unknown entry "<item>"`, and the context's own parser is untouched.
#include <cstdio>
#include <string>
#include <vector>
struct CStringParser {
    std::string Text; size_t Pos;
    long SkipWhitespace() { long n = 0; while (Pos < Text.size() && Text[Pos] == ' ') { ++Pos; ++n; } return n; }
    bool NextItemExists() const { return Pos < Text.size(); }
    std::string ReadAsStringUntilWhitespace() { size_t s = Pos; while (Pos < Text.size() && Text[Pos] != ' ') ++Pos; return Text.substr(s, Pos - s); }
};
struct CParsedItem { long Type; long Value; std::string String; };
struct CPersistContext {
    CStringParser* PParser;
    std::vector<std::string> Errors;
    void PersistError(const CStringParser&, const std::string& message) { Errors.push_back(message); }
    void CheckUnknownSymbols();
};
void CPersistContext::CheckUnknownSymbols()
{
    CStringParser parser(*PParser);
    CParsedItem item;
    (void)item;
    parser.SkipWhitespace();
    while (parser.NextItemExists()) {
        PersistError(parser, "Unknown entry \"" + parser.ReadAsStringUntilWhitespace() + "\"");
        parser.SkipWhitespace();
    }
}
int main()
{
    CStringParser p; p.Text = "Known = 1;  Foo   Bar"; p.Pos = 11;
    CPersistContext c; c.PParser = &p;
    c.CheckUnknownSymbols();
    bool ok = c.Errors.size() == 2 && c.Errors[0] == "Unknown entry \"Foo\"" && c.Errors[1] == "Unknown entry \"Bar\"" && p.Pos == 11;
    CStringParser q; q.Text = "a b   "; q.Pos = 3; c.PParser = &q; c.Errors.clear();
    c.CheckUnknownSymbols();
    ok = ok && c.Errors.empty();
    if (!ok) { std::printf("009bbbf0_TEST FAIL\n"); return 1; }
    std::printf("009bbbf0_TEST PASS\n");
    return 0;
}