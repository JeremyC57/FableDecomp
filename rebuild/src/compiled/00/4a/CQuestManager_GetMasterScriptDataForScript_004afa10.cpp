// CQuestManager::GetMasterScriptDataForScript(const CScriptInfo&) -- the script data of the active master-script quest whose script info shares the given info's master-data allocator, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFA10, length 67 bytes. No callees.
// Identity: FableWin.pdb ?GetMasterScriptDataForScript@CQuestManager@@AAEPAVCScriptDataBase@@ABVCScriptInfo@@@Z.
// Layout: ActiveQuests (one head pointer) at +0x38; node +8 = the CActiveQuest object of the counted pointer.
// Retail CActiveQuest: PScriptInfo +4, PScript (counted: object +8), PScriptData (counted: object +0x28).
// CScriptInfo (PDB, 20 bytes): AllocFunc +0, MasterScriptDataAlloc +4, Name +8, ID +0xc, MasterScript +0x10.
// Shape: no calls in the loop, so the head stays in esi; the three tests (a running script, a master script,
// the same allocator) are one `&&` chain; the data pointer is returned straight from the quest.
class CScriptBase;
class CScriptDataBase;

class CScriptInfo { public:
    void*         AllocFunc;                       // +0x00
    void*         MasterScriptDataAlloc;           // +0x04
    void*         Name;                            // +0x08 CCharString
    long          ID;                              // +0x0c
    bool          MasterScript;                    // +0x10
};

struct CActiveQuest {
    long             ID;                           // +0x00
    CScriptInfo*     PScriptInfo;                  // +0x04
    CScriptBase*     PScript;                      // +0x08 CCountedPointer<CScriptBase>::Data
    void*            PScriptInfoBlock;             // +0x0c CCountedPointer<CScriptBase>::Info
    unsigned char    _pad_0x10[0x18];
    CScriptDataBase* PScriptData;                  // +0x28 CCountedPointer<CScriptDataBase>::Data
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
    CScriptDataBase* GetMasterScriptDataForScript(const CScriptInfo& info);
};

CScriptDataBase* CQuestManager::GetMasterScriptDataForScript(const CScriptInfo& info)
{
    for (CActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
        CActiveQuest* quest = node->quest;
        if (quest->PScript && quest->PScriptInfo->MasterScript &&
            quest->PScriptInfo->MasterScriptDataAlloc == info.MasterScriptDataAlloc)
            return quest->PScriptData;
    }
    return 0;
}