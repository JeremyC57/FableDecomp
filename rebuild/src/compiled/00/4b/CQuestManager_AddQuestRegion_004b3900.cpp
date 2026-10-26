// CQuestManager::AddQuestRegion(const CCharString&, const CRegion*) -- find the active quest with the given name and add the region to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B3900, length 95 bytes.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString ==),
// CQuestManager::CActiveQuest::AddPRegion(const CRegion*) @ 0x004B3860 (manifest "AddRegionToQuestArray").
// Identity: FableWin.pdb ?AddQuestRegion@CQuestManager@@QAEXABVCCharString@@PBVCRegion@@@Z (`ret 8`).
// Layout: ActiveQuests (list<CCountedPointer<CActiveQuest>>, one head pointer) at +0x38 (PDB +0x44); the node holds
// the counted pointer's object at +8. Retail CActiveQuest (PDB 56 bytes, its Regions vector 12 bytes here):
// ID +0, PScriptInfo +4, PScript +8, Regions +0x10, ..., PScriptData +0x28, QuestName +0x30.
// Shape: the GetAvailableQuestCardDefNameFromScriptName (0x004AF670) search: head in ebp, the key's string block
// loaded once inside the non-empty guard (esi), the quest object in ebx, the do/while walk; the found branch
// calls AddPRegion on the quest and returns through its own epilogue.
class CRegion;
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

class CCharString { public:
    CCharStringData* PStringData;
};

__forceinline bool SameText(const CCharStringData* a, const CCharStringData* b)
{
    if (a == b)
        return true;
    if (a && b && a->m_length == b->m_length)
        return Compare_00411570(a->m_pData, b->m_pData) == 0;
    return false;
}

struct CActiveQuest {
    unsigned char _pad_0x00[0x30];
    CCharString   QuestName;                       // +0x30
    void AddPRegion(const CRegion* region);        // 0x004B3860
};

struct CActiveQuestNode {                          // list node: next, prev, CCountedPointer<CActiveQuest> {Data, Info}
    CActiveQuestNode* next;
    CActiveQuestNode* prev;
    CActiveQuest*     quest;                       // +0x08
    void*             questInfo;                   // +0x0c
};

class CQuestManager { public:
    unsigned char     _pad_0x00[0x38];
    CActiveQuestNode* ActiveQuests;                // +0x38 (head node)
    void AddQuestRegion(const CCharString& questName, const CRegion* region);
};

void CQuestManager::AddQuestRegion(const CCharString& questName, const CRegion* region)
{
    CActiveQuestNode* head = ActiveQuests;
    CActiveQuestNode* node = head->next;
    if (node != head) {
        const CCharStringData* key = questName.PStringData;
        do {
            CActiveQuest* quest = node->quest;
            if (SameText(quest->QuestName.PStringData, key)) {
                quest->AddPRegion(region);
                return;
            }
            node = node->next;
        } while (node != head);
    }
}