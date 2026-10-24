#include <cstdio>
#include <cstring>

// Behaviour model of CQuestManager::LoadMasterScriptDataFromGameState (0x004B3760): no persisted master-data script ->
// nothing; otherwise copy the text, build a parser over it (file name "") and a loading context (report errors, the
// definition manager, source text = the copy) and hand the context to the script data's OnPersist.
static int g_strings = 0;
struct CCharString {
    char text[32];
    CCharString(const char* s) { std::strcpy(text, s); ++g_strings; }
    CCharString(const CCharString& o) { std::strcpy(text, o.text); ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CDefinitionManager { int tag; };
static CDefinitionManager g_defs = { 77 };
static CDefinitionManager& GetDefs() { return g_defs; }
struct CStringParser { const char* source; const char* filename; CStringParser(const CCharString& t, void*, const CCharString& f, long) : source(t.text), filename(f.text[0] ? "x" : "") { } };
struct CPersistContext { CStringParser* parser; bool report; CDefinitionManager* defs; CCharString* text; };
struct CScriptDataBase { int calls; char seen[32]; bool report; int defsTag; virtual void OnPersist(CPersistContext& c) { ++calls; std::strcpy(seen, c.parser->source); report = c.report; defsTag = c.defs->tag; } };
struct CRuntimeQuestPersistence {
    const char* key; const CCharString* script;
    const CCharString* GetQuestMasterDataPersistScript(const CCharString& name) const { return std::strcmp(name.text, key) == 0 ? script : 0; }
};
struct CQuestManager {
    CRuntimeQuestPersistence RuntimeQuestPersistence;
    void Load(CScriptDataBase& data, const CCharString& name)
    {
        const CCharString* persisted = RuntimeQuestPersistence.GetQuestMasterDataPersistScript(name);
        if (persisted) {
            CCharString text(*persisted);
            CStringParser parser(text, 0, CCharString(""), 0);
            CPersistContext context = { &parser, true, &GetDefs(), &text };
            data.OnPersist(context);
        }
    }
};

int main()
{
    CCharString saved("Gold 10 Renown 3");
    CQuestManager qm = { { "Q_Arena", &saved } };
    CScriptDataBase data; data.calls = 0;
    qm.Load(data, CCharString("Q_Guild"));
    if (data.calls != 0) { std::printf("004b3760_TEST FAIL\n"); return 1; }
    qm.Load(data, CCharString("Q_Arena"));
    if (data.calls != 1 || std::strcmp(data.seen, "Gold 10 Renown 3") != 0 || !data.report || data.defsTag != 77) { std::printf("004b3760_TEST FAIL\n"); return 1; }
    if (g_strings != 1) { std::printf("004b3760_TEST FAIL\n"); return 1; }                      // only `saved` left alive
    std::printf("004b3760_TEST PASS\n");
    return 0;
}