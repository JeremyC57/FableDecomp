// Behaviour test for CQuestManager::GetMasterScriptDataForScript(const CScriptInfo&) @ 0x004AFA10
#include <cstdio>

struct CScriptBase { int m_dummy; };
struct CScriptDataBase { int m_dummy; };

struct CScriptInfo { void* AllocFunc; void* MasterScriptDataAlloc; void* Name; long ID; bool MasterScript; };
struct CActiveQuest { long ID; CScriptInfo* PScriptInfo; CScriptBase* PScript; CScriptDataBase* PScriptData; };
struct CActiveQuestNode { CActiveQuestNode* next; CActiveQuestNode* prev; CActiveQuest* quest; };

struct CQuestManager {
    CActiveQuestNode* ActiveQuests;
    CScriptDataBase* GetMasterScriptDataForScript(const CScriptInfo& info)
    {
        for (CActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
            CActiveQuest* quest = node->quest;
            if (quest->PScript && quest->PScriptInfo->MasterScript &&
                quest->PScriptInfo->MasterScriptDataAlloc == info.MasterScriptDataAlloc)
                return quest->PScriptData;
        }
        return 0;
    }
};

int main()
{
    int allocA, allocB;
    CScriptBase script;
    CScriptDataBase dataA, dataB, dataC;
    CScriptInfo infoA = { 0, &allocA, 0, 1, true };
    CScriptInfo infoNotMaster = { 0, &allocB, 0, 2, false };
    CScriptInfo infoB = { 0, &allocB, 0, 3, true };
    CActiveQuest q1 = { 1, &infoA, 0, &dataA };              // no running script
    CActiveQuest q2 = { 2, &infoNotMaster, &script, &dataB }; // not a master script
    CActiveQuest q3 = { 3, &infoB, &script, &dataC };
    CActiveQuestNode head, n1, n2, n3;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head;
    n1.quest = &q1; n2.quest = &q2; n3.quest = &q3;
    CQuestManager m; m.ActiveQuests = &head;
    CScriptInfo askB = { 0, &allocB, 0, 9, true };
    CScriptInfo askA = { 0, &allocA, 0, 9, true };
    if (m.GetMasterScriptDataForScript(askB) != &dataC || m.GetMasterScriptDataForScript(askA) != 0) { std::printf("004afa10_TEST FAIL\n"); return 1; }
    std::printf("004afa10_TEST PASS\n");
    return 0;
}
