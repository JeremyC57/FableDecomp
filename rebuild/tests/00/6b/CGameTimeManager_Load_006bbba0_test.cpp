#include <cstdio>
#include <cstring>
// Host model of CGameTimeManager::Load(text): parser over the text (empty file name, line offset 0), a reporting
// text context over the parser, then the context load.
struct Parser { const char* text; const char* file; long offset; Parser(const char* t, const char* f, long o) : text(t), file(f), offset(o) {} };
struct Context { Parser* parser; bool report; Context(Parser* p, bool r) : parser(p), report(r) {} };
struct TimeManager {
    const char* loadedFrom; bool reported; const char* file;
    TimeManager() : loadedFrom(0), reported(false), file(0) {}
    void Load(Context& c) { loadedFrom = c.parser->text; reported = c.report; file = c.parser->file; }
    void Load(const char* text) { Parser parser(text, "", 0); Context context(&parser, true); Load(context); }
};
int main()
{
    TimeManager m;
    m.Load("Time 0.5;");
    if (!m.loadedFrom || std::strcmp(m.loadedFrom, "Time 0.5;") != 0 || !m.reported || std::strcmp(m.file, "") != 0) { std::printf("006bbba0_TEST FAIL\n"); return 1; }
    std::printf("006bbba0_TEST PASS\n");
    return 0;
}