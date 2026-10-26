// CQuestManager::IsQuestCompleted(const CCharString&) -- whether a living completed quest-card thing carries the named quest script's card
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0FC0, length 175 bytes, `ret 4`
// (PDB ?IsQuestCompleted@CQuestManager@@QBE_NABVCCharString@@@Z).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0
// (TC type 0x6c = UsingComponents[3] bit 12), the CCharString compare 0x00411570 (`__fastcall`, inlined operator==).
// Layout: retail CompletedQuestCards (STLport list<CIntelligentPointer<CThing>>, node data +8) at +0x60 (PDB +0x94);
// CThing::Dead +0x91 bit 0 (PDB +0xa1); CTCQuestCard::ScriptQuestName at +0x28.
// Shape: a null or dead card is skipped, the TC local starts at null (the absent path jumps to the next node); `this`
// stays in esi until the lookup reuses it and is then reloaded from the frame word; `return true` / `return false` tails.
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14 (MSVCR71)

class CCharStringData { public:                                 // the shared string block
    const char*   Data;                                  // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570 (strcmp-style, landed)
class CCharString { public:
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->Length == b->Length)
            return Compare_00411570(a->Data, b->Data) == 0;
        return false;
    }
};

class CTCBase;
struct CTCEntry {                                        // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     Type;
    CTCBase* TC;
};
struct CTCMap {                                          // CThing::Components: a sorted vector of pairs (12 bytes)
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long type) const         // the inlined map find: the end entry when absent
    {
        CTCEntry* it = LowerBound((unsigned long)type);
        if (it == _M_finish || type < it->Type)
            it = _M_finish;
        return it->TC;
    }
};

class CThing { public:                                          // retail CThing (local model: engine/CThing.h is the quarantined donor)
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20 which TC types the thing holds
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x10];
    class CTCPhysicsBase* PhysicsTC;                    // +0x60 (PDB +0x70)
    unsigned char _pad64[0x2d];
    unsigned char Dead : 1;                              // +0x91 bit 0 (PDB +0xa1)
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;                           // bit 5
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const { return Components.Find(type); }
    void Kill(bool immediately);                         // 0x004C9B80
};

class CTCQuestCard;                                     // TC type 0x6c
__forceinline CTCQuestCard* GetQuestCard(CThing* thing)  // the unchecked lookup: unset when the thing has no quest card TC
{
    CTCQuestCard* tc;
    if (thing->HasTC(0x6c))
        tc = (CTCQuestCard*)thing->GetTC(0x6c);
    return tc;
}

struct CBaseIntelligentPointer {                         // 8 bytes: vptr, item block
    void* PItem;
    virtual ~CBaseIntelligentPointer();
    CThing* GetPItem() const;                            // 0x00A01B50
};

struct CCardNode {                                       // STLport list<CIntelligentPointer<CThing>> node
    CCardNode*              _M_next;
    CCardNode*              _M_prev;
    CBaseIntelligentPointer _M_data;                     // +0x08
};
inline void EraseNode(CCardNode* node)                   // STLport list::erase(iterator)
{
    CCardNode* next = node->_M_next;
    CCardNode* prev = node->_M_prev;
    prev->_M_next = next;
    next->_M_prev = prev;
    node->_M_data.~CBaseIntelligentPointer();
    free(node);
}

class CPlayerGui { public:
    void Refresh(long flag);                             // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790

class CTCQuestCard { public:
    unsigned char _pad00[0x28];
    CCharString   ScriptQuestName;                       // +0x28 (PDB name)
};

class CQuestManager { public:
    unsigned char _pad00[0x5c];
    CCardNode*    ActiveQuestCards;                      // +0x5c list<CIntelligentPointer<CThing>> (head node)
    CCardNode*    CompletedQuestCards;                   // +0x60
    bool IsQuestCompleted(const CCharString& scriptName) const;
};

bool CQuestManager::IsQuestCompleted(const CCharString& scriptName) const
{
    for (CCardNode* node = CompletedQuestCards->_M_next; node != CompletedQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card && !card->Dead) {
            CTCQuestCard* tc = 0;
            if (card->HasTC(0x6c))
                tc = (CTCQuestCard*)card->GetTC(0x6c);
            if (tc && tc->ScriptQuestName == scriptName)
                return true;
        }
    }
    return false;
}