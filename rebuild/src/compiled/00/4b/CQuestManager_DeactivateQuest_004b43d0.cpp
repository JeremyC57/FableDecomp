// CQuestManager::DeactivateQuest(const CCharString& questName, unsigned long reason) -- find the active quest with this name and deactivate it through the counted-pointer overload
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B43D0, length 118 bytes (`ret 8`).
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString ==),
// CQuestManager::DeactivateQuest(CCountedPointer<CActiveQuest>, unsigned long) @ 0x004B3A20.
// Layout: ActiveQuests (list<CCountedPointer<CActiveQuest>>, head pointer) at +0x38 (PDB +0x44); node {next, prev,
// CCountedPointer {Data +8, Info +0xc}}; retail CActiveQuest QuestName +0x30; CCPPointerInfo's count at +0.
// Shape: the landed AddQuestRegion (0x004B3900) search -- head in ebx, the key's string block loaded once inside the
// non-empty guard, do/while walk; the found branch passes the node's counted pointer BY VALUE (its user copy
// constructor builds it in the argument area: `sub esp,8 ; mov eax,esp`, `inc dword ptr [esi]`) and returns. The
// quest object is a named local (inline member reads keep it in ecx and shift the compare registers).
// Leaf: ?DeactivateQuest@CQuestManager@@QAEXABVCCharString@@K@Z
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

class CCPPointerInfo { public:
    long m_count;                                  // +0x00
};

template <class T> class CCountedPointer { public:
    T*              Data;                          // +0x00
    CCPPointerInfo* Info;                          // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_count; }
};

struct CActiveQuest {
    unsigned char _pad_0x00[0x30];
    CCharString   QuestName;                       // +0x30
};

struct CActiveQuestNode {                          // list node: next, prev, CCountedPointer<CActiveQuest>
    CActiveQuestNode*             next;
    CActiveQuestNode*             prev;
    CCountedPointer<CActiveQuest> quest;           // +0x08
};

class CQuestManager { public:
    unsigned char     _pad_0x00[0x38];
    CActiveQuestNode* ActiveQuests;                // +0x38 (head node)
    void DeactivateQuest(CCountedPointer<CActiveQuest> quest, unsigned long reason);   // 0x004B3A20
    void DeactivateQuest(const CCharString& questName, unsigned long reason);
};

void CQuestManager::DeactivateQuest(const CCharString& questName, unsigned long reason)
{
    CActiveQuestNode* head = ActiveQuests;
    CActiveQuestNode* node = head->next;
    if (node != head) {
        const CCharStringData* key = questName.PStringData;
        do {
            CActiveQuest* quest = node->quest.Data;
            if (SameText(quest->QuestName.PStringData, key)) {
                DeactivateQuest(node->quest, reason);
                return;
            }
            node = node->next;
        } while (node != head);
    }
}