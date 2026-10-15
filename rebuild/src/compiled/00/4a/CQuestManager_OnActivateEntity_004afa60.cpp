// CQuestManager::OnActivateEntity(CThing* thing) -- a thing with a real script name (not "NULL"/"", not of type 0x0d) is offered to every active quest's script that binds that name, as (UID, CScriptThing); the thing is then flagged ActivatedScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFA60, length 160 bytes, `ret 4`.
// Callees: CCharString::operator!=(const char*) @ 0x0099E960 (x2: "NULL" @ 0x012393E4, "" @ 0x0122D70E), the script's
// name-binding test @ 0x00CB7D60 (manifest "CScriptManager::FindScriptBinding", on the CScriptBase), CThing::GetScriptThing
// @ 0x004C7E50 (by-value result in the frame; the manifest's GFPredicateAnd label is propagated), the script's entity
// activation @ 0x00CB8960 ((UID by value, const CScriptThing&); manifest "CScriptBase::AttachNavigator" is unverified),
// CScriptThing::~CScriptThing @ 0x004AA840.
// Layout: retail CThing (PDB offsets -0x10): type byte +0x10, 64-bit UID +0x18, ScriptName +0x74, ActivatedScript
// +0x93 bit 3; retail CQuestManager ActiveQuests list at +0x38, CActiveQuest PScript (CCountedPointer<CScriptBase>)
// Data at +0x08 (landed GetActiveQuestFromScriptName 0x004AF5B0 model).
// Shape: one `&&` guard chain, the list head re-read for every end test, the script pointer re-read from the quest for
// the second call; the CScriptThing is a named local of the hit block (retail pushes its frame address with
// `lea ; push`, an unnamed temporary would push eax); the trailing bit set is a byte
// load/or/store.
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
    bool BindsEntityScript(const CCharString& name);                              // 0x00CB7D60
    void ActivateEntity(unsigned __int64 uid, const CScriptThing& thing);         // 0x00CB8960
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
    void OnActivateEntity(CThing* thing);
};
void CQuestManager::OnActivateEntity(CThing* thing)
{
    if (thing->ScriptName != "NULL" && thing->ScriptName != "" && thing->ThingType != 0x0d) {
        for (SActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
            CActiveQuest* quest = node->m_pData;
            if (quest->PScript_Data && quest->PScript_Data->BindsEntityScript(thing->ScriptName)) {
                CScriptThing scriptThing = thing->GetScriptThing();
                quest->PScript_Data->ActivateEntity(thing->UID, scriptThing);
            }
        }
    }
    thing->ActivatedScript = true;
}