// behaviour test for 0x006E7460 CGameScriptInterface::PostAddScriptedEntities
#include <stdio.h>
class CScriptBase { public: int Id; };

class CScriptInfo;
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
class CQuestManager {
public:
    class CActiveQuest {
    public:
        long                         ID;                 // +0x00
        CScriptInfo*                 PScriptInfo;        // +0x04
        CCountedPointer<CScriptBase> PScript;            // +0x08
    };
    unsigned char _pad00[0x88];
    CActiveQuest* PQuestCurrentlyRunning;                // +0x88
    void AttachExistingScriptedEntitiesToScript(CScriptBase& script);   // 0x004B26B0
};
CQuestManager* g_pQuestManager;
class CGameScriptInterface {
public:
    virtual void PostAddScriptedEntities() const;
};

void CGameScriptInterface::PostAddScriptedEntities() const
{
    g_pQuestManager->AttachExistingScriptedEntitiesToScript(*g_pQuestManager->PQuestCurrentlyRunning->PScript);
}
static CScriptBase* g_attached;
void CQuestManager::AttachExistingScriptedEntitiesToScript(CScriptBase& script) { g_attached = &script; }
int main()
{
    CScriptBase script; script.Id = 4;
    CQuestManager::CActiveQuest quest; quest.PScript.Data = &script; quest.PScript.Info = 0;
    CQuestManager qm; qm.PQuestCurrentlyRunning = &quest; g_pQuestManager = &qm;
    CGameScriptInterface gsi; const CGameScriptInterface* p = &gsi;
    p->PostAddScriptedEntities();
    int ok = g_attached == &script;
    printf(ok ? "006e7460_TEST PASS\n" : "006e7460_TEST FAIL\n");
    return ok ? 0 : 1;
}