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
class CCreatureInteraction; class CTCCreatureModeManager;
class CHeroCombatDef;
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
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

#ifdef FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
public:bool IsThingInFOV(const CThing&,bool)const;
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
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_COMBATRADIUS
public:
 // Retail opt-in (Codex pass18): both PDBs QBEMXZ; native00661f90, sphere caller00814ea0.
 float GetCombatRadius() const;
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
public:
 CDefPointer<const CCombatTypeDef> GetPCombatTypeDef()const;
 long GetCombatType()const;
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
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): FableWin.pdb's inline setters
    // ?SetLeaveDeadBodyOverride@CThingCreatureBase@@QAEX_N@Z, ?SetLeaveExperienceOrbsOverride@CThingCreatureBase@@QAEX_N@Z and
    // ?SetAsFollowingHeroThroughTeleporters@CThingCreatureBase@@QAEX_N@Z (retail RMWs the +0x15C byte in place). Bodies only.
    void SetLeaveDeadBodyOverride(bool value) { LeaveDeadBodyOverride = value; }
    void SetLeaveExperienceOrbsOverride(bool value) { LeaveExperienceOrbsOverride = value; }
    void SetAsFollowingHeroThroughTeleporters(bool value) { FollowHeroThroughTeleporters = value; }
#endif
protected:
    void ReactToGenericStrike(const CHitParameters&);
    // FableWin-only ?PeekDefinition@CThingCreatureBase@@IBEABVCThingCreatureDef@@XZ (0x01D6DD10; inline in Ego_r and retail): the body
    // `return *PDef;` needs the complete CThingCreatureDef, so it is defined inline below once both classes are complete.
    const CThingCreatureDef& PeekDefinition() const;
public:
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_LASTHANDEDNESS
    // Retail opt-in (chunk 778, row 0x00839420 NThingHelpers::CreateEffectOnHit): Ego_r.pdb / FableWin.pdb
    // ?GetLastActionHandedness@CThingCreatureBase@@QBE?AW4ECombatAnimationHandedness@@XZ, inline in retail (`mov eax,[ebp+138h]`
    // on the striking creature). The enum stays the opaque 4-byte member; the body reads it as its long value. No layout change.
    long GetLastActionHandedness() const { return *reinterpret_cast<const long*>(PreviousActionHandedness); }
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_CHILD
    // Both original PDBs: public const IsChild; donor GetPrioritisedAbilityDefs and native00661f70.
    bool IsChild()const;
#endif
    bool IsInMelee()const;bool IsInMeleeAsAttacker()const;
    bool IsUnderInfluenceOfEpicSpell() const { return UnderInfluenceOfEpicSpell; }
#ifdef FABLE_RETAIL_DIVINE_FINALCLEANUP
    void SetUnderInfluenceOfEpicSpell(bool value){UnderInfluenceOfEpicSpell=value;} // Both original PDBs; native0078fc48.
#endif
    bool AutoPlaceNearBuilding(const CThingBuilding*,float);
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PDef)==0xe0);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PCurrentAction)==0x100);
        FABLE_STATIC_ASSERT(offsetof(CThingCreatureBase,PQueuedActions)==0x108);
    }
    virtual void OnHit(CHitParameters&);
    bool SetCurrentAction(const CCreatureActionBase&);
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
    // Nested opt-in (chunk 1010, row 0x00896390 CGameScriptInterface::OpenChest): FableWin.pdb ?FinishCurrentAction@CThingCreatureBase@@QAEXXZ,
    // out of line in retail at 0x00662650 (`mov ecx,ebp ; call 0x00662650` before the open-chest SetCurrentAction). Declaration only.
    void FinishCurrentAction();                                          // 0x00662650
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
    CCombatManager& GetCombatManager();                                  // 0x00661ED0
    // Chunk 774 (rows 0x007A4520 CTCWillEnflame::PlaySoundW, ...): Ego_r.pdb 0x0053CF90 / FableWin.pdb 0x01D9C460
    // ?MakeNoise@CThingCreatureBase@@QAEXABVCCharString@@@Z, out of line in retail at 0x00662110 (landed row). Declaration only.
    void MakeNoise(const CCharString& noiseName);                        // 0x00662110
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF
    // Nested opt-in (chunk 777, row 0x00803540 CTCTargetingPlayer::UpdateTargetedPositionRandomWalk): Ego_r.pdb / FableWin.pdb
    // ?GetPHeroCombatDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ, out of line in retail at 0x00663340
    // (hidden CDefPointer result; landed callers 0x006D6EA0 / 0x006D7E10 / 0x006D7E70). Declaration only: no layout change.
    CDefPointer<const CHeroCombatDef> GetPHeroCombatDef() const;        // 0x00663340
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
    const CCreatureActionBase* PeekPCurrentAction() const { return PCurrentAction.Data; }
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
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
    // Retail opt-in (chunk 1010b, row 0x008A8C60 CGameScriptInterface::EntitySetAsSittingOnFloor): Ego_r.pdb / FableWin.pdb
    // ?GetRandomAnimFromNameWithFilter@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@@Z (const,
    // returns a reference), out of line in retail at 0x00662EF0. Declaration only: no layout change.
    const class CAnimationEntry& GetRandomAnimFromNameWithFilter(const CCharString& name, const class CAnimFilterBase& filter) const;   // 0x00662EF0
#endif
#ifdef FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
    // Retail opt-in (chunk 802, row 0x008EACB0 NCreatureMode::CModeHoverBase::OnActivate): Ego_r.pdb
    // ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ (out of line in retail at 0x006622A0) and
    // ?GetRandomAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z (0x00662D80; FableWin.pdb the same).
    // Declarations only: no layout change.
    bool IsPerformingAnyAction() const;                                  // 0x006622A0
    const class CAnimationEntry& GetRandomAnimFromName(const CCharString& name, long index) const;   // 0x00662D80
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
};
FABLE_STATIC_ASSERT(sizeof(CThingCreatureBase)==0x160);
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
