// behaviour test for 00a38670 CCRCSymbolMap::ParseEnum (same control flow over a canned token stream)
#include <cstdio>
#include <cstring>
struct Tok { int type; const char* text; int num; };
struct Parser {
    const Tok* toks; int n, pos; int skipped;
    bool Read(Tok& t) { if (pos >= n) return false; t = toks[pos++]; return true; }
    int ReadNumber() { return toks[pos - 1].num; }
};
struct Map {
    char names[8][16]; int values[8]; int count; int errors;
    void Add(const char* s, int v) { std::strcpy(names[count], s); values[count] = v; ++count; }
};
static bool sym(const Tok& t, const char* s) { return t.type == 1 && std::strcmp(t.text, s) == 0; }
static bool ParseEnum(Parser& p, Map& m, const char* wanted)
{
    Tok t;
    if (!p.Read(t)) { ++m.errors; return false; }
    const char* name;
    if (t.type == 2) { name = t.text; if (!p.Read(t)) { ++m.errors; return false; } }
    else name = "NULL";
    if (std::strcmp(wanted, "") != 0 && std::strcmp(wanted, name) != 0) { p.skipped = 1; return true; }
    if (!sym(t, "{")) { ++m.errors; return false; }
    for (int value = 0; p.Read(t); ++value) {
        if (sym(t, "}")) {
            if (!p.Read(t)) { ++m.errors; return false; }
            if (sym(t, ";")) return true;
            break;
        }
        if (t.type != 2) ++m.errors;
        const char* vn = t.text;
        if (!p.Read(t)) { ++m.errors; return false; }
        if (sym(t, ",")) m.Add(vn, value);
        else if (sym(t, "}")) {
            m.Add(vn, value);
            if (!p.Read(t)) { ++m.errors; return false; }
            if (!sym(t, ";")) ++m.errors;
            return true;
        } else if (sym(t, "=")) {
            int read = 0;
            if (!p.Read(t)) { ++m.errors; return false; }
            read = t.num;
            value = read; m.Add(vn, read);
            if (!p.Read(t)) { ++m.errors; return false; }
            if (t.type == 1) {
                if (sym(t, ",")) {}
                else if (sym(t, "}")) {
                    if (!p.Read(t)) { ++m.errors; return false; }
                    if (!sym(t, ";")) ++m.errors;
                    return true;
                } else ++m.errors;
            } else ++m.errors;
        } else ++m.errors;
    }
    return true;
}
int main()
{
    // enum E { A, B = 5, C };
    Tok s[] = { {2,"E",0}, {1,"{",0}, {2,"A",0}, {1,",",0}, {2,"B",0}, {1,"=",0}, {3,"5",5}, {1,",",0},
                {2,"C",0}, {1,"}",0}, {1,";",0} };
    Parser p; p.toks = s; p.n = 11; p.pos = 0; p.skipped = 0;
    Map m; m.count = 0; m.errors = 0;
    int ok = ParseEnum(p, m, "") && m.count == 3 && m.values[0] == 0 && m.values[1] == 5 && m.values[2] == 6
             && std::strcmp(m.names[2], "C") == 0 && m.errors == 0;
    Parser q; q.toks = s; q.n = 11; q.pos = 0; q.skipped = 0;
    Map m2; m2.count = 0; m2.errors = 0;
    ok &= ParseEnum(q, m2, "Other") && q.skipped == 1 && m2.count == 0;
    Tok bad[] = { {2,"E",0}, {1,"(",0} };
    Parser r; r.toks = bad; r.n = 2; r.pos = 0; r.skipped = 0;
    Map m3; m3.count = 0; m3.errors = 0;
    ok &= !ParseEnum(r, m3, "") && m3.errors == 1;
    std::printf(ok ? "00a38670_TEST PASS\n" : "00a38670_TEST FAIL\n");
    return ok ? 0 : 1;
}