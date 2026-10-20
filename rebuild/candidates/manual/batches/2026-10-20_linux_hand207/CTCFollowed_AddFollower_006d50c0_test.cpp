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
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CIntelligentPointerAICreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerAICreature(CThingAICreature* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CThingPointerList {                          // retail std::list: one sentinel pointer
    void* head;
    void push_back(const CIntelligentPointer& value);   // 0x004BA4E0
};
struct CAICreaturePointerList {
    void* head;
    void push_back(const CIntelligentPointerAICreature& value);   // 0x006604EB
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

struct CPlayerGui {
    void Refresh(long flag);                        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

struct CTCFollowed {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    CThingPointerList Followers;                    // +0x0C
    CAICreaturePointerList AIFollowers;             // +0x10
    bool IsFollower(const CThing* thing) const;     // 0x006D4D70
    bool IsAIFollower(const CThing* thing) const;   // 0x006D4DB0
    void AddEmoteIcon(CThing& thing);               // 0x006D4FD0
    void AddFollower(CThing& follower, bool aiFollower);
};

void CTCFollowed::AddFollower(CThing& follower, bool aiFollower)
{
    if (!IsFollower(&follower)) {
        Followers.push_back(CIntelligentPointer(&follower));
        AddEmoteIcon(follower);
        g_pPlayerGui->Refresh(0);
    }
    if (aiFollower && follower.ThingClass == 1) {
        if (!IsAIFollower(&follower))
            AIFollowers.push_back(CIntelligentPointerAICreature((CThingAICreature*)&follower));
        follower.SetAsGameStatePersisted(true);
    }
}

// ---- test stubs for the retail callees ----
static CThing* g_list[8]; static int g_count; static CThing* g_ai[8]; static int g_aiCount;
static int g_icons, g_refresh, g_dtors; static long g_refreshArg = -1; static int g_persisted = -1;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointer::v00() {}
void CIntelligentPointerAICreature::v00() {}
void CThingPointerList::push_back(const CIntelligentPointer& v) { g_list[g_count++] = v.m_pItem; }
void CAICreaturePointerList::push_back(const CIntelligentPointerAICreature& v) { g_ai[g_aiCount++] = v.m_pItem; }
void CThing::SetAsGameStatePersisted(bool p) { g_persisted = p ? 1 : 0; }
void CPlayerGui::Refresh(long flag) { ++g_refresh; g_refreshArg = flag; }
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
bool CTCFollowed::IsFollower(const CThing* t) const { for (int i = 0; i < g_count; ++i) if (g_list[i] == t) return true; return false; }
bool CTCFollowed::IsAIFollower(const CThing* t) const { for (int i = 0; i < g_aiCount; ++i) if (g_ai[i] == t) return true; return false; }
void CTCFollowed::AddEmoteIcon(CThing&) { ++g_icons; }
void CTCFollowed::v00() {}

int main()
{
    int ok = 1;
    CThing villager; std::memset(&villager, 0, sizeof(villager)); villager.ThingClass = 1;
    CThing chicken; std::memset(&chicken, 0, sizeof(chicken)); chicken.ThingClass = 2;
    CTCFollowed f;
    f.AddFollower(villager, true);
    if (g_count != 1 || g_list[0] != &villager || g_icons != 1 || g_refresh != 1 || g_refreshArg != 0) ok = 0;
    if (g_aiCount != 1 || g_ai[0] != &villager || g_persisted != 1 || g_dtors != 2) ok = 0;
    g_persisted = -1;
    f.AddFollower(villager, true);           // already in both lists: only persisted again
    if (g_count != 1 || g_aiCount != 1 || g_icons != 1 || g_refresh != 1 || g_persisted != 1) ok = 0;
    g_persisted = -1;
    f.AddFollower(chicken, true);            // not an AI creature: plain follower only
    if (g_count != 2 || g_aiCount != 1 || g_persisted != -1 || g_icons != 2) ok = 0;
    if (!ok) { std::printf("006d50c0_TEST FAIL\n"); return 1; }
    std::printf("006d50c0_TEST PASS\n");
    return 0;
}
