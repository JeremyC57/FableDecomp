// CQuestManager::OnEntityInterrupted(CThing&) -- tell every running quest script that has the thing's entity script bound that the entity (by unique id) was interrupted
// Grade: RELOCATION_MATCH
// Leaf: ?OnEntityInterrupted@CQuestManager@@QAEXAAVCThing@@@Z
// Lever: reference locals for the thing's binding name and unique id taken at entry: VC7.1 then loads the thing at entry into ebp and `this` into ebx (retail colouring), the id through eax/ecx.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFBB0, length 72 bytes.
// Callees: CScriptBase::IsEntityScriptBound(const CCharString&) const @ 0x00CB7D60,
// CScriptBase::OnScriptedEntityInterrupted(unsigned __int64) @ 0x00CB7BC0 (FableWin.pdb names; the manifest labels
// "CScriptManager::FindScriptBinding" / "NavQuadTree_MarkNodeActive" are rejected).
// Identity: FableWin.pdb ?OnEntityInterrupted@CQuestManager@@QAEXAAVCThing@@@Z.
// Layout: ActiveQuests (one head pointer) at +0x38; CActiveQuest::PScript object at +8; CThing UniqueID at +0x18
// (PDB) and the thing's script-binding name (a CCharString) at +0x74 in the shipped build.
// Shape: the quest object in edi, the script re-read from it for the second call (the first call may change it),
// the head re-read for every end test; the 64-bit id is pushed high word first.
class CCharString { public:
    void* PStringData;
};

class CScriptBase { public:
    bool IsEntityScriptBound(const CCharString& name) const;      // 0x00CB7D60
    void OnScriptedEntityInterrupted(unsigned __int64 uid);       // 0x00CB7BC0
};

class CThing { public:
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UniqueID;                     // +0x18
    unsigned char    _pad_0x20[0x54];
    CCharString      ScriptBindingName;            // +0x74
};

struct CActiveQuest {
    long          ID;                              // +0x00
    void*         PScriptInfo;                     // +0x04
    CScriptBase*  PScript;                         // +0x08 CCountedPointer<CScriptBase>::Data
};

struct CActiveQuestNode {
    CActiveQuestNode* next;
    CActiveQuestNode* prev;
    CActiveQuest*     quest;                       // +0x08
    void*             questInfo;                   // +0x0c
};

class CQuestManager { public:
    unsigned char     _pad_0x00[0x38];
    CActiveQuestNode* ActiveQuests;                // +0x38 (head node)
    void OnEntityInterrupted(CThing& thing);
};

void CQuestManager::OnEntityInterrupted(CThing& thing)
{
    const CCharString& name = thing.ScriptBindingName;
    const unsigned __int64& uid = thing.UniqueID;
    for (CActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
        CActiveQuest* quest = node->quest;
        if (quest->PScript && quest->PScript->IsEntityScriptBound(name))
            quest->PScript->OnScriptedEntityInterrupted(uid);
    }
}