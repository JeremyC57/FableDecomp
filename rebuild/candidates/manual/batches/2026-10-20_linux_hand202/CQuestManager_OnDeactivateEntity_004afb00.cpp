// CQuestManager::OnDeactivateEntity(CThing&, bool) -- a thing with a real script name (not "NULL"/"") is reported as deactivated to every active quest's script that binds that name, as (UID, CScriptThing, flag); the thing's ActivatedScript flag is then cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFB00, length 165 bytes, `ret 8`.
// PDB ?OnDeactivateEntity@CQuestManager@@QAEXAAVCThing@@_N@Z. The twin of the landed OnActivateEntity (0x004AFA60, modelled
// there with a CThing*): same guard chain without the thing-type test, the script's
// deactivation hook takes the caller's flag as a third argument and the trailing bit store clears instead of sets.
// Callees: CCharString::operator!=(const char*) 0x0099E960 (x2: "NULL" 0x012393E4, "" 0x0122D70E), the script's
// name-binding test 0x00CB7D60, CThing::GetScriptThing 0x004C7E50 (by-value result in the frame),
// CScriptBase::OnScriptedEntityDeactivated(unsigned __int64, CScriptThing&, bool) 0x00CB88B0 (PDB signature),
// CScriptThing::~CScriptThing 0x004AA840.
// Layout: retail CThing (PDB offsets -0x10): 64-bit UID +0x18, ScriptName +0x74, ActivatedScript +0x93 bit 3;
// retail CQuestManager ActiveQuests list at +0x38, CActiveQuest PScript (CCountedPointer<CScriptBase>) Data at +0x08.
// Shape: the CScriptThing is a named local of the hit block (`lea ; push` of its frame word); the flag argument is
// pushed first, so it is the first value loaded.
struct CCharString {
    void* PStringData;
    bool operator!=(const char* text) const;       // 0x0099E960
};
struct CScriptThing {
    unsigned char _body[12];
    ~CScriptThing();                               // 0x004AA840
};
struct CThing {
    unsigned char    _pad00[0x10];
    unsigned char    ThingType;                    // +0x10
    unsigned char    _pad11[7];
    unsigned __int64 UID;                          // +0x18
    unsigned char    _pad20[0x74 - 0x20];
    CCharString      ScriptName;                   // +0x74
    unsigned char    _pad78[0x93 - 0x78];
    unsigned char    NotPausedDuringCutscenes : 1; // +0x93 bit 0
    unsigned char    DeathPersistent : 1;          // +0x93 bit 1
    unsigned char    KillOnLevelUnload : 1;        // +0x93 bit 2
    unsigned char    ActivatedScript : 1;          // +0x93 bit 3
    unsigned char    _93_4 : 4;
    CScriptThing GetScriptThing();                 // 0x004C7E50
};
struct CScriptBase {
    bool BindsEntityScript(const CCharString& name);                                              // 0x00CB7D60
    void OnScriptedEntityDeactivated(unsigned __int64 uid, CScriptThing& thing, bool flag);        // 0x00CB88B0
};
struct CActiveQuest {                              // CQuestManager::CActiveQuest, retail layout
    long         ID;                               // +0x00
    void*        PScriptInfo;                      // +0x04
    CScriptBase* PScript_Data;                     // +0x08 CCountedPointer<CScriptBase>
    void*        PScript_Info;                     // +0x0c
};
struct SActiveQuestNode {                          // STLport list node of CCountedPointer<CActiveQuest>
    SActiveQuestNode* next;                        // +0x00
    SActiveQuestNode* prev;                        // +0x04
    CActiveQuest*     m_pData;                     // +0x08
    void*             m_pInfo;                     // +0x0c
};
struct CQuestManager {
    unsigned char     _pad00[0x38];
    SActiveQuestNode* ActiveQuests;                // +0x38
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
