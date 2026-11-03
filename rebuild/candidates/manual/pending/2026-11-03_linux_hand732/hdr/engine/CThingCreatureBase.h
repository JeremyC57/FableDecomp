#ifndef FABLE_ENGINE_CTHINGCREATUREBASE_H
#define FABLE_ENGINE_CTHINGCREATUREBASE_H
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE
// Retail opt-in (Codex pass 11 consolidation 2): the retail CThingCreatureBase model (needs the retail CThing chain: FABLE_RETAIL_CCLASS, FABLE_RETAIL_DEF_CLASS_BASE); see below.
#include "engine/CThingGameObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
class CThingBuilding; class CThingCreatureDef; class CCreatureActionBase;
class CCreatureInteraction; class CTCCreatureModeManager;
class CHeroCombatDef;
class CEnginePrimitivePrepareInfo;
class CCombatManager;
enum ESex;
class CThingCreatureBase : public CThingGameObject {

#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

#ifdef FABLE_AUTOPLACE_TEST_ACCESS
    friend class CAutoPlaceFixture;
#endif
private:
    unsigned long LastMessageEventICreatedID;
    unsigned char CombatCollisionDebugGraphics[12];
protected:
    CDefPointer<const CThingCreatureDef> PDef;
    long ShotAccuracyPercentage;
    C3DVector InitialPos;
    CIntelligentPointer<CThingCreatureBase> PLastAttackedByCreature;
    unsigned long WFLastAttackedByCreature;
    CCountedPointer<CCreatureActionBase> PCurrentAction;
    std::list<CCountedPointer<CCreatureActionBase> > PQueuedActions;
    C3DVector MovementVector;
    C3DVector HeadPosOffset;
    long IdleCounter;
    float TurnSpeed;
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;
    CTCCreatureModeManager* PTCModeManager;
    unsigned char PreviousActionHandedness[4];
    long PreviousActionHandednessWF;
    unsigned char BodyReorienter[4];
    unsigned char CombatDebugGraphics[12];
    CIntelligentPointer<CThing> PItemToUnsheatheAfterCutscene;
    char* DebugText;
public:
    // chunk 719: the flag bytes are public data in the retail class: CreateRelevantPersonAndUpdateStats 0x004FF750 (a free
    // function) sets FollowHeroThroughTeleporters (`or byte [creature+15Ch],40h`) directly, and neither PDB name set has a setter
    // for it (FableWin.pdb emits every inline accessor a TU uses). Access only; no layout change.
    bool CurrentlyFrameUpdating:1,Blinking:1,MeleeAttacker:1,MeleeAttacked:1,
        LeaveDeadBodyOverride:1,LeaveExperienceOrbsOverride:1,FollowHeroThroughTeleporters:1,HeadPosCached:1;
    bool UseMovementInActions:1,UpdateMovementVectorThisFrame:1,AbortingCurrentAction:1,DemonDoor:1,
        Oracle:1,HadFacingAngleYZSetByBodyOrientation:1,UnderInfluenceOfEpicSpell:1,HiddenOnMiniMap:1;
    bool IsShowingDebugInfo:1;
protected:
    void ReactToGenericStrike(const CHitParameters&);
    // FableWin-only ?PeekDefinition@CThingCreatureBase@@IBEABVCThingCreatureDef@@XZ (0x01D6DD10; inline in Ego_r and retail): the body
    // `return *PDef;` needs the complete CThingCreatureDef, so it is defined inline below once both classes are complete.
    const CThingCreatureDef& PeekDefinition() const;
public:
    bool IsInMelee()const;bool IsInMeleeAsAttacker()const;
    bool IsUnderInfluenceOfEpicSpell() const { return UnderInfluenceOfEpicSpell; }
    bool AutoPlaceNearBuilding(const CThingBuilding*,float);
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb / FableWin.pdb ?AutoPlaceNear@CThingCreatureBase@@QAE_N
    // ABVC3DVector@@M@Z (0x00665DF0) and ?GetSex@CThingCreatureBase@@QBE?AW4ESex@@XZ (0x00661FF0), both out of line. ESex is a
    // forward enum (no enumerator names on disk; GlobalUpdate compares the result with 2).
    bool AutoPlaceNear(const C3DVector& pos, float radius);             // 0x00665DF0
    ESex GetSex() const;                                                 // 0x00661FF0
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PDef)==0xe0);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PCurrentAction)==0x100);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PQueuedActions)==0x108);
    }
    virtual void OnHit(CHitParameters&);
    bool SetCurrentAction(const CCreatureActionBase&);
    // Retail opt-in (2026-11-03, chunk 720 row 0x0065B854): FableWin.pdb ?GetProperty@CThingCreatureBase@@QBEKXZ, out of line in retail
    // at 0x00662010 (mov eax,[ecx+0E0h]; mov eax,[eax+0F0h]: the PDef pointee's Property, Ego_r CThingCreatureDef +0xF8 less the two
    // STLport vector shrinks; the virtual CThingAICreature::GetProperties 0x00830140, slot 48, has the same body).
    // AddThingToVillage compares it with 3 to count guards. Declaration only: no layout change.
    unsigned long GetProperty() const;                                   // 0x00662010
    // Chunk 732 (CThingPlayerCreature rows): Ego_r.pdb ?GetNavigationLevelAt@CThingCreatureBase@@QBEJABVC3DVector@@@Z (out of line,
    // 0x00661D20: PeekWorldMap then CWorldMap::GetNavigationLayerAt) and the protected virtual
    // ?DrawPreparePrimitivesForRendering@CThingCreatureBase@@MAE_NABVCEnginePrimitivePrepareInfo@@@Z (0x00663C40, vtable 0x0125AC0C
    // slot 36; the derived CThingPlayerCreature override calls it directly). Declarations only: no layout change.
    long GetNavigationLevelAt(const C3DVector& pos) const;               // 0x00661D20
    // Ego_r.pdb ?GetCombatManager@CThingCreatureBase@@QAEAAVCCombatManager@@XZ (out of line, 0x00661ED0).
    CCombatManager& GetCombatManager();                                  // 0x00661ED0
protected:
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x00663C40
public:
};
FABLE_STATIC_ASSERT(sizeof(CThingCreatureBase)==0x160);
#ifdef FABLE_ENGINE_CTHINGCREATUREDEF_H
inline const CThingCreatureDef& CThingCreatureBase::PeekDefinition() const { return *PDef; }
#endif
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CThingCreatureBase = 0x188.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CThingCreatureBase = 0x160.
// Retail evidence: 5 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCPPointerInfo;
class CCreatureActionBase;
class CCreatureInteraction;
class CDefPointeeBase;
class CTCCreatureModeManager;
class CThingBodyReorienter;

#pragma pack(push, 1)
class CThingCreatureBase {
public:
    void*                   __vftable;                       // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char           _pad_0x04[0x4];                  // +0x04
    unsigned char           _pad_0x08[0x4];                  // +0x08
    unsigned char           _pad_0x0c[0x4];                  // +0x0c
    unsigned char           _pad_0x10[0x1];                  // +0x10
    unsigned char           _pad_0x11[0x7];                  // +0x11
    unsigned char           _pad_0x18[0x4];                  // +0x18
    unsigned char           _pad_0x1c[0x4];                  // +0x1c
    unsigned char           _pad_0x20[0x38];                 // +0x20
    unsigned char           _pad_0x58[0x4];                  // +0x58
    unsigned char           _pad_0x5c[0x4];                  // +0x5c
    long                    q;                               // +0x60 retail-only long (previous header)
    unsigned char           _pad_0x64[0x4];                  // +0x64
    unsigned char           _pad_0x68[0x4];                  // +0x68
    unsigned char           _pad_0x6c[0x4];                  // +0x6c
    unsigned char           _pad_0x70[0x4];                  // +0x70
    unsigned char           _pad_0x74[0x4];                  // +0x74
    unsigned char           _pad_0x78[0x4];                  // +0x78
    unsigned char           _pad_0x7c[0x4];                  // +0x7c
    unsigned char           _pad_0x80[0x4];                  // +0x80
    unsigned char           _pad_0x84[0x4];                  // +0x84
    unsigned char           _pad_0x88[0x4];                  // +0x88
    unsigned char           _pad_0x8c[0x4];                  // +0x8c
    unsigned char           _pad_0x90[0x4];                  // +0x90
    unsigned char           _pad_0x94[0x1];                  // +0x94
    unsigned char           _pad_0x95[0x7];                  // +0x95
    unsigned char           _pad_0x9c[0x4];                  // +0x9c
    unsigned char           _pad_0xa0[0x4];                  // +0xa0
    unsigned char           _pad_0xa4[0x4];                  // +0xa4
    unsigned char           _pad_0xa8[0x4];                  // +0xa8
    unsigned char           _pad_0xac[0x4];                  // +0xac
    unsigned char           _pad_0xb0[0x4];                  // +0xb0
    unsigned char           _pad_0xb4[0x4];                  // +0xb4
    unsigned char           _pad_0xb8[0x4];                  // +0xb8
    unsigned char           _pad_0xbc[0x1];                  // +0xbc
    unsigned char           _pad_0xbd[0x3];                  // +0xbd
    unsigned char           _pad_0xc0[0x4];                  // +0xc0
    unsigned char           _pad_0xc4[0x4];                  // +0xc4
    unsigned char           _pad_0xc8[0x8];                  // +0xc8
    unsigned long           LastMessageEventICreatedID;      // +0xd0
    unsigned char           CombatCollisionDebugGraphics[0xc];  // +0xd4 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    CDefPointeeBase*        PDef_Object;                     // +0xe0 CDefPointer<CThingCreatureDef_const_>::Object
    long                    ShotAccuracyPercentage;          // +0xe4
    float                   InitialPos_X;                    // +0xe8 C3DVector::X
    float                   InitialPos_Y;                    // +0xec C3DVector::Y
    float                   InitialPos_Z;                    // +0xf0 C3DVector::Z
    unsigned char           PLastAttackedByCreature[0x8];    // +0xf4 CIntelligentPointer<CThingCreatureBase>
    unsigned long           WFLastAttackedByCreature;        // +0xfc
    CCreatureActionBase*    PCurrentAction_Data;             // +0x100 CCountedPointer<CCreatureActionBase>::Data
    CCPPointerInfo*         PCurrentAction_Info;             // +0x104 CCountedPointer<CCreatureActionBase>::Info
    unsigned char           PQueuedActions[0x4];             // +0x108 list<CCountedPointer<CCreatureActionBase>,std::allocator<CCountedPointer<CCreatureActionBase>_>_>
    float                   MovementVector_X;                // +0x10c C3DVector::X
    float                   MovementVector_Y;                // +0x110 C3DVector::Y
    float                   MovementVector_Z;                // +0x114 C3DVector::Z
    float                   HeadPosOffset_X;                 // +0x118 C3DVector::X
    float                   HeadPosOffset_Y;                 // +0x11c C3DVector::Y
    float                   HeadPosOffset_Z;                 // +0x120 C3DVector::Z
    long                    IdleCounter;                     // +0x124
    float                   TurnSpeed;                       // +0x128
    CCreatureInteraction*   PCreatureInteraction_Data;       // +0x12c CCountedPointer<CCreatureInteraction>::Data
    CCPPointerInfo*         PCreatureInteraction_Info;       // +0x130 CCountedPointer<CCreatureInteraction>::Info
    CTCCreatureModeManager* PTCModeManager;                  // +0x134
    unsigned char           PreviousActionHandedness[0x4];   // +0x138 ECombatAnimationHandedness
    long                    PreviousActionHandednessWF;      // +0x13c
    CThingBodyReorienter*   BodyReorienter_ptr;              // +0x140 scoped_ptr<CThingBodyReorienter>::ptr
    unsigned char           CombatDebugGraphics[0xc];        // +0x144 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char           PItemToUnsheatheAfterCutscene[0x8]; // +0x150 CIntelligentPointer<CThing>
    void*                   DebugText;                       // +0x158 char *
    unsigned char           HeadPosCached[0x1];              // +0x15c bitfield group: bool:1
    unsigned char           HiddenOnMiniMap[0x1];            // +0x15d bitfield group: bool:1
    unsigned char           IsShowingDebugInfo[0x2];         // +0x15e bitfield group: bool:1
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CThingCreatureBase) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, q) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, LastMessageEventICreatedID) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, CombatCollisionDebugGraphics) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PDef_Object) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, ShotAccuracyPercentage) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_X) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_Y) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, InitialPos_Z) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PLastAttackedByCreature) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, WFLastAttackedByCreature) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCurrentAction_Data) == 0x100);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCurrentAction_Info) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PQueuedActions) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_X) == 0x10c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_Y) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, MovementVector_Z) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_X) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_Y) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosOffset_Z) == 0x120);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, IdleCounter) == 0x124);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, TurnSpeed) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCreatureInteraction_Data) == 0x12c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PCreatureInteraction_Info) == 0x130);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PTCModeManager) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PreviousActionHandedness) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PreviousActionHandednessWF) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, BodyReorienter_ptr) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, CombatDebugGraphics) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, PItemToUnsheatheAfterCutscene) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, DebugText) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HeadPosCached) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, HiddenOnMiniMap) == 0x15d);
FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase, IsShowingDebugInfo) == 0x15e);

#endif // FABLE_RETAIL_CTHINGCREATUREBASE
#endif // FABLE_ENGINE_CTHINGCREATUREBASE_H
