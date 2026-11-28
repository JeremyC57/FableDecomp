#ifndef FABLE_ENGINE_CTHINGCREATUREBASE_H
#define FABLE_ENGINE_CTHINGCREATUREBASE_H
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE
// Retail opt-in (Codex pass 11 consolidation 2): the retail CThingCreatureBase model (needs the retail CThing chain: FABLE_RETAIL_CCLASS, FABLE_RETAIL_DEF_CLASS_BASE); see below.
#include "engine/CThingGameObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
class CThingBuilding; class CThingCreatureDef; class CCreatureActionBase;
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
class CAnimationEntry;                                                   // chunk 796 opt-in: GetAnimFromName's return type
#endif
#ifdef FABLE_RETAIL_CREATURE_ONCREATE_PASS27
#include "engine/ECombatAnimationHandedness.h"
#endif
class CCreatureInteraction; class CTCCreatureModeManager;
class CHeroCombatDef;
#if defined(FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE) || defined(FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF)
class CCombatTypeDef;
#endif
class CEnginePrimitivePrepareInfo;
class CCombatManager;
enum ESex;
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#include "engine/ECreatureType.h"
#endif
#ifdef FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
class CCreatureAbilityDef;class CSoundPair;
#endif
class CThingCreatureBase : public CThingGameObject {
#if defined(FABLE_RETAIL_AI_CREATURE_ONHIT_PASS29) || defined(FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29)
public: const CThingCreatureBase* PeekPLastAttackedByCreature()const{return PLastAttackedByCreature.Get();}
#endif

#ifdef FABLE_RETAIL_KNOCKDOWN_INITIALISE_PASS28
public:
    // Both original symbol tables; native caller targets 00663490.
    void SetBodyReorienterActive(bool active);
private:
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ONDIE
// NF62 opt-in (2026-11-17, row 0x006A9590 CThingPlayerCreature::OnDie, which calls it qualified last): Ego_r.pdb ?OnDie@CThingCreatureBase@@UAEXXZ
// (public virtual; the CThing slot-11 override, out of line at 0x00663F50, vtable 0x0125AC38 is CThingCreatureBase's slot 11). Declaration only.
public:
    virtual void OnDie();                                        // slot 11 0x00663F50
private:
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ONKILL
// NF120 opt-in (PRIORITY q3944, row 0x00664370): Ego_r.pdb 0x00540300 / FableWin.pdb 0x01D935F0 ?OnKill@CThingCreatureBase@@UAEXXZ (public
// virtual; the CThing slot-12 override, out of line at 0x00664370, slot 12 of the CThingPlayerCreature vtable 0x012457FC). Declaration only.
public:
    virtual void OnKill();                                       // slot 12 0x00664370
private:
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_GETCURRENTSPEED
// Opt-in (hand3c, row 0x006CC800 CTCCreatureNavigation::FrameUpdate): Ego_r.pdb / FableWin.pdb ?GetCurrentSpeed@CThingCreatureBase@@QBEMXZ
// (public const, out of line at 0x00661CD0, landed; called at 0x006CC88B / 0x006CC8AA). Declaration only.
public:
    float GetCurrentSpeed() const;                                       // 0x00661CD0
private:
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ISCHILD
// Retail opt-in (agentC23, row 0x0058578B CTCInventory::OpenInventory; first written by the acctB lane): Ego_r.pdb / FableWin.pdb
// ?IsChild@CThingCreatureBase@@QBE_NXZ, out of line at 0x00661F70 (the default branch declares it too). Declaration only.
public:
    bool IsChild() const;                                        // 0x00661F70
private:
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_MESHSIZE
public:
 // Ego_r.pdb ?GetNominalMeshRadius / ?GetNominalMeshHeight / ?GetCurrentMeshRadius@CThingCreatureBase@@QBEMXZ (all out of line in retail).
 float GetNominalMeshRadius() const;                                     // 0x006625C0
 float GetNominalMeshHeight() const;                                     // 0x006625E0
#ifdef FABLE_RETAIL_PHYSICAL_SHIELD_EFFECT_PASS26
 float GetCurrentMeshHeight() const;
 float GetApproxMaxMeshHeight() const;
#endif
 float GetCurrentMeshRadius() const;                                     // 0x00664BA0
private:
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
// Evidence: both original PDB type records and donor OnHit/helper disassembly.
public:
void PerformBlockedReaction(CThingCreatureBase*,bool);
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
public: CThingCreatureBase* GetPLastAttackedByCreature()const{return PLastAttackedByCreature.Get();}
#endif
#ifdef FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
protected:
 virtual unsigned long GetPhonemeAnim(const CCharString & p0) const;
 virtual bool SetActionForMelee(const CCreatureAbilityDef & p0);
public:
 virtual bool AnimExists(const CCharString & p0, bool p1) const;
 virtual bool IsNewAttackerGreaterThreat(const CThingCreatureBase & p0, const CThingCreatureBase & p1) const;
 virtual bool IsWillable() const;
 virtual bool IsControlled() const = 0;
 virtual bool IsPlayerCharacter() const = 0;
 virtual float GetMaxPushingSpeed(const C3DVector & p0) const = 0;
 virtual float GetMaxWalkingSpeed(const C3DVector & p0) const = 0;
 virtual float GetMaxJoggingSpeed(const C3DVector & p0) const = 0;
 virtual float GetMaxRunningSpeed(const C3DVector & p0) const = 0;
 virtual float GetMaxSprintingSpeed(const C3DVector & p0) const = 0;
 virtual float GetMaxFlyingSpeed(const C3DVector & p0) const = 0;
 virtual float GetMaxHoverSpeedSlow(const C3DVector & p0) const = 0;
 virtual float GetMaxHoverSpeedMedium(const C3DVector & p0) const = 0;
 virtual float GetMaxHoverSpeedFast(const C3DVector & p0) const = 0;
 virtual float GetMaxAcceleration() const = 0;
 virtual bool IsToUnsheatheItemsAfterCutscenes() const = 0;
 virtual CSoundPair GetSoundPair(const CCharString & p0) const;
#endif

#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
public:
 // Retail opt-in (chunk 2015, row 0x0097FEA0): Ego_r.pdb / FableWin.pdb ?ClearAttackedByCreature@CThingCreatureBase@@QAEXXZ (out of
 // line at 0x006691C0: PLastAttackedByCreature (+0xF4) = 0 through a CIntelligentPointer<CThingCreatureBase> temporary; Ego_r donor
 // 0x005454B0) and the FableWin.pdb inline ?GetPLastAttackedByCreature@CThingCreatureBase@@QBEPAV1@XZ (`lea ecx,[eax+0F4h] ; call
 // GetPItem`, 0x00980354), as the FABLE_RETAIL_CTCTARGETINGAI_ATTACK accessor above. No layout change.
 void ClearAttackedByCreature();                                         // 0x006691C0
#ifndef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
 CThingCreatureBase* GetPLastAttackedByCreature()const{return PLastAttackedByCreature.Get();}
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_PEEKMOVEMENTVECTOR
public:
 // NF130 opt-in (q6453, row 0x00862EC0 CModeStandBase::AnimUpdate): FableWin.pdb 0x01E46990
 // ?PeekMovementVector@CThingCreatureBase@@QBEABVC3DVector@@XZ, inline in retail (the row reads MovementVector +0x10c / +0x110 in place,
 // Ego_r.pdb CThingCreatureBase::MovementVector). No layout change.
 const C3DVector& PeekMovementVector() const { return MovementVector; }
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_GETDEFHOVERSPEEDS
public:
 // NF128 opt-in (q6453, row 0x008EA500 NCreatureMode::CModeHoverBase::InitialiseMovementSpeeds): Ego_r.pdb 0x0053CF60 / 0x0053CF70 /
 // 0x0053CF80 + FableWin.pdb 0x01D9AF70 / 0x01D9AFA0 / 0x01D9AFD0 ?GetDefHover{Slow,Medium,Fast}Speed@CThingCreatureBase@@QBEMXZ, out of line
 // in retail at 0x006620E0 / 0x006620F0 / 0x00662100 (consecutive 16-byte bodies as in Ego_r: the creature def at +0xE0, then its
 // MaxHoverSpeedSlow / Medium / Fast float). Declarations only: no layout change.
 float GetDefHoverSlowSpeed() const;                                    // 0x006620E0
 float GetDefHoverMediumSpeed() const;                                  // 0x006620F0
 float GetDefHoverFastSpeed() const;                                    // 0x00662100
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_UPDATELASTATTACKINGCREATURE
protected:
 // NF55 opt-in (2026-11-17, row 0x006624A0): Ego_r.pdb 0x0053D330 + FableWin.pdb 0x01D8C170 ?UpdateLastAttackingCreature@CThingCreatureBase@@IAEXXZ
 // (protected; out of line at 0x006624A0, called from WorldUpdate 0x006666A0). Declaration only: no layout change.
 void UpdateLastAttackingCreature();                                     // 0x006624A0
public:
#endif
#ifdef FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
public:bool IsThingInFOV(const CThing&,bool)const;
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ISPOSINVIEWANGLE
// Retail opt-in (wf7, row 0x0097D6F0 CAIStateGroupBase::AIAction_LookAtThing): Ego_r.pdb / FableWin.pdb
// ?IsPosInViewAngle@CThingCreatureBase@@QBE_NABVC3DVector@@M_N1@Z (out of line, retail 0x006650E0). Declaration only.
public:bool IsPosInViewAngle(const C3DVector& pos,float angle,bool,bool)const;   // 0x006650E0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
// Retail opt-in (chunk Q6, row 0x00912840): Ego_r.pdb / FableWin.pdb ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z
// (public const), out of line in retail (0x00666D40). Declaration only.
public:float GetHeightZBelowAt(const C3DVector& pos,bool,bool)const;
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTBELOW
// Retail opt-in (chunk 2004, row 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath): Ego_r.pdb / FableWin.pdb
// ?GetHeightZBelowAt@CThingCreatureBase@@QBEMABVC3DVector@@_N1@Z, out of line in retail at 0x00666D40 (thiscall `ret 0Ch`, float in st0;
// called with `push 1 ; push 1 ; push <probe>` and ecx = the brain's creature, 0x00912E5D / 0x00912F35). Declaration only.
public:float GetHeightZBelowAt(const C3DVector& pos,bool,bool)const;      // 0x00666D40
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_GETDAZEDHEALTHTHRESHOLD
// NF93 opt-in (round 8, q11261 row 0x008663D0 CCombatAction_GenericStrikeResponseBase::FinishAction): Ego_r.pdb 0x0053CEF0 / FableWin.pdb
// 0x01D9A580 ?GetDazedHealthThreshold@CThingCreatureBase@@QBEMXZ (public const, float in st0), out of line in retail at 0x00662090.
// Declaration only: no layout change.
public:float GetDazedHealthThreshold()const;                              // 0x00662090
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_SETHEALTH
public:
 void SetHealth(float health){Health=health;}
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
public:
 // Retail opt-in (Codex pass19): both PDB names; native00662000.
 ECreatureType GetCreatureType() const;
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
public:
 // agent wfE10 (row 0x00502E90 CWorldMap::PopulateRegionWithVillagers; chunk 879's SEXTYPE text re-based): Ego_r.pdb / FableWin.pdb
 // ?GetSex@CThingCreatureBase@@QBE?AW4ESex@@XZ, out of line in retail at 0x00661FF0 (`mov eax,[ecx+0E0h] ; mov eax,[eax+0C0h]`,
 // the def's Sex), next to GetCreatureType 0x00662000. Declaration only: no layout change.
 // Retail opt-in (acctC wfA1, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): Ego_r.pdb ?GetSex@CThingCreatureBase@@QBE?AW4ESex@@XZ,
 // out of line in retail at 0x00661FF0 (calls 0x00682E1A / 0x00683256). Declaration only.
 // wfA1 (row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb 0x0053CE40 / FableWin.pdb 0x01D9A430 (`mov ecx,[esi+4] ; call ; cmp eax,2`, 0x006D0894).
 ESex GetSex() const;                                                 // 0x00661FF0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_AUTOPLACENEAR
public:
 // Retail opt-in (acctC wfA1, row 0x006D0830 CTCWife::GlobalUpdate; restored after the GETSEX re-copy dropped it): Ego_r.pdb 0x00541790
 // ?AutoPlaceNear@CThingCreatureBase@@QAE_NABVC3DVector@@M@Z, out of line in retail at 0x00665DF0 (reloc_bindings). Declaration only.
 // (Ego_r lists it right before AutoPlaceNearBuilding 0x00541840, as retail 0x00665DF0 before 0x00666EF0.)
 bool AutoPlaceNear(const C3DVector&, float);                         // 0x00665DF0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_COMBATRADIUS
public:
 // Retail opt-in (Codex pass18): both PDBs QBEMXZ; native00661f90, sphere caller00814ea0.
 float GetCombatRadius() const;
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
public:
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
 const CCombatTypeDef& GetCombatTypeDef()const;
#endif
 CDefPointer<const CCombatTypeDef> GetPCombatTypeDef()const;
 long GetCombatType()const;
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
public:
 // Opt-in (chunk 2034, row 0x0097BDA0 CAIStateGroup_ScorpionKing::UpdateCombat): Ego_r.pdb
 // ?GetCombatTypeDef@CThingCreatureBase@@QBEABVCCombatTypeDef@@XZ (donor 0x00540EE0), out of line in retail at 0x00666D10 (the catalog
 // label; `mov ecx,<creature> ; call`, then FrontNearLimit +0x74 read off the returned reference, 0x0097BFC2). Declaration only.
 const CCombatTypeDef& GetCombatTypeDef()const;                       // 0x00666D10
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
public:
 static unsigned long FABLE_FASTCALL StatGetTypeFlags(){return 15;}
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CREATEWEAPON
public:
 // Opt-in (w22 agent1016, row 0x00663990): Ego_r.pdb / FableWin.pdb ?CreateWeaponForThingFromDef@CThingCreatureBase@@SIXAAVCThing@@J@Z
 // (static __fastcall; the landed 0x008C8960 BanditKingChangeWeapons::PerformAction calls it). Declaration only: no layout change.
 static void FABLE_FASTCALL CreateWeaponForThingFromDef(CThing& thing, long defIndex);   // 0x00663990
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_HEADPOS
public:
 // Chunk 1013 (row 0x0063E6F0 CWorldMap::IsLineOfSight(const CThing&,...)): Ego_r.pdb 0x0053C900 / FableWin.pdb 0x01D9A630
 // ?GetHeadPos@CThingCreatureBase@@QBE?AVC3DVector@@_N@Z, out of line in retail (0x00661B40, `push 1 ; push hidden ; call`).
 // Declaration only: no layout change.
 C3DVector GetHeadPos(bool) const;                                       // 0x00661B40
#endif


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
#ifdef FABLE_RETAIL_GENERIC_STRIKE_RESPONSE_PASS29
    ECombatAnimationHandedness PreviousActionHandedness;
#else
#ifdef FABLE_RETAIL_CREATURE_ONCREATE_PASS27
    ECombatAnimationHandedness PreviousActionHandedness;
#else
    unsigned char PreviousActionHandedness[4];
#endif
#endif
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
#ifdef FABLE_RETAIL_BODY_REORIENT_SLOPE_PASS29
public:
    void SetHadFacingAngleYZSetByBodyOrientation(bool value) { HadFacingAngleYZSetByBodyOrientation=value; }
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): FableWin.pdb's inline setters
    // ?SetLeaveDeadBodyOverride@CThingCreatureBase@@QAEX_N@Z, ?SetLeaveExperienceOrbsOverride@CThingCreatureBase@@QAEX_N@Z and
    // ?SetAsFollowingHeroThroughTeleporters@CThingCreatureBase@@QAEX_N@Z (retail RMWs the +0x15C byte in place). Bodies only.
    void SetLeaveDeadBodyOverride(bool value) { LeaveDeadBodyOverride = value; }
    void SetLeaveExperienceOrbsOverride(bool value) { LeaveExperienceOrbsOverride = value; }
    void SetAsFollowingHeroThroughTeleporters(bool value) { FollowHeroThroughTeleporters = value; }
#endif
protected:
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ISFOLLOWINGTELEPORTERS
public:
    // Opt-in (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad): FableWin.pdb ?IsFollowingHeroThroughTeleporters@CThingCreatureBase@@QBE_NXZ, inline in
    // retail (`test byte [esi+15Ch],40h` = FollowHeroThroughTeleporters, 0x004A3D4F). Body only: no layout change.
    bool IsFollowingHeroThroughTeleporters() const { return FollowHeroThroughTeleporters; }
protected:
#endif // FABLE_RETAIL_CTHINGCREATUREBASE_ISFOLLOWINGTELEPORTERS
#ifdef FABLE_RETAIL_CREATURE_ONCREATE_PASS27
    virtual bool OnCreate();
    void PlayCreatureContinuousSounds();
#endif
    void ReactToGenericStrike(const CHitParameters&);
    // FableWin-only ?PeekDefinition@CThingCreatureBase@@IBEABVCThingCreatureDef@@XZ (0x01D6DD10; inline in Ego_r and retail): the body
    // `return *PDef;` needs the complete CThingCreatureDef, so it is defined inline below once both classes are complete.
    const CThingCreatureDef& PeekDefinition() const;
public:
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_LEAVEORBS
    // Retail opt-in (row 0x00795300): FableWin.pdb ?GetLeaveExperienceOrbsOverride@CThingCreatureBase@@QBE_NXZ and
    // ?SetLeaveExperienceOrbsOverride@CThingCreatureBase@@QAEX_N@Z (bit 5 of the +0x15C flag byte; out-of-line copies 0x0078DDD0 /
    // 0x0078DDB0 in the CTCDivineWrath TU). Bodies only: no layout change.
    bool GetLeaveExperienceOrbsOverride() const { return LeaveExperienceOrbsOverride; }
#ifndef FABLE_RETAIL_CTCTARGETINGAI_ATTACK
    // FableWin.pdb ?GetPLastAttackedByCreature@CThingCreatureBase@@QBEPAV1@XZ (the FABLE_RETAIL_CTCTARGETINGAI_ATTACK body).
    CThingCreatureBase* GetPLastAttackedByCreature()const{return PLastAttackedByCreature.Get();}
#endif
#ifndef FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
    void SetLeaveExperienceOrbsOverride(bool value) { LeaveExperienceOrbsOverride = value; }
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_LASTHANDEDNESS
    // Retail opt-in (chunk 778, row 0x00839420 NThingHelpers::CreateEffectOnHit): Ego_r.pdb / FableWin.pdb
    // ?GetLastActionHandedness@CThingCreatureBase@@QBE?AW4ECombatAnimationHandedness@@XZ, inline in retail (`mov eax,[ebp+138h]`
    // on the striking creature). The enum stays the opaque 4-byte member; the body reads it as its long value. No layout change.
#ifdef FABLE_RETAIL_GENERIC_STRIKE_RESPONSE_PASS29
    ECombatAnimationHandedness GetLastActionHandedness() const { return PreviousActionHandedness; }
#else
    long GetLastActionHandedness() const { return *reinterpret_cast<const long*>(PreviousActionHandedness); }
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CHILD
    // Both original PDBs: public const IsChild; donor GetPrioritisedAbilityDefs and native00661f70.
    bool IsChild()const;
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_SETATTACKEDBYCREATURE
    // Opt-in (chunk 2053, row 0x0087F3A0): Ego_r.pdb / FableWin.pdb ?SetAttackedByCreature@CThingCreatureBase@@QAEXPAV1@@Z (public, out of
    // line at 0x006624F0, landed). Declaration only: no layout change.
    void SetAttackedByCreature(CThingCreatureBase* creature);           // 0x006624F0
#endif
#ifdef FABLE_RETAIL_TARGET_INFO_PASS30
    bool IsBlocking() const;
#endif
    bool IsInMelee()const;bool IsInMeleeAsAttacker()const;
#ifdef FABLE_RETAIL_MAZE_DEFENSIVE_MELEE_PASS27
    bool IsAbleToStrafe() const;                                        // 0x006621A0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED
    // Opt-in (NF210, 2026-11-20, row 0x006EAC50): Ego_r.pdb (0x0053D2C0) + FableWin.pdb (0x01DA3370)
    // ?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ, out of line at 0x00662430 (the IsInMeleeAsAttacker 0x00662410 twin; already bound
    // under this spelling in reloc_bindings). Declaration only: no layout change.
    bool IsInMeleeAsAttacked()const;                                     // 0x00662430
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ISDEMONDOOR
    // Retail opt-in (agentC14, row 0x00603000 CreateMiniMapMarkers; first found by chunk 729): Ego_r.pdb / FableWin.pdb
    // ?IsDemonDoor@CThingCreatureBase@@QBE_NXZ, inline; retail emits it out of line at 0x005FA7F0 (`mov al,[ecx+15Dh]; shr al,3;
    // and al,1`) because CreateMiniMapMarkers takes its address for std::mem_fun_ref.
    bool IsDemonDoor() const { return DemonDoor; }
#endif
    bool IsUnderInfluenceOfEpicSpell() const { return UnderInfluenceOfEpicSpell; }
#ifdef FABLE_RETAIL_DIVINE_FINALCLEANUP
    void SetUnderInfluenceOfEpicSpell(bool value){UnderInfluenceOfEpicSpell=value;} // Both original PDBs; native0078fc48.
#endif
    bool AutoPlaceNearBuilding(const CThingBuilding*,float);
    // (AutoPlaceNear / GetSex: the FABLE_RETAIL_CTHINGCREATUREBASE_AUTOPLACENEAR / _GETSEX declarations above.)
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT_AT(221, offsetof(CThingCreatureBase,PDef)==0xe0);
        FABLE_STATIC_ASSERT_AT(222, offsetof(CThingCreatureBase,PCurrentAction)==0x100);
        FABLE_STATIC_ASSERT_AT(223, offsetof(CThingCreatureBase,PQueuedActions)==0x108);
    }
    virtual void OnHit(CHitParameters&);
#ifdef FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
    bool IsPickPocketable() const; // Both donor PDBs, retail 0x00665E90.
#endif
    bool SetCurrentAction(const CCreatureActionBase&);
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_SETCURRENTACTION_COUNTED
    // NF83 opt-in (round 7, row 0x008D6C80 NCreatureMode::CModeSitBase::FrameUpdate): Ego_r.pdb 0x005429A0 / FableWin.pdb 0x01D94200
    // ?SetCurrentAction@CThingCreatureBase@@QAE_NABV?$CCountedPointer@VCCreatureActionBase@@@@@Z, out of line in retail at 0x00664670
    // (the landed 0x00712B60 binds it so). Declaration only: no layout change.
    bool SetCurrentAction(const CCountedPointer<CCreatureActionBase>&);  // 0x00664670
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_WORLDUPDATE
    // NF66 opt-in (row 0x006666A0): Ego_r.pdb / FableWin.pdb ?WorldUpdate@CThingCreatureBase@@UAE_NXZ (the CThing slot-19 override,
    // retail 0x006666A0) and the protected ?UpdateBodyReorientation@CThingCreatureBase@@IAEXXZ (out of line at 0x006622D0).
    // Declarations only: an override adds no slot and no layout.
    virtual bool WorldUpdate();                                        // 0x006666A0
protected:
    void UpdateBodyReorientation();                                    // 0x006622D0
public:
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_SETPITEMTOUNSHEATHE
    // NF66 opt-in (row 0x00694430): the setter of PItemToUnsheatheAfterCutscene (+0x150) out of line at 0x00662450 (a CIntelligentPointer
    // temporary from the argument, then assigned). No PDB / FSE / Ego_d name: DESCRIPTIVE name, the spelling the landed caller already used.
    void SetPItemToUnsheatheAfterCutscene(CThing* item);               // 0x00662450
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
    // Nested opt-in (chunk 1010, row 0x00896390 CGameScriptInterface::OpenChest): FableWin.pdb ?FinishCurrentAction@CThingCreatureBase@@QAEXXZ,
    // out of line in retail at 0x00662650 (`mov ecx,ebp ; call 0x00662650` before the open-chest SetCurrentAction). Declaration only.
    void FinishCurrentAction();                                          // 0x00662650
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FREETOFINISH
    // Retail opt-in (chunk Q17, row 0x0093C030): Ego_r.pdb ?IsFreeToFinishAction@CThingCreatureBase@@QBE_NXZ (public const), out of
    // line in retail at 0x006621C0. Declaration only: no layout change.
    bool IsFreeToFinishAction() const;                                   // 0x006621C0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
    void AddQueuedAction(const CCreatureActionBase&);
#endif
    // Chunk 751 (row 0x0072C930 CTCSpecialAbilities::SetupFireballSpell): Ego_r.pdb ?IsFreeToPerformAction@CThingCreatureBase@@QBE_NABVCCreatureActionBase@@@Z,
    // out of line in retail at 0x00662210. Declaration only: no layout change.
    bool IsFreeToPerformAction(const CCreatureActionBase&) const;       // 0x00662210
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
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_GETSHOTACCURACYPERCENTAGE
    // Opt-in (wf9, row 0x008C6860 CCreatureAction_TrollThrowProjectile::PerformAction): Ego_r.pdb 0x0053E950 / FableWin.pdb
    // 0x017CE205, 0x01D8C0B0 ?GetShotAccuracyPercentage@CThingCreatureBase@@QBEJXZ (const), out of line in retail at
    // 0x00663B10 (landed row, whose leaf is still the non-const QAEJXZ spelling -- see NAME_FIXES). Declaration only.
    long GetShotAccuracyPercentage() const;                              // 0x00663B10
#endif
    CCombatManager& GetCombatManager();                                  // 0x00661ED0
    // Chunk 774 (rows 0x007A4520 CTCWillEnflame::PlaySoundW, ...): Ego_r.pdb 0x0053CF90 / FableWin.pdb 0x01D9C460
    // ?MakeNoise@CThingCreatureBase@@QAEXABVCCharString@@@Z, out of line in retail at 0x00662110 (landed row). Declaration only.
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_PERSONALSPACE_PASS24
    // Ego_r and FableWin agree on the static method. Native 0x006A05A0 and 0x006A0614 call it.
    static float FABLE_FASTCALL GetPersonalSpaceRadius();              // 0x00661CC0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_EYEGRAPHIC_PASS24
    // Both original PDBs and the native 0x0075430D call.
    const class CEngineGraphic& GetEyeGraphic() const;                 // 0x00662160
#endif
    void MakeNoise(const CCharString& noiseName);                        // 0x00662110
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF
    // Nested opt-in (chunk 777, row 0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk): Ego_r.pdb / FableWin.pdb
    // ?GetPHeroCombatDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ, out of line in retail at 0x00663340
    // (hidden CDefPointer result; landed callers 0x006D6EA0 / 0x006D7E10 / 0x006D7E70). Declaration only: no layout change.
    CDefPointer<const CHeroCombatDef> GetPHeroCombatDef() const;        // 0x00663340
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CLEARCURRENTACTION
    // NF79 opt-in (row 0x008916A0 CGameScriptInterface::SheatheHeroWeapons): Ego_r.pdb 0x0053E2D0 / FableWin.pdb 0x01D9E6D0
    // ?ClearCurrentAction@CThingCreatureBase@@QAEXXZ, out of line in retail at 0x006633A0 (landed row). Declaration only: no layout change.
    void ClearCurrentAction();                                           // 0x006633A0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CLEARQUEUED
    // Nested opt-in (chunk 765, row 0x00663600): Ego_r.pdb / FableWin.pdb ?ClearQueuedActions@CThingCreatureBase@@QAEXXZ, out of line in
    // retail (`add ecx,108h ; jmp` the PQueuedActions list's out-of-line clear 0x00669B50). Declaration only: no layout change.
    void ClearQueuedActions();                                           // 0x00663600
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_LEVELSCRIPT
    // Retail opt-in (chunk 796, row 0x008315C0 CThingAICreature::ConstructFromLevelScript): Ego_r.pdb / FableWin.pdb
    // ?ConstructFromLevelScript@CThingCreatureBase@@UAE_NPBD@Z (public virtual override of CThingGameObject's, landed row 0x00662930),
    // called by name from the CThingAICreature override. Declaration only: no layout change.
    virtual bool ConstructFromLevelScript(const char* script);          // 0x00662930
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMEXISTS
    // Retail opt-in (agentNF9, row 0x00662CC0): Ego_r.pdb / FableWin.pdb ?AnimExists@CThingCreatureBase@@UBE_NABVCCharString@@_N@Z
    // (public const virtual, vtable slot 73; CThingAICreature overrides it at 0x00830F20). Declaration only: no layout change.
    virtual bool AnimExists(const CCharString& name, bool useModeFilterOnly) const;   // 0x00662CC0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CONSTRUCT
    // Retail opt-in (agentNF9, rows 0x00662880 / 0x006628E0): Ego_r.pdb / FableWin.pdb
    // ?ConstructFromParams@CThingCreatureBase@@IAE_NJABVC3DVector@@JPAVCThingPhysical@@1ABVCCharString@@_N@Z (protected, 0x00662880; it
    // forwards to CThingGameObject's eight-argument form) and ?SerialiseGameState@CThingCreatureBase@@UAE_NAAVCPersistContext@@@Z (public
    // virtual, CThingCreatureBase vtable slot 3 = 0x006628E0; Ego_r donor order ConstructFromParams / SerialiseGameState /
    // ConstructFromLevelScript = retail 0x00662880 / 0x006628E0 / 0x00662930). Declarations only: no layout change.
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, CThingPhysical* creator,
                             CThingPhysical* originalCreator, const CCharString& scriptName, bool flag);   // 0x00662880
public:
    virtual bool SerialiseGameState(class CPersistContext& context);    // 0x006628E0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
    // Retail opt-in (chunk 796, row 0x008E9B70 NCreatureMode::CModeStrafe::AnimUpdate): Ego_r.pdb
    // ?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z (const, returns a reference; out of line in retail at
    // 0x00662A00) and ?PeekPCurrentAction@CThingCreatureBase@@QBEPBVCCreatureActionBase@@XZ (inline in retail: `mov eax,[creature+100h]`).
    // Declaration plus an accessor body: no layout change.
    const CAnimationEntry& GetAnimFromName(const CCharString& name, long index) const;   // 0x00662A00
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_PEEKPCURRENTACTION_GET
    // e05 opt-in (egod lane, row 0x006AD700): the same accessor through CCountedPointer::Get (FableWin.pdb
    // ?Get@?$CCountedPointer@VCCreatureActionBase@@@@QBEPAVCCreatureActionBase@@XZ; FableWin's PeekPCurrentAction debug copy is
    // 10i/25b, this text at /Od 10i/25b, the `.Data` text 9i/20b). Same bytes in release.
    const CCreatureActionBase* PeekPCurrentAction() const { return PCurrentAction.Get(); }
#else
    const CCreatureActionBase* PeekPCurrentAction() const { return PCurrentAction.Data; }
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP
    // Retail opt-in (chunk 2026, row 0x0092DA20 CAIStateGroup_MinionSleep::Update): Ego_r.pdb / FableWin.pdb
    // ?SheatheAllWeapons@CThingCreatureBase@@QAEXXZ (public, out of line in retail at 0x006647F0, `mov ecx,<creature> ; call`,
    // 0x0092DCD2; already bound by that spelling) and, unless ANIMQUERY declares it, ?HasAnim@CThingCreatureBase@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z
    // (0x00662C70, 0x0092DB22). Declarations only: no layout change.
    void SheatheAllWeapons();                                                                                                    // 0x006647F0
#ifndef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
    bool HasAnim(const CCharString& name, const class CAnimFilterBase& filter) const;                                           // 0x00662C70
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHERANGEDWEAPON
    // NF247 opt-in (q12679, row 0x00664A10): Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?UnsheatheRangedWeapon@CThingCreatureBase@@QAEXXZ (public,
    // the row's own leaf) and UnsheatheWeapon (out of line in retail at 0x006648D0, `push 1 ; push <def> ; mov ecx,esi ; call`), protected as
    // in all three PDBs (?UnsheatheWeapon@CThingCreatureBase@@IAEXJ_N@Z; NF249 q12690: its self row 0x006648D0 and binders 0x006649C0 /
    // 0x00664A50 moved to IAE in the same unit, so the address keeps one spelling). The `public:` after it restores the region's access.
    // Declarations only: no layout change (same line count as the NF247 form: no line below moves).
    void UnsheatheRangedWeapon();                                                                                                // 0x00664A10
protected: void UnsheatheWeapon(long weapon_def, bool primary); public:                                                         // 0x006648D0
#endif

#ifdef FABLE_RETAIL_CTCCOMBAT_ISTOBLOCK_PASS30
public:
    void UnsheathePrimaryWeapon();
#ifdef FABLE_RETAIL_ENTITY_UNSHEATHE_OLD_BINDER_PASS30
    void UnsheatheAllWeapons();
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON
    // NF253 opt-in (q12711, rows 0x00890C80 / 0x006649C0): Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ
    // (public, out of line in retail at 0x006649C0: `call` from 0x00890CE7, 0x00664A67, 0x00666842, 0x008B7C37, 0x0096B61C). Declaration only.
public:
    void UnsheatheMeleeWeapon();                                                                                                 // 0x006649C0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_SETHANDEDNESS
    // Retail opt-in (chunk 2026, row 0x0094E7B0 CAIStateGroup_EarthTroll::Update): Ego_r.pdb / FableWin.pdb
    // ?SetLastActionHandedness@CThingCreatureBase@@QAEXW4ECombatAnimationHandedness@@@Z (public, out of line in retail at 0x006619F0,
    // `push 0 ; mov ecx,<creature> ; call`, 0x0094ED10; already bound by that spelling). ECombatAnimationHandedness is an MS forward enum
    // here (its enumerator names are in neither PDB name list). Declaration only: no layout change.
    void SetLastActionHandedness(enum ECombatAnimationHandedness handedness);   // 0x006619F0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
    // Retail opt-in (agentNF8, the appearance-animation queries over GetSubDef<CAppearanceDef> 0x0042B0A2): Ego_r.pdb spellings (all
    // public, out of line in retail; Ego_r donor order 0x0053D8E0 .. 0x0053E560 = retail 0x00662A00 .. 0x00663610):
    // ?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@J@Z 0x00662B10,
    // ?GetAnimsWithNameStartingWith@..@@QBEXABVCCharString@@AAV?$vector@PBVCAnimationEntry@@..@std@@@Z 0x00662C10,
    // ?HasAnim@..@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z 0x00662C70,
    // ?GetRandomLoadedAnimFromName@..@@QBEPBVCAnimationEntry@@ABVCCharString@@_N@Z 0x00662FA0, ?PreloadAnims@..@@QAEXXZ 0x00663140,
    // ?PreloadAnimation@..@@QAEXABVCCharString@@@Z 0x006631C0, ?GetCombatAnimationSet@..@@QBEABVCAnimationSet@@XZ 0x00663260,
    // ?IsCombatAnimationValid@..@@QBE_NABVCCharString@@@Z 0x006632A0, ?PreloadBasicAnimations@..@@QAEXXZ 0x00663610. Declarations only.
    const class CAnimationEntry& GetAnimFromName(const CCharString& name, const class CAnimFilterBase& filter, long index) const;   // 0x00662B10
    bool HasAnim(const CCharString& name, const class CAnimFilterBase& filter) const;                                           // 0x00662C70
    const class CAnimationEntry* GetRandomLoadedAnimFromName(const CCharString& name, bool flag) const;                          // 0x00662FA0
    void GetAnimsWithNameStartingWith(const CCharString& prefix, std::vector<const class CAnimationEntry*>& anims) const;        // 0x00662C10
    void PreloadAnims();                                                                                                         // 0x00663140
    void PreloadAnimation(const CCharString& name);                                                                              // 0x006631C0
    const class CAnimationSet& GetCombatAnimationSet() const;                                                                    // 0x00663260
    bool IsCombatAnimationValid(const CCharString& name) const;                                                                  // 0x006632A0
    void PreloadBasicAnimations();                                                                                               // 0x00663610
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_COMBATANIM
    // Retail opt-in (agentNF11, row 0x00667B30): Ego_r.pdb / FableWin.pdb
    // ?GetRandomCombatAnimationFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@@Z (Ego_r 0x00542250, followed by
    // GetPrefferedCombatDistance 0x00542380 = retail 0x00667B30 / 0x00667C00) and
    // ?GetAppropriateCombatAnimationFromList@CThingCreatureBase@@QBEJABV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
    // (out of line at 0x00666B50, landed). Declarations only (needs std::vector from stlport/fable_containers.h).
    const class CAnimationEntry& GetRandomCombatAnimationFromName(const CCharString& name) const;                              // 0x00667B30
    long GetAppropriateCombatAnimationFromList(const std::vector<const class CAnimationEntry*>& anims) const;                 // 0x00666B50
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_POSSIBLECOMBATANIMS
    // Retail opt-in (hand4c, row 0x006668A0): Ego_r.pdb / FableWin.pdb
    // ?GetPossibleCombatAnimations@CThingCreatureBase@@QBE_NABVCCharString@@AAV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
    // (Ego_r release\thing_creature_base.obj 0x00540A20, FableWin debug 0x01D9B740). Declaration only (needs std::vector).
    bool GetPossibleCombatAnimations(const CCharString& name, std::vector<const class CAnimationEntry*>& anim_list) const;      // 0x006668A0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
    // Retail opt-in (chunk 1010b, row 0x008A8C60 CGameScriptInterface::EntitySetAsSittingOnFloor): Ego_r.pdb / FableWin.pdb
    // ?GetRandomAnimFromNameWithFilter@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@@Z (const,
    // returns a reference), out of line in retail at 0x00662EF0. Declaration only: no layout change.
    const class CAnimationEntry& GetRandomAnimFromNameWithFilter(const CCharString& name, const class CAnimFilterBase& filter) const;   // 0x00662EF0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMMOVEMENTDISTANCE
    // NF62 opt-in (2026-11-17, row 0x00747330 CTCStealth::InitialiseSneakSpeed): Ego_r.pdb / FableWin.pdb
    // ?GetAnimMovementDistance@CThingCreatureBase@@QBEMJ@Z (const, by bank index; out of line in retail at 0x006627A0, `push anim ;
    // mov ecx,creature ; call`, the distance in st0). Declaration only: no layout change.
    float GetAnimMovementDistance(long anim_index) const;                // 0x006627A0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
    // Retail opt-in (chunk 802, row 0x008EACB0 NCreatureMode::CModeHoverBase::OnActivate): Ego_r.pdb
    // ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ (out of line in retail at 0x006622A0) and
    // ?GetRandomAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z (0x00662D80; FableWin.pdb the same).
    // Declarations only: no layout change.
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_HIDDENMINIMAP_PASS26
    // Both original PDBs; native006626a0 and original02be4250 caller.
    void SetHiddenOnMiniMap(bool hidden);
#endif
    bool IsPerformingAnyAction() const;                                  // 0x006622A0
    const class CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long index) const;   // 0x00662D80
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMDURATION
    // Nested opt-in (chunk 2010, row 0x00716DE0 CActionTalkBase::Update): Ego_r.pdb / FableWin.pdb ?GetAnimDuration@CThingCreatureBase@@QBEMJ@Z
    // (retail 0x006627F0, landed; the animation's bank index pushed, the duration in st0). Declaration only.
    float GetAnimDuration(long anim_index) const;                        // 0x006627F0
#endif
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_INTERACTION
    // Retail opt-in (chunk 2023, row 0x0094C050 CAIStateGroup_SellToBuyer::Update): FableWin.pdb
    // ?GetPCreatureInteraction@CThingCreatureBase@@QAEAAV?$CCountedPointer@VCCreatureInteraction@@@@XZ (+ the QBE const twin; no Ego_r.pdb
    // or Fable.exe copy: inline, the row reads the Data word +0x12C in place) and Ego_r.pdb / FableWin.pdb
    // ?RemoveFromCreatureInteraction@CThingCreatureBase@@QAEXXZ, out of line at 0x006629B0 (bound there; called at 0x0094C71B). No layout change.
    CCountedPointer<CCreatureInteraction>& GetPCreatureInteraction() { return PCreatureInteraction; }
    const CCountedPointer<CCreatureInteraction>& GetPCreatureInteraction() const { return PCreatureInteraction; }
    void RemoveFromCreatureInteraction();                                // 0x006629B0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_SAVE
    // Nested opt-in (chunk 794, row 0x008304A0 CThingAICreature::Save): Ego_r.pdb ?Save@CThingCreatureBase@@UAEXAAVCCharString@@@Z (public
    // virtual), retail 0x00661A60 (the catalog row CThingCreatureBase::Save: `jmp 0x006A4BA0`, CThingPhysical::Save), called by name from the
    // CThingAICreature override. Declaration only: no layout change.
    virtual void Save(CCharString& out);                                 // 0x00661A60
#endif
protected:
    virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);   // 0x00663C40
public:
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual void DrawUpdate(CInterpolationInfo const *);  // slot 26, 0x00661A40
public:
    virtual bool IsHidden() const;  // slot 50, 0x0052AB80
public:
    virtual long GetMaterialOnHitByMelee() const;  // slot 70, 0x00661D00
public:
    virtual bool IsNewAttackerGreaterThreat(CThingCreatureBase const &, CThingCreatureBase const &) const;  // slot 74, 0x0052AB90
public:
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS2
// agentFAM04 (2026-11-11): retail CThingCreatureBase virtuals (TLC vtable slots aligned with the Xbox 360 Test PDB order;
// Ego_r.pdb / FableWin.pdb spellings) and the protected non-virtual
// ?GetMovementSpeedFromCreatureMode@CThingCreatureBase@@IBEMABVC3DVector@@W4EMovementType@@@Z (out of line at 0x00663BA0; the
// CThingAICreature hover-speed overrides call it with AI_MOVEMENT_HOVER_SLOW / _MEDIUM / _FAST). Declarations only (no layout
// change). Needs EMovementType declared (engine/EMovementType.h).
public:
    virtual void CopyGameState(const CThing&);                           // slot 7, 0x006619D0
    virtual void ForceAnim(long, bool);                                  // slot 68, 0x00663E90
    virtual void ForceAnim(const CCharString&, bool);                    // slot 69, 0x006635D0
protected:
    virtual unsigned long GetPhonemeAnim(const CCharString&) const;      // slot 71, 0x00666850
public:
    virtual bool IsWillable() const;                                     // slot 75, 0x00661FD0
protected:
    float GetMovementSpeedFromCreatureMode(const C3DVector&, EMovementType) const;   // 0x00663BA0
public:
#endif // FABLE_RETAIL_CTHINGCREATUREBASE_VSLOTS2
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_UPDATEANIMATION
    // Retail opt-in (w22 agent2007, row 0x00665860): Ego_r.pdb / FableWin.pdb ?UpdateAnimation@CThingCreatureBase@@IAEXM@Z (protected,
    // retail 0x00665860; Ego_r donor order UpdateAnimation 0x00541160 / UpdateAnimationBulletTime 0x00541780 = retail 0x00665860 /
    // 0x00665DE0, the second a `push 0 ; call 0x00665860 ; ret` forwarder) and its callees, out of line in retail:
    // ?GetAnimationSpeedMultiplier@CThingCreatureBase@@QBEMXZ 0x006655A0, ?IsAnimCombinationAnim@CThingCreatureBase@@QBE_NJ@Z 0x00662820
    // (Ego_r donor order IsAnimCombinationAnim 0x0053D700 before GetAnimFromName 0x0053D8E0 = retail 0x00662A00). Declarations only.
    float GetAnimationSpeedMultiplier() const;                           // 0x006655A0
    bool IsAnimCombinationAnim(long anim) const;                         // 0x00662820
protected:
    void UpdateAnimation(float time);                                    // 0x00665860
    void UpdateAnimationBulletTime();                                    // 0x00665DE0
public:
#endif
};
FABLE_STATIC_ASSERT_AT(434, sizeof(CThingCreatureBase)==0x160);
#ifdef FABLE_ENGINE_CTHINGCREATUREDEF_H
inline const CThingCreatureDef& CThingCreatureBase::PeekDefinition() const { return *PDef; }
#endif
#if defined(FABLE_RETAIL_CTHINGCREATUREBASE_COMBATANIMSET_SAMETU) && defined(FABLE_ENGINE_CAPPEARANCEDEF_H)
// Per-TU opt-in (agentNF11, row 0x00667B30): the body of GetCombatAnimationSet (out of line in retail at 0x00663260, landed with this
// body) visible to its own TU, where VC7.1 /O2 auto-inlines it as retail did: GetRandomCombatAnimationFromName fetches the sub-def,
// takes &Animation and releases the def before the lookup (`lea edi,[ecx+34h]` then `dec dword ptr [ecx+4]`, 0x00667B52). Inline here
// so the copy is a COMDAT. Needs FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY, engine/CAppearanceDef.h and the GetSubDef body.
inline const CAnimationSet& CThingCreatureBase::GetCombatAnimationSet() const
{
    CDefPointer<const CAppearanceDef> appearance;
    PBaseDef->GetSubDef(&appearance);
    return appearance->Animation;
}
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
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_ISFREETOPERFORMACTION
    // NF83 opt-in (round 7, row 0x00969AE0 CAIStateGroup_SleepOnFloor::OnReplace) for the flattened view, the same declaration as the
    // retail branch: Ego_r.pdb 0x0053D090 / FableWin.pdb 0x01D9E350 ?IsFreeToPerformAction@CThingCreatureBase@@QBE_NABVCCreatureActionBase@@@Z,
    // out of line in retail at 0x00662210. Declaration only; no layout change.
    bool IsFreeToPerformAction(const CCreatureActionBase&) const;       // 0x00662210
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_CLEARCURRENTACTION
    // NF83 opt-in (round 7, row 0x00692DB0 CCreatureActionBase::EditUpdate) for the flattened view, the same declaration as the retail
    // branch's CLEARCURRENTACTION: Ego_r.pdb 0x0053E2D0 / FableWin.pdb 0x01D9E6D0 ?ClearCurrentAction@CThingCreatureBase@@QAEXXZ, out of
    // line in retail at 0x006633A0. Declaration only; no layout change.
    void ClearCurrentAction();                                           // 0x006633A0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_GETPROPERTY
    // NF115 opt-in (PRIORITY q4488; rows 0x0065C781, 0x00936BA0, 0x00937D20) for the flattened view, the same declaration as the retail
    // branch: FableWin.pdb / Ego_r.pdb ?GetProperty@CThingCreatureBase@@QBEKXZ, out of line in retail at 0x00662010 (`mov eax,[ecx+0E0h] ;
    // mov eax,[eax+0F0h]`: the creature def's Property). Declaration only; no layout change.
    unsigned long GetProperty() const;                                   // 0x00662010
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_FINISHCURRENTACTION
    // NF191 opt-in (q11936; row 0x00890C80) for the flattened view, the same declaration as the retail branch's FINISHACTION block:
    // FableWin.pdb / Ego_r.pdb (0x0053D510) ?FinishCurrentAction@CThingCreatureBase@@QAEXXZ, out of line in retail at 0x00662650.
    // Declaration only; no layout change.
    void FinishCurrentAction();                                          // 0x00662650
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_GETRANDOMANIMFROMNAME
    // NF196 opt-in (q6456; row 0x0093C6D0 CAIStateGroup_StealthSeekEnemy::SetWhatsThatAnimation) for the flattened view, the same declaration
    // as the retail branch's ACTIONQUERY block: Ego_r.pdb / FableWin.pdb ?GetRandomAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z,
    // out of line in retail at 0x00662D80. Declaration only; no layout change.
    const class CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long index) const;   // 0x00662D80
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAT_ANIMSPEED
    // NF55 opt-in (2026-11-17, row 0x00692BC0 CCreatureActionBase::GetFrameUpdateTime) for the flattened view, the same declaration as the
    // retail branch's UPDATEANIMATION block: Ego_r.pdb / FableWin.pdb ?GetAnimationSpeedMultiplier@CThingCreatureBase@@QBEMXZ, out of line
    // in retail at 0x006655A0. Declaration only; no layout change.
    float GetAnimationSpeedMultiplier() const;                           // 0x006655A0
#endif
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

FABLE_STATIC_ASSERT_AT(544, sizeof(CThingCreatureBase) == 0x160);
FABLE_STATIC_ASSERT_AT(545, offsetof(CThingCreatureBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(546, offsetof(CThingCreatureBase, q) == 0x60);
FABLE_STATIC_ASSERT_AT(547, offsetof(CThingCreatureBase, LastMessageEventICreatedID) == 0xd0);
FABLE_STATIC_ASSERT_AT(548, offsetof(CThingCreatureBase, CombatCollisionDebugGraphics) == 0xd4);
FABLE_STATIC_ASSERT_AT(549, offsetof(CThingCreatureBase, PDef_Object) == 0xe0);
FABLE_STATIC_ASSERT_AT(550, offsetof(CThingCreatureBase, ShotAccuracyPercentage) == 0xe4);
FABLE_STATIC_ASSERT_AT(551, offsetof(CThingCreatureBase, InitialPos_X) == 0xe8);
FABLE_STATIC_ASSERT_AT(552, offsetof(CThingCreatureBase, InitialPos_Y) == 0xec);
FABLE_STATIC_ASSERT_AT(553, offsetof(CThingCreatureBase, InitialPos_Z) == 0xf0);
FABLE_STATIC_ASSERT_AT(554, offsetof(CThingCreatureBase, PLastAttackedByCreature) == 0xf4);
FABLE_STATIC_ASSERT_AT(555, offsetof(CThingCreatureBase, WFLastAttackedByCreature) == 0xfc);
FABLE_STATIC_ASSERT_AT(556, offsetof(CThingCreatureBase, PCurrentAction_Data) == 0x100);
FABLE_STATIC_ASSERT_AT(557, offsetof(CThingCreatureBase, PCurrentAction_Info) == 0x104);
FABLE_STATIC_ASSERT_AT(558, offsetof(CThingCreatureBase, PQueuedActions) == 0x108);
FABLE_STATIC_ASSERT_AT(559, offsetof(CThingCreatureBase, MovementVector_X) == 0x10c);
FABLE_STATIC_ASSERT_AT(560, offsetof(CThingCreatureBase, MovementVector_Y) == 0x110);
FABLE_STATIC_ASSERT_AT(561, offsetof(CThingCreatureBase, MovementVector_Z) == 0x114);
FABLE_STATIC_ASSERT_AT(562, offsetof(CThingCreatureBase, HeadPosOffset_X) == 0x118);
FABLE_STATIC_ASSERT_AT(563, offsetof(CThingCreatureBase, HeadPosOffset_Y) == 0x11c);
FABLE_STATIC_ASSERT_AT(564, offsetof(CThingCreatureBase, HeadPosOffset_Z) == 0x120);
FABLE_STATIC_ASSERT_AT(565, offsetof(CThingCreatureBase, IdleCounter) == 0x124);
FABLE_STATIC_ASSERT_AT(566, offsetof(CThingCreatureBase, TurnSpeed) == 0x128);
FABLE_STATIC_ASSERT_AT(567, offsetof(CThingCreatureBase, PCreatureInteraction_Data) == 0x12c);
FABLE_STATIC_ASSERT_AT(568, offsetof(CThingCreatureBase, PCreatureInteraction_Info) == 0x130);
FABLE_STATIC_ASSERT_AT(569, offsetof(CThingCreatureBase, PTCModeManager) == 0x134);
FABLE_STATIC_ASSERT_AT(570, offsetof(CThingCreatureBase, PreviousActionHandedness) == 0x138);
FABLE_STATIC_ASSERT_AT(571, offsetof(CThingCreatureBase, PreviousActionHandednessWF) == 0x13c);
FABLE_STATIC_ASSERT_AT(572, offsetof(CThingCreatureBase, BodyReorienter_ptr) == 0x140);
FABLE_STATIC_ASSERT_AT(573, offsetof(CThingCreatureBase, CombatDebugGraphics) == 0x144);
FABLE_STATIC_ASSERT_AT(574, offsetof(CThingCreatureBase, PItemToUnsheatheAfterCutscene) == 0x150);
FABLE_STATIC_ASSERT_AT(575, offsetof(CThingCreatureBase, DebugText) == 0x158);
FABLE_STATIC_ASSERT_AT(576, offsetof(CThingCreatureBase, HeadPosCached) == 0x15c);
FABLE_STATIC_ASSERT_AT(577, offsetof(CThingCreatureBase, HiddenOnMiniMap) == 0x15d);
FABLE_STATIC_ASSERT_AT(578, offsetof(CThingCreatureBase, IsShowingDebugInfo) == 0x15e);

#endif // FABLE_RETAIL_CTHINGCREATUREBASE
#endif // FABLE_ENGINE_CTHINGCREATUREBASE_H
