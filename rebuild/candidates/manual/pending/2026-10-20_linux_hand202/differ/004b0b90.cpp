// CQuestManager::RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString&, bool) -- the RemoveActiveQuestCard body under the PDB name with the extra (unused) flag: kill the named script's active card thing unless completed, drop it, refresh the GUI
// Grade: DIFFER(233v238) -- register allocation: retail keeps `this` in ebp and re-reads the name argument per pass (ebx node, edi card, esi the unset TC), ours parks `this` in the frame word and caches the name in ebp; tried continue / block / function-scope TC forms
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0B90, length 238 bytes, `ret 8`
// (PDB ?RemoveActiveQuestCardAndMakeAvailableAgain@CQuestManager@@QAEXABVCCharString@@_N@Z; the bool is never read).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0,
// the CCharString compare 0x00411570, CThing::Kill(bool) 0x004C9B80, the list element's virtual destructor (slot 0),
// free 0x00BFEA14, CPlayerGui::Refresh(long) 0x004378C0 on g_pPlayerGui (0x013B8790).
// Layout: ActiveQuestCards +0x5c, CompletedQuestCards +0x60 (PDB +0x88/+0x94; 4-byte STLport lists); ScriptName +0x28 of
// the quest card TC.
// Shape: the card pointer is null-tested but the TC comes from the unchecked GetQuestCard helper (unset when absent; VC7.1
// loads its word, the argument slot, into esi before the loop), unlike the RemoveActiveQuestCard twin (0x004B0AA0); the completed-list search is an inline bool helper whose early "return true" jumps past the Kill; STLport's inline erase unlinks the
// node, destroys the intelligent pointer through its virtual destructor and frees the node.
extern "C" void __cdecl free(void* p);                   // 0x00BFEA14 (MSVCR71)

struct CCharStringData {                                 // the shared string block
    const char*   Data;                                  // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570 (strcmp-style, landed)
struct CCharString {
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

struct CTCBase;
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

struct CThing {                                          // retail CThing (local model: engine/CThing.h is the quarantined donor)
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20 which TC types the thing holds
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const { return Components.Find(type); }
    void Kill(bool immediately);                         // 0x004C9B80
};

struct CTCQuestCard;                                     // TC type 0x6c
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

struct CPlayerGui {
    void Refresh(long flag);                             // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790

struct CTCQuestCard {
    unsigned char _pad00[0x28];
    CCharString   ScriptName;                            // +0x28
};

struct CQuestManager {
    unsigned char _pad00[0x5c];
    CCardNode*    ActiveQuestCards;                      // +0x5c list<CIntelligentPointer<CThing>> (head node)
    CCardNode*    CompletedQuestCards;                   // +0x60
    bool IsCompletedQuestCard(CThing* card) const
    {
        for (CCardNode* node = CompletedQuestCards->_M_next; node != CompletedQuestCards; node = node->_M_next)
            if (node->_M_data.GetPItem() == card)
                return true;
        return false;
    }
    void RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString& scriptName, bool available);
};

void CQuestManager::RemoveActiveQuestCardAndMakeAvailableAgain(const CCharString& scriptName, bool available)
{
    for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card) {
            CTCQuestCard* tc = GetQuestCard(card);
            if (tc && tc->ScriptName == scriptName) {
                if (!IsCompletedQuestCard(card))
                    card->Kill(false);
                EraseNode(node);
                g_pPlayerGui->Refresh(0);
                return;
            }
        }
    }
}
