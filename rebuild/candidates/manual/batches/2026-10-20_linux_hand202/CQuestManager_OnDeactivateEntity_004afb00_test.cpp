// Behaviour test for CQuestManager::OnDeactivateEntity (0x004AFB00): only quests whose script binds the thing's
// script name get (UID, script thing, flag); names "NULL" and "" skip the walk; ActivatedScript is always cleared.
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct CCharString {
    const char* Text;
    bool operator!=(const char* t) const { return std::strcmp(Text, t) != 0; }
};
struct CScriptThing { const char* Name; };
struct CThing {
    unsigned char    ThingType;
    u64 UID;
    CCharString      ScriptName;
    unsigned char    NotPausedDuringCutscenes : 1;
    unsigned char    DeathPersistent : 1;
    unsigned char    KillOnLevelUnload : 1;
    unsigned char    ActivatedScript : 1;
    unsigned char    _93_4 : 4;
    CScriptThing GetScriptThing() { CScriptThing t; t.Name = ScriptName.Text; return t; }
};
struct CScriptBase {
    const char* Binds; int Calls; u64 LastUID; const char* LastName; bool LastFlag;
    bool BindsEntityScript(const CCharString& name) { return std::strcmp(name.Text, Binds) == 0; }
    void OnScriptedEntityDeactivated(u64 uid, CScriptThing& thing, bool flag)
    { ++Calls; LastUID = uid; LastName = thing.Name; LastFlag = flag; }
};
struct CActiveQuest { long ID; void* PScriptInfo; CScriptBase* PScript_Data; void* PScript_Info; };
struct SActiveQuestNode { SActiveQuestNode* next; SActiveQuestNode* prev; CActiveQuest* m_pData; void* m_pInfo; };
struct CQuestManager {
    SActiveQuestNode* ActiveQuests;
    void OnDeactivateEntity(CThing& thing, bool flag);
};
void CQuestManager::OnDeactivateEntity(CThing& thing, bool flag)
{
    if (thing.ScriptName != "NULL" && thing.ScriptName != "") {
        for (SActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
            CActiveQuest* quest = node->m_pData;
            if (quest->PScript_Data && quest->PScript_Data->BindsEntityScript(thing.ScriptName)) {
                CScriptThing scriptThing = thing.GetScriptThing();
                quest->PScript_Data->OnScriptedEntityDeactivated(thing.UID, scriptThing, flag);
            }
        }
    }
    thing.ActivatedScript = false;
}
int main()
{
    CScriptBase s1 = { "Bob", 0, 0, 0, false }, s2 = { "Ann", 0, 0, 0, false }, s3 = { "Bob", 0, 0, 0, false };
    CActiveQuest q1 = { 1, 0, &s1, 0 }, q2 = { 2, 0, &s2, 0 }, q3 = { 3, 0, 0, 0 }, q4 = { 4, 0, &s3, 0 };
    SActiveQuestNode head, n1, n2, n3, n4;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &n4; n4.next = &head;
    n1.m_pData = &q1; n2.m_pData = &q2; n3.m_pData = &q3; n4.m_pData = &q4;
    CQuestManager qm; qm.ActiveQuests = &head;
    CThing bob; std::memset(&bob, 0, sizeof bob);
    bob.UID = 0x123456789ULL; bob.ScriptName.Text = "Bob"; bob.ActivatedScript = 1; bob.KillOnLevelUnload = 1; bob._93_4 = 0xf;
    qm.OnDeactivateEntity(bob, true);
    int bad = 0;
    if (s1.Calls != 1 || s3.Calls != 1 || s2.Calls != 0 || s1.LastUID != 0x123456789ULL || std::strcmp(s3.LastName, "Bob") || !s1.LastFlag) bad |= 1;
    if (bob.ActivatedScript || !bob.KillOnLevelUnload || bob._93_4 != 0xf) bad |= 2;
    CThing none; std::memset(&none, 0, sizeof none); none.ScriptName.Text = "NULL"; none.ActivatedScript = 1;
    qm.OnDeactivateEntity(none, false);
    CThing empty; std::memset(&empty, 0, sizeof empty); empty.ScriptName.Text = ""; empty.ActivatedScript = 1;
    qm.OnDeactivateEntity(empty, false);
    if (s1.Calls != 1 || s2.Calls != 0 || none.ActivatedScript || empty.ActivatedScript) bad |= 4;
    CThing ann; std::memset(&ann, 0, sizeof ann); ann.ScriptName.Text = "Ann"; ann.UID = 7;
    qm.OnDeactivateEntity(ann, false);
    if (s2.Calls != 1 || s2.LastUID != 7 || s2.LastFlag || s1.Calls != 1) bad |= 8;
    if (bad) { std::printf("004afb00_TEST FAIL\n"); return 1; }
    std::printf("004afb00_TEST PASS\n");
    return 0;
}
