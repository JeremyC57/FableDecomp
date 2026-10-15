// CQuestManager::SetQuestAsPersistent(const CCharString& questName, bool persistent) -- flag every active quest of that name as (not) persistent and pass the flag on to its live quest card
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B2490, length 119 bytes (`ret 8`).
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString ==),
// CQuestManager::GetActiveQuestCardFromScriptName(const CCharString&) @ 0x004B0C80,
// CThing::SetAsGameStatePersisted(bool) @ 0x004C7880.
// Layout: ActiveQuests (list<CCountedPointer<CActiveQuest>>, head pointer) at +0x38 (PDB +0x44), node {next, prev,
// quest +8, info +0xc}, as in the landed AddQuestRegion 0x004B3900; retail CActiveQuest QuestName +0x30 and the
// persistence flag byte at +0x24; CThing's dead flag is bit 0 of +0x91.
// Shape: a top-tested walk (head re-read for each end test, the name block re-read every pass because Compare is
// only declared) that does not stop at the first match; the card is skipped when null or dead. The quest's name
// block is read into a named local first: passing both blocks inline loads the key first (eax/ecx swapped).
// Leaf: SetQuestAsPersistent
struct CCharStringData { const char* m_pData; unsigned long m_length; };
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharString {
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

struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    void SetAsGameStatePersisted(bool persisted);  // 0x004C7880
};

struct CActiveQuest {
    unsigned char _pad_0x00[0x24];
    bool          Persistent;                      // +0x24
    unsigned char _pad_0x25[0xb];
    CCharString   QuestName;                       // +0x30
};

struct CActiveQuestNode {                          // list node: next, prev, CCountedPointer<CActiveQuest> {Data, Info}
    CActiveQuestNode* next;
    CActiveQuestNode* prev;
    CActiveQuest*     quest;                       // +0x08
    void*             questInfo;                   // +0x0c
};

struct CQuestManager {
    unsigned char     _pad_0x00[0x38];
    CActiveQuestNode* ActiveQuests;                // +0x38 (head node)
    CThing* GetActiveQuestCardFromScriptName(const CCharString& scriptName);   // 0x004B0C80
    void SetQuestAsPersistent(const CCharString& questName, bool persistent);
};

void CQuestManager::SetQuestAsPersistent(const CCharString& questName, bool persistent)
{
    for (CActiveQuestNode* node = ActiveQuests->next; node != ActiveQuests; node = node->next) {
        CActiveQuest* quest = node->quest;
        const CCharStringData* name = quest->QuestName.PStringData;
        if (SameText(name, questName.PStringData)) {
            quest->Persistent = persistent;
            CThing* card = GetActiveQuestCardFromScriptName(questName);
            if (card && !card->Dead)
                card->SetAsGameStatePersisted(persistent);
        }
    }
}