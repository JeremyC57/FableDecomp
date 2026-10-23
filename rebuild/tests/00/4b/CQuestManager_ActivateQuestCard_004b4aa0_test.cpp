// CQuestManager::ActivateQuestCard(CThing* card, bool loadResources) -- clear the objectives; if the living card thing's quest card is activatable, has a def, no quest is between its start and finish screens in the card's region and its script quest activates, make the card active (in limbo, persisted), reset the guild-seal recall point, drop the card's type from the available cards and refresh the player GUI
// Leaf: ActivateQuestCard
// Shape: the quest-card component lookup is a template-constant-key GetTC: LowerBound binds its const long& to the constant, so the
//   key temporary is homed in the dead `card` argument slot (shared with the uninitialised-on-miss tc) and the compare stays an
//   immediate; a by-value key copy local takes its own frame word.
struct CCharString {
    void* PStringData;
    ~CCharString();                                      // 0x0099EAE0
};
namespace N_00415dd0 { struct CDefString {
    long m_index;
    operator CCharString() const;                        // 0x00415DD0
}; } using N_00415dd0::CDefString;
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct CTCBase;
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const long& type);              // 0x0040F020 (the non-const instantiation)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                    // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                            // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                              // +0x91 bit 0
    unsigned char _91_1 : 7;
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <long K> __forceinline CTCBase* GetTCK()
    {
        CTCMap* map = &Components;
        CTCEntry* it = map->LowerBound(K);
        if (it == map->_M_finish || K < it->Type)
            it = map->_M_finish;
        return it->TC;
    }
    void SetInLimbo(bool inLimbo);                       // 0x004C8CF0
    void SetAsGameStatePersisted(bool persisted);        // 0x004C7880
    CDefString GetDefName();                             // 0x004C7CC0
};

struct CQuestCardDef {
    virtual void v00();
    virtual void DeleteThis();                           // slot 1
    long RefCount;                                       // +4
};
struct CQuestCardDefPointer {                            // CDefPointer<CQuestCardDef const>
    CQuestCardDef* PDef;
    CQuestCardDefPointer(const CQuestCardDefPointer& other) : PDef(other.PDef) { if (PDef) ++PDef->RefCount; }
    ~CQuestCardDefPointer() { if (PDef && --PDef->RefCount == 0) PDef->DeleteThis(); }
    bool IsNull() const { return PDef == 0; }
};
struct CTCQuestCard {                                    // TC type 0x6c (retail layout = PDB)
    unsigned char  _pad00[0xc];
    CQuestCardDefPointer PDef;                           // +0x0c
    bool           Finished;                             // +0x10
    bool           Replayable;                           // +0x11
    long           FeatIndex;                            // +0x14
    CCharString    QuestName;                            // +0x18
    CCharString    ObjectiveName;                        // +0x1c
    CCharString    RegionName;                           // +0x20
    CCharString    TeleporterRegionName;                 // +0x24
    CCharString    ScriptQuestName;                      // +0x28
    bool IsActivatable_00702810() const;                 // 0x00702810
    CCharString GetRegionName() const;                   // 0x007025A0
};

struct CBaseIntelligentPointer {
    void* __vftable;
    void* PItem;
    ~CBaseIntelligentPointer();                          // 0x00A01C10
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);                        // 0x004A96F0 (T = CThing)
};
struct CThingPointerList {
    void* _M_node;
    void push_back(const CIntelligentPointer<CThing>& value);   // 0x004BA4E0
};
struct CObjectiveList {
    void* _M_node;
    void clear();                                        // 0x004B8C50
};
struct CWorld {
    unsigned char _pad00[0x154];
    C3DVector     GuildSealRecallPos;                    // +0x154 (PDB +0x164)
    float         GuildSealRecallAngleXY;                // +0x160 (PDB +0x170)
};
struct CPlayerGui {
    void Refresh(long flag);                             // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790

struct CQuestManager {
    unsigned char     _pad00[0x5c];
    CThingPointerList ActiveQuestCards;                  // +0x5c (PDB +0x88)
    unsigned char     _pad60[0x74 - 0x60];
    CObjectiveList    Objectives;                        // +0x74 (PDB +0xc8)
    void*             ScriptInfoManager;                 // +0x78
    CWorld*           World;                             // +0x7c
    bool IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region);   // 0x004B1340
    bool ActivateQuest(const CCharString& name, bool a2, bool loadResources);                 // 0x004B4A10
    void RemoveAvailableQuestCardType(const CCharString& name);                               // 0x004B02F0
    bool ActivateQuestCard(CThing* card, bool loadResources);
};

bool CQuestManager::ActivateQuestCard(CThing* card, bool loadResources)
{
    Objectives.clear();
    if (card && !card->Dead) {
        CTCQuestCard* tc;
        if (card->HasTC(0x6c))
            tc = (CTCQuestCard*)card->GetTCK<0x6c>();
        if (tc->IsActivatable_00702810()) {
            CQuestCardDef* def = tc->PDef.PDef;
            if (def) {
                ++def->RefCount;
                if (!IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(tc->GetRegionName())) {
                    if (ActivateQuest(tc->ScriptQuestName, true, loadResources)) {
                        {
                            CIntelligentPointer<CThing> pointer(card);
                            ActiveQuestCards.push_back(pointer);
                        }
                        card->SetInLimbo(true);
                        card->SetAsGameStatePersisted(true);
                        World->GuildSealRecallPos = C3DVector(0.0f, 0.0f, 0.0f);
                        World->GuildSealRecallAngleXY = 0.0f;
                        RemoveAvailableQuestCardType(card->GetDefName());
                        g_pPlayerGui->Refresh(0);
                        if (--def->RefCount == 0)
                            def->DeleteThis();
                        return true;
                    }
                }
                if (--def->RefCount == 0)
                    def->DeleteThis();
            }
        }
    }
    return false;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
CCharString::~CCharString() { }
CDefString::operator CCharString() const { CCharString s; s.PStringData = (void*)m_index; return s; }
CTCEntry* CTCMap::LowerBound(const long& type)
{
    CTCEntry* it = _M_start;
    while (it != _M_finish && it->Type < type) ++it;
    return it;
}
static int g_limbo = 0, g_persist = 0, g_clears = 0, g_pushes = 0, g_refresh = -1, g_removes = 0, g_activates = 0, g_deletes = 0;
static void* g_pushedItem = 0; static void* g_removedName = 0; static void* g_activateName = 0; static bool g_activateLoad = false, g_activateA2 = false;
static void* g_regionAsked = 0;
static bool g_activatable = true, g_busy = false, g_activateOk = true;
void CThing::SetInLimbo(bool inLimbo) { if (inLimbo) ++g_limbo; }
void CThing::SetAsGameStatePersisted(bool persisted) { if (persisted) ++g_persist; }
CDefString CThing::GetDefName() { CDefString d; d.m_index = 77; return d; }
struct TDef : CQuestCardDef { void v00() { } void DeleteThis() { ++g_deletes; } };
void CQuestCardDef::v00() { }
void CQuestCardDef::DeleteThis() { }
bool CTCQuestCard::IsActivatable_00702810() const { return g_activatable; }
CCharString CTCQuestCard::GetRegionName() const { CCharString s; s.PStringData = (void*)"region"; return s; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
template<> CIntelligentPointer<CThing>::CIntelligentPointer(CThing* item) { __vftable = 0; PItem = item; }
void CThingPointerList::push_back(const CIntelligentPointer<CThing>& value) { ++g_pushes; g_pushedItem = value.PItem; }
void CObjectiveList::clear() { ++g_clears; }
void CPlayerGui::Refresh(long flag) { g_refresh = flag; }
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
bool CQuestManager::IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region) { g_regionAsked = region.PStringData; return g_busy; }
bool CQuestManager::ActivateQuest(const CCharString& name, bool a2, bool loadResources) { ++g_activates; g_activateName = name.PStringData; g_activateA2 = a2; g_activateLoad = loadResources; return g_activateOk; }
void CQuestManager::RemoveAvailableQuestCardType(const CCharString& name) { ++g_removes; g_removedName = name.PStringData; }
static void Reset() { g_limbo = g_persist = g_clears = g_pushes = g_removes = g_activates = g_deletes = 0; g_refresh = -1; g_pushedItem = g_removedName = g_activateName = g_regionAsked = 0; }
int main()
{
    static CQuestManager qm; static long wbuf[0x60]; CWorld& world = *(CWorld*)wbuf; static CThing card;
    static TDef def;
    static long tcbuf[16];
    CTCQuestCard& tc = *(CTCQuestCard*)tcbuf;
    memset(&qm, 0, sizeof(qm)); memset(&card, 0, sizeof(card));
    qm.World = &world;
    tc.PDef.PDef = &def; tc.ScriptQuestName.PStringData = (void*)"Q_Script";
    static CTCEntry entries[3] = { { 0x10, 0 }, { 0x6c, 0 }, { 0x70, 0 } };
    entries[1].TC = (CTCBase*)&tc;
    card.Components._M_start = entries; card.Components._M_finish = entries + 3; card.Components._M_end_of_storage = entries + 3;
    card.UsingComponents[0x6c >> 5] |= 1UL << (0x6c & 31);
    // null card
    Reset(); CHECK(qm.ActivateQuestCard(0, true) == false); CHECK(g_clears == 1);
    // dead card
    Reset(); card.Dead = 1; CHECK(qm.ActivateQuestCard(&card, true) == false); CHECK(g_activates == 0); card.Dead = 0;
    // not activatable
    Reset(); def.RefCount = 1; g_activatable = false;
    CHECK(qm.ActivateQuestCard(&card, true) == false); CHECK(def.RefCount == 1 && g_regionAsked == 0); g_activatable = true;
    // region busy
    Reset(); g_busy = true;
    CHECK(qm.ActivateQuestCard(&card, true) == false); CHECK(def.RefCount == 1 && g_deletes == 0 && strcmp((const char*)g_regionAsked, "region") == 0 && g_activates == 0); g_busy = false;
    // ActivateQuest fails, def released to zero
    Reset(); g_activateOk = false; def.RefCount = 0;
    CHECK(qm.ActivateQuestCard(&card, false) == false); CHECK(g_activates == 1 && g_deletes == 1 && def.RefCount == 0 && g_pushes == 0); g_activateOk = true;
    // success
    Reset(); def.RefCount = 1; world.GuildSealRecallPos = C3DVector(1.0f, 2.0f, 3.0f); world.GuildSealRecallAngleXY = 5.0f;
    CHECK(qm.ActivateQuestCard(&card, false) == true);
    CHECK(g_activates == 1 && strcmp((const char*)g_activateName, "Q_Script") == 0 && g_activateA2 == true && g_activateLoad == false);
    CHECK(g_pushes == 1 && g_pushedItem == &card && g_limbo == 1 && g_persist == 1);
    CHECK(world.GuildSealRecallPos.X == 0.0f && world.GuildSealRecallPos.Y == 0.0f && world.GuildSealRecallPos.Z == 0.0f && world.GuildSealRecallAngleXY == 0.0f);
    CHECK(g_removes == 1 && g_removedName == (void*)77 && g_refresh == 0);
    CHECK(def.RefCount == 1 && g_deletes == 0 && g_clears == 1);
    if (fails == 0) printf("004b4aa0_TEST PASS\n");
    return fails;
}