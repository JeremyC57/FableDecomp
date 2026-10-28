// Behaviour test for 0094bca0: src/0094bca0.cpp copied verbatim; the world clock, the shop-keeper component, the
// opinion manager and the intelligent pointers stubbed so the frame gate, the speaking/work-hours gate, the stall pick
// (shop thing, else the creature itself when on the spot) and the attitude veto are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_MinionHawkingFromStall::IsRunnable() -- once the next hawk frame is reached, a silent shopkeeper in work hours with a shop hawks from the shop thing; with no shop thing (and a stall-less shop) it hawks on the spot unless the creature dislikes the player (attitude 3, 4 or 8)
// Retry 10b (getsw sweep, FIXES B-15): the opinion reaction manager is read through an inline getter `GetOpinionReactionManager()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BCA0, length 336 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame @ 0x0049D870 (static), CVectorMap::LowerBound
//          @ 0x0040F020 (inlined GetInterface 0x90 = CTCShopKeeper, bit 16 of +0x30 tested in memory), IsSpeaking
//          @ 0x008644E0 (__fastcall), CAIStateGroup_WorkBase::IsInWorkHours @ 0x0094BEF0, CTCShopKeeper::GetShopTC
//          @ 0x00798F50 / GetShopThing @ 0x00798D80, CIntelligentPointer<const CThing>(CThing*) @ 0x00668B60 and
//          operator= @ 0x00668B80 (both out of line), ~CBaseIntelligentPointer @ 0x00A01C10, GetPItem @ 0x00A01B50,
//          CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0, COpinionReactionManager::GetThingAttitudeToPlayer @ 0x00701840.
// Shape: HasTC + entry pick (keeper in edi); each PStall assignment is an implicit conversion (the frame temporary's
//          address is re-taken for the out-of-line operator=); the result is
//          `PStall.GetPItem() != 0` (neg/sbb/neg).
// Layout: no engine header for this class; modelled locally like the landed OnReplace / IsStillValid rows (next hawk
//          frame +0x14, PStall +0x18); the shop's +0x50 flag is descriptive.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
bool __fastcall IsSpeaking(CThing* creature);   // 0x008644E0
class CTCShop { public:
    unsigned char _pad[0x50];
    bool OnTheSpot;                             // +0x50 (descriptive)
};
class CTCShopKeeper { public:                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
    CThing* GetShopThing();                     // 0x00798D80
};
class COpinionReactionManager { public:
    long GetThingAttitudeToPlayer(CThing* thing);   // 0x00701840
};
class CWorld { public:
    unsigned char _pad[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74
    COpinionReactionManager* GetOpinionReactionManager() const { return POpinionReactionManager; }
    static long GetWorldFrame();                // 0x0049D870
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;                            // +0x04
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
namespace N_00668b60 { struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    CIntelligentPointerConstThing(CThing* item);    // 0x00668B60
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other);   // 0x00668B80
}; } using N_00668b60::CIntelligentPointerConstThing;
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase {
    long NextHawkFrame;                         // +0x14 (descriptive)
    CIntelligentPointerConstThing PStall;       // +0x18
    bool IsInWorkHours();                       // 0x0094BEF0 (CAIStateGroup_WorkBase)
    bool IsRunnable();
};

bool CAIStateGroup_MinionHawkingFromStall::IsRunnable()
{
    if (GetPWorld()->GetWorldFrame() < NextHawkFrame)
        return false;
    CThing* creature = GetCreature();
    if (creature->HasTC(0x90)) {
        CTCShopKeeper* keeper = creature->GetTC<CTCShopKeeper>(0x90);
        if (!IsSpeaking(creature) && IsInWorkHours()) {
            CTCShop* shop = keeper->GetShopTC();
            if (shop) {
                PStall = keeper->GetShopThing();
                if (!PStall.GetPItem() && shop->OnTheSpot) {
                    PStall = GetCreature();
                    CThing* self = GetCreature();
                    long attitude = GetPThingWorld()->GetOpinionReactionManager()->GetThingAttitudeToPlayer(self);
                    if (attitude == 3 || attitude == 4 || attitude == 8)
                        PStall = (CThing*)0;
                }
                return PStall.GetPItem() != 0;
            }
        }
    }
    return false;
}
// ---- end source ----
static long g_frame = 100; static bool g_speaking = false, g_workHours = true; static long g_attitude = 0;
static CTCShop g_shop; static CTCShop* g_shopPtr = &g_shop; static CThing* g_shopThing = 0;
static CTCShopKeeper g_keeper;
static CInterfaceMap::SEntry g_entries[1] = { { 0x90, (CTCBase*)&g_keeper } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 0x90 ? first : last; }
bool __fastcall IsSpeaking(CThing*) { return g_speaking; }
CTCShop* CTCShopKeeper::GetShopTC() { return g_shopPtr; }
CThing* CTCShopKeeper::GetShopThing() { return g_shopThing; }
long COpinionReactionManager::GetThingAttitudeToPlayer(CThing*) { return g_attitude; }
long CWorld::GetWorldFrame() { return g_frame; }
static COpinionReactionManager g_orm; static CWorld g_world;
CWorld* CAIStateGroupBase::GetPThingWorld() { return &g_world; }
CWorld* CAIStateGroupBase::GetPWorld() { return &g_world; }
CAIStateGroupBase::~CAIStateGroupBase() {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CIntelligentPointerConstThing::CIntelligentPointerConstThing(CThing* item) { m_pItem = item; }
CIntelligentPointerConstThing& CIntelligentPointerConstThing::operator=(const CIntelligentPointerConstThing& o) { m_pItem = o.m_pItem; return *this; }
bool CAIStateGroup_MinionHawkingFromStall::IsInWorkHours() { return g_workHours; }
int main()
{
    g_world.POpinionReactionManager = &g_orm;
    static CThing creature; memset(&creature, 0, sizeof(creature));
    creature.m_interfaces.first = g_entries; creature.m_interfaces.last = g_entries + 1;
    static CAIBrain brain; brain.PCreature = &creature;
    static unsigned char raw[sizeof(CAIStateGroup_MinionHawkingFromStall)]; memset(raw, 0, sizeof(raw));
    CAIStateGroup_MinionHawkingFromStall* g = (CAIStateGroup_MinionHawkingFromStall*)raw;
    g->PBrain = &brain; g->NextHawkFrame = 200;
    CHECK(g->IsRunnable() == false);                                   // too early
    g->NextHawkFrame = 50;
    CHECK(g->IsRunnable() == false);                                   // no shop-keeper component
    creature.m_presentBits[0x90 >> 5] |= 1UL << (0x90 & 31);
    static CThing stall; g_shopThing = &stall;
    CHECK(g->IsRunnable() == true && g->PStall.m_pItem == &stall);
    g_shopThing = 0; g_shop.OnTheSpot = true;
    CHECK(g->IsRunnable() == true && g->PStall.m_pItem == &creature);  // the creature hawks on the spot
    g_attitude = 4;
    CHECK(g->IsRunnable() == false && g->PStall.m_pItem == 0);         // vetoed by attitude
    g_attitude = 0; g_shop.OnTheSpot = false;
    CHECK(g->IsRunnable() == false);
    g_shopThing = &stall; g_speaking = true;
    CHECK(g->IsRunnable() == false);
    g_speaking = false; g_shopPtr = 0;
    CHECK(g->IsRunnable() == false);
    if (fails == 0) printf("0094bca0_TEST PASS\n"); else printf("0094bca0_TEST FAIL\n");
    return fails;
}