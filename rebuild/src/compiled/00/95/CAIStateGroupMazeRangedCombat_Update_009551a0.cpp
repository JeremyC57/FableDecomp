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