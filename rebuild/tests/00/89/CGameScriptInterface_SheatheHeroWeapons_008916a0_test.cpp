// Behaviour test for 0x008916A0 CGameScriptInterface::SheatheHeroWeapons (NF79 round 7): the row object now calls the PDB-spelled members
// (CPlayerManager::GetMainPlayer, CPlayer::GetPControlledCreature, CTCCarrying::IsThingCarryingWeapon, CThingCreatureBase::ClearQueuedActions /
// FinishCurrentAction / ClearCurrentAction / SetCurrentAction, the CCreatureAction_SheatheItemToInventory ctor) and carries the action's implicit
// destructor COMDAT, which runs ~CCreatureActionBase. This fixture supplies those imports as recording stubs with the same signatures (link
// stand-ins only: the row itself is on the shared headers), and checks the call order. Keeps the catalog PassPattern FSE2_008916a0_TEST.
#include <cstdio>
#include <cstring>

static unsigned long g_Events[12];
static unsigned long g_EventCount;
static void RecordEvent(unsigned long event) { if (g_EventCount < 12) g_Events[g_EventCount] = event; ++g_EventCount; }

class CThing { public: unsigned char m_Bytes[0x91]; unsigned char m_Flags; };   // CThing::Dead is bit 0 of +0x91
class CCreatureActionBase { public: virtual ~CCreatureActionBase(); };
class CThingCreatureBase : public CThing {
public:
    void ClearQueuedActions();
    void FinishCurrentAction();
    void ClearCurrentAction();
    bool SetCurrentAction(const CCreatureActionBase& action);
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };
class CPlayerManager { public: CPlayer& GetMainPlayer(); };
class CTCCarrying { public: static bool __fastcall IsThingCarryingWeapon(const CThing& thing); };
class CCreatureAction_SheatheItemToInventory {
public:
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase& creature, CCreatureActionBase* followUp, long priority);
};
class CGameScriptInterface {
public:
    virtual void SheatheHeroWeapons() const;
    unsigned char m_Unused[0x10];
    CPlayerManager* PlayerManager;                                        // +0x14
};

static CPlayerManager* g_ObservedManager;
static CPlayer* g_Player;
static CThingPlayerCreature* g_Target;
static const void* g_ObservedTarget;
static bool g_IsCarryingWeapon;
static void* g_ConstructedAction;
static CThingCreatureBase* g_ConstructCreature;
static CCreatureActionBase* g_ConstructFollowUp;
static long g_ConstructPriority;
static const void* g_SetAction;
static void* g_DestroyedAction;

CPlayer& CPlayerManager::GetMainPlayer() { g_ObservedManager = this; RecordEvent(1); return *g_Player; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { RecordEvent(2); return g_Target; }
bool __fastcall CTCCarrying::IsThingCarryingWeapon(const CThing& thing) { g_ObservedTarget = &thing; RecordEvent(3); return g_IsCarryingWeapon; }
void CThingCreatureBase::ClearQueuedActions() { g_ObservedTarget = this; RecordEvent(4); }
void CThingCreatureBase::FinishCurrentAction() { g_ObservedTarget = this; RecordEvent(5); }
void CThingCreatureBase::ClearCurrentAction() { g_ObservedTarget = this; RecordEvent(6); }
CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase& creature, CCreatureActionBase* followUp,
                                                                               long priority)
{
    std::memset(this, 0, 0xB0);                     // the retail temporary: vptr .. PFollowUpAction (+0xA8 Data / +0xAC Info) all null
    g_ConstructedAction = this; g_ConstructCreature = &creature; g_ConstructFollowUp = followUp; g_ConstructPriority = priority;
    RecordEvent(7);
}
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) { g_ObservedTarget = this; g_SetAction = &action; RecordEvent(8); return true; }
CCreatureActionBase::~CCreatureActionBase() { g_DestroyedAction = this; RecordEvent(9); }

static void Reset() { g_EventCount = 0; g_ObservedManager = 0; g_ObservedTarget = 0; g_ConstructedAction = 0; g_SetAction = 0; g_DestroyedAction = 0; }
static bool Check(const unsigned long* expected, unsigned long count)
{
    if (g_EventCount != count) return false;
    for (unsigned long i = 0; i < count; ++i) if (g_Events[i] != expected[i]) return false;
    return true;
}

int main()
{
    static unsigned long storage[0x20];
    CPlayerManager manager;
    CPlayer player;
    static CThingPlayerCreature target;
    CGameScriptInterface* iface = reinterpret_cast<CGameScriptInterface*>(storage);
    iface->PlayerManager = &manager;
    g_Player = &player;

    const unsigned long lookup[] = { 1, 2 };
    Reset(); g_Target = 0;
    iface->CGameScriptInterface::SheatheHeroWeapons();
    if (!Check(lookup, 2) || g_ObservedManager != &manager) { std::printf("FSE2_008916a0_TEST FAIL 1\n"); return 1; }

    Reset(); g_Target = &target; target.m_Flags = 1;                  // dead
    iface->CGameScriptInterface::SheatheHeroWeapons();
    if (!Check(lookup, 2) || g_ObservedTarget != 0) { std::printf("FSE2_008916a0_TEST FAIL 2\n"); return 2; }

    const unsigned long unarmed[] = { 1, 2, 3 };
    Reset(); target.m_Flags = 0; g_IsCarryingWeapon = false;
    iface->CGameScriptInterface::SheatheHeroWeapons();
    if (!Check(unarmed, 3) || g_ObservedTarget != &target) { std::printf("FSE2_008916a0_TEST FAIL 3\n"); return 3; }

    const unsigned long sheathe[] = { 1, 2, 3, 4, 5, 5, 6, 7, 8, 9 };
    Reset(); target.m_Flags = 2; g_IsCarryingWeapon = true;           // alive (only bit 0 is Dead)
    iface->CGameScriptInterface::SheatheHeroWeapons();
    if (!Check(sheathe, 10) || g_ObservedTarget != &target || g_ConstructCreature != &target || g_ConstructFollowUp != 0
        || g_ConstructPriority != 100 || g_SetAction != g_ConstructedAction || g_DestroyedAction != g_ConstructedAction) {
        std::printf("FSE2_008916a0_TEST FAIL 4\n"); return 4;
    }
    std::printf("FSE2_008916a0_TEST PASS\n");
    return 0;
}
