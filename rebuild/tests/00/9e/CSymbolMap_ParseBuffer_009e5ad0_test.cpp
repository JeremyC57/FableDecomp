// Behaviour test for CSymbolMap::ParseBuffer (0x009E5AD0): parse every "enum" block, fail on the first bad one.
#include <cstdio>
#include <cstring>
struct Parser {
    const char* p;
    Parser(const char* text) : p(text) {}
    bool FindKeyword(const char* kw)
    {
        const char* f = std::strstr(p, kw);
        if (!f) return false;
        p = f + std::strlen(kw);
        return true;
    }
};
struct SymbolMap {
    int enums;
    bool ParseEnum(Parser* parser)
    {
        while (*parser->p == ' ') ++parser->p;
        if (*parser->p != '{') return false;
        ++enums;
        return true;
    }
    bool ParseBuffer(const char* buffer)
    {
        Parser parser(buffer);
        while (parser.FindKeyword("enum")) {
            if (!ParseEnum(&parser))
                return false;
        }
        return true;
    }
};
int main()
{
    SymbolMap a = { 0 }; bool ra = a.ParseBuffer("enum { A } enum { B } enum {C}");
    SymbolMap b = { 0 }; bool rb = b.ParseBuffer("enum { A } enum oops enum { C }");
    SymbolMap c = { 0 }; bool rc = c.ParseBuffer("no blocks here");
    if (!ra || a.enums != 3 || rb || b.enums != 1 || !rc || c.enums != 0) { std::printf("009e5ad0_TEST FAIL\n"); return 1; }
    std::printf("009e5ad0_TEST PASS\n");
    return 0;
}