// Behaviour test for CQuestManager::SaveMasterScriptDataToGameState(CScriptDataBase&, const CCharString&) @ 0x004B5030
#include <cstdio>
#include <string.h>

static int g_stringsAlive = 0;

class CCharString { public:
    char m_text[32];
    CCharString() { m_text[0] = 0; ++g_stringsAlive; }
    ~CCharString() { --g_stringsAlive; }
};

class CPersistContext { public:
    CCharString* m_pOutput;
    CPersistContext(CCharString& output) : m_pOutput(&output) { }
    void Write(const char* text) { strcat(m_pOutput->m_text, text); }
};

class CScriptDataBase { public:
    virtual ~CScriptDataBase() { }
    virtual void OnPersist(CPersistContext& context) { context.Write("MASTER=1;"); }
};

struct CRuntimeQuestPersistence {
    char m_quest[32]; char m_script[32]; int m_calls;
    void SetQuestMasterDataPersistScript(const CCharString& quest, const CCharString& script)
    {
        strcpy(m_quest, quest.m_text); strcpy(m_script, script.m_text); ++m_calls;
    }
};

class CQuestManager { public:
    CRuntimeQuestPersistence RuntimeQuestPersistence;
    void SaveMasterScriptDataToGameState(CScriptDataBase& data, const CCharString& quest)
    {
        CCharString script;
        CPersistContext context(script);
        data.OnPersist(context);
        RuntimeQuestPersistence.SetQuestMasterDataPersistScript(quest, script);
    }
};

int main()
{
    CQuestManager m; m.RuntimeQuestPersistence.m_calls = 0;
    CScriptDataBase data;
    CCharString quest; strcpy(quest.m_text, "Q_MASTER");
    int alive = g_stringsAlive;
    m.SaveMasterScriptDataToGameState(data, quest);
    if (m.RuntimeQuestPersistence.m_calls != 1 || strcmp(m.RuntimeQuestPersistence.m_quest, "Q_MASTER") != 0 || strcmp(m.RuntimeQuestPersistence.m_script, "MASTER=1;") != 0) { std::printf("004b5030_TEST FAIL\n"); return 1; }
    if (g_stringsAlive != alive) { std::printf("004b5030_TEST FAIL\n"); return 1; }
    std::printf("004b5030_TEST PASS\n");
    return 0;
}