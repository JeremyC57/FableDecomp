// CAIStateGroup_JackDragon::UpdateHover() -- hover phase of the Jack-dragon boss: push a CModeDragonHover at the enemy, turn to face the GoLeft-side hover marker, find the DRAGON_MOUTH_DUMMY, then for up to CJackDragonDef::MaxHoverLoops rounds (stopping when the enemy rises 1.5 above the landing marker) fire a beam into the enemy behind a 60-second physical obstruction when within MaxFirebeamDistance, else a fire blast (forward when GoLeft or facing within 30 degrees, left when the -0.25 rad turn of the direction still points the same way, else forward); then drop the hover mode, hover out, fly the ground-exit route on the GoLeft side towards the marker and pick the tower (4) or into-ground state; terminate whenever the enemy is invalid or the group stops
// Grade: DIFFER(2248v2272) -- chunk 826 (7 attempts + 2 diagnostics, md 394 -> 357): the break test through a named bool keeps the toEnemy products and sqrt above the marker calls and spilled as retail (mini-proved: VC7.1 sinks them below the break when the comparison is the if's own expression); residue = the frame 4 bytes short with the slots shifted from the GetTC key on, the hover-marker GetPhysicsTC `mov ecx,eax`, and the fire-blast arms: retail expands the Forward ctor at the GoLeft/facing arm and calls 0x00953780 (Left) / 0x00953750 (Forward) at the other two, VC7.1 here expands all three (per-site inline split; /d2inlS20 and /d2inlT40 do not reproduce it), so the later push_back expansions and the hover-out exit layout follow
// Next idea: the per-site ctor split (a frequency shape that keeps the else-if arms cold), then the frame packing; the block order follows from those
// Leaf: ?UpdateHover@CAIStateGroup_JackDragon@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00951FB0, 2272 bytes, private thiscall, plain ret, frame 0x528, no EH frame.
// Identity (proven): Ego_r.pdb ?UpdateHover@CAIStateGroup_JackDragon@@AAEXXZ; the landed Update 0x00952E80 dispatches state 3 here.
// Callees: CDefClassBase::GetSubDef<CJackDragonDef> 0x008E3300 (landed body, compiled in this TU), CVectorMap::LowerBound 0x0040F020
//   (inlined GetTC<CTCCreatureModeManager>, 0x31), operator new 0x00BFEA1A (0x80 / 0xC), CModeDragonHover(CThing&, CThing&) 0x008D8580,
//   CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0 / RemoveMode(unsigned long) 0x0073ABA0 (mode 0x47),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, StayIdleAndTurnToFace 0x0094FEB0, CAIStateGroupBase::IsStopping 0x008FD010 /
//   Terminate 0x008FCF40 / AIAction_PerformActionAndWait 0x0097DB70, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0,
//   NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0, CThing::GetPos 0x004C73D0 / Kill 0x004C9B80, SetHeadTurning
//   0x00950BB0, WaitForAnimLoop(float) 0x00950950, CTCDPhysicalObstruction::Create 0x007FD4B0 / SetLifeTimeSeconds 0x007FD3D0 /
//   SetThingToIgnoreObstruction 0x007898A3, CCreatureAction_DragonFireBeamInto(CThingCreatureBase&, CThing&, float) 0x009535B0,
//   ~CCombatActionBase 0x0062E900, IsStillRunnable 0x0094FDF0, StayIdle 0x0094FE30, C3DVector::RotateXY 0x00A14040,
//   CCreatureAction_DragonFireBlastLeft / Forward ctors 0x00953780 / 0x00953750 (the third, Forward, site expands the ctor: base
//   0x00953630 + vtable 0x012940E4), ~CCreatureAction_DragonFireBlastBase 0x009536D0, CCombatActionBase::GetSoundPair 0x008561A0 /
//   CCombatActionBase(CThingCreatureBase&, long, const CSoundPair&) 0x00858030 (the inline CCreatureAction_DragonHoverOutOf ctor,
//   vtable 0x01293D0C), vector<C3DVector>::_M_insert_overflow 0x0066E370 / push_back 0x0066E630, StartFlying 0x00950C30,
//   IsTargetOnATower 0x0094FBB0, ChooseIntoGroundState 0x00950A30, free 0x00BFEA14.
// Layout: CAIStateGroup_JackDragon (Ego_r, the landed FlyOutOfGround offsets): PEnemy +0x14, PLandedMarker +0x1c, PHoverMarker1/2
//   +0x24/+0x2c, CurrentState +0x5c, GoLeft +0x60, PreviousFlightPos +0x65, DragonPosOutOfGround3Left/3Right/4Left/4Right
//   +0x8a/+0x96/+0xa2/+0xae; CJackDragonDef MaxFirebeamDistance +0x30, MaxHoverLoops +0x44, HoverObstructionRadius +0x60 (Ego_r =
//   retail); physics TC slots 63 GetFacingAngleXY (+0xfc) / 70 GetFacingVector (+0x118).
// Header overlays: engine/CCombatActionBase.h FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR, engine/CTCDPhysicalObstruction.h
//   FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE (both opt-in, declarations only).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"

class CTCPhysicsBase {                                   // partial view of the physics component's vtable
public:
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41();
    virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53();
    virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61(); virtual void s62();
    virtual float GetFacingAngleXY() const;              // 63 (+0xfc)
    virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69();
    virtual C3DVector GetFacingVector() const;           // 70 (+0x118)
};

class CJackDragonDef : public CSubDefClassBase {         // RTTI .?AVCJackDragonDef@@ : CSubDefClassBase (Ego_r offsets = retail)
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CJackDragonDef", -1); }   // inline
    float MinTimeBetweenSlams;                           // +0x28 (the base is 0x25 packed; members from +0x28)
    long MaxHitsBeforeFlyAway;                           // +0x2c
    float MaxFirebeamDistance;                           // +0x30
    float MinSnapDistance;                               // +0x34
    float MaxSnapDistance;                               // +0x38
    float MaxTimeAtLongRangeBeforeFlyAway;               // +0x3c
    float MaxSlamDistance;                               // +0x40
    long MaxHoverLoops;                                  // +0x44
    float PercentChanceOfFakeLandingSwoop;               // +0x48
    float MaxTimeOnGround;                               // +0x4c
    float DragonGroundHeightOffsetBodge;                 // +0x50
    float HoverBackOffOffset;                            // +0x54
    long FramesBetweenBurninateUpdate;                   // +0x58
    long FramesBetweenFireBeamUpdate;                    // +0x5c
    float HoverObstructionRadius;                        // +0x60
};
FABLE_STATIC_ASSERT(offsetof(CJackDragonDef, MaxHoverLoops) == 0x44);
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};
// CDefClassBase::GetSubDef<CJackDragonDef> 0x008E3300 (the landed body), compiled in this TU so VC7.1 knows the def pointer's
// address is not captured.
#pragma auto_inline(off)
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

class CSoundPair { public: long Sound[2]; };

namespace NCreatureMode {
class CModeDragonHover : public CTCCreatureModeManager::CModeBase {   // sizeof 0x80
public:
    CModeDragonHover(CThing& dragon, CThing& target);    // 0x008D8580
    virtual unsigned long GetType() const;
    unsigned char _pad[0x80 - sizeof(CTCCreatureModeManager::CModeBase)];
};
}

class CCreatureAction_DragonFireBeamInto : public CCombatActionBase {   // retail 0x118
public:
    CCreatureAction_DragonFireBeamInto(CThingCreatureBase& creature, CThing& target, float time);   // 0x009535B0
    long _pad_0x114;
};
class CCreatureAction_DragonFireBlastBase : public CCombatActionBase {  // retail 0x124
public:
    CCreatureAction_DragonFireBlastBase(CThingCreatureBase& creature, CThing& target);   // 0x00953630
    virtual ~CCreatureAction_DragonFireBlastBase();      // 0x009536D0
    unsigned char _pad_0x114[0x10];
};
class CCreatureAction_DragonFireBlastLeft : public CCreatureAction_DragonFireBlastBase {   // vtable 0x0129422C
public:
    CCreatureAction_DragonFireBlastLeft(CThingCreatureBase& creature, CThing& target) : CCreatureAction_DragonFireBlastBase(creature, target) { }
};
class CCreatureAction_DragonFireBlastForward : public CCreatureAction_DragonFireBlastBase {   // vtable 0x012940E4
public:
    CCreatureAction_DragonFireBlastForward(CThingCreatureBase& creature, CThing& target) : CCreatureAction_DragonFireBlastBase(creature, target) { }
};
class CCreatureAction_DragonHoverOutOf : public CCombatActionBase {   // vtable 0x01293D0C
public:
    __forceinline CCreatureAction_DragonHoverOutOf(CThingCreatureBase& creature)
        : CCombatActionBase(creature, 250, GetSoundPair(creature, CCharString("SWORD_SWING"))) { }
};

// IS_THING_ALIVE(ptr): set and not dead (inline; the landed JackDragon rows' helper).
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!static_cast<const CThing*>(p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIStateGroup_JackDragon : public CAIStateGroupBase {
public:
    enum EDragonState { };                               // Ego_r nested enum (Update 0x00952E80 switches on 0..5; enumerator names unproven)
protected:
    virtual void Update();                               // 0x00952E80
private:
    CIntelligentPointer<CThingCreatureBase> PEnemy;      // +0x14
    CIntelligentPointer<CThing> PLandedMarker;           // +0x1c
    CIntelligentPointer<CThing> PHoverMarker1;           // +0x24
    CIntelligentPointer<CThing> PHoverMarker2;           // +0x2c
    CIntelligentPointer<CThing> PStrafeMarker1;          // +0x34
    CIntelligentPointer<CThing> PStrafeMarker2;          // +0x3c
    CIntelligentPointer<CThingAICreature> PInvisibleBodgeCreature;   // +0x44
    long FrameOfLastSlam;                                // +0x4c
    long FrameWhenLastInRange;                           // +0x50
    long FrameWhenLanded;                                // +0x54
    long FramesInSnapRange;                              // +0x58
    EDragonState CurrentState;                           // +0x5c
    long GoLeft;                                         // +0x60
    bool BattleTriggered;                                // +0x64
    C3DVector PreviousFlightPos;                         // +0x65
    bool IsRangeAttack;                                  // +0x71
    C3DVector DragonPosOutOfGround1;                     // +0x72
    C3DVector DragonPosOutOfGround2;                     // +0x7e
    C3DVector DragonPosOutOfGround3Left;                 // +0x8a
    C3DVector DragonPosOutOfGround3Right;                // +0x96
    C3DVector DragonPosOutOfGround4Left;                 // +0xa2
    C3DVector DragonPosOutOfGround4Right;                // +0xae
    bool IsTargetOnATower();                             // 0x0094FBB0
    bool IsStillRunnable();                              // 0x0094FDF0
    void StayIdle(float time);                           // 0x0094FE30
    void StayIdleAndTurnToFace(float time, const C3DVector& facing);   // 0x0094FEB0
    void WaitForAnimLoop(float time);                    // 0x00950950
    EDragonState ChooseIntoGroundState();                // 0x00950A30
    void SetHeadTurning(bool on);                        // 0x00950BB0
    void StartFlying(CThingCreatureBase& target, std::vector<C3DVector>& path, C3DVector& pos,
                     CCountedPointer<CCreatureActionBase> action);   // 0x00950C30
    void UpdateHover();
};

void CAIStateGroup_JackDragon::UpdateHover()
{
    CTCCreatureModeManager* modes;
    CDefPointer<const CJackDragonDef> def;
    GetCreature().PeekBaseDef().GetSubDef(&def);
    GetCreature().GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
    modes->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeDragonHover(GetCreature(), *PEnemy.Get())));
    CThing* marker = (GoLeft ? PHoverMarker1 : PHoverMarker2).Get();
    StayIdleAndTurnToFace(1.5f, marker->GetPhysicsTC()->GetFacingVector());
    bool valid = IS_THING_ALIVE(PEnemy) && !IsStopping();
    if (!valid) {
        Terminate();
        return;
    }
    C3DVector mouthPos;
    CRightHandedSet mouthSet;
    {
        CCharString name("DRAGON_MOUTH_DUMMY");
        GetCreature().GetTCGraphicAppearance()->GetDummyObject(name, 0, 0, mouthPos, mouthSet, 1.0f);
    }
    mouthPos.Z = GetCreature().GetPos().Z;
    float distance;
    long loops = def->MaxHoverLoops;
    for (long i = 0; i < loops; ++i) {
        SetHeadTurning(true);
        C3DVector toEnemy = PEnemy->GetPos() - GetCreature().GetPos();
        toEnemy.Z = 0.0f;
        distance = toEnemy.GetMagnitude();
        CThing* landed = PLandedMarker.Get();
        CThing* enemy = PEnemy.Get();
        bool enemyAboveLanding = enemy->GetPos().Z > landed->GetPos().Z + 1.5f;
        if (enemyAboveLanding)
            break;
        if (distance < def->MaxFirebeamDistance) {
            SetHeadTurning(false);
            WaitForAnimLoop(0.0f);
            if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
                Terminate();
                return;
            }
            CThing* barrier = CTCDPhysicalObstruction::Create(GetCreature().GetPos(), mouthPos, def->HoverObstructionRadius);
            if (barrier) {
                CTCDPhysicalObstruction* obstruction = 0;
                barrier->PeekTCDriver(&obstruction);
                obstruction->SetLifeTimeSeconds(60.0f);
                obstruction->SetThingToIgnoreObstruction(&GetCreature());
            }
            AIAction_PerformActionAndWait(CCreatureAction_DragonFireBeamInto(GetCreature(), *PEnemy.Get(), 5.0f), false);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            if (barrier)
                barrier->Kill(true);
            SetHeadTurning(true);
            StayIdle(2.0f);
        } else {
            C3DVector direction = toEnemy;
            direction.Normalise();
            C3DVector side = direction;
            side.RotateXY(-0.25f);
            if (GoLeft || direction.Dot(GetCreature().GetPhysicsTC()->GetFacingVector()) > 0.866f)
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastForward(GetCreature(), *PEnemy.Get()), false);
            else if (side.Dot(direction) > 0.0f)
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastLeft(GetCreature(), *PEnemy.Get()), false);
            else
                AIAction_PerformActionAndWait(CCreatureAction_DragonFireBlastForward(GetCreature(), *PEnemy.Get()), false);
            if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
                Terminate();
                return;
            }
            StayIdle(2.0f);
        }
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
    }
    SetHeadTurning(false);
    WaitForAnimLoop(0.0f);
    if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
        Terminate();
        return;
    }
    modes->RemoveMode(71);
    AIAction_PerformActionAndWait(CCreatureAction_DragonHoverOutOf(GetCreature()), false);
    if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
        Terminate();
        return;
    }
    C3DVector offset(0.0f, 100.0f, 50.0f);
    offset.RotateXY(GetCreature().GetPhysicsTC()->GetFacingAngleXY());
    GetCreature().GetPos();
    std::vector<C3DVector> route;
    route.push_back(GetCreature().GetPos());
    GetCreature().GetPhysicsTC()->GetFacingVector();
    GetCreature().GetPos();
    if (GoLeft) {
        route.push_back(DragonPosOutOfGround3Right);
        route.push_back(DragonPosOutOfGround3Left);
        route.push_back(DragonPosOutOfGround4Left);
        PreviousFlightPos = DragonPosOutOfGround3Left;
    } else {
        route.push_back(DragonPosOutOfGround3Left);
        route.push_back(DragonPosOutOfGround3Right);
        route.push_back(DragonPosOutOfGround4Right);
        PreviousFlightPos = DragonPosOutOfGround3Right;
    }
    C3DVector target = marker->GetPos();
    StartFlying(GetCreature(), route, target, CCountedPointer<CCreatureActionBase>());
    if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
        Terminate();
        return;
    }
    if (IsTargetOnATower())
        CurrentState = (EDragonState)4;
    else
        CurrentState = ChooseIntoGroundState();
}
