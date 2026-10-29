// CQuestManager::GetActiveQuestCardFromScriptName(const CCharString&) -- the active quest-card thing whose quest card belongs to the named quest script, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0C80, length 168 bytes, `ret 4`
// (PDB ?GetActiveQuestCardFromScriptName@CQuestManager@@QBEPAVCThing@@ABVCCharString@@@Z).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0
// (TC type 0x6c = UsingComponents[3] bit 12), the CCharString compare 0x00411570 (`__fastcall`, inlined operator==).
// Layout: retail ActiveQuestCards (STLport list<CIntelligentPointer<CThing>>, node data +8) at +0x5c (PDB +0x88);
// CTCQuestCard::ScriptQuestName at +0x28.
// Shape: the quest card comes from the unchecked GetQuestCard helper (an unset local when the TC is absent, landed
// IsAnyQuestActive 0x004B1080): VC7.1 loads that local's word once before the loop; `this` is parked in the frame word.
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                          // CThing::Components: a sorted vector of pairs (12 bytes)
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    const CTCEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long type) const         // the inlined map find: the end entry when absent
    {
        const CTCEntry* it = LowerBound((ETCInterfaceType)type);
        if (it == _M_finish || type < it->first)
            it = _M_finish;
        return it->second;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CThing { public:                                          // retail CThing (local model: engine/CThing.h is the quarantined donor)
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20 which TC types the thing holds
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
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

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CCardNode {                                       // STLport list<CIntelligentPointer<CThing>> node
    CCardNode*              _M_next;
    CCardNode*              _M_prev;
    CIntelligentPointer<CThing> _M_data;                     // +0x08
};
inline void EraseNode(CCardNode* node)                   // STLport list::erase(iterator)
{
    CCardNode* next = node->_M_next;
    CCardNode* prev = node->_M_prev;
    prev->_M_next = next;
    next->_M_prev = prev;
    node->_M_data.CBaseIntelligentPointer::~CBaseIntelligentPointer();
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
    CThing* GetActiveQuestCardFromScriptName(const CCharString& scriptName) const;
};

CThing* CQuestManager::GetActiveQuestCardFromScriptName(const CCharString& scriptName) const
{
    for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        CTCQuestCard* tc = GetQuestCard(card);
        if (tc && tc->ScriptQuestName == scriptName)
            return card;
    }
    return 0;
}