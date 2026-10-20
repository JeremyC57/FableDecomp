// Behaviour test for 0065c36e: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillage::SetVillageLimbo(bool limbo) -- records the village limbo state; entering limbo also puts every living villager that is not a scripted quest thing into limbo (kept on level unload) and hands every living building's door locking to the limbo flag, then refreshes the mirrored crime data
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CBaseObject;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad21[0x71];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;      // bit 6
    unsigned char PausedDuringCutscenes : 1;
    bool HasTC(long type) const { return m_present.test(type); }
    void SetInLimbo(bool inLimbo);              // 0x004C8CF0
    void SetPersistOnUnloadWhenInLimbo(bool persist) { PersistOnUnloadWhenInLimbo = persist; }
};
bool __fastcall GFHasScriptThing(CThing* thing);   // 0x004C7E70

struct CThingBuilding : CThing {
    unsigned char _pad_0x93[0xd4 - 0x93];
    bool OverrideAutomaticLocking : 1;          // +0xd4 bit 0
    void LockAllDoors(bool lock, bool silent);  // 0x0082DB00
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    void* __vftable;
    CBaseObject* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer { };   // CIntelligentPointer<CThingBuilding>
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing);   // 0x004AB8F0
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);     // 0x0065F8CA (T = CIntelligentPointer<CThingBuilding>)

template <class T> struct SListNode {           // STLport list node
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;                         // +0x08
};
struct CPeopleList {                            // list<CIntelligentPointer<CThingAICreature> >: the head node pointer
    SListNode<CBaseIntelligentPointer>* _M_node;
    CPeopleList() { }
    CPeopleList(const CPeopleList& other);      // 0x00519420
    ~CPeopleList();                             // 0x00512890
};
struct CBuildingList {                          // list<CIntelligentPointer<CThingBuilding> >
    SListNode<CIntelligentPointerBuilding>* _M_node;
};

struct CTCVillage {
    unsigned char _pad_0x00[0x28];
    CBuildingList VillageBuildings;             // +0x28
    CPeopleList   VillagePeople;                // +0x2c
    unsigned char _pad_0x30[0x82];
    bool          Limbo;                        // +0xb2
    bool          LaggyLimbo;                   // +0xb3
    void ProcessCrimeDeeds();                   // 0x0065C2D0
    void SetVillageLimbo(bool limbo);
};

void CTCVillage::SetVillageLimbo(bool limbo)
{
    Limbo = limbo;
    if (limbo) {
        LaggyLimbo = true;
        CPeopleList people(VillagePeople);
        for (SListNode<CBaseIntelligentPointer>* it = people._M_node->_M_next; it != people._M_node; it = it->_M_next) {
            CBaseIntelligentPointer& person = it->_M_data;
            if (IS_THING_ALIVE(person)) {
                if (!GFHasScriptThing(person.GetPItem()) || !person.GetPItem()->HasTC(0x1f)) {
                    person.GetPItem()->SetInLimbo(true);
                    person.GetPItem()->SetPersistOnUnloadWhenInLimbo(true);
                }
            }
        }
        for (SListNode<CIntelligentPointerBuilding>* it = VillageBuildings._M_node->_M_next; it != VillageBuildings._M_node; it = it->_M_next) {
            CIntelligentPointerBuilding& building = it->_M_data;
            if (IS_THING_ALIVE(building)) {
                ((CThingBuilding*)building.GetPItem())->OverrideAutomaticLocking = limbo;
                ((CThingBuilding*)building.GetPItem())->LockAllDoors(true, false);
            }
        }
        ProcessCrimeDeeds();
    }
}

// ---- stubs and driver ----
bool CBitset::test(unsigned long position) const { return position == 0x1f && ((const unsigned char*)this)[1] == 1; }   // test marker byte
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)PData; }
static CThing* g_dead;
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing) { return thing.PData != 0 && (CThing*)thing.PData != g_dead; }
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing.PData != 0 && (CThing*)thing.PData != g_dead; }
static CThing* g_scripted;
bool __fastcall GFHasScriptThing(CThing* thing) { return thing == g_scripted; }
static int g_limbo; static bool g_limboArg;
void CThing::SetInLimbo(bool inLimbo) { ++g_limbo; g_limboArg = inLimbo; }
static int g_locks; static bool g_lockArg, g_silentArg = true;
void CThingBuilding::LockAllDoors(bool lock, bool silent) { ++g_locks; g_lockArg = lock; g_silentArg = silent; }
static int g_copies, g_dtors;
static SListNode<CBaseIntelligentPointer>* g_copyHead;
CPeopleList::CPeopleList(const CPeopleList& other) : _M_node(other._M_node) { ++g_copies; }
CPeopleList::~CPeopleList() { ++g_dtors; }
static int g_refresh;
void CTCVillage::ProcessCrimeDeeds() { ++g_refresh; }

int main()
{
    static CThing people[3];
    static CThingBuilding buildings[2];
    static SListNode<CBaseIntelligentPointer> phead, pnodes[3];
    static SListNode<CIntelligentPointerBuilding> bhead, bnodes[2];
    static CTCVillage village;
    phead._M_next = &pnodes[0];
    for (int i = 0; i < 3; ++i) { pnodes[i]._M_next = i < 2 ? &pnodes[i + 1] : &phead; pnodes[i]._M_data.PData = (CBaseObject*)&people[i]; }
    bhead._M_next = &bnodes[0];
    for (int i = 0; i < 2; ++i) { bnodes[i]._M_next = i < 1 ? &bnodes[i + 1] : &bhead; bnodes[i]._M_data.PData = (CBaseObject*)&buildings[i]; }
    village.VillagePeople._M_node = &phead; village.VillageBuildings._M_node = &bhead;
    // people[1] is a scripted thing with interface 0x1F (kept out of limbo); people[2] is dead
    g_scripted = &people[1]; ((unsigned char*)&people[1].m_present)[1] = 1; g_dead = &people[2];
    village.SetVillageLimbo(false);
    if (village.Limbo || g_limbo || g_locks || g_refresh || g_copies) { std::printf("0065c36e_TEST FAIL (off)\n"); return 1; }
    village.SetVillageLimbo(true);
    if (!village.Limbo || !village.LaggyLimbo || g_limbo != 1 || !g_limboArg || !people[0].PersistOnUnloadWhenInLimbo
        || people[1].PersistOnUnloadWhenInLimbo || people[2].PersistOnUnloadWhenInLimbo) { std::printf("0065c36e_TEST FAIL (people)\n"); return 1; }
    if (g_locks != 2 || !g_lockArg || g_silentArg || !buildings[0].OverrideAutomaticLocking || !buildings[1].OverrideAutomaticLocking
        || g_refresh != 1 || g_copies != 1 || g_dtors != 1) { std::printf("0065c36e_TEST FAIL (buildings)\n"); return 1; }
    std::printf("0065c36e_TEST PASS\n");
    return 0;
}