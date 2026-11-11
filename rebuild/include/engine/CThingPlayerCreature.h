#ifndef FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#define FABLE_ENGINE_CTHINGPLAYERCREATURE_H
#include "engine/CThingCreatureBase.h"
#if defined(FABLE_RETAIL_CTHINGPLAYERCREATURE) && defined(FABLE_RETAIL_CTHINGCREATUREBASE)
// Retail branch (chunk 732, rows 0x006AA2E0 / 0x006AA720 / 0x006AABE0 / 0x006AB820 / 0x006ACA30): the Ego_r.pdb CThingPlayerCreature
// members over the retail CThingCreatureBase (0x160), at the retail offsets every landed CThingPlayerCreature row reads (OnCreate
// 0x006A9EF0, OnCollision 0x006AD580, ResolveMovementAcceleration 0x006AB770, GetCurrentMaxMovementSpeed 0x006AB5D0): ControlPos
// +0x160 .. MovementBandType +0x200 (Ego_r +0x188 .. +0x22C: the retail creature chain sits 0x28 lower, and the STLport map is 0xC
// where Ego_r's is 0x10). The two collision vectors are byte-packed in the PDB too (CollidedWithThing +0x1CC, LastThingCollisionNormal
// +0x1CD, LastThingCollisionPosition +0x1D9; Ego_r +0x1F8 / +0x1F9 / +0x205), so the class is packed to 1 with the PDB's alignment
// gaps spelled out. Allocation size 0x208 (Create 0x006AC910 `push 208h`): the last dword is not in the Ego_r member list.
#include "engine/CDefPointer.h"
enum EControlledMovementType;
enum EMovementBandType;
enum ECombatZone;
#ifdef FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT
enum EGameAction;                                            // MS forward enum (engine/fable_nscript_forward.h)
#endif
class CHeroCombatDef;
class CPlayerMovementDef;
class CCreatureAbilityDef;
class CEnginePrimitivePrepareInfo;
#pragma pack(push, 1)
class CThingPlayerCreature : public CThingCreatureBase {
public:
    CThingPlayerCreature();
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingPlayerCreature@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
    // Ego_r.pdb / FableWin.pdb spellings (vtable 0x012457FC).
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x006AA2E0 UAE
    virtual bool SetActionForMelee(const CCreatureAbilityDef& ability);                     // 0x006AABE0 UAE
    void OnCollisionWithObject(CThing* thing);                                             // 0x006ACA30 QAE
    float GetCurrentRelativeMovementAngleXY() const;                                      // 0x006A9640 QBE
#ifdef FABLE_RETAIL_CTHINGPLAYERCREATURE_RELATIVEMOVEMENT
    // Nested opt-in (chunk 1012, row 0x0062BD80 CWorld::EAControlledCreatureStrafe): Ego_r.pdb / FableWin.pdb
    // ?ApplyRelativeMovementAcceleration@CThingPlayerCreature@@QAEXABVC3DVector@@W4EGameAction@@@Z, out of line in retail at
    // 0x006AD3C0. Declaration only (EGameAction is the MS forward enum of engine/fable_nscript_forward.h).
    void ApplyRelativeMovementAcceleration(const C3DVector& acceleration, EGameAction action);   // 0x006AD3C0 QAE
#endif
#ifdef FABLE_RETAIL_CTHINGPLAYERCREATURE_MOVEMENTSPEEDS
    // Retail opt-in (chunk 802, row 0x008EACB0): Ego_r.pdb / FableWin.pdb ?InitialiseMovementSpeeds@CThingPlayerCreature@@QAEXXZ (the
    // landed row 0x006A9A00), called out of line. Declaration only: no layout change.
    void InitialiseMovementSpeeds();                                                       // 0x006A9A00 QAE
#endif
#ifdef FABLE_RETAIL_CTHINGPLAYERCREATURE_CONSTRUCT
    // Nested opt-in (agentNF11, row 0x006A9DD0): Ego_r.pdb / FableWin.pdb
    // ?ConstructFromParams@CThingPlayerCreature@@IAE_NJABVC3DVector@@JABVCThingPlayerCreatureInit@@@Z (protected non-virtual, `ret 10h`).
    // Declaration only (CThingPlayerCreatureInit forward-declared).
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, const class CThingPlayerCreatureInit& init);   // 0x006A9DD0
#endif
protected:
    void SetPlayerCreatureActionForMelee(ECombatZone zone, const C3DVector& direction, CThing* target,
                                         const CCreatureAbilityDef& ability);              // 0x006AA720 IAE
    void ResolveFacingDirection();                                                         // 0x006AB820 IAE
    C3DVector ControlPos;                                    // +0x160
    C3DVector ControlMoveByVector;                           // +0x16c
    unsigned char MovedByPlayer:1, Jumping:1, Pad1:1;        // +0x178
    unsigned char _pad_0x179[3];
    EControlledMovementType ControlledMovementType;          // +0x17c
    C3DVector MovementAcceleration;                          // +0x180
    float MaxSlowWalkingSpeed;                               // +0x18c
    float MaxWalkingSpeed;                                   // +0x190
    float MaxJoggingSpeed;                                   // +0x194
    float MaxRunningSpeed;                                   // +0x198
    float MaxSprintingSpeed;                                 // +0x19c
    float MaxFlyingSpeed;                                    // +0x1a0
    float MaxStrafingSpeed;                                  // +0x1a4
    unsigned char RelativeMovementAccelerationComponents[0xc];   // +0x1a8 map<EGameAction,pair<C3DVector,long> > (STLport, 0xC)
    C3DVector RelativeMovementAcceleration;                  // +0x1b4
    C3DVector LastRelativeMovementAcceleration;              // +0x1c0
    bool CollidedWithThing;                                  // +0x1cc
    C3DVector LastThingCollisionNormal;                      // +0x1cd
    C3DVector LastThingCollisionPosition;                    // +0x1d9
    unsigned char _pad_0x1e5[3];
    CDefPointer<const CHeroCombatDef> PHeroCombatDef;        // +0x1e8
    CDefPointer<const CPlayerMovementDef> PPlayerMovementDef;    // +0x1ec
    C3DVector ImpulseVelocity;                               // +0x1f0
    float LeanAngle;                                         // +0x1fc
    EMovementBandType MovementBandType;                      // +0x200
    unsigned char _pad_0x204[4];
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThingPlayerCreature, ControlledMovementType) == 0x17c);
        FABLE_STATIC_ASSERT(offsetof(CThingPlayerCreature, LastRelativeMovementAcceleration) == 0x1c0);
        FABLE_STATIC_ASSERT(offsetof(CThingPlayerCreature, LastThingCollisionNormal) == 0x1cd);
        FABLE_STATIC_ASSERT(offsetof(CThingPlayerCreature, PHeroCombatDef) == 0x1e8);
        FABLE_STATIC_ASSERT(offsetof(CThingPlayerCreature, MovementBandType) == 0x200);
    }
#ifdef FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool DrawIsDisplayingInformation() const;  // slot 33, 0x006A9560
public:
    virtual bool IsAffectedByDamage() const;  // slot 47, 0x0052ABA0
public:
    virtual bool IsControlled() const;  // slot 76, 0x0052ABB0
public:
    virtual bool IsPlayerCharacter() const;  // slot 77, 0x0052ABE0
public:
    virtual bool IsToUnsheatheItemsAfterCutscenes() const;  // slot 88, 0x0052ABD0
public:
    virtual bool FeelsGravity() const;  // slot 91, 0x006A9570
public:
#endif
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==0x208);
#else
// Original sole base CThingCreatureBase; native Create006ac91c allocates208.
// Complete original tail ControlPos through MovementBandType is unused here.
class CThingPlayerCreature:public CThingCreatureBase {
private: unsigned char _pad_0x160[0xa8];
public: CThingPlayerCreature();
#ifdef FABLE_RETAIL_CTHINGPLAYERCREATURE
    // Header wave 2026-11-03 (row 0x00522A20): Ego_r.pdb ?Alloc@CThingPlayerCreature@@SIPAVCClass@@XZ.
    static CClass* __fastcall Alloc();
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingPlayerCreature)==0x208);
#endif
#if defined(FABLE_RETAIL_PLAYER) && defined(FABLE_RETAIL_CINTELLIGENTPOINTER) && defined(FABLE_ENGINE_CPLAYER_H)
// Retail opt-in (chunk 728): the inline FableWin-only CPlayer::GetPPlayerCharacter declared in engine/CPlayer.h (PlayerCharacter +0x34).
inline CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return PlayerCharacter.Get(); }
#ifdef FABLE_RETAIL_PLAYER_NEARESTVILLAGE
// Original FableWin01a828b0, expanded by native00488620/004886f0.
inline const CThingPlayerCreature* CPlayer::PeekPPlayerCharacter() const {return PlayerCharacter.Get();}
#endif
#endif
#endif
