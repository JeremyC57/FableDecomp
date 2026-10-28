#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CBaseObject;
class CThingAICreature;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

template<class T> struct CRetailList {              // retail std::list: one sentinel pointer
    void* m_head;
    void remove(const T& value);                    // 0x0065DE8A (CThing) / 0x0065DE48 (CThingAICreature)
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    unsigned char _pad11[0x91 - 0x11];
    unsigned char Dead : 1;                         // +0x91 bit 0
    unsigned char _flags91 : 7;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;              // +0x92 bit 4
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

class CThingAICreature : public CThing {
public:
    unsigned char _pad93[0x168 - 0x93];
    CBaseIntelligentPointer HomeBuilding;           // +0x168 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer WorkBuilding;           // +0x170
};

__forceinline bool IsThingAlive(const CBaseIntelligentPointer& p)   // one GetPItem per pointer here
{
    CThing* thing = p.GetPItem();
    if (thing && !thing->Dead)
        return true;
    return false;
}

class CPlayerGui { public:
    void Refresh(long flag);                        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

class CTCFollowed { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    CRetailList< CIntelligentPointer<CThing> > Followers;              // +0x0C
    CRetailList< CIntelligentPointer<CThingAICreature> > AIFollowers;  // +0x10
    bool IsAIFollower(const CThing* thing) const;   // 0x006D4DB0
    void RemoveEmoteIcon(CThing& thing);            // 0x006D5050
    void RemoveFollower(CThing& follower);
};

void CTCFollowed::RemoveFollower(CThing& follower)
{
    Followers.remove(CIntelligentPointer<CThing>(&follower));
    if (follower.ThingClass == 1) {
        if (IsAIFollower(&follower)) {
            CThingAICreature& creature = (CThingAICreature&)follower;
            if (!IsThingAlive(creature.HomeBuilding) && !IsThingAlive(creature.WorkBuilding)) {
                follower.SetAsGameStatePersisted(false);
                follower.LevelPersistent = false;
            }
        }
        AIFollowers.remove(CIntelligentPointer<CThingAICreature>((CThingAICreature*)&follower));
    }
    RemoveEmoteIcon(follower);
    g_pPlayerGui->Refresh(0);
}

// ---- test stubs for the retail callees ----
static CThing* g_removed; static CThing* g_aiRemoved; static int g_icons, g_refresh, g_dtors; static int g_persisted = -1;
static bool g_isAI;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
static void* g_followersList;
template<class T> void CRetailList<T>::remove(const T& v) { if ((void*)this == g_followersList) g_removed = v.m_pItem; else g_aiRemoved = v.m_pItem; }
void CThing::SetAsGameStatePersisted(bool p) { g_persisted = p ? 1 : 0; }
void CPlayerGui::Refresh(long) { ++g_refresh; }
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
bool CTCFollowed::IsAIFollower(const CThing*) const { return g_isAI; }
void CTCFollowed::RemoveEmoteIcon(CThing&) { ++g_icons; }
void CTCFollowed::v00() {}

int main()
{
    int ok = 1;
    CThingAICreature villager; std::memset(&villager, 0, sizeof(villager)); villager.ThingClass = 1;
    villager.LevelPersistent = 1; villager.GameStatePersisted = 1;
    CThing house; std::memset(&house, 0, sizeof(house));
    CTCFollowed f; g_followersList = &f.Followers;
    g_isAI = true; villager.HomeBuilding.m_pItem = &house;
    f.RemoveFollower(villager);              // has a live home: stays persisted
    if (g_removed != &villager || g_aiRemoved != &villager || g_persisted != -1 || !villager.LevelPersistent) ok = 0;
    if (g_icons != 1 || g_refresh != 1 || g_dtors != 2) ok = 0;
    house.Dead = 1; g_aiRemoved = 0;
    f.RemoveFollower(villager);              // dead home, no work building: no longer persisted
    if (g_persisted != 0 || villager.LevelPersistent || !villager.GameStatePersisted || g_aiRemoved != &villager) ok = 0;
    villager.LevelPersistent = 1; g_persisted = -1; g_isAI = false; g_aiRemoved = 0;
    f.RemoveFollower(villager);              // not an AI follower: AI list entry removed anyway
    if (g_persisted != -1 || !villager.LevelPersistent || g_aiRemoved != &villager) ok = 0;
    CThing chicken; std::memset(&chicken, 0, sizeof(chicken)); chicken.ThingClass = 2; g_aiRemoved = 0;
    f.RemoveFollower(chicken);
    if (g_removed != &chicken || g_aiRemoved != 0 || g_icons != 4) ok = 0;
    if (!ok) { std::printf("006d5180_TEST FAIL\n"); return 1; }
    std::printf("006d5180_TEST PASS\n");
    return 0;
}