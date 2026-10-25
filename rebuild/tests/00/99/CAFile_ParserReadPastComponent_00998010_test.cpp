// Behaviour test for 0x00998010 CAFile::ParserReadPastComponent: reads one path component up to the next separator,
// leaves the parser on the separator, and rejects components with forbidden symbols, a trailing space or a stray dot.
#include <cstdio>
#include <cstring>
#include <string>

struct Parser {
    std::string String; unsigned int Pos;
    bool IsSeparatorNext() const { return Pos < String.size() && (String[Pos] == '\\' || String[Pos] == '/'); }
    bool NextItemExists() const { return Pos < String.size(); }
};
static bool IsForbidden(const std::string& c) { return c.find_first_of("<>:\"/\\|") != std::string::npos; }

static bool ReadPastComponent(Parser& parser, std::string& component)
{
    if (parser.IsSeparatorNext() || !parser.NextItemExists())
        return false;
    bool result = false;
    const std::string text = parser.String;
    long start = (long)parser.Pos;
    long pos = start;
    char last = ' ';
    while (pos < (long)text.size()) {
        last = text[pos];
        parser.Pos = (unsigned int)(pos + 1);
        if (parser.IsSeparatorNext() || !parser.NextItemExists())
            break;
        ++pos;
    }
    long end = pos + 1 < (long)text.size() ? pos + 1 : (long)text.size();
    component = text.substr(start, end - start);
    if (!IsForbidden(component) && last != ' ' && (last != '.' || component == "." || component == ".."))
        result = true;
    return result;
}

static int Check(const char* path, unsigned int at, bool expect, const char* comp, unsigned int posAfter)
{
    Parser p; p.String = path; p.Pos = at;
    std::string c;
    bool r = ReadPastComponent(p, c);
    if (r != expect) return 0;
    if (comp && c != comp) return 0;
    return p.Pos == posAfter;
}

int main()
{
    int ok = 1;
    ok &= Check("data\\levels\\x.lev", 0, true, "data", 4);     // stops on the separator
    ok &= Check("data\\levels\\x.lev", 5, true, "levels", 11);
    ok &= Check("data\\levels\\x.lev", 12, true, "x.lev", 17);  // last component, parser at the end
    ok &= Check("\\data", 0, false, 0, 0);                      // separator first: nothing read
    ok &= Check("abc", 3, false, 0, 3);                         // nothing left
    ok &= Check("..\\a", 0, true, "..", 2);                     // dot components are fine
    ok &= Check(".\\a", 0, true, ".", 1);
    ok &= Check("name.\\a", 0, false, "name.", 5);              // a stray trailing dot
    ok &= Check("name \\a", 0, false, "name ", 5);              // trailing space
    ok &= Check("a|b\\c", 0, false, "a|b", 3);                  // forbidden symbol
    std::printf("00998010_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}