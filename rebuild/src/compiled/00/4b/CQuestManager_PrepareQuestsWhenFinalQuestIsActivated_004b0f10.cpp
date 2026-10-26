// CQuestManager::PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) -- queue every active, non-vignette quest card's quest except the final one for deactivation (reason 2), then stop the map table showing quest cards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0F10, length 165 bytes, `ret 4`
// (PDB ?PrepareQuestsWhenFinalQuestIsActivated@CQuestManager@@QAEXABVCCharString@@@Z).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const
// 0x004365B0 (TC type 0x6c, the quest card), CTCQuestCard::IsVignette 0x007022E0 (landed caller 0x004B1110; the
// manifest's CTCShotDefinitionBase label is a byte-twin propagation), CCharString::operator!=(const CCharString&)
// 0x0099E900, CQuestManager::AddPendingQuestToDeactivate(const CCharString&, unsigned long) 0x004B0A30 (it appends a
// {name, reason} pair to the retail PendingQuestsToDeactivate list at +0x3c; manifest "ListNode_InsertEntry").
// Layout: retail ActiveQuestCards +0x5c (PDB +0x88), World +0x7c (PDB +0xd8); CWorld::MapTableShowQuestCardsOnUsed at
// retail +0xda (PDB +0xe6, the CWorld block 0xc below the PDB); the quest card's ScriptQuestName +0x28.
// Shape: the card is not null-tested and its quest card comes from the unchecked GetQuestCard helper (an unset local
// when the TC is absent): VC7.1 seeds that register from the argument word once before the walk, exactly as in
// GetActiveQuestCardFromScriptName 0x004B0C80; the closing World store is duplicated into both exits.
// Layout (supersedes any local-model note above): engine/CWorld.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14 (MSVCR71)

class CCharStringData { public:                                 // the shared string block
    const char*   Data;                                  // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570 (strcmp-style, landed)
class CCharString { public:
    CCharStringData* PStringData;
    bool operator!=(const CCharString& other) const;     // 0x0099E900
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
    bool IsVignette() const;                             // 0x007022E0
};

class CWorld : public CWorldDonor {
 public:
};

class CQuestManager { public:
    unsigned char _pad00[0x5c];
    CCardNode*    ActiveQuestCards;                      // +0x5c list<CIntelligentPointer<CThing>> (head node)
    CCardNode*    CompletedQuestCards;                   // +0x60
    unsigned char _pad64[0x7c - 0x64];
    CWorld*       World;                                 // +0x7c (PDB +0xd8)
    void AddPendingQuestToDeactivate(const CCharString& name, unsigned long reason);   // 0x004B0A30
    void PrepareQuestsWhenFinalQuestIsActivated(const CCharString& finalQuestName);
};

void CQuestManager::PrepareQuestsWhenFinalQuestIsActivated(const CCharString& finalQuestName)
{
    for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        CTCQuestCard* tc = GetQuestCard(card);
        if (tc && !tc->IsVignette() && tc->ScriptQuestName != finalQuestName)
            AddPendingQuestToDeactivate(tc->ScriptQuestName, 2);
    }
    World->MapTableShowQuestCardsOnUsed = false;
}