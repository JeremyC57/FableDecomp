// Behaviour test for 004b1590: src/004b1590.cpp copied verbatim; the card thing, its quest-card component, the lists
// and the GUI stubbed so the objective clear, the card gates (alive, component, activatable, feat activated) and the
// activation side effects (pushed, limbo, persisted, available type removed, flag, GUI refresh) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
struct CCharString { struct CCharStringData* PStringData; };

struct CTCBase;
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
struct CThing {                                          // retail CThing (local model: offsets PDB -0x10 past the containers)
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

struct CTCQuestCard {                                    // TC type 0x6c (retail layout = PDB)
    unsigned char _pad00[0x14];
    long          FeatIndex;                             // +0x14
    long GetFeatIndex() const { return FeatIndex; }
    bool IsActivatable_00702810() const;                 // 0x00702810
};

struct CBaseIntelligentPointer {                         // 8 bytes: vptr, item block
    void* __vftable;
    void* PItem;
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);                        // 0x004A96F0 (T = CThing)
};
struct CThingPointerList {                               // STLport list<CIntelligentPointer<CThing>>: one sentinel pointer
    void* _M_node;
    void push_back(const CIntelligentPointer<CThing>& value);   // 0x004BA4E0
};
struct CIsQuestCardWithFeatIndex {                       // remove_if predicate (PDB: one long)
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

struct CPlayerGui {
    void Refresh(long flag);                             // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790

struct CQuestManager {
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
// ---- end source ----
static int g_clears = 0, g_pushes = 0, g_limbo = 0, g_persist = 0, g_removes = 0, g_refresh = 0, g_ptrDtors = 0;
static long g_removedFeat = -1, g_activatedFeat = -1; static bool g_activatable = true, g_activateOk = true; static void* g_pushed = 0;
void CObjectiveList::clear() { ++g_clears; }
void CThingPointerList::push_back(const CIntelligentPointer<CThing>& v) { ++g_pushes; g_pushed = v.PItem; }
void CQuestCardList::remove_if(CIsQuestCardWithFeatIndex p) { ++g_removes; g_removedFeat = p.FeatIndex; }
void CPlayerGui::Refresh(long) { ++g_refresh; }
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
void CThing::SetInLimbo(bool b) { if (b) ++g_limbo; }
void CThing::SetAsGameStatePersisted(bool b) { if (b) ++g_persist; }
bool CTCQuestCard::IsActivatable_00702810() const { return g_activatable; }
bool CQuestManager::ActivateFeat(long f) { g_activatedFeat = f; return g_activateOk; }
template<> CIntelligentPointer<CThing>::CIntelligentPointer(CThing* item) { __vftable = 0; PItem = item; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ptrDtors; }
static CTCQuestCard g_card;
static CTCEntry g_entries[1] = { { 0x6c, (CTCBase*)&g_card } };
CTCEntry* CTCMap::LowerBound(const long& type) { return type <= 0x6c ? _M_start : _M_finish; }
int main()
{
    g_card.FeatIndex = 12;
    static CThing card; memset(&card, 0, sizeof(card));
    card.Components._M_start = g_entries; card.Components._M_finish = g_entries + 1;
    static unsigned char raw[sizeof(CQuestManager)]; memset(raw, 0, sizeof(raw));
    CQuestManager* qm = (CQuestManager*)raw;
    CHECK(qm->ActivateFeatCard(0) == false && g_clears == 1);
    CHECK(qm->ActivateFeatCard(&card) == false && g_clears == 2);                 // no quest-card component
    card.UsingComponents[0x6c >> 5] |= 1UL << (0x6c & 31);
    CHECK(qm->ActivateFeatCard(&card) == true);
    CHECK(g_activatedFeat == 12 && g_pushes == 1 && g_pushed == &card && g_ptrDtors == 1 && g_limbo == 1 && g_persist == 1);
    CHECK(g_removes == 1 && g_removedFeat == 12 && qm->QuestCardsNeedUpdating && g_refresh == 1);
    g_activateOk = false; CHECK(qm->ActivateFeatCard(&card) == false && g_pushes == 1);
    g_activateOk = true; g_activatable = false; CHECK(qm->ActivateFeatCard(&card) == false && g_pushes == 1);
    g_activatable = true; card.Dead = 1; CHECK(qm->ActivateFeatCard(&card) == false && g_clears == 6);
    if (fails == 0) printf("004b1590_TEST PASS\n"); else printf("004b1590_TEST FAIL\n");
    return fails;
}