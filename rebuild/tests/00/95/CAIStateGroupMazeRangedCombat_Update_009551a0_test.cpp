#include <assert.h>
#include <string.h>
#include <stdio.h>
#ifndef __fastcall
#define __fastcall
#endif
// CAIStateGroup_MazeRangedCombat::Update @ 0x009551A0, 280 retail bytes.
// Attempt a shot, play SPECIAL_IDLE on success, retry while eligible, and
// terminate when the action sequence ends.
#ifdef _MSC_VER
#include "fable_string.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MazeRangedCombat.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_ThrowShotObject.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#else
// Host-test adapters: VC7.1 compiles against the shared donor headers above;
// the retail offsets below are explicit so the same body can run on a 64-bit host.
class CAIBrain { public: unsigned char pad[0x18]; void* Creature; };
class CAIStateGroupBase { public:
    void* __vftable;
    CAIBrain* PBrain;
    char _tail[12];
};
struct CAIStateGroup_MazeRangedCombat { unsigned char raw[0x24]; };
class CCreatureActionBase { public: };
class CCreatureAction_ThrowShotObject { public: };
class CCreatureAction_PlayCombatAnimation { public: };
#endif

class CThing;
class CThingCreatureBase;
#ifndef _MSC_VER
class CCharString { public:
    void* data;
    CCharString(const char*, long);
    ~CCharString();
};
#endif
class CSoundPair { public:
    long first, last;
    CSoundPair() : first(0), last(0) {}
};
CSoundPair __fastcall GetSoundPair(CThingCreatureBase*, const CCharString&); // 0x008561A0

struct CCharStringLiteral {
    CCharString value;
    CCharStringLiteral(const char* text) : value(text, -1) {} // CCharString ctor/dtor 0x0099EBF0/0x0099EAE0
};

// Retail CCreatureActionBase is 0xa8 (the donor header says 0xb4); retail
// CCombatActionBase is 0x114 (the donor layout is 0x12c). Neighboring landed
// constructors 009566c0 and 008b7ff0 establish those offsets and lifetimes.
struct CCreatureActionBaseRetail {
    virtual ~CCreatureActionBaseRetail(); // 0x00693EF0
    unsigned char body[0xa4];
};
struct CCombatActionBaseRetail : CCreatureActionBaseRetail {
    unsigned char combat[0x6c];
    CCombatActionBaseRetail(CThingCreatureBase*, CThing*, long, const CSoundPair&); // 0x00857F30
    CCombatActionBaseRetail(CThingCreatureBase*, long, const CSoundPair&); // 0x00858030
    virtual ~CCombatActionBaseRetail(); // 0x0062E900
    void SetAnimationName(const CCharString&);
};
// CCreatureAction_ThrowShotObject ctor: 0x009566C0.
struct CCreatureAction_ThrowShotObjectRetail : CCombatActionBaseRetail {
    CCreatureAction_ThrowShotObjectRetail(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBaseRetail(owner, thing, 150,
            GetSoundPair(owner, CCharStringLiteral("SWORD_SWING").value)) {}
};
struct CCreatureAction_PlayCombatAnimationRetail : CCombatActionBaseRetail {
    unsigned char loop;
    unsigned char flag;
    unsigned char started;
    CCreatureAction_PlayCombatAnimationRetail(CThingCreatureBase*, const CCharString&, bool); // 0x008B7FF0
};

struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer(); // 0x00A01C10
    CThing* item;
    CBaseIntelligentPointer(); // 0x00A01B10
    CThing* GetPItem() const; // 0x00A01B50
};
// The donor Maze record is flattened/nonpolymorphic. Model the retail base
// and observed members without adding a second vptr.
struct MazeRangedMethods : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget; // +0x14 (retail)
#ifdef _MSC_VER
    unsigned char _pad_0x1c[4];
#endif
    long LastCompletedPhase; // +0x20 (retail)
    CAIBrain* Brain() const { return PBrain; }
    CBaseIntelligentPointer& Target() { return PTarget; }
    bool CanRun(); // 0x00954220
    long AIAction_PerformActionAndWait(CCreatureActionBaseRetail*, bool); // 0x0097DB70
    void Terminate(); // 0x008FCF40
    void Update();
};

void MazeRangedMethods::Update() {
    long result = 2;
    do {
        if (CanRun()) {
            CThingCreatureBase* creature = (CThingCreatureBase*)Brain()->Creature;
            result = AIAction_PerformActionAndWait(
                &CCreatureAction_ThrowShotObjectRetail(creature, Target().GetPItem()), false);
        } else {
            Terminate();
        }
        if (CanRun() && result == 2) {
            CCharString animation("SPECIAL_IDLE", -1);
            CThingCreatureBase* creature = (CThingCreatureBase*)Brain()->Creature;
            result = AIAction_PerformActionAndWait(
                &CCreatureAction_PlayCombatAnimationRetail(creature, animation, true), false);
        } else {
            Terminate();
        }
    } while (CanRun() && result == 2);
    Terminate();
}

class CThing { public: };
class CThingCreatureBase { public: };

static const int kCapacity = 64;
static bool gCanRunValues[kCapacity];
static int gCanRunCount;
static int gCanRunIndex;
static long gActionResults[kCapacity];
static int gActionResultCount;
static int gActionIndex;
static int gConstructedCount;
static int gTerminateCount;
static int gActionCodes[kCapacity];
static bool gWaitValues[kCapacity];
static CThingCreatureBase* gActionOwners[kCapacity];
static CThing* gActionTargets[kCapacity];
static CThingCreatureBase* gExpectedOwners[kCapacity];
static CThing* gExpectedTarget;
static CThingCreatureBase* gExpectedSoundOwners[kCapacity];
static int gSoundCalls;
static int gStringCtorCount;
static int gStringDtorCount;
static int gCombatDtorCount;
static int gAnimationCtorCount;
static int gAnimationNameSetCount;
static CAIBrain* gBrain;
static CThingCreatureBase* gReplacementOwner;
static int gMutateAfterAction;

static bool isName(const CCharString& s, const char* expected) {
#ifdef _MSC_VER
    return strcmp((const char*)s, expected) == 0;
#else
    return strcmp((const char*)s.data, expected) == 0;
#endif
}

static void setBrainCreature(CAIBrain* brain, CThingCreatureBase* creature) {
#ifdef _MSC_VER
    brain->Creature = (CThingAICreature*)creature;
#else
    brain->Creature = creature;
#endif
}

static CThingCreatureBase* getBrainCreature(CAIBrain* brain) {
    return (CThingCreatureBase*)brain->Creature;
}

CCharString::CCharString(const char* text, long) {
#ifdef _MSC_VER
    *(void**)this = (void*)text;
#else
    data = (void*)text;
#endif
    ++gStringCtorCount;
}
CCharString::~CCharString() { ++gStringDtorCount; }
#ifdef _MSC_VER
CCharString::operator const char*() const { return (const char*)*(void* const*)this; }
#endif

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name) {
    assert(isName(name, "SWORD_SWING"));
    assert(gSoundCalls < kCapacity);
    assert(gSoundCalls < gActionResultCount);
    assert(owner == gExpectedSoundOwners[gSoundCalls]);
    ++gSoundCalls;
    CSoundPair result;
    result.first = 11;
    result.last = 12;
    return result;
}

CCreatureActionBaseRetail::~CCreatureActionBaseRetail() { }
CCombatActionBaseRetail::CCombatActionBaseRetail(
    CThingCreatureBase* owner, CThing* target, long code, const CSoundPair& sound) {
    assert(code == 150 && sound.first == 11 && sound.last == 12);
    assert(gConstructedCount < kCapacity);
    assert(owner == gExpectedOwners[gConstructedCount]);
    assert(target == gExpectedTarget);
    gActionCodes[gConstructedCount] = (int)code;
    gActionOwners[gConstructedCount] = owner;
    gActionTargets[gConstructedCount] = target;
    ++gConstructedCount;
}
CCombatActionBaseRetail::CCombatActionBaseRetail(
    CThingCreatureBase* owner, long code, const CSoundPair& sound) {
    assert(code == 50 && sound.first == 0 && sound.last == 0);
    assert(gConstructedCount < kCapacity);
    assert(owner == gExpectedOwners[gConstructedCount]);
    gActionCodes[gConstructedCount] = (int)code;
    gActionOwners[gConstructedCount] = owner;
    gActionTargets[gConstructedCount] = 0;
    ++gConstructedCount;
}
CCombatActionBaseRetail::~CCombatActionBaseRetail() { ++gCombatDtorCount; }
void CCombatActionBaseRetail::SetAnimationName(const CCharString& name) {
    assert(isName(name, "SPECIAL_IDLE"));
    ++gAnimationNameSetCount;
}
CCreatureAction_PlayCombatAnimationRetail::CCreatureAction_PlayCombatAnimationRetail(
    CThingCreatureBase* owner, const CCharString& name, bool flag)
    : CCombatActionBaseRetail(owner, 50, CSoundPair()), loop(0), flag(flag), started(0) {
    assert(isName(name, "SPECIAL_IDLE") && flag);
    ++gAnimationCtorCount;
    SetAnimationName(name);
}

CBaseIntelligentPointer::CBaseIntelligentPointer() : item(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return item; }

bool MazeRangedMethods::CanRun() {
    assert(gCanRunIndex < gCanRunCount);
    // Every prior full expression has released its string/action temporaries.
    assert(gStringCtorCount == gStringDtorCount);
    assert(gCombatDtorCount == gActionIndex);
    return gCanRunValues[gCanRunIndex++];
}

long MazeRangedMethods::AIAction_PerformActionAndWait(
    CCreatureActionBaseRetail* action, bool wait) {
    assert(gActionIndex < gActionResultCount);
    assert(gConstructedCount == gActionIndex + 1);
    assert(gCombatDtorCount == gActionIndex);
    assert(gActionOwners[gActionIndex] == gExpectedOwners[gActionIndex]);
    assert(gWaitValues[gActionIndex] == false);
    assert(wait == false);
    if (gActionCodes[gActionIndex] == 150) {
        assert(gActionIndex % 2 == 0);
        assert(gActionTargets[gActionIndex] == gExpectedTarget);
        // The SWORD_SWING temporary is already gone before the action callback.
        assert(gStringCtorCount == gStringDtorCount);
    } else {
        CCreatureAction_PlayCombatAnimationRetail* idle =
            (CCreatureAction_PlayCombatAnimationRetail*)action;
        assert(gActionCodes[gActionIndex] == 50);
        assert(gActionIndex % 2 == 1);
        assert(gActionTargets[gActionIndex] == 0);
        assert(idle->flag == 1);
        // The animation name remains alive through the callback and action dtor.
        assert(gStringCtorCount == gStringDtorCount + 1);
    }
    long result = gActionResults[gActionIndex];
    if (gActionIndex == gMutateAfterAction) {
        setBrainCreature(gBrain, gReplacementOwner);
    }
    ++gActionIndex;
    return result;
}

void MazeRangedMethods::Terminate() {
    // Termination must observe all action and string temporaries cleaned up.
    assert(gStringCtorCount == gStringDtorCount);
    assert(gCombatDtorCount == gActionIndex);
    ++gTerminateCount;
}

static void runScenario(
    const bool* gates, int gateCount,
    const long* results, int resultCount,
    int expectedTerminates, bool mutateOwner) {
    assert(gateCount <= kCapacity && resultCount <= kCapacity);
    gCanRunCount = gateCount;
    gCanRunIndex = 0;
    gActionResultCount = resultCount;
    gActionIndex = 0;
    gConstructedCount = 0;
    gTerminateCount = 0;
    gSoundCalls = 0;
    gStringCtorCount = 0;
    gStringDtorCount = 0;
    gCombatDtorCount = 0;
    gAnimationCtorCount = 0;
    gAnimationNameSetCount = 0;
    gMutateAfterAction = mutateOwner ? 0 : -1;
    int i;
    for (i = 0; i != kCapacity; ++i) {
        gCanRunValues[i] = false;
        gActionResults[i] = 0;
        gActionCodes[i] = 0;
        gWaitValues[i] = false;
        gActionOwners[i] = 0;
        gActionTargets[i] = 0;
        gExpectedOwners[i] = 0;
        gExpectedSoundOwners[i] = 0;
    }
    for (i = 0; i != gateCount; ++i) gCanRunValues[i] = gates[i];
    for (i = 0; i != resultCount; ++i) {
        gActionResults[i] = results[i];
        gExpectedOwners[i] = (mutateOwner && i > 0)
            ? gReplacementOwner : getBrainCreature(gBrain);
    }
    for (i = 0; i != resultCount; ++i) {
        if ((i % 2) == 0) gExpectedSoundOwners[gSoundCalls++] = gExpectedOwners[i];
    }
    gSoundCalls = 0;
    MazeRangedMethods state;
    state.PBrain = gBrain;
    state.PTarget.item = gExpectedTarget;
    state.LastCompletedPhase = 0;
    state.MazeRangedMethods::Update();
    assert(gCanRunIndex == gateCount);
    assert(gActionIndex == resultCount);
    assert(gConstructedCount == resultCount);
    assert(gTerminateCount == expectedTerminates);
    assert(gCombatDtorCount == resultCount);
    assert(gStringCtorCount == resultCount && gStringDtorCount == resultCount);
    assert(gSoundCalls == (resultCount + 1) / 2);
    assert(gAnimationCtorCount == resultCount / 2);
    assert(gAnimationNameSetCount == resultCount / 2);
}

int main() {
    CAIBrain brain;
    memset(&brain, 0, sizeof(brain));
    CThingCreatureBase initialOwner;
    CThingCreatureBase replacementOwner;
    CThing targetThing;
    setBrainCreature(&brain, &initialOwner);
    gBrain = &brain;
    gReplacementOwner = &replacementOwner;
    gExpectedTarget = &targetThing;

    // Initial eligibility fails, then the idle gate and loop gate fail.
    {
        const bool gates[] = { false, false, false };
        runScenario(gates, 3, 0, 0, 3, false);
    }
    // A shot succeeds, but the second gate blocks the idle action.
    {
        const bool gates[] = { true, false, false };
        const long results[] = { 2 };
        runScenario(gates, 3, results, 1, 2, false);
    }
    // The shot result skips idle despite a true second gate.
    {
        const bool gates[] = { true, true, false };
        const long results[] = { 1 };
        runScenario(gates, 3, results, 1, 2, false);
    }
    // Idle returns a non-success result; the loop guard is still evaluated.
    {
        const bool gates[] = { true, true, true };
        const long results[] = { 2, 1 };
        runScenario(gates, 3, results, 2, 1, false);
    }
    // Two complete shot+idle iterations; callback one changes Brain->Creature.
    {
        const bool gates[] = { true, true, true, true, true, true };
        const long results[] = { 2, 2, 2, 1 };
        runScenario(gates, 6, results, 4, 1, true);
    }
    puts("009551a0 behavior PASS");
    return 0;
}