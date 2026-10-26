// CQuestManager::ActivateFeatCard(CThing*) -- clear the objectives; if the living card thing's quest card is activatable and its feat activates, make the card active (in limbo, persisted), drop its type from the available cards, flag the quest cards for an update and refresh the player GUI
// Retry 10b (getsw sweep, FIXES B-15): the feat index is read through an inline getter `GetFeatIndex()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1590, length 223 bytes, `ret 4`
// (PDB ?ActivateFeatCard@CQuestManager@@QAE_NPAVCThing@@@Z).
// Callees: list<CQuestManager::CObjective>::clear 0x004B8C50 (Objectives at +0x74; the manifest's
// CGuiControlTreePane::SortTreeRecursively label is propagated), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// 0x0040F020 (non-const, TC type 0x6c), CTCQuestCard::IsActivatable 0x00702810 (PDB name inferred: the card's only
// bool query that consults the hero; manifest "CViewBase_FindParticleMorph"), CQuestManager::ActivateFeat(long)
// 0x004AF300 (landed), CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0,
// list<CIntelligentPointer<CThing>>::push_back 0x004BA4E0, ~CBaseIntelligentPointer 0x00A01C10, CThing::SetInLimbo
// 0x004C8CF0, CThing::SetAsGameStatePersisted 0x004C7880, list<CQuestManager::CQuestCard>::remove_if<
// CIsQuestCardWithFeatIndex> 0x004B91A0, CPlayerGui::Refresh(long) 0x004378C0 on g_pPlayerGui 0x013B8790.
// Layout (retail, 4-byte STLport lists): AvailableQuestCardTypes +0x58, ActiveQuestCards +0x5c, Objectives +0x74,
// QuestCardsNeedUpdating +0x8f (PDB +0x7c/+0x88/+0xc8/+0xeb); CThing Dead +0x91 bit 0; CTCQuestCard FeatIndex +0x14.
// Shape: the TC lookup is the checked HasTC + GetTC pair whose key copy VC7.1 homes in the dead `card` argument slot;
// the intelligent pointer is a named temporary built out of line, pushed by reference and destroyed after the call;
// the remove_if predicate is a 4-byte functor passed by value.
class CCharString { public: class CCharStringData* PStringData; };

class CTCBase;
struct CTCEntry {                                        // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     Type;
    CTCBase* TC;
};
struct CTCMap {                                          // CThing::Components: a sorted vector of pairs (12 bytes)
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const long& type);              // 0x0040F020 (the non-const instantiation)
};
class CThing { public:                                          // retail CThing (local model: offsets PDB -0x10 past the containers)
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20 which TC types the thing holds
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                              // +0x91 bit 0 (PDB +0xa1)
    unsigned char _91_1 : 7;
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        CTCMap* map = &Components;
        long key = type;
        CTCEntry* it = map->LowerBound(key);
        if (it == map->_M_finish || type < it->Type)
            it = map->_M_finish;
        return it->TC;
    }
    void SetInLimbo(bool inLimbo);                       // 0x004C8CF0
    void SetAsGameStatePersisted(bool persisted);        // 0x004C7880
};

class CTCQuestCard { public:                                    // TC type 0x6c (retail layout = PDB)
    unsigned char _pad00[0x14];
    long          FeatIndex;                             // +0x14
    long GetFeatIndex() const { return FeatIndex; }
    bool IsActivatable_00702810() const;                 // 0x00702810
};

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                        // 0x004A96F0 (T = CThing)

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CThingPointerList {                               // STLport list<CIntelligentPointer<CThing>>: one sentinel pointer
    void* _M_node;
    void push_back(const CIntelligentPointer<CThing>& value);   // 0x004BA4E0
};
class CIsQuestCardWithFeatIndex { public:                       // remove_if predicate (PDB: one long)
    long FeatIndex;
    CIsQuestCardWithFeatIndex(long featIndex) : FeatIndex(featIndex) {}
};
struct CQuestCardList {                                  // STLport list<CQuestManager::CQuestCard>
    void* _M_node;
    void remove_if(CIsQuestCardWithFeatIndex predicate); // 0x004B91A0
};
struct CObjectiveList {                                  // STLport list<CQuestManager::CObjective>
    void* _M_node;
    void clear();                                        // 0x004B8C50
};

class CPlayerGui { public:
    void Refresh(long flag);                             // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790

class CQuestManager { public:
    unsigned char     _pad00[0x58];
    CQuestCardList    AvailableQuestCardTypes;           // +0x58 (PDB +0x7c)
    CThingPointerList ActiveQuestCards;                  // +0x5c (PDB +0x88)
    unsigned char     _pad60[0x74 - 0x60];
    CObjectiveList    Objectives;                        // +0x74 (PDB +0xc8)
    unsigned char     _pad78[0x8f - 0x78];
    bool              QuestCardsNeedUpdating;            // +0x8f (PDB +0xeb)
    bool ActivateFeat(long featIndex);                   // 0x004AF300
    bool ActivateFeatCard(CThing* card);
};

bool CQuestManager::ActivateFeatCard(CThing* card)
{
    Objectives.clear();
    if (card && !card->Dead && card->HasTC(0x6c)) {
        CTCQuestCard* tc = (CTCQuestCard*)card->GetTC(0x6c);
        if (tc->IsActivatable_00702810()) {
            if (ActivateFeat(tc->GetFeatIndex())) {
                {
                    CIntelligentPointer<CThing> pointer(card);
                    ActiveQuestCards.push_back(pointer);
                }
                card->SetInLimbo(true);
                card->SetAsGameStatePersisted(true);
                AvailableQuestCardTypes.remove_if(CIsQuestCardWithFeatIndex(tc->GetFeatIndex()));
                QuestCardsNeedUpdating = true;
                g_pPlayerGui->Refresh(0);
                return true;
            }
        }
    }
    return false;
}