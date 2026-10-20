// Behaviour test for 0x00cb7da0 CScriptBase::DoEntityScriptPersist
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

struct CEntityScriptBase {
    bool ok; CCharString* expectedData; int calls;
    void OnPersist(CPersistContext& c)
    {
        Log('P'); ++calls;
        ok = c.saving ? c.output == expectedData
                      : (c.parser && c.parser->source == expectedData && c.report && c.sourceText == expectedData);
    }
};
struct CActiveEntityScriptBase { CEntityScriptBase* Script; };
struct CScriptBase {
    void DoEntityScriptPersist(CActiveEntityScriptBase& script, CCharString& data, bool saving)
    {
        if (saving) {
            CPersistContext context(data);
            script.Script->OnPersist(context);
        }
        else {
            CStringParser parser(data, 0, CCharString("", -1), 0);
            CPersistContext context(&parser, 0, true, 0, 0, &data);
            script.Script->OnPersist(context);
        }
    }
};

int main()
{
    CCharString data("hp=3;", -1);
    CEntityScriptBase es; es.expectedData = &data; es.calls = 0;
    CActiveEntityScriptBase active; active.Script = &es;
    CScriptBase sb;
    g_n = 0; es.ok = false; sb.DoEntityScriptPersist(active, data, true);
    if (!es.ok || std::strcmp(g_log, "wP") != 0) { std::printf("00cb7da0_TEST FAIL\n"); return 1; }
    g_n = 0; es.ok = false; sb.DoEntityScriptPersist(active, data, false);
    if (!es.ok || std::strcmp(g_log, "sp~rPd") != 0 || es.calls != 2) { std::printf("00cb7da0_TEST FAIL\n"); return 1; }
    std::printf("00cb7da0_TEST PASS\n");
    return 0;
}