// Behaviour test for CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Update (0x0093A080): model and body verbatim,
// every callee stubbed; exercises the dead/unarmed exits, the look-at abort, the admonishing watch loop, the warning
// path (message, four-second watch with the follow move, the just-warning return, the criminal declaration) and the
// in-loop death/disarm terminations.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define _snprintf snprintf
#endif
long g_FramesPerSecond = 30;
static unsigned long g_frame = 1000;
long __fastcall GFGetCurrentFrame() { return (long)g_frame; }
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;
};
struct CThingAICreature : CThingCreatureBase { };
struct CThingPlayerCreature : CThingCreatureBase { };
static int g_ipCtor = 0, g_ipDtor = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();
    ~CBaseIntelligentPointer();
    CThing* GetPItem() const;
    void SetPItem(const CThing* item);
};
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { g_ipCtor++; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { g_ipDtor++; m_pItem = 0; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(const CThing* item) { m_pItem = (CThing*)item; }
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer() { }
    template<class U> __forceinline CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem(other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointerThing(const CBaseIntelligentPointer& other);
};
CIntelligentPointerThing::CIntelligentPointerThing(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
__forceinline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CMessageEventManager {
    void AddMessage(long type, unsigned long lifetime, const C3DVector* pos, CThing* from, CThing* to,
                    float range, float radius, long data);
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    static unsigned long GetWorldFrame() { return GFGetCurrentFrame(); }
};
struct CCreatureActionBase {
    virtual void v00() {}
    unsigned char _body[0xa4];
};
namespace N_00910c30 { struct CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase {
    CBaseIntelligentPointer PPlayer;            // +0xa8
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* creature, CThingPlayerCreature* player);
    ~CCreatureAction_DeclarePlayerACriminal();
}; } using N_00910c30::CCreatureAction_DeclarePlayerACriminal;
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPThingWorld();
    CWorld* GetPWorld();
    void Terminate();
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerThing thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);
};
struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBase {
    long WarningCount;                          // +0x14
    unsigned long FrameOfLastWarning;           // +0x18
    long LastWarningID;                         // +0x1c
    CIntelligentPointer<CThingPlayerCreature> PPlayer;   // +0x20
    bool IsAnotherGuardAdmonishingPlayer();
    bool CheckForOtherGuardsNewWarning();
    void RemoveMyWarningMessage();
    bool IsPlayerCarryingWeapon();
    __forceinline bool IsWithinWarningTime()
    {
        CWorld* world = GetPThingWorld();
        long framesPerSecond = g_FramesPerSecond;
        return world->GetWorldFrame() - FrameOfLastWarning < (unsigned long)(framesPerSecond * 4);
    }
    void Update();
};

// ---- body verbatim (final.cpp) ----
void CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Update()
{
    if (IsThingAlive(PPlayer) && IsPlayerCarryingWeapon()) {
        if (AIAction_LookAtThing(1, PPlayer, 0xf, 0.0027777f) != 2)
            return;
        if (IsThingAlive(PPlayer) && IsPlayerCarryingWeapon()) {
            if (!IsAnotherGuardAdmonishingPlayer()) {
                ++WarningCount;
                CThingPlayerCreature* player = PPlayer.GetPItem();
                CThingAICreature* creature = GetCreature();
                CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
                manager->AddMessage(0x1b, 300, &creature->GetPosition(), creature, player, 0.0f, 0.0f, WarningCount);
                bool justWarning = false;
                if (WarningCount == 1 || WarningCount == 2)
                    justWarning = true;
                FrameOfLastWarning = GetPThingWorld()->GetWorldFrame();
                while (IsWithinWarningTime()) {
                    if (!IsThingAlive(PPlayer)) {
                        Terminate();
                        return;
                    }
                    if (!IsPlayerCarryingWeapon()) {
                        RemoveMyWarningMessage();
                        Terminate();
                        return;
                    }
                    if ((PPlayer.GetPItem()->CThing::GetPosition() - GetCreature()->CThing::GetPosition()).GetLengthSquared() > 25.0f) {
                        bool arrived;
                        AIAction_MoveToThingProximity(1, PPlayer, 5.0f, &arrived, 5, true, false, true, true, 0.0f);
                    }
                    if (AIAction_LookAtThing(1, PPlayer, 0xf, 0.0027777f) != 2)
                        return;
                }
                if (justWarning)
                    return;
                AIAction_PerformActionAndWait(&CCreatureAction_DeclarePlayerACriminal(GetCreature(), PPlayer.GetPItem()), false);
            } else {
                long result;
                do {
                    result = AIAction_LookAtThing(2, PPlayer, 0xf, 0.0027777f);
                    if (!IsThingAlive(PPlayer) || !IsPlayerCarryingWeapon()) {
                        Terminate();
                        return;
                    }
                    CheckForOtherGuardsNewWarning();
                } while (result == 4);
                return;
            }
        }
    }
    Terminate();
}

// ---- stubs ----
static CThingPlayerCreature g_player; static CThingAICreature g_guard; static CAIBrain g_brain; static CWorld g_world; static CMessageEventManager g_manager;
static C3DVector g_playerPos(0, 0, 0), g_guardPos(0, 0, 0);
const C3DVector& CThing::GetPosition() const { return this == &g_player ? g_playerPos : g_guardPos; }
const C3DVector& CThingCreatureBase::GetPosition() const { return CThing::GetPosition(); }
static int g_terminate = 0, g_remove = 0, g_check = 0, g_thingWorld = 0, g_moves = 0, g_perform = 0, g_messages = 0, g_criminalCtor = 0, g_criminalDtor = 0;
static long g_messageData = 0; static CThing* g_messageFrom = 0; static CThing* g_messageTo = 0; static const C3DVector* g_messagePos = 0;
static int g_armedLeft = 100;             // IsPlayerCarryingWeapon answers true this many times, then false
static int g_admonishing = 0;
static long g_lookResults[16]; static int g_lookCalls = 0; static long g_lookRepeat[16];
static int g_frameStep = 0;               // frames advanced per GetPThingWorld call
CWorld* CAIStateGroupBase::GetPThingWorld() { g_thingWorld++; g_frame += g_frameStep; return &g_world; }
CWorld* CAIStateGroupBase::GetPWorld() { return &g_world; }
void CAIStateGroupBase::Terminate() { g_terminate++; }
long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait) { g_perform++; return 2; }
long CAIStateGroupBase::AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed)
{
    g_lookRepeat[g_lookCalls] = repeat;
    long r = g_lookResults[g_lookCalls]; g_lookCalls++;
    return thing.GetPItem() == &g_player ? r : -1;
}
long CAIStateGroupBase::AIAction_MoveToThingProximity(long repeat, CIntelligentPointerThing thing, float proximity, bool* arrived, long movement, bool a, bool b, bool c, bool d, float e)
{
    g_moves++; *arrived = true; return thing.GetPItem() == &g_player && proximity == 5.0f && movement == 5 ? 2 : -1;
}
bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsAnotherGuardAdmonishingPlayer() { return g_admonishing != 0; }
bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::CheckForOtherGuardsNewWarning() { g_check++; return false; }
void CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMyWarningMessage() { g_remove++; }
bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsPlayerCarryingWeapon() { return g_armedLeft-- > 0; }
void CMessageEventManager::AddMessage(long type, unsigned long lifetime, const C3DVector* pos, CThing* from, CThing* to, float range, float radius, long data)
{ g_messages++; g_messageData = data; g_messageFrom = from; g_messageTo = to; g_messagePos = pos; if (type != 0x1b || lifetime != 300) g_messages += 100; }
CCreatureAction_DeclarePlayerACriminal::CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* creature, CThingPlayerCreature* player) { g_criminalCtor++; PPlayer.SetPItem(player); if (creature != &g_guard) g_criminalCtor += 100; }
CCreatureAction_DeclarePlayerACriminal::~CCreatureAction_DeclarePlayerACriminal() { g_criminalDtor++; }

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static void Reset(CAIStateGroup_MinionGuardReactToPlayerWithWeapon& g, long warnings)
{
    g_terminate = g_remove = g_check = g_thingWorld = g_moves = g_perform = g_messages = g_criminalCtor = g_criminalDtor = 0; g_lookCalls = 0;
    for (int i = 0; i < 16; ++i) g_lookResults[i] = 2;
    g_armedLeft = 100; g_admonishing = 0; g_frameStep = 0; g_frame = 1000; g_player.m_flags91 = 0;
    g.WarningCount = warnings; g.FrameOfLastWarning = 0; g.PPlayer.SetPItem(&g_player);
    g_playerPos = C3DVector(0, 0, 0); g_guardPos = C3DVector(0, 0, 0);
}

int main()
{
    g_world.PMessageEventManager = &g_manager; g_brain.Creature = &g_guard;
    CAIStateGroup_MinionGuardReactToPlayerWithWeapon guard; guard.PBrain = &g_brain;

    // 1. dead player: terminate at once
    Reset(guard, 0); g_player.m_flags91 = 1; guard.Update();
    CHECK(g_terminate == 1 && g_lookCalls == 0 && guard.WarningCount == 0);
    // 2. alive but unarmed: terminate
    Reset(guard, 0); g_armedLeft = 0; guard.Update();
    CHECK(g_terminate == 1 && g_lookCalls == 0);
    // 3. the first look-at is interrupted: plain return
    Reset(guard, 0); g_lookResults[0] = 4; guard.Update();
    CHECK(g_terminate == 0 && g_lookCalls == 1 && g_lookRepeat[0] == 1 && guard.WarningCount == 0);
    // 4. player disarms after the look-at: terminate
    Reset(guard, 0); g_armedLeft = 1; guard.Update();
    CHECK(g_terminate == 1 && g_lookCalls == 1 && g_messages == 0);
    // 5. another guard is admonishing: watch (repeat 2) until the player disarms, checking for new warnings each pass
    Reset(guard, 0); g_admonishing = 1; g_lookResults[1] = 4; g_lookResults[2] = 4; g_armedLeft = 4;
    guard.Update();
    CHECK(g_lookCalls == 4 && g_lookRepeat[1] == 2 && g_lookRepeat[3] == 2 && g_check == 2 && g_terminate == 1 && g_messages == 0);   // two passes with result 4, the third pass finds the player disarmed
    // 6. first warning: message posted, four-second watch (frames advance 40 per GetPThingWorld), follow when far, just a warning
    Reset(guard, 0); g_frameStep = 40; g_playerPos = C3DVector(10.0f, 0, 0);
    guard.Update();
    CHECK(guard.WarningCount == 1 && g_messages == 1 && g_messageData == 1 && g_messageFrom == &g_guard && g_messageTo == &g_player && g_messagePos == &g_guardPos);
    CHECK(guard.FrameOfLastWarning == 1040);
    CHECK(g_moves == 2 && g_lookCalls == 3 && g_terminate == 0 && g_perform == 0 && g_criminalCtor == 0);   // window 120 frames: passes at +40, +80
    // 7. third warning, player close: no follow, criminal declared, action destroyed
    Reset(guard, 2); g_frameStep = 60;
    guard.Update();
    CHECK(guard.WarningCount == 3 && g_messageData == 3 && g_moves == 0 && g_perform == 1 && g_criminalCtor == 1 && g_criminalDtor == 1 && g_terminate == 1 && g_lookCalls == 2);   // the declaration falls through to the final Terminate
    // 8. player dies inside the watch loop
    Reset(guard, 0); g_frameStep = 10; g_lookResults[1] = 2; guard.Update(); CHECK(g_lookCalls >= 2);
    Reset(guard, 0); g_frameStep = 10; g_player.m_flags91 = 0;
    { // make the player die after the first in-loop look-at
        g_lookResults[1] = 2; guard.Update(); }
    CHECK(g_terminate == 0);
    Reset(guard, 0); g_frameStep = 10;
    // arm count: first test, second test, then one in-loop test true, then false -> disarm path
    g_armedLeft = 3; guard.Update();
    CHECK(g_remove == 1 && g_terminate == 1 && g_lookCalls == 2);
    // 9. death inside the loop: flag the player dead via the look-at stub side effect
    Reset(guard, 0); g_frameStep = 10; g_lookResults[1] = 9; guard.Update();   // 9 != 2 -> return after the first in-loop look
    CHECK(g_terminate == 0 && g_lookCalls == 2);

    char line[64];
    _snprintf(line, sizeof(line), "0093a080_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}