// CAIStateGroup_TavernServer::PickUpJug() -- collect the jug from the bar: free the bar point and fail when the jug
// is dead; otherwise run a CCreatureAction_PickUpJug ("COLLECT_JUG_FROM_BAR", mode 0x29) to completion with the
// creature unpushable, then free the bar point, restore pushability and report whether the action succeeded.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967930, length 232 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PJug +0x14, three times), FreeBarPoint @ 0x00967110
// (manifest files it under CAIStateGroup_TavernKeeper; it sits among the server bodies), CAIStateGroupBase::
// SetPushableByHero(bool) @ 0x0097E330, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
// CCreatureAction_PickUpThingAtTable ctor @ 0x008440C0 (owner, thing, const CCharString&, mode), the derived vtable
// 0x012963A4 (CCreatureAction_PickUpJug, rtti-vtables.tsv), CAIStateGroupBase::AIAction_PerformActionAndWait
// @ 0x0097DB70 (action, false; 2 = done), the action's pointer member dtor @ 0x00A01C10 (+0xa8) and the action base
// dtor @ 0x00693EF0 (manifest "CPhysicsMeshInfo::~CPhysicsMeshInfo" is propagated).
// Shape: the creature a named local read before the jug fetch; the action is a block-scoped local (0xb4 bytes) built by an inline derived ctor whose animation name comes
// from an inline by-value helper (string built in the one-word slot before the pushes, destroyed before the vtable
// store); the `!= 2` bool (`setne bl`) is declared outside the block so it survives the action's destructors; the
// two tails each repeat FreeBarPoint + SetPushableByHero(true); IS_THING_ALIVE tests the dead bit through an inline
// CThing::IsDead(), which gives retail's colouring (&PJug/jug in edi, creature in ebx). Engine header is padding only,
// so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CCharString { public:                            // 4 bytes: the shared string data pointer
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCreatureActionBase { public:                    // the action root (vptr + body up to the pointer member)
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};

class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:   // retail sizeof 0xb4
    CBaseIntelligentPointer PThing;             // +0xa8
    unsigned char _pad_0xb0[4];
    CCreatureAction_PickUpThingAtTable(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long mode);   // 0x008440C0
};

static inline CCharString PickUpJugAnimationName() { return CCharString("COLLECT_JUG_FROM_BAR", -1); }   // 0x012964BC

class CCreatureAction_PickUpJug : public CCreatureAction_PickUpThingAtTable { public:   // vtable 0x012963A4
    virtual void v00();
    CCreatureAction_PickUpJug(CThingCreatureBase* owner, CThing* jug)
        : CCreatureAction_PickUpThingAtTable(owner, jug, PickUpJugAnimationName(), 0x29) { }
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
    void SetPushableByHero(bool pushable);                              // 0x0097E330
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool interruptible);   // 0x0097DB70
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    void FreeBarPoint();                        // 0x00967110
    bool PickUpJug();
};

bool CAIStateGroup_TavernServer::PickUpJug()
{
    if (!IS_THING_ALIVE(PJug)) {
        FreeBarPoint();
        return false;
    }
    SetPushableByHero(false);
    bool failed;
    {
        CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
        CCreatureAction_PickUpJug action(creature, PJug.GetPItem());
        failed = AIAction_PerformActionAndWait(&action, false) != 2;
    }
    if (failed) {
        FreeBarPoint();
        SetPushableByHero(true);
        return false;
    }
    FreeBarPoint();
    SetPushableByHero(true);
    return true;
}

// ---- test harness ----
#include <stdio.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_free = 0, g_push[4], g_npush = 0, g_actions = 0, g_ptrDtors = 0, g_baseDtors = 0, g_liveStrings = 0;
static long g_result = 2; static CThing* g_actThing = 0; static void* g_actOwner = 0; static long g_actMode = 0; static char g_anim[64];
void CBaseIntelligentPointer::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ptrDtors; }
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; ++g_liveStrings; }
CCharString::~CCharString() { --g_liveStrings; }
void CCreatureActionBase::v00() { }
CCreatureActionBase::~CCreatureActionBase() { ++g_baseDtors; }
CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(CThingCreatureBase* owner, CThing* thing, const CCharString& animation, long mode)
{ g_actOwner = owner; g_actThing = thing; g_actMode = mode; strcpy(g_anim, (const char*)animation.m_pData); PThing.SetPItem(thing); }
void CCreatureAction_PickUpJug::v00() { }
CAIStateGroupBase::~CAIStateGroupBase() { }
void CAIStateGroupBase::SetPushableByHero(bool pushable) { g_push[g_npush++ & 3] = pushable; }
long CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool interruptible) { ++g_actions; return interruptible ? -1 : g_result; }
void CAIStateGroup_TavernServer::FreeBarPoint() { ++g_free; }

int main()
{
    static CAIStateGroup_TavernServer sg;
    static CAIBrain brain; static CThing creature, jug;
    memset(&creature, 0, sizeof(creature)); memset(&jug, 0, sizeof(jug));
    brain.PCreature = &creature; sg.PBrain = &brain;
    sg.PJug.SetPItem(0);
    CHECK(!sg.PickUpJug() && g_free == 1 && g_npush == 0 && g_actions == 0);
    sg.PJug.SetPItem(&jug); jug.m_flags91 = 0x81;
    CHECK(!sg.PickUpJug() && g_free == 2 && g_npush == 0);
    jug.m_flags91 = 0x80; g_result = 2;
    CHECK(sg.PickUpJug());
    CHECK(g_free == 3 && g_npush == 2 && g_push[0] == 0 && g_push[1] == 1 && g_actions == 1);
    CHECK(g_actOwner == &creature && g_actThing == &jug && g_actMode == 0x29 && strcmp(g_anim, "COLLECT_JUG_FROM_BAR") == 0);
    CHECK(g_liveStrings == 0 && g_ptrDtors == 1 && g_baseDtors == 1);
    g_result = 1;
    CHECK(!sg.PickUpJug());
    CHECK(g_free == 4 && g_npush == 4 && g_push[2] == 0 && g_push[3] == 1 && g_actions == 2 && g_ptrDtors == 2 && g_baseDtors == 2);
    if (fails == 0) printf("00967930_TEST PASS\n");
    return fails;
}