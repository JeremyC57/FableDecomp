#include <cstdio>
#include <cstring>
// behaviour test: CScriptBase::LoadGameState hands the script-data and entity-script sections to their loaders
struct CCharString {
    char m[64];
    CCharString(const char* text, long) { std::strncpy(m, text, 63); m[63] = 0; }
    CCharString() { m[0] = 0; }
};
static const char* g_text = 0;
struct CStringParser {
    const char* pos;
    CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long) : pos(g_text) {}
    void skip() { while (*pos == ' ') ++pos; }
    bool NextItemIs(const CCharString& item) { skip(); size_t n = std::strlen(item.m); if (std::strncmp(pos, item.m, n)) return false; pos += n; return true; }
    CCharString ReadAsStringUntilString(const CCharString& end)
    { skip(); CCharString r; int n = 0; while (*pos && std::strncmp(pos, end.m, std::strlen(end.m))) r.m[n++] = *pos++; r.m[n] = 0; return r; }
    CCharString ReadUntilText(const char* text) { CCharString end(text, -1); return ReadAsStringUntilString(end); }
    bool NextItemIsText(const char* text) { CCharString item(text, -1); return NextItemIs(item); }
};
static const char g_emptyString_0122d70e[] = "";
static const char g_startScriptData[] = "START_SCRIPT_DATA";
static const char g_endScriptData[] = "END_SCRIPT_DATA";
static const char g_startEntityScripts[] = "START_ENTITY_SCRIPTS";
static const char g_endEntityScripts[] = "END_ENTITY_SCRIPTS";
static char g_scriptData[64], g_entityData[64]; static bool g_savingFlag = true;
namespace NScript {
struct CRuntimeScriptedEntityPersist { void LoadFromGameStateScript(const CCharString& t) { std::strcpy(g_entityData, t.m); } };
}
class CScriptBase {
public:
    void DoScriptDataPersist(CCharString& data, bool saving) { std::strcpy(g_scriptData, data.m); g_savingFlag = saving; }
    void LoadGameState(const CCharString& state);
    NScript::CRuntimeScriptedEntityPersist RuntimeScriptedEntityPersist;
};
void CScriptBase::LoadGameState(const CCharString& state)
{
    CStringParser parser(state, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    if (parser.NextItemIsText(g_startScriptData)) {
        CCharString data = parser.ReadUntilText(g_endScriptData);
        DoScriptDataPersist(data, false);
        parser.NextItemIsText(g_endScriptData);
    }
    if (parser.NextItemIsText(g_startEntityScripts)) {
        CCharString data = parser.ReadUntilText(g_endEntityScripts);
        RuntimeScriptedEntityPersist.LoadFromGameStateScript(data);
        parser.NextItemIsText(g_endEntityScripts);
    }
}
int main()
{
    CScriptBase s; CCharString in("", -1);
    g_text = "START_SCRIPT_DATA abc END_SCRIPT_DATA START_ENTITY_SCRIPTS xyz END_ENTITY_SCRIPTS";
    s.LoadGameState(in);
    bool ok = std::strcmp(g_scriptData, "abc ") == 0 && !g_savingFlag && std::strcmp(g_entityData, "xyz ") == 0;
    g_scriptData[0] = 0; g_entityData[0] = 0; g_text = "START_ENTITY_SCRIPTS q END_ENTITY_SCRIPTS";
    s.LoadGameState(in);
    ok = ok && g_scriptData[0] == 0 && std::strcmp(g_entityData, "q ") == 0;
    if (!ok) { std::printf("00cb8690_TEST FAIL\n"); return 1; }
    std::printf("00cb8690_TEST PASS\n");
    return 0;
}
