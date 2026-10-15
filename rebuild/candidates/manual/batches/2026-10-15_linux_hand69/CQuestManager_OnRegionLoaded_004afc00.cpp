// CQuestManager::OnRegionLoaded(const CCharString&) -- forward the region-loaded notification to the script of every active quest that has one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFC00, length 47 bytes.
// Callees: CScriptBase::OnRegionLoaded(const CCharString&) @ 0x00CB7B00 (FableWin.pdb name; the manifest's
// "CScriptBase::KillSpawnedFunction" label is propagated over the byte-identical twins 0x00CB7B00 / 0x00CB7B60).
// Identity: FableWin.pdb ?OnRegionLoaded@CQuestManager@@QAEXABVCCharString@@@Z; the twin at 0x004AFC30 is OnRegionUnloaded.
// Layout: ActiveQuests (list<CCountedPointer<CActiveQuest>>, one head pointer) at +0x38 (PDB +0x44); the node holds
// the quest object at +8, CActiveQuest::PScript's object at +8.
// Shape: the region reference is loaded once in the preheader (ebx), the script pointer tested and used from ecx,
// the head re-read from the member for every end test.
struct CCharString {
    void* PStringData;
};

struct CScriptBase {
    void OnRegionLoaded(const CCharString& region);   // 0x00CB7B00
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

struct CQuestManager {
    unsigned char     _pad_0x00[0x38];
    CActiveQuestNode* ActiveQuests;                // +0x38 (head node)
    void OnRegionLoaded(const CCharString& region);
};

void CQuestManager::OnRegionLoaded(const CCharString& region)
{
    for (CActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
        CScriptBase* script = node->quest->PScript;
        if (script)
            script->OnRegionLoaded(region);
    }
}
