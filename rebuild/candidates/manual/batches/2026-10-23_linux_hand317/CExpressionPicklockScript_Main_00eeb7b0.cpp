// CExpression_PicklockScript::Main() -- the hero "picklock" expression quest: register a conscious-hero predicate, mark the quest persistent, take the hero's targeted thing and quit unless it is pick-lockable and no movie is running; otherwise open a movie sequence and the picklock mini-game bar, scale the def's picklock time by the hero's skill level/max and the frame rate into a frame count, post a keep-searching-for-witnesses deed, start sneaking and face the lock, then each frame fail on releasing the d-pad button, being hit or being witnessed, update the bar with 1 - frames/time, finish when the bar is full (fail if the lock vanished) and count down; afterwards remove the deed, hide the bar and, unless failed, unlock the thing with progress% chance and apply the morality change; finally deactivate this quest later
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EEB7B0, length 1272 bytes (+ int3 padding), no EH frame.
// Callees: CScriptThing copy ctor 0x004ABE90 / dtor 0x004AA840 / IsAlive 0x004AB130 (virtual slot 75, called directly here once and
//   expanded inline once), CScriptBase::SetPredicate 0x00CB7920 / IsActiveThreadTerminating 0x00CB7940, CBaseObject ctor 0x0099A380,
//   movie object dtor 0x006E7B80, CCharString 0x0099EBF0 / 0x0099EAE0, rand 0x00BFEB16, __ftol2 0x00BFEA70; CGameScriptInterface slots below.
//   Literals "" 0x0122D70E, "EXPRESSION_PICKLOCK" 0x0124F520, 1.0f 0x0122DED8, 100.0f 0x01230A08.
// Data: g_pScriptDef 0x0143E90C (as the landed HTGood/HTEvil boast rows); the picklock time float at +0x101C.
// Layout: the retail quest-script base (as the landed CExpressionWaitScript_Main 00eea460): CScriptBase 0x40 bytes, the interface pointer
//   at +0x40. CScriptThing is modelled with its virtual slots (MsgIsHitBy 21, IsAlive 75) because the shared header flattens the vfptr.
// Shape: the first lock-alive test is CScriptThing::IsAlive expanded inline (PImp_Data && PImp_Data->IsThingAlive(), the 0x004AB130 body);
//   the second is the direct call. The movie handle lives in an inner block so its dtor runs before the final DeactivateQuestLater.
#include <stdlib.h>
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
#define VSLOTS_THING VPAD16(t0) VPAD4(t1) VPAD1(t2) \
    virtual bool MsgIsHitBy(const CCharString& name) const; \
    VPAD32(t3) VPAD16(t4) VPAD4(t5) VPAD1(t6) \
    virtual bool IsAlive() const;
#define VSLOTS_IMPL VPAD64(i0) VPAD8(i1) VPAD2(i2) VPAD1(i3) \
    virtual bool IsThingAlive() const;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
};

struct CScriptThingImplementation;

struct CScriptThing {                                      // 12 bytes: vfptr + CCountedPointer PImp
    VSLOTS_THING
    CScriptThingImplementation* PImp_Data;                 // +0x04
    void* PImp_Info;                                       // +0x08
    CScriptThing(const CScriptThing& other);               // 0x004ABE90
    ~CScriptThing();                                       // 0x004AA840
};

struct CScriptThingImplementation {
    VSLOTS_IMPL
};

__forceinline bool IsThingHandleAlive(const CScriptThing& thing)
{
    return thing.PImp_Data != 0 && thing.PImp_Data->IsThingAlive();
}

struct CIsThingConscious {                                 // vtable 0x012C2FE8
    CScriptThing Thing;                                    // +0x04
    CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingConscious() { }
    virtual bool operator()() const;
};

struct CBaseObject {
    void* IntelligentPointer;                              // +0x04
    CBaseObject();                                         // 0x0099A380
    virtual ~CBaseObject();                                // 0x0099A430
};

struct CScriptGameResourceObjectBase : CBaseObject {       // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() { }
};

struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4
    CCountedPointerWords PImp;                             // +0x08
    CScriptGameResourceObjectMovieBase() { }
    ~CScriptGameResourceObjectMovieBase();                 // 0x006E7B80
    virtual void v01();
};

struct CScriptDef {
    unsigned char _pad[0x101c];
    float PicklockTime;                                    // +0x101C
};
extern CScriptDef* g_pScriptDef;                           // 0x0143E90C

enum EHeroTrainableStatType { HERO_STAT_SKILL = 5 };
enum EMiniGameType { MINI_GAME_PICKLOCK = 4 };
enum EOpinionDeedType { OPINION_DEED_PICKLOCK = 4 };

struct CGameScriptInterface {
    VPAD4(p0_0) VPAD2(p0_1) VPAD1(p0_2)
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    VPAD32(p8_0) VPAD16(p8_1) VPAD8(p8_2) VPAD4(p8_3) VPAD2(p8_4)
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    virtual CScriptThing GetHeroTargetedThing() const;   // slot 71 (+0x11c)
    VPAD64(p72_0) VPAD16(p72_1) VPAD8(p72_2)
    virtual void ChangeHeroMoralityDueToPicklock() const;   // slot 160 (+0x280)
    VPAD4(p161_0) VPAD2(p161_1) VPAD1(p161_2)
    virtual long GetHeroStatLevel(EHeroTrainableStatType stat) const;   // slot 168 (+0x2a0)
    virtual long GetHeroStatMax(EHeroTrainableStatType stat) const;   // slot 169 (+0x2a4)
    VPAD32(p170_0) VPAD16(p170_1) VPAD8(p170_2) VPAD2(p170_3)
    virtual bool IsDPadButtonHeldForExpression(const CCharString& name) const;   // slot 228 (+0x390)
    VPAD32(p229_0) VPAD16(p229_1) VPAD4(p229_2)
    virtual void DeactivateQuestLater(const CCharString& name, unsigned long flags) const;   // slot 281 (+0x464)
    VPAD8(p282_0)
    virtual void SetQuestAsPersistent(const CCharString& name, bool persistent);   // slot 290 (+0x488)
    VPAD32(p291_0) VPAD16(p291_1) VPAD2(p291_2) VPAD1(p291_3)
    virtual void DisplayMiniGameInfo(bool show, EMiniGameType type) const;   // slot 342 (+0x558)
    virtual void UpdateMiniGameInfoBar(float value) const;   // slot 343 (+0x55c)
    VPAD1(p344_0)
    virtual bool IsEntityPickLockable(const CScriptThing& thing) const;   // slot 345 (+0x564)
    VPAD2(p346_0)
    virtual void EntitySetAsPickLocked(const CScriptThing& thing) const;   // slot 348 (+0x570)
    VPAD16(p349_0) VPAD4(p349_1) VPAD1(p349_2)
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    VPAD16(p371_0) VPAD8(p371_1)
    virtual bool IsInMovieSequence() const;   // slot 395 (+0x62c)
    VPAD64(p396_0) VPAD8(p396_1) VPAD4(p396_2) VPAD2(p396_3) VPAD1(p396_4)
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& thing, const CScriptThing& target, bool instant);   // slot 475 (+0x76c)
    VPAD128(p476_0) VPAD16(p476_1) VPAD8(p476_2) VPAD1(p476_3)
    virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing& thing, EOpinionDeedType deed, const CScriptThing& target) const;   // slot 629 (+0x9d4)
    virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing& thing, long deed) const;   // slot 630 (+0x9d8)
    virtual bool IsDeedWitnessed(long deed) const;   // slot 631 (+0x9dc)
    VPAD16(p632_0) VPAD4(p632_1) VPAD2(p632_2)
    virtual long GetConstantFPS() const;   // slot 654 (+0xa38)
    virtual CCharString GetActiveQuestName() const;   // slot 655 (+0xa3c)
    VPAD64(p656_0) VPAD16(p656_1) VPAD2(p656_2) VPAD1(p656_3)
    virtual void StartSneaking() const;   // slot 739 (+0xb8c)
};

struct CScriptBase {                                       // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    void SetPredicate(CIsThingConscious& predicate);       // 0x00CB7920
    bool IsActiveThreadTerminating();                      // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* GSI;                             // +0x40
};

struct CExpression_PicklockScript : CQuestScriptBase {
    void Main();
};

void CExpression_PicklockScript::Main()
{
    {
        CIsThingConscious predicate(GSI->GetHero());
        SetPredicate(predicate);
    }
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->SetQuestAsPersistent(GSI->GetActiveQuestName(), false);
    CScriptThing chest = GSI->GetHeroTargetedThing();
    if (!GSI->IsEntityPickLockable(chest)) {
        if (IsActiveThreadTerminating())
            return;
        GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
        return;
    }
    if (GSI->IsInMovieSequence()) {
        if (IsActiveThreadTerminating())
            return;
        GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
        return;
    }
    {
        CScriptGameResourceObjectMovieBase movie;
        GSI->StartMovieSequence("", movie);
        GSI->DisplayMiniGameInfo(true, MINI_GAME_PICKLOCK);
        bool done = false;
        bool failed = false;
        float time = g_pScriptDef->PicklockTime;
        time /= (GSI->GetHeroStatLevel(HERO_STAT_SKILL) + 1.0f) / (GSI->GetHeroStatMax(HERO_STAT_SKILL) + 1.0f);
        time *= GSI->GetConstantFPS();
        long count = (long)time;
        long deed = GSI->EntityPostOpinionDeedKeepSearchingForWitnesses(GSI->GetHero(), OPINION_DEED_PICKLOCK, chest);
        GSI->StartSneaking();
        GSI->EntitySetFacingAngleTowardsThing(GSI->GetHero(), chest, true);
        float progress;
        while (!done) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (!GSI->IsDPadButtonHeldForExpression("EXPRESSION_PICKLOCK")) {
                if (IsActiveThreadTerminating())
                    return;
                done = true;
                failed = true;
            }
            if (GSI->GetHero().MsgIsHitBy("")) {
                if (IsActiveThreadTerminating())
                    return;
                done = true;
                failed = true;
            }
            if (GSI->IsDeedWitnessed(deed)) {
                if (IsActiveThreadTerminating())
                    return;
                done = true;
                failed = true;
            }
            progress = 1.0f - count / time;
            if (progress > 1.0f) {
                if (IsActiveThreadTerminating())
                    return;
                progress = 1.0f;
            }
            GSI->UpdateMiniGameInfoBar(progress);
            if (IsThingHandleAlive(chest)) {
                if (IsActiveThreadTerminating())
                    return;
                if (progress >= 1.0f) {
                    if (IsActiveThreadTerminating())
                        return;
                    done = true;
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                failed = true;
                done = true;
            }
            if (count > 0) {
                if (IsActiveThreadTerminating())
                    return;
                --count;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->RemoveOpinionDeedStillSearchingForWitnesses(GSI->GetHero(), deed);
        GSI->DisplayMiniGameInfo(false, MINI_GAME_PICKLOCK);
        if (!failed) {
            if (IsActiveThreadTerminating())
                return;
            if (chest.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                if (rand() % 100 < progress * 100.0f) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->EntitySetAsPickLocked(chest);
                    GSI->ChangeHeroMoralityDueToPicklock();
                }
            }
        }
    }
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
}
