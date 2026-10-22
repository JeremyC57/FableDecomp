// Behaviour test for 00629c40: src/00629c40.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CWorld::EAControlledCreatureUseAbility(const CGameEvent*)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CWorld::EAControlledCreatureUseAbility(const CGameEvent*) -- game-event action: make the event player's hero use the ability from the event (main player only) and flash the GUI attack event
// Leaf: ?EAControlledCreatureUseAbility@CWorld@@QAEXPBVCGameEvent@@@Z
// Flags: /GS
// Shape: one CPlayer* local holds both GetPlayer results (the mode test, then the hero lookup), which gives retail's
//   callee-saved assignment (event in ebp, creature in edi).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CMemoryBuffer {
    unsigned char Data[0x20];                      // +0x00
    long          Length;                          // +0x20
    long          ReadPos;                         // +0x24
    long          WritePos;                        // +0x28
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data);   // 0x009F18F0
    __forceinline unsigned short ReadUShort() { unsigned short value; Read(2, &value); return value; }
    __forceinline float ReadFloat() { float value; Read(4, &value); return value; }
};
class CGameEvent {
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    long GetData(CMemoryBuffer& buffer) const;     // 0x009F17A0
};
struct CThingCreatureBase {
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    bool IsBlocking();                             // 0x00664D00
    void UseAbility(long ability, float charge);   // 0x00665510
};
enum EPlayerMode { PLAYER_MODE_0x2F = 0x2f };
struct CPlayer {
    bool HasMode(EPlayerMode mode);                // 0x00634160
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
};
struct CPlayerManager {
    long GetMainPlayerNumber();                    // 0x00449700
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CPlayerGui {
    void OnEventAttack();                          // 0x004379B0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
struct CWorld {
    unsigned char       _pad00[0xc];
    CPlayerManager*     PlayerManager;             // +0x0C
    CDefinitionManager* DefinitionManager;         // +0x10
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EAControlledCreatureUseAbility(const CGameEvent* event);
};
void CWorld::EAControlledCreatureUseAbility(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (player->HasMode(PLAYER_MODE_0x2F))
        return;
    player = GetPlayer(event->Player);
    CThingCreatureBase* creature = player->GetHeroCreature();
    if (!creature || creature->Dead || creature->IsBlocking())
        return;
    signed char playerNumber = event->Player;
    if (playerNumber != PlayerManager->GetMainPlayerNumber())
        return;
    CMemoryBuffer buffer;
    event->GetData(buffer);
    unsigned short ability = buffer.ReadUShort();
    float charge = buffer.ReadFloat();
    creature->UseAbility(ability, 0.0f);
    unsigned short attack = (unsigned short)DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK");
    if (attack != 0)
        g_pPlayerGui->OnEventAttack();
}

// ---- stubs and driver ----
static const char* lastName = 0;
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; lastName = text; }
CCharString::~CCharString() { }
void CMemoryBuffer::Read(unsigned long length, void* data) { memcpy(data, Data + ReadPos, length); ReadPos += length; }
static int getDataCalls = 0;
long CGameEvent::GetData(CMemoryBuffer& buffer) const
{
    ++getDataCalls;
    memcpy(buffer.Data, Data, 32); buffer.Length = EndPos; buffer.WritePos = EndPos;
    return EndPos;
}
static bool blocking = false;
static long usedAbility = -1; static float usedCharge = -1.0f; static int useCalls = 0;
bool CThingCreatureBase::IsBlocking() { return blocking; }
void CThingCreatureBase::UseAbility(long ability, float charge) { usedAbility = ability; usedCharge = charge; ++useCalls; }
static CThingCreatureBase heroStore;
static CThingCreatureBase* hero = &heroStore;
static bool inMode = false; static long modeAsked = -1;
static CPlayer players[4];
static long getPlayerArgs[4]; static int getPlayerCalls = 0;
bool CPlayer::HasMode(EPlayerMode mode) { modeAsked = mode; return inMode; }
CThingCreatureBase* CPlayer::GetHeroCreature() { return hero; }
static long mainPlayer = 1;
long CPlayerManager::GetMainPlayerNumber() { return mainPlayer; }
static long attackIndex = 7;
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name)
{
    return strcmp((const char*)name.PStringData, "CREATURE_ABILITY_ATTACK") == 0 ? attackIndex : -99;
}
static int attackEvents = 0;
void CPlayerGui::OnEventAttack() { ++attackEvents; }
static CPlayerGui guiStore;
CPlayerGui* g_pPlayerGui = &guiStore;
CPlayer* CWorld::GetPlayer(long number) { if (getPlayerCalls < 4) getPlayerArgs[getPlayerCalls] = number; ++getPlayerCalls; return &players[number & 3]; }
static CPlayerManager pmStore; static CDefinitionManager dmStore;
static void Reset()
{
    getPlayerCalls = 0; useCalls = 0; attackEvents = 0; getDataCalls = 0; usedAbility = -1; usedCharge = -1.0f;
    inMode = false; blocking = false; hero = &heroStore; heroStore.Dead = 0; mainPlayer = 1; attackIndex = 7; modeAsked = -1;
}
int main()
{
    CWorld world; memset(&world, 0, sizeof(world));
    world.PlayerManager = &pmStore; world.DefinitionManager = &dmStore;
    CGameEvent ev; memset(&ev, 0, sizeof(ev));
    ev.Player = 1; ev.EndPos = 6;
    unsigned short ab = 0x1234; float ch = 0.75f;
    memcpy(ev.Data, &ab, 2); memcpy(ev.Data + 2, &ch, 4);
    // normal path
    Reset();
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(getPlayerCalls == 2 && getPlayerArgs[0] == 1 && getPlayerArgs[1] == 1);
    CHECK(modeAsked == 0x2f);
    CHECK(getDataCalls == 1);
    CHECK(useCalls == 1 && usedAbility == 0x1234 && usedCharge == 0.0f);
    CHECK(attackEvents == 1);
    CHECK(lastName && strcmp(lastName, "CREATURE_ABILITY_ATTACK") == 0);
    // attack index 0 (low word) -> no GUI flash
    Reset(); attackIndex = 0x10000;
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(useCalls == 1 && attackEvents == 0);
    // in mode 0x2f -> nothing
    Reset(); inMode = true;
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(getPlayerCalls == 1 && useCalls == 0 && getDataCalls == 0);
    // no hero
    Reset(); hero = 0;
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(getPlayerCalls == 2 && useCalls == 0 && getDataCalls == 0);
    // dead hero
    Reset(); heroStore.Dead = 1;
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(useCalls == 0 && getDataCalls == 0);
    // blocking hero
    Reset(); blocking = true;
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(useCalls == 0 && getDataCalls == 0);
    // not the main player
    Reset(); mainPlayer = 0;
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(useCalls == 0 && getDataCalls == 0);
    // negative player number is sign-extended
    Reset(); ev.Player = -1; mainPlayer = -1;
    world.EAControlledCreatureUseAbility(&ev);
    CHECK(getPlayerArgs[0] == -1 && useCalls == 1);
    char buf[64];
    _snprintf(buf, sizeof(buf), "%s", fails == 0 ? "00629c40_TEST PASS" : "00629c40_TEST FAIL");
    printf("%s\n", buf);
    return fails;
}
