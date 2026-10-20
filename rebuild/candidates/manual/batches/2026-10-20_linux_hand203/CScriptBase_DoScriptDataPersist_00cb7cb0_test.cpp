// Behaviour test for 0x00cb7cb0 CScriptBase::DoScriptDataPersist
#include <cstdio>
#include <cstring>

static char g_log[64]; static int g_n = 0;
static void Log(char c) { if (g_n < 63) { g_log[g_n++] = c; g_log[g_n] = 0; } }

struct CCharString {
    const char* text;
    CCharString(const char* t, long) : text(t) { Log('s'); }
    ~CCharString() { Log('~'); }
};
struct CStringParser {
    const CCharString* source; const char* filename; long base;
    CStringParser(const CCharString& text, void (*)(CCharString*), const CCharString& name, long b)
        : source(&text), filename(name.text), base(b) { Log('p'); }
    ~CStringParser() { Log('d'); }
};
struct CPersistContext {
    bool saving; CCharString* output; CStringParser* parser; bool report; CCharString* sourceText;
    CPersistContext(CCharString& out) : saving(true), output(&out), parser(0), report(false), sourceText(0) { Log('w'); }
    CPersistContext(CStringParser* p, void (*)(CCharString*), bool r, void*, void*, CCharString* src)
        : saving(false), output(0), parser(p), report(r), sourceText(src) { Log('r'); }
};

struct CScriptBase {
    const CPersistContext* seen;
    void OnPersist(CPersistContext& c) { Log('P'); seen = &c; check(c); }
    bool ok; CCharString* expectedData;
    void check(const CPersistContext& c)
    {
        ok = c.saving ? c.output == expectedData
                      : (c.parser && c.parser->source == expectedData && std::strcmp(c.parser->filename, "") == 0
                         && c.parser->base == 0 && c.report && c.sourceText == expectedData);
    }
    void DoScriptDataPersist(CCharString& data, bool saving)
    {
        if (saving) {
            CPersistContext context(data);
            OnPersist(context);
        }
        else {
            CStringParser parser(data, 0, CCharString("", -1), 0);
            CPersistContext context(&parser, 0, true, 0, 0, &data);
            OnPersist(context);
        }
    }
};

int main()
{
    CCharString data("x=1;", -1);
    CScriptBase s; s.expectedData = &data;
    g_n = 0; s.ok = false; s.DoScriptDataPersist(data, true);
    if (!s.ok || std::strcmp(g_log, "wP") != 0) { std::printf("00cb7cb0_TEST FAIL\n"); return 1; }
    g_n = 0; s.ok = false; s.DoScriptDataPersist(data, false);
    if (!s.ok || std::strcmp(g_log, "sp~rPd") != 0) { std::printf("00cb7cb0_TEST FAIL\n"); return 1; }
    std::printf("00cb7cb0_TEST PASS\n");
    return 0;
}
