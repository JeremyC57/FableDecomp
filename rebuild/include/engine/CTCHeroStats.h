#ifdef FABLE_RETAIL_CTCHEROSTATS
#ifndef FABLE_ENGINE_CTCHEROSTATS_H
#define FABLE_ENGINE_CTCHEROSTATS_H
// Retail opt-in (chunk 735, CTCInventoryExperience::AddAbilityToList(PC) 0x00570332 / 0x00571280): the thing component on the
// polymorphic CTCBase (define FABLE_RETAIL_CTCBASE), interface type TCI_HERO_STATS = 4 (the rows' inline CThing::GetTC key),
// sizeof 0x1DC (the generated retail layout below; members opaque). ?GetMoralityPercentage@CTCHeroStats@@QBEMXZ (Ego_r.pdb,
// out of line at 0x0057A690).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCHEROSTATS_RENDERDRUNKENNESS_PASS26
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CDefPointer.h"
class CIEngine;
class CSpecialAbilitiesDrunkennessDef;
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_SERIALISE
// Nested opt-in (chunk 849, row 0x0057E2F1 CTCHeroStats::OnSerialise): the typed members the serialiser transfers (map, vectors,
// CCharString, list), so the includes they need come in only under the macro.
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CHeroMarriageInfo.h"
#include "engine/EHeroTrainingStatus.h"
class CPersistContext;
#ifdef FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#include "engine/EHeroExperienceStatCategory.h"
enum EHeroSexuality;                                         // MS forward enum
#endif
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_MARRIAGE
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
class CThingBuilding;
#endif
class CThingAICreature;
enum EMarriageState;                                         // MS forward enum (the row defines the enumerators it uses)
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
namespace NHeroStatModifierEvent {class CBase;}
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_WIVES
#include "stlport/fable_containers.h"
class CThingAICreature; class CHeroMarriageInfo;
enum EMarriageState;                                         // MS forward enum
#endif
class CTCHeroStats : public CTCBase {
#ifdef FABLE_RETAIL_CTCHEROSTATS_TROPHYGAME_PASS26
public:
    // Original FableWin021bc9c3/0249be20 and native member+0xd8; setter parameter val.
#ifndef FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
    long GetNumTrophyGamePlayed() const { return NumTrophyGamePlayed; }
#endif
    void SetNumTrophyGamePlayed(long val) { NumTrophyGamePlayed=val; }
#endif
#ifdef FABLE_RETAIL_GREETING_PASS23
public:
    // Both original PDB constructors: public CThing&; normal fixture lifetimes.
    CTCHeroStats(CThing&);
private:
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:void InformOfEvent(const NHeroStatModifierEvent::CBase&);
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_STATS; }
    float GetMoralityPercentage() const;
#ifdef FABLE_RETAIL_CTCHEROSTATS_WIVES
    // Retail opt-in (acctC wfA1, row 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed): Ego_r.pdb
    // ?GetWivesInMarriageState@CTCHeroStats@@QBEXAAV?$list@PBVCThingAICreature@@V?$allocator@PBVCThingAICreature@@@std@@@std@@W4EMarriageState@@@Z
    // and ?PeekMarriageInfo@CTCHeroStats@@QBE_N_KAAVCHeroMarriageInfo@@@Z, out of line in retail at 0x0057AE53 / 0x0057AC89
    // (calls 0x00682F84 / 0x00683044). Declarations only.
    void GetWivesInMarriageState(std::list<const CThingAICreature*>& wives, EMarriageState state) const;   // 0x0057AE53
    bool PeekMarriageInfo(unsigned __int64 uid, CHeroMarriageInfo& info) const;                          // 0x0057AC89
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_KICK_PASS23
 // Both original PDBs: ordinary public CThing& constructor and kick method.
#ifndef FABLE_RETAIL_GREETING_PASS23
 CTCHeroStats(CThing&);
#endif
 void KickHeroToMostRecentSafeRegion();
#ifndef FABLE_RETAIL_CTCHEROSTATS_CRIME_REPORT
 void GetOutstandingFineAndFramesLeft(const CCharString&,long&,long&);
#endif
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_TRADESKILL
    // Retail opt-in (wf7, rows 0x007735D0 CTCShop::GetAttitudeBuyPriceMultiplier and 0x00773790 GetAttitudeSellPriceMultiplier):
    // Ego_r.pdb (0x0056E5CA) + FableWin.pdb ?GetTradeSkill@CTCHeroStats@@QBEMXZ, out of line in retail at 0x0057C239 (landed).
    // Declaration only.
    float GetTradeSkill() const;                                         // 0x0057C239
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_CRIME_REPORT
    // Both original PDB types and native0057d9fd/0057daf5/0057dc22.
    void UpdateOutstandingCrimes(const CCharString&);
    void GetOutstandingFineAndFramesLeft(const CCharString&,long&,long&);
    void GetCrimeReport(const CCharString&,CWideString&,bool);
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_QUEST_COMPLETION_PASS26
    // FableWin 0x01C5B4B0..0x01C5B590 and Ego_d quest_manager.obj, native +0xBC..+0xC8.
    long GetNumCompletedAQuest() const { return NumCompletedAQuest; }
    long GetNumCompletedACoreQuest() const { return NumCompletedACoreQuest; }
    long GetNumCompletedAFeatQuest() const { return NumCompletedAFeatQuest; }
    long GetNumCompletedAnOptionalQuest() const { return NumCompletedAnOptionalQuest; }
    void SetNumCompletedAQuest(long count) { NumCompletedAQuest = count; }
    void SetNumCompletedACoreQuest(long count) { NumCompletedACoreQuest = count; }
    void SetNumCompletedAFeatQuest(long count) { NumCompletedAFeatQuest = count; }
    void SetNumCompletedAnOptionalQuest(long count) { NumCompletedAnOptionalQuest = count; }
    void AddMoney(long amount, bool display);
    void AddRewardMoney(long amount);
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_FAILEDQUEST
    // Original FW01c5c050/70; Ego typed NumFailedAQuest, native+b8.
    long GetNumFailedAQuest()const{return NumFailedAQuest;}
    void SetNumFailedAQuest(long value){NumFailedAQuest=value;}
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_FOLLOWER_LIMIT
    // Original FableWin01e6e164/01e69a40 and native006d4f2a/0057ab14.
    long GetRenownLevel()const{return RenownLevel;}
    long GetMaxFollowersForRenownLevel(long level)const;
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_KICKPOSITION
    // Nested opt-in (chunk 1012, row 0x0049FFD0 CWorld::SetKickPos): Ego_r.pdb
    // ?SetHeroKickPositionForCurrentRegion@CTCHeroStats@@QAEXABVC3DVector@@M_N@Z (Ego_r 0x00570317), out of line in retail at 0x0057D990
    // (`push pos ; push angle ; push flag ; mov ecx,stats ; call`, 0x004A00C6). Declaration only.
    void SetHeroKickPositionForCurrentRegion(const C3DVector& position, float angleXY, bool flag);   // 0x0057D990
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_ADDRENOWN
    // Nested opt-in (chunk 1011, row 0x005EB590 CTCTavernGame::GivePrize): Ego_r.pdb ?AddRenown@CTCHeroStats@@QAEXJ@Z (Ego_r 0x0056FD53),
    // out of line in retail at 0x0057D8BE (`push [def+184h] ; mov ecx,stats ; call 0x0057D8BE`, 0x005EB6BA). Declaration only.
    void AddRenown(long amount);                                         // 0x0057D8BE
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_TROPHYWITNESSES
    // Nested opt-in (w22 agent2012, row 0x006DE280 CTCTrophy::TerminateMinigame): FableWin.pdb ?GetBestTrophyWitnesses@CTCHeroStats@@QBEJXZ
    // (0x021BC9D9) and ?SetBestTrophyWitnesses@CTCHeroStats@@QAEXJ@Z (COMDAT 0x0249D490, emitted right after TerminateMinigame 0x0249BE40),
    // inline in retail (`cmp ebx,[ebp+0DCh] ; jle ; mov [ebp+0DCh],ebx`, 0x006DE571..0x006DE579). No layout change.
    long GetBestTrophyWitnesses() const { return BestTrophyWitnesses; }
    void SetBestTrophyWitnesses(long value) { BestTrophyWitnesses = value; }
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_PEEKMARRIAGEINFO
    // Overlay opt-in (wfD4, row 0x006D1E30): Ego_r.pdb 0x0056E06C ?PeekMarriageInfo@CTCHeroStats@@QBE_N_KAAVCHeroMarriageInfo@@@Z, out of line
    // in retail at 0x0057AC89 (called once by 0x006D1E30). Declaration only.
    bool PeekMarriageInfo(unsigned __int64 uid, class CHeroMarriageInfo& info) const;   // 0x0057AC89
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_LOVERS
    // Overlay opt-in (wfD4 s8, row 0x006D1E30): FableWin.pdb ?LogLoverAcquired@CTCHeroStats@@QAEXXZ (0x024884F0, 32 bytes / 12 instructions at /Od
    // = `NumLoversAcquired++`, its COMDAT in the CTCWife TU run) and ?GetNumberOfLoversAcquired@CTCHeroStats@@QBEJXZ (0x021BCADD); inline (no Ego_r copy).
    void LogLoverAcquired() { NumLoversAcquired++; }
    long GetNumberOfLoversAcquired() const { return NumLoversAcquired; }
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
    // Opt-in (chunk 1008, row 0x005EE77C CTCInventoryAbilities::GetExperienceCostToUpgradeAbility): Ego_r.pdb
    // ?GetMoralityMax@CTCHeroStats@@QBEJXZ (public const), out of line at 0x0057A67E (landed). Declaration only: no layout change.
    long GetMoralityMax() const;                                          // 0x0057A67E
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_DRUNKENNESS
    // Retail opt-in (agentNF8, rows 0x0057A874 / 0x0057A89D): Ego_r.pdb + FableWin.pdb ?IsDrunk@CTCHeroStats@@QBE_NXZ and
    // ?GetMaxDrunkenness@CTCHeroStats@@QBEMXZ (public const), out of line in retail at 0x0057A874 / 0x0057A89D (both read the
    // hero-stats def through CTCBase::GetDefinitionManager: PintsToDrunk +0xA0 / PintsToVomit +0xA4). Declarations only.
    bool IsDrunk() const;                                                 // 0x0057A874
#ifdef FABLE_RETAIL_CTCHEROSTATS_RENDERDRUNKENNESS_PASS26
    // Ego_r 0x0056F397 and FableWin 0x01E6A6F7. The original getter is FableWin 0x01E6AE0A.
    void RenderHeroDrunkenness(CIEngine* engine);
    float GetDrunkenness() const { return Drunkenness; }
#endif
    float GetMaxDrunkenness() const;                                      // 0x0057A89D
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_ONHEALTHMODIFIED
    // Retail opt-in (agentNF8, row 0x0057A753): Ego_r.pdb + FableWin.pdb ?OnHealthModified@CTCHeroStats@@QAEXXZ (public), out of line
    // in retail at 0x0057A753 (Ego_r donor order GetMoralityMax 0x0056DA4B / OnHealthModified 0x0056DB3E / IsDrunk 0x0056DE4A =
    // retail 0x0057A67E / 0x0057A753 / 0x0057A874; CThingPhysical::InternalModifyHealth calls it on TC 4 after a health change;
    // the body resets FramesUntilHealthRefills +0x64). Declaration only.
    void OnHealthModified();                                              // 0x0057A753
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_STAMINA
    // Retail opt-in (Codex pass 17): exact public float method in both donors.
    void ForceStaminaTo(float);
#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
    void AddMoney(long amount, bool display_change);
#endif

#ifdef FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
    void SoberUp();
#endif

#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA
    // Nested opt-in (w22 agent2027, row 0x007C6E50 CTCTurncoatSpell::FrameUpdate; needs FABLE_RETAIL_CTCHEROSTATS_STAMINA): Ego_r.pdb /
    // FableWin.pdb ?ModifyStaminaBy@CTCHeroStats@@QAEXJ@Z (out of line in retail at 0x0057B1DB, the bound spelling; `push ecx ; mov ecx,
    // <stats> ; call`, 0x007C7319) and FableWin.pdb's inline ?GetStamina@CTCHeroStats@@QBEJXZ (no Ego_r.pdb / Fable.exe copy: the row
    // reads Stamina +0x58 in place, `mov eax,[ebx+58h] ; test eax,eax ; jle`, 0x007C73A0). No layout change.
    void ModifyStaminaBy(long amount);                                   // 0x0057B1DB
#ifdef FABLE_RETAIL_CONTEXTSENSITIVEITEM_QUERY_PASS28
    long GetMaxStamina() const { return MaxStamina; } // FableWin 0x01EF53A0, retail health/mana override reads +0x5C.
#endif
    long GetStamina() const { return Stamina; }
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_ADDEXPERIENCE
    // Retail opt-in (Codex pass 17): both PDBs prove category parameter, native0057a8da.
    void AddHeroStatExperience(EHeroExperienceStatCategory,long);
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_MARRIAGE
    // Retail opt-in (chunk 754, row 0x006D00A0 CTCWife::Murdered): Ego_r.pdb ?SetMarriageState@CTCHeroStats@@AAEXPBVCThingAICreature@@W4EMarriageState@@@Z
    // (private; retail 0x0057BC6E, called by CTCWife). Declaration only: no layout change.
private:
    friend class CTCWife;
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
    void AddNewMarriage(const CThingAICreature*,const CThingBuilding*,EMarriageState);
#endif
    void SetMarriageState(const CThingAICreature* wife, EMarriageState state);   // 0x0057BC6E
public:
#endif
#ifdef FABLE_RETAIL_GREETING_PASS23
    // Both original PDBs: TrainingStatus; native00683c85 reads +114.
    EHeroTrainingStatus GetTrainingStatus() const { return TrainingStatus; }
private:
    unsigned char _greet_before_training[0x114-sizeof(CTCBase)];
    EHeroTrainingStatus TrainingStatus;
    unsigned char _greet_after_training[0x1dc-0x118];
public:
#elif defined(FABLE_RETAIL_CTCHEROSTATS_MORALITY)
    // Nested opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): Ego_r.pdb Morality (long; PDB +0x30, retail +0x28: OutstandingCrimes
    // map and ExperienceSpentOn vector are 4 bytes smaller each in STLport, OnHit reads `[esi+28h]`), FableWin.pdb's inline
    // ?GetMorality@CTCHeroStats@@QBEJXZ and Ego_r.pdb ?SetMorality@CTCHeroStats@@QAEXJ@Z (retail 0x0057C2E6, out of line).
    long GetMorality() const { return Morality; }
    void SetMorality(long morality);                                     // 0x0057C2E6
    // Morality is reached directly, not through the inline getter: OnHit reads `[esi+28h]` after the PeekDefinitionManager call as the
    // second operand of the add (the inline GetMorality() form is hoisted above the call, 2 lines off), as the landed hero-stat
    // modifier rows do (0x008F5BF0 / 0x008F5C20 / 0x008F5C50 `stats->Morality + ...` from three unrelated classes) -- so public.
private:
    unsigned char _pad_0x0c[0x28 - 0x0c];                                // +0x0c OutstandingCrimes .. NumberOfDecapitations (opaque)
public:
    long Morality;                                                       // +0x28
private:
    unsigned char _pad_0x2c[0x1dc - 0x2c];                               // +0x2c MoralityChangingEnabled .. (opaque)
public:
#elif defined(FABLE_RETAIL_CTCHEROSTATS_BOASTS)
    // Retail opt-in (chunk 795, row 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate): Ego_r.pdb ?AddMoney@CTCHeroStats@@QAEXJ_N@Z (retail
    // 0x0057B327, landed), ?AddRewardMoney@CTCHeroStats@@QAEXJ@Z (0x0057A59B, landed) and NumBoastsCompleted (+0xD0, the generated
    // default branch's offset; the row's `inc dword ptr [ebp+0D0h]`, 0x005E7D35).
    void AddMoney(long amount, bool flag);                               // 0x0057B327
    void AddRewardMoney(long amount);                                    // 0x0057A59B
#ifdef FABLE_RETAIL_CTCHEROSTATS_BOASTTAKEN
    // Retail opt-in (Codex pass18): original01c58eb0/ed0, native004b1af6 increments+cc.
    long GetNumBoastsTaken()const {return NumBoastsTaken;}
    void SetNumBoastsTaken(long value) {NumBoastsTaken=value;}
    unsigned char _pad_after_base[0xcc-sizeof(CTCBase)];
    long NumBoastsTaken;
#else
    unsigned char _pad_after_base[0xd0 - sizeof(CTCBase)];
#endif
    long NumBoastsCompleted;                                             // +0xd0
#ifdef FABLE_RETAIL_CTCHEROSTATS_BOASTS_COMPLETED_ACCESSORS
    long GetNumBoastsCompleted()const{return NumBoastsCompleted;}
    void SetNumBoastsCompleted(long val){NumBoastsCompleted=val;}
#endif
    unsigned char _pad_0xd4[0x1dc - 0xd4];
#elif defined(FABLE_RETAIL_CTCHEROSTATS_BRIBE)
    // Retail opt-in (chunk 795, row 0x0092FAB0 CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction): Money +0x3C (the
    // row's `mov edi,[eax+3Ch]` against CTCVillage::GetCostToBribe) and TrainingStatus +0x114 (EHeroTrainingStatus, the row's
    // `mov ecx,[eax+114h] ; test ecx,ecx`), the generated default branch's offsets and Ego_r.pdb names.
    unsigned char _pad_after_base[0x3c - sizeof(CTCBase)];
    long Money;                                                          // +0x3c
#ifdef FABLE_RETAIL_CTCHEROSTATS_GETMONEY
    // Nested opt-in (chunk 867, row 0x005A8C66 CTCInventoryTrade::ConstructActualSpecialInventoryPC): Ego_r.pdb ?GetMoney@CTCHeroStats@@QBEJXZ,
    // inline in retail (the row reads Money +0x3C in place after the hero's PeekTC: `mov edx,[eax+3Ch]`, 0x005A92E2).
    long GetMoney() const { return Money; }
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
    // Nested opt-in (chunk 867, rows 0x005AF64E BuyItem / 0x005B01FE SellItemPC): the BOASTS branch's AddMoney for a TU that needs the
    // BRIBE layout (Money +0x3C) as well -- Ego_r.pdb ?AddMoney@CTCHeroStats@@QAEXJ_N@Z, out of line at 0x0057B327. Declaration only.
    void AddMoney(long amount, bool flag);                               // 0x0057B327
#endif
#ifdef FABLE_RETAIL_MINION_CONFIRMATION_PASS29
    unsigned char _pad_0x40[0x108 - 0x40];
    long TotalBribeAmount;
    unsigned char _pad_0x10c[0x114 - 0x10c];
    long GetTotalBribeAmount() const { return TotalBribeAmount; }
    void SetTotalBribeAmount(long amount) { TotalBribeAmount = amount; }
#else
    unsigned char _pad_0x40[0x114 - 0x40];
#endif
    long TrainingStatus;                                                 // +0x114 EHeroTrainingStatus
    unsigned char _pad_0x118[0x1dc - 0x118];
#elif defined(FABLE_RETAIL_CTCHEROSTATS_SERIALISE)
    // Nested opt-in (chunk 849, row 0x0057E2F1 ?OnSerialise@CTCHeroStats@@MAEXAAVCPersistContext@@_N@Z, the protected virtual that
    // overrides CTCBase slot 1): the generated default branch's offsets and Ego_r.pdb names, typed (STLport map/vector 0xC, list 4).
    // MirroredCriminalStatus is Ego_r.pdb's 68-byte nested struct (Crimes[10] .. EntranceAngle +0x40); its ctor is out of line
    // (??0MirroredCriminalStatus@CTCHeroStats@@QAE@XZ, retail 0x0057A5DD). The row's offsets: Morality `[ebx+28h]` .. Marriages `[ebx+1D8h]`.
    struct MirroredCriminalStatus {
        long Crimes[10];                                                 // +0x00
        long LastTimeSeenByGuard;                                        // +0x28
        long LastTimeExited;                                             // +0x2c
        bool ExitedUsingTeleporter;                                      // +0x30
        float EntrancePosX;                                              // +0x34
        float EntrancePosY;                                              // +0x38
        float EntrancePosZ;                                              // +0x3c
        float EntranceAngle;                                             // +0x40
        MirroredCriminalStatus();                                        // 0x0057A5DD
    };
protected:
    void OnSerialise(CPersistContext& context, bool in_game);            // 0x0057E2F1 (virtual through CTCBase)
public:
#ifdef FABLE_RETAIL_CTCHEROSTATS_APPLYBONUSITEMEFFECTS
    // Opt-in (hand3c3, row 0x0057C4F1): Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?ApplyBonusItemEffects@CTCHeroStats@@QAEXABVCBonusItemDef@@@Z
    // and the members it calls, all out of line in retail: ?SetFatness@..@@QAEXM@Z 0x0057B169, ?GetMaxExtraMaxHealth@..@@QBEMXZ
    // 0x0057A90B, ?GetMaxExtraMaxStamina@..@@QBEJXZ 0x0057A962, ?Drink@..@@QAEXM@Z 0x0057A82A, ?SetMorality@..@@QAEXJ@Z 0x0057C2E6,
    // ?ModifyStaminaBy@..@@QAEXJ@Z 0x0057B1DB. Declarations only: no layout change.
    void ApplyBonusItemEffects(const class CBonusItemDef& bonus_item_def);   // 0x0057C4F1
    void SetFatness(float val);                                          // 0x0057B169
    float GetMaxExtraMaxHealth() const;                                  // 0x0057A90B
    long GetMaxExtraMaxStamina() const;                                  // 0x0057A962
    void Drink(float drink);                                             // 0x0057A82A
    // Inline getters (FableWin.pdb / Ego_d.pdb ?GetFatness@..@@QBEMXZ, ?GetMorality@..@@QBEJXZ; no retail copy: the row reads +0x38 / +0x28).
    float GetFatness() const { return Fatness; }
    long GetMorality() const { return Morality; }
#ifndef FABLE_RETAIL_CTCHEROSTATS_MORALITY
    void SetMorality(long morality);                                     // 0x0057C2E6
#endif
#ifndef FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA
    void ModifyStaminaBy(long change_in_stamina);                        // 0x0057B1DB
#endif
#endif
    std::map<CCharString,MirroredCriminalStatus> OutstandingCrimes;      // +0x0c
    std::vector<long> ExperienceSpentOn;                                 // +0x18
    long             NumberOfDecapitations;                              // +0x24
    long             Morality;                                           // +0x28
    bool             MoralityChangingEnabled;                            // +0x2c
    float            Age;                                                // +0x30
#ifdef FABLE_RETAIL_CTCHEROSTATS_AGE
    // Retail opt-in (chunk Q8, row 0x006D8980 CTCHeroExperience::UpgradeTrainableStat): Ego_r.pdb / FableWin.pdb
    // ?SetAge@CTCHeroStats@@QAEXM@Z (out of line at retail 0x0057B05F) and FableWin.pdb ?GetAge@CTCHeroStats@@QBEMXZ (inline: the
    // row reads +0x30 in place). No layout change.
    void SetAge(float age);                                              // 0x0057B05F
    float GetAge() const { return Age; }
#endif
    float            SunTan;                                             // +0x34
    float            Fatness;                                            // +0x38
    long             Money;                                              // +0x3c
    long             MaxMoney;                                           // +0x40
    long             ConfiscatedMoney;                                   // +0x44
    long             TotalMoneyEarned;                                   // +0x48
    long             TotalMoneySpent;                                    // +0x4c
    long             RewardMoney;                                        // +0x50
    long             RentMoney;                                          // +0x54
    long             Stamina;                                            // +0x58
    long             MaxStamina;                                         // +0x5c
    long             FramesUntilStaminaRefills;                          // +0x60
    long             FramesUntilHealthRefills;                           // +0x64
    bool             ScriptOverridePreventStaminaRefill;                 // +0x68
    long             FramesUntilFatnessReduces;                          // +0x6c
    long             RenownLevel;                                        // +0x70
    long             RenownMaxLevel;                                     // +0x74
    long             RenownPointsInLevel;                                // +0x78
    long             RenownTotal;                                        // +0x7c
    float            Agreeableness;                                      // +0x80
    float            InherentAttractiveness;                             // +0x84
    float            InherentScariness;                                  // +0x88
    float            ExtraMaxHealth;                                     // +0x8c
    long             ExtraMaxStamina;                                    // +0x90
    long             FramesLeftForSpeedMultiplier;                       // +0x94
    long             MaximumMultiplier;                                  // +0x98
    float            Attractiveness;                                     // +0x9c
    float            Scariness;                                          // +0xa0
    float            AppearanceGoodstrength;                             // +0xa4
    float            SoundRadiusMultiplier;                              // +0xa8
    float            VisibilityMultiplier;                               // +0xac
    long             FrameUpdatedClothingStats;                          // +0xb0
    long             FrameUpdatedTattooStats;                            // +0xb4
    long             NumFailedAQuest;                                    // +0xb8
    long             NumCompletedAQuest;                                 // +0xbc
    long             NumCompletedACoreQuest;                             // +0xc0
    long             NumCompletedAFeatQuest;                             // +0xc4
    long             NumCompletedAnOptionalQuest;                        // +0xc8
    long             NumBoastsTaken;                                     // +0xcc
    long             NumBoastsCompleted;                                 // +0xd0
    long             NumMountedTrophies;                                 // +0xd4
    long             NumTrophyGamePlayed;                                // +0xd8
    long             BestTrophyWitnesses;                                // +0xdc
    long             NumberOfShopsOwned;                                 // +0xe0
    long             NumberOfHousesOwned;                                // +0xe4
    long             NumberOfBuildingsRented;                            // +0xe8
    long             NumberOfVomits;                                     // +0xec
    long             NumberOfDrinks;                                     // +0xf0
    long             NumberOfCrimes;                                     // +0xf4
    long             NumberOfCrimesOfMurder;                             // +0xf8
    long             TotalFine;                                          // +0xfc
    long             FinePaid;                                           // +0x100
    long             TimesThrownOut;                                     // +0x104
    long             TotalBribeAmount;                                   // +0x108
    long             NumTeleports;                                       // +0x10c
    float            MaxChickenThrow;                                    // +0x110
    EHeroTrainingStatus TrainingStatus;                                  // +0x114
    std::vector<long> HeroStatsExperience;                               // +0x118
    std::vector<long> RegionTime;                                        // +0x124
    std::vector<long> NumberOfTimesUsedAbility;                          // +0x130
    long             TotalSpentExperience;                               // +0x13c
    long             NumLoversAcquired;                                  // +0x140
    long             NumWeddings;                                        // +0x144
    long             NumSpousesMurdered;                                 // +0x148
    long             NumSpousesDied;                                     // +0x14c
    long             NumDivorces;                                        // +0x150
    long             NumSex;                                             // +0x154
    bool             HadScriptedSex;                                     // +0x158
    bool             HadScriptedGaySex;                                  // +0x159
    float            BiggestFish;                                        // +0x15c
    unsigned char    HousePositions[0xc];                                // +0x160 vector<pair<const CRegion*,C3DVector> >
    bool             InfiniteStamina;                                    // +0x16c
    CCharString      DeathRecoveryMarkerName;                            // +0x170
    bool             FailQuestOnDeath;                                   // +0x174
    float            BestTimePairs;                                      // +0x178
    float            BestTimeSorting;                                    // +0x17c
    float            BestScoreBlackjack;                                 // +0x180
    float            BestScoreCoinGolfOakVale;                           // +0x184
    float            BestScoreCoinGolfSnowSpire;                         // +0x188
    float            BestScoreShoveHaPenny;                              // +0x18c
    float            BestTimeGuessTheAddition;                           // +0x190
    bool             PrizeGivenPairs;                                    // +0x194
    bool             PrizeGivenSorting;                                  // +0x195
    bool             PrizeGivenBlackjack;                                // +0x196
    bool             PrizeGivenCoinGolfOakVale;                          // +0x197
    bool             PrizeGivenCoinGolfSnowSpire;                        // +0x198
    bool             PrizeGivenShoveHaPenny;                             // +0x199
    bool             PrizeGivenGuessTheAddition;                         // +0x19a
    float            Drunkenness;                                        // +0x19c
    float            BeerReservoir;                                      // +0x1a0
    float            DrinkAffectMultiplier;                              // +0x1a4
    float            DrinkAffectMultiplierMultiplier;                    // +0x1a8
    long             FrameToVomit;                                       // +0x1ac
    long             DeedHandle;                                         // +0x1b0
    long             FrameEndVomit;                                      // +0x1b4
    bool             UpdateDrunkenness;                                  // +0x1b8
#ifdef FABLE_RETAIL_CTCHEROSTATS_RENDERDRUNKENNESS_PASS26
    CDefPointer<const CSpecialAbilitiesDrunkennessDef> PDef;             // +0x1bc
    CEnginePrimitiveHandle PrimitiveHandle;                              // +0x1c0
#else
    unsigned char    PDef[0x4];                                          // +0x1bc CDefPointer<const CSpecialAbilitiesDrunkennessDef>
    unsigned char    PrimitiveHandle[0x8];                               // +0x1c0 CEnginePrimitiveHandle
#endif
    float            UCounter;                                           // +0x1c8
    float            VCounter;                                           // +0x1cc
    float            RotCounter;                                         // +0x1d0
    bool             IsInCutSceneFirstTime;                              // +0x1d4
    std::list<CHeroMarriageInfo> Marriages;                              // +0x1d8
#ifdef FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
    // FITTED body (acctC agentC1): the value passes through a named local. Retail 0x005D4698 / 0x005D00AB `lea edx,[eax+ecx]` (houses
    // Nested opt-in (chunk 4006, rows 0x005D1F0C / 0x005CE0E6 CTCInventoryStats::ConstructPersonalityListPC / ConstructPersonalityList):
    // the statistics the personality page prints. Out of line in retail (Ego_r.pdb + FableWin.pdb manglings, public):
    // ?UpdateAppearanceStats@CTCHeroStats@@QAEX_N@Z 0x0057D23B, ?GetFavouriteWeapon@..@@QBE?AVCWideString@@J@Z 0x0057AEB4,
    // ?GetFavouriteAbility@..@@QBEJXZ 0x0057AA5B, ?GetNumberOfAbilityCasts@..@@QBEJXZ 0x0057AAA8, ?GetNumberOfAvailableAbilities@..@@QBEJXZ
    // 0x0057B3E2, ?GetHeroSexuality@..@@QBE?AW4EHeroSexuality@@XZ 0x0057ACC4, ?GetNumberOfCurrentSpouses@..@@QBEJXZ 0x0057AD1D,
    // ?GetFavouriteRegion@..@@QBE?AVCWideString@@AAJ@Z 0x0057A989, ?GetNumRegionsExplored@..@@QBEJXZ 0x0057AA1B (FableWin.pdb),
    // ?GetDrinkResistance@..@@QBE?AVCWideString@@XZ 0x0057A7B9, ?GetHeroStatExperience@..@@QBEJW4EHeroExperienceStatCategory@@@Z
    // 0x0057A8AF, ?GetPercentageThroughRenownLevel@..@@QBEMXZ 0x0057AAD9.
    void UpdateAppearanceStats(bool);
    CWideString GetFavouriteWeapon(long) const;
    long GetFavouriteAbility() const;
    long GetNumberOfAbilityCasts() const;
    long GetNumberOfAvailableAbilities() const;
    EHeroSexuality GetHeroSexuality() const;
    long GetNumberOfCurrentSpouses() const;
    CWideString GetFavouriteRegion(long&) const;
    long GetNumRegionsExplored() const;
    CWideString GetDrinkResistance() const;
    long GetHeroStatExperience(EHeroExperienceStatCategory) const;
    float GetPercentageThroughRenownLevel() const;
    // Inline one-line getters (FableWin.pdb keeps an out-of-line copy of each, all `QBE..XZ` except GetMaxMoney `QAEJXZ`; Ego_r.pdb and
    // retail have none: the rows read the member in place, e.g. `mov edx,[edi+24h]` 0x005D3285 for GetNumberOfDecapitations).
    float GetFatness() const { return Fatness; }
#ifdef FABLE_RETAIL_DECAPITATE_PASS29
    void SetNumberOfDecapitations(long value){NumberOfDecapitations=value;}
#endif
    long GetNumberOfDecapitations() const { return NumberOfDecapitations; }
    long GetMaximumMultiplier() const { return MaximumMultiplier; }
    long GetNumberOfLoversAcquired() const { return NumLoversAcquired; }
    long GetNumberOfWeddings() const { return NumWeddings; }
    long GetNumberOfDivorces() const { return NumDivorces; }
    long GetNumberOfSpousesMurdered() const { return NumSpousesMurdered; }
    long GetNumberOfTimesHadSex() const { return NumSex; }
    long GetNumTeleports() const { return NumTeleports; }
    long GetNumCompletedAQuest() const { return NumCompletedAQuest; }
    long GetRewardMoney() const { return RewardMoney; }
    long GetNumCompletedACoreQuest() const { return NumCompletedACoreQuest; }
    long GetNumCompletedAnOptionalQuest() const { return NumCompletedAnOptionalQuest; }
    long GetNumBoastsTaken() const { return NumBoastsTaken; }
    long GetNumBoastsCompleted() const { return NumBoastsCompleted; }
    long GetNumMountedTrophies() const { return NumMountedTrophies; }
    long GetNumTrophyGamePlayed() const { return NumTrophyGamePlayed; }
    long GetNumberOfShopsOwned() const { return NumberOfShopsOwned; }
    // Inferred: FITTED body (acctC agentC1): the value passes through a named local. Retail 0x005D4698 / 0x005D00AB `lea edx,[eax+ecx]` (houses
    // the SIB base, shops the index) needs one more record on the houses side of the NUMBUILDINGS sum; written plainly the row gives
    // `[ecx+eax]`. Same out-of-line bytes as retail's COMDAT copy 0x005CC5FD (`mov eax,[ecx+0E4h] ; ret`). The made-up sum helper of
    // the stale chunk-4006 overlay is NOT used: retail keeps this TU's inline getter COMDATs (0x005CC58E..0x005CC685, header order)
    // and has no sum helper among them or anywhere in .text; FableWin.pdb and the Anniversary class list have none either.
    long GetNumberOfHousesOwned() const { long houses = NumberOfHousesOwned; return houses; }
    long GetNumberOfBuildingsRented() const { return NumberOfBuildingsRented; }
    long GetRentMoney() const { return RentMoney; }
    float GetMaxChickenThrow() const { return MaxChickenThrow; }
    float GetBiggestFish() const { return BiggestFish; }
    float GetBestScoreCoinGolfOakVale() const { return BestScoreCoinGolfOakVale; }
    float GetBestScoreCoinGolfSnowSpire() const { return BestScoreCoinGolfSnowSpire; }
    float GetBestScoreShoveHaPenny() const { return BestScoreShoveHaPenny; }
    float GetBestTimeGuessTheAddition() const { return BestTimeGuessTheAddition; }
    float GetBestTimePairs() const { return BestTimePairs; }
    float GetBestTimeSorting() const { return BestTimeSorting; }
    long GetNumberOfVomits() const { return NumberOfVomits; }
    long GetNumberOfDrinks() const { return NumberOfDrinks; }
    long GetNumberOfCrimes() const { return NumberOfCrimes; }
    long GetTotalFine() const { return TotalFine; }
#ifdef FABLE_RETAIL_CTCHEROSTATS_CRIMECOUNTS
    // e10 nested opt-in (row 0x0065CB6B CTCVillage::ReportCrime; needs FABLE_RETAIL_CTCHEROSTATS_PERSONALITY): the inline accessors Ego_d.pdb
    // keeps in tc_village.obj right after ReportCrime (CTCHeroStats::SetNumberOfCrimes / GetNumberOfCrimesOfMurder / SetNumberOfCrimesOfMurder /
    // SetTotalFine; FableWin.pdb ?SetNumberOfCrimes@CTCHeroStats@@QAEXJ@Z family); retail `inc [esi+0F4h]` / `add [esi+0FCh],edi` / `inc [esi+0F8h]`
    // (0x0065CC58 / 0x0065CC5E / 0x0065CD77). Bodies only: no layout change.
    void SetNumberOfCrimes(long val) { NumberOfCrimes = val; }
    long GetNumberOfCrimesOfMurder() const { return NumberOfCrimesOfMurder; }
    void SetNumberOfCrimesOfMurder(long val) { NumberOfCrimesOfMurder = val; }
    void SetTotalFine(long val) { TotalFine = val; }
#endif
    long GetFinePaid() const { return FinePaid; }
    long GetTimesThrownOut() const { return TimesThrownOut; }
    long GetTotalBribeAmount() const { return TotalBribeAmount; }
    // 0x005D5A76: `lea ecx,[edi+40h] ; mov edx,[ecx] ; lea eax,[edi+3Ch] ; cmp edx,[eax] ; jl ; mov eax,ecx ; mov edx,[eax]` -- the
    // larger of MaxMoney and Money through STLport's reference-returning max (FableWin.pdb ?GetMaxMoney@CTCHeroStats@@QAEJXZ, non-const).
    long GetMaxMoney() { return std::max(MaxMoney, Money); }
    long GetTotalMoneyEarned() const { return TotalMoneyEarned; }
    long GetTotalMoneySpent() const { return TotalMoneySpent; }
    long GetSpentExperience() const { return TotalSpentExperience; }
    long GetRenownLevel() const { return RenownLevel; }
    long GetRenownLevelMaxLevel() const { return RenownMaxLevel; }
    // GetAge / GetBestTrophyWitnesses: FABLE_RETAIL_CTCHEROSTATS_AGE / FABLE_RETAIL_CTCHEROSTATS_TROPHYWITNESSES.
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_SPENTEXPERIENCE
    // NF76 opt-in (row 0x005778CA CTCInventoryExperience::CloseInventoryInternalPC; needs FABLE_RETAIL_CTCHEROSTATS_SERIALISE): Ego_r.pdb /
    // FableWin.pdb ?AddSpentExperience@CTCHeroStats@@QAEXJ@Z and ?GetExperienceSpentOn@CTCHeroStats@@QAEAAV?$vector@JV?$allocator@J@std@@@std@@XZ,
    // inline in retail (`add [eax+13Ch],edi` 0x0057792F; `mov eax,[eax+18h] ; lea eax,[eax+ecx*4] ; add [eax],edi` 0x0057793F). Bodies only.
public:
    void AddSpentExperience(long amount) { TotalSpentExperience += amount; }
    std::vector<long>& GetExperienceSpentOn() { return ExperienceSpentOn; }
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_GETRENOWNLEVEL
    // Nested opt-in (chunk 882, row 0x0061E6D0 CTCInventoryQuests::ConstructQuestListPC): FableWin.pdb ?GetRenownLevel@CTCHeroStats@@QBEJXZ
    // (0x01E6E164), inline in retail (absent from Ego_r.pdb's out-of-line set; the row reads RenownLevel +0x70 in place, `cmp [ecx+70h],edi`
    // 0x00620481).
    long GetRenownLevel() const { return RenownLevel; }
#endif
#elif defined(FABLE_RETAIL_CTCHEROSTATS_FINEPAID)
    // Retail opt-in (wf17, row 0x007E2A60 CTCFineDialogue::GlobalUpdate): Ego_r.pdb ?AddMoney@CTCHeroStats@@QAEXJ_N@Z (0x0057B327),
    // ?KickHeroToMostRecentSafeRegion@CTCHeroStats@@QAEXXZ (0x0057DF85, out of line), and the inline Ego_r.pdb accessors
    // ?GetFinePaid@CTCHeroStats@@QBEJXZ / ?SetFinePaid@CTCHeroStats@@QAEXJ@Z / ?GetTimesThrownOut@CTCHeroStats@@QBEJXZ /
    // ?SetTimesThrownOut@CTCHeroStats@@QAEXJ@Z over FinePaid +0x100 / TimesThrownOut +0x104 (the generated default branch's offsets;
    // the row's `mov eax,[esi+100h]` / `mov [esi+104h],eax`).
    void AddMoney(long amount, bool flag);                               // 0x0057B327
    void KickHeroToMostRecentSafeRegion();                               // 0x0057DF85
    long GetFinePaid() const { return FinePaid; }
    void SetFinePaid(long fine_paid) { FinePaid = fine_paid; }
    long GetTimesThrownOut() const { return TimesThrownOut; }
    void SetTimesThrownOut(long times_thrown_out) { TimesThrownOut = times_thrown_out; }
#ifdef FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
    void PrepareToKickHeroToMostRecentSafeRegion();
    long GetMoney() const { return Money; }
private:
    unsigned char _pad_after_base[0x3c - sizeof(CTCBase)];
    long Money;
    unsigned char _pad_after_money[0x100 - 0x40];
public:
#else
    unsigned char _pad_after_base[0x100 - sizeof(CTCBase)];
#endif
    long FinePaid;                                                       // +0x100
    long TimesThrownOut;                                                 // +0x104
    unsigned char _pad_0x108[0x1dc - 0x108];
#elif defined(FABLE_RETAIL_CTCHEROSTATS_PROPERTYOWNED)
    // Retail opt-in (workflow wf8, row 0x0072EB40 CTCActionUseBuyHouse::ReceivedConfirmation): Ego_r.pdb ?AddMoney@CTCHeroStats@@QAEXJ_N@Z
    // (retail 0x0057B327, as the BOASTS branch) with NumberOfShopsOwned +0xE0 / NumberOfHousesOwned +0xE4 (the generated default
    // branch's offsets and Ego_r.pdb names; the row's `inc dword ptr [edi+0E0h]` / `[edi+0E4h]`, 0x0072EC64 / 0x0072EC70).
    void AddMoney(long amount, bool flag);                               // 0x0057B327
    unsigned char _pad_after_base[0xe0 - sizeof(CTCBase)];
    long NumberOfShopsOwned;                                             // +0xe0
    long NumberOfHousesOwned;                                            // +0xe4
    unsigned char _pad_0xe8[0x1dc - 0xe8];
#elif defined(FABLE_RETAIL_CTCHEROSTATS_RENTMONEY)
    // Retail opt-in (chunk 2039, row 0x006ACA30 CThingPlayerCreature::OnCollisionWithObject): Ego_r.pdb ?AddMoney@CTCHeroStats@@QAEXJ_N@Z
    // (out of line at 0x0057B327, landed) and FableWin.pdb's inline ?AddRentMoney@CTCHeroStats@@QAEXJ@Z (0x01DF38C0) /
    // ?GetRentMoney@CTCHeroStats@@QBEJXZ (0x01DF38F0), whose first FableWin COMDAT copies follow OnCollisionWithObject (0x01DF10E0):
    // no Ego_r.pdb or Fable.exe copy, retail adds the gold to RentMoney +0x54 in place (`mov ecx,[ebx+54h] ; add ecx,eax ; mov
    // [ebx+54h],ecx`, 0x006AD14F) and reads the sum back. RentMoney +0x54 is the generated default branch's offset (Ego_r.pdb name).
    void AddMoney(long amount, bool flag);                               // 0x0057B327
    void AddRentMoney(long amount) { RentMoney += amount; }
    long GetRentMoney() const { return RentMoney; }
private:
    unsigned char _pad_after_base[0x54 - sizeof(CTCBase)];
    long RentMoney;                                                      // +0x54
    unsigned char _pad_0x58[0x1dc - 0x58];
public:
#elif defined(FABLE_RETAIL_CTCHEROSTATS_STAMINA)
    // Retail opt-in (Codex pass 17): original fields at native58/5C/60; all remaining bytes retained.
private:unsigned char _before_stamina[0x58-sizeof(CTCBase)];
    long Stamina,MaxStamina,FramesUntilStaminaRefills;
    unsigned char _after_stamina[0x1dc-0x64];
#elif defined(FABLE_RETAIL_CTCHEROSTATS_FATNESS)
    // Nested opt-in layout (w22 agent2012, row 0x006D1250 CTCWife::CheckAppearanceChanges): Ego_r.pdb Fatness (float) at the generated
    // default branch's +0x38 (the row's `fld dword ptr [ecx+38h]` on the hero's TC 4 and the integer copy `mov ecx,[ecx+38h]`,
    // 0x006D180E / 0x006D183A) with FableWin.pdb's inline ?GetFatness@CTCHeroStats@@QBEMXZ (0x01E65BE1).
public:
    float GetFatness() const { return Fatness; }
private:
    unsigned char _pad_after_base[0x38 - sizeof(CTCBase)];
    float Fatness;                                                       // +0x38
    unsigned char _pad_0x3c[0x1dc - 0x3c];
public:
#elif defined(FABLE_RETAIL_CTCHEROSTATS_TROPHYWITNESSES)
    // Nested opt-in layout (w22 agent2012, row 0x006DE280): Ego_r.pdb BestTrophyWitnesses (long) at the generated default branch's +0xDC
    // (retail TerminateMinigame `cmp ebx,[ebp+0DCh]` / `mov [ebp+0DCh],ebx` on the hero's TC 4, 0x006DE571 / 0x006DE579).
private:
    unsigned char _pad_after_base[0xdc - sizeof(CTCBase)];
    long BestTrophyWitnesses;                                            // +0xdc
    unsigned char _pad_0xe0[0x1dc - 0xe0];
public:
#else
    unsigned char _pad_after_base[0x1dc - sizeof(CTCBase)];
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x005814C3
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x005814C6
public:
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual override this declaration lacks, declaration only (no layout change; the vtable exists). Slot from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spelling from the Ego_r / FableWin name sets.
public:
    virtual ETCInterfaceType GetInterfaceType() const;  // slot 21, 0x005814A8
public:
#endif // FABLE_RETAIL_CTCHEROSTATS_VSLOTS3
#ifdef FABLE_RETAIL_CTCHEROSTATS_TOTALS
public:
    // Ego_r.pdb / FableWin.pdb ?GetRenownTotal@CTCHeroStats@@QBEJXZ / ?GetTotalMoneyEarned@CTCHeroStats@@QBEJXZ (inline in retail).
    long GetRenownTotal() const { return RenownTotal; }
    long GetTotalMoneyEarned() const { return TotalMoneyEarned; }
#endif
};
FABLE_STATIC_ASSERT_AT(405, sizeof(CTCHeroStats) == 0x1dc);
#endif
#else
#ifndef FABLE_ENGINE_CTCHEROSTATS_H
#define FABLE_ENGINE_CTCHEROSTATS_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCHeroStats = 0x1fc.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCHeroStats = 0x1dc.
// Retail evidence: 2 trusted landed function(s); reconciliation 1 conflict(s) in 1 file(s) vs 1 agreeing; PINNED by 1 retyped file(s) despite conflicts.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCharStringData;
class CDefPointeeBase;

#pragma pack(push, 1)
class CTCHeroStats {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    unsigned char    OutstandingCrimes[0xc];                 // +0x0c map<CCharString,CTCHeroStats::MirroredCriminalStatus,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus>_>_>
    unsigned char    ExperienceSpentOn[0xc];                 // +0x18 vector<long,std::allocator<long>_>
    long             NumberOfDecapitations;                  // +0x24
    long             Morality;                               // +0x28
    bool             MoralityChangingEnabled;                // +0x2c
    unsigned char    _pad_0x2d[0x3];                         // +0x2d
    float            Age;                                    // +0x30
    float            SunTan;                                 // +0x34
    float            Fatness;                                // +0x38
    long             Money;                                  // +0x3c
    long             MaxMoney;                               // +0x40
    long             ConfiscatedMoney;                       // +0x44
    long             TotalMoneyEarned;                       // +0x48
    long             TotalMoneySpent;                        // +0x4c
    long             RewardMoney;                            // +0x50
    long             RentMoney;                              // +0x54
    long             Stamina;                                // +0x58
    long             MaxStamina;                             // +0x5c
    long             FramesUntilStaminaRefills;              // +0x60
    long             FramesUntilHealthRefills;               // +0x64
    bool             ScriptOverridePreventStaminaRefill;     // +0x68
    unsigned char    _pad_0x69[0x3];                         // +0x69
    long             FramesUntilFatnessReduces;              // +0x6c
    long             RenownLevel;                            // +0x70
    long             RenownMaxLevel;                         // +0x74
    long             RenownPointsInLevel;                    // +0x78
    long             RenownTotal;                            // +0x7c
    float            Agreeableness;                          // +0x80
    float            InherentAttractiveness;                 // +0x84
    float            InherentScariness;                      // +0x88
    float            ExtraMaxHealth;                         // +0x8c
    long             ExtraMaxStamina;                        // +0x90
    long             FramesLeftForSpeedMultiplier;           // +0x94
    long             MaximumMultiplier;                      // +0x98
    float            Attractiveness;                         // +0x9c
    float            Scariness;                              // +0xa0
    float            AppearanceGoodstrength;                 // +0xa4
    float            SoundRadiusMultiplier;                  // +0xa8
    float            VisibilityMultiplier;                   // +0xac
    long             FrameUpdatedClothingStats;              // +0xb0
    long             FrameUpdatedTattooStats;                // +0xb4
    long             NumFailedAQuest;                        // +0xb8
    long             NumCompletedAQuest;                     // +0xbc
    long             NumCompletedACoreQuest;                 // +0xc0
    long             NumCompletedAFeatQuest;                 // +0xc4
    long             NumCompletedAnOptionalQuest;            // +0xc8
    long             NumBoastsTaken;                         // +0xcc
    long             NumBoastsCompleted;                     // +0xd0
    long             NumMountedTrophies;                     // +0xd4
    long             NumTrophyGamePlayed;                    // +0xd8
    long             BestTrophyWitnesses;                    // +0xdc
    long             NumberOfShopsOwned;                     // +0xe0
    long             NumberOfHousesOwned;                    // +0xe4
    long             NumberOfBuildingsRented;                // +0xe8
    long             NumberOfVomits;                         // +0xec
    long             NumberOfDrinks;                         // +0xf0
    long             NumberOfCrimes;                         // +0xf4
    long             NumberOfCrimesOfMurder;                 // +0xf8
    long             TotalFine;                              // +0xfc
    long             FinePaid;                               // +0x100
    long             TimesThrownOut;                         // +0x104
    long             TotalBribeAmount;                       // +0x108
    long             NumTeleports;                           // +0x10c
    float            MaxChickenThrow;                        // +0x110
    unsigned char    TrainingStatus[0x4];                    // +0x114 EHeroTrainingStatus
    unsigned char    HeroStatsExperience[0xc];               // +0x118 vector<long,std::allocator<long>_>
    unsigned char    RegionTime[0xc];                        // +0x124 vector<long,std::allocator<long>_>
    unsigned char    NumberOfTimesUsedAbility[0xc];          // +0x130 vector<long,std::allocator<long>_>
    long             TotalSpentExperience;                   // +0x13c
    long             NumLoversAcquired;                      // +0x140
    long             NumWeddings;                            // +0x144
    long             NumSpousesMurdered;                     // +0x148
    long             NumSpousesDied;                         // +0x14c
    long             NumDivorces;                            // +0x150
    long             NumSex;                                 // +0x154
    bool             HadScriptedSex;                         // +0x158
    bool             HadScriptedGaySex;                      // +0x159
    unsigned char    _pad_0x15a[0x2];                        // +0x15a
    float            BiggestFish;                            // +0x15c
    unsigned char    HousePositions[0xc];                    // +0x160 vector<std::pair<CRegion_const_*,C3DVector>,std::allocator<std::pair<CRegion_const_*,C3DVector>_>_>
    bool             InfiniteStamina;                        // +0x16c
    unsigned char    _pad_0x16d[0x3];                        // +0x16d
    CCharStringData* DeathRecoveryMarkerName_PStringData;    // +0x170 CCharString::PStringData
    bool             FailQuestOnDeath;                       // +0x174
    unsigned char    _pad_0x175[0x3];                        // +0x175
    float            BestTimePairs;                          // +0x178
    float            BestTimeSorting;                        // +0x17c
    float            BestScoreBlackjack;                     // +0x180
    float            BestScoreCoinGolfOakVale;               // +0x184
    float            BestScoreCoinGolfSnowSpire;             // +0x188
    float            BestScoreShoveHaPenny;                  // +0x18c
    float            BestTimeGuessTheAddition;               // +0x190
    bool             PrizeGivenPairs;                        // +0x194
    bool             PrizeGivenSorting;                      // +0x195
    bool             PrizeGivenBlackjack;                    // +0x196
    bool             PrizeGivenCoinGolfOakVale;              // +0x197
    bool             PrizeGivenCoinGolfSnowSpire;            // +0x198
    bool             PrizeGivenShoveHaPenny;                 // +0x199
    bool             PrizeGivenGuessTheAddition;             // +0x19a
    unsigned char    _pad_0x19b[0x1];                        // +0x19b
    float            Drunkenness;                            // +0x19c
    float            BeerReservoir;                          // +0x1a0
    float            DrinkAffectMultiplier;                  // +0x1a4
    float            DrinkAffectMultiplierMultiplier;        // +0x1a8
    long             FrameToVomit;                           // +0x1ac
    long             DeedHandle;                             // +0x1b0
    long             FrameEndVomit;                          // +0x1b4
    bool             UpdateDrunkenness;                      // +0x1b8
    unsigned char    _pad_0x1b9[0x3];                        // +0x1b9
    CDefPointeeBase* PDef_Object;                            // +0x1bc CDefPointer<CSpecialAbilitiesDrunkennessDef_const_>::Object
    unsigned char    PrimitiveHandle[0x8];                   // +0x1c0 CEnginePrimitiveHandle
    float            UCounter;                               // +0x1c8
    float            VCounter;                               // +0x1cc
    float            RotCounter;                             // +0x1d0
    bool             IsInCutSceneFirstTime;                  // +0x1d4
    unsigned char    _pad_0x1d5[0x1];                        // +0x1d5
    unsigned char    _pad_0x1d6[0x2];                        // +0x1d6
    unsigned char    Marriages[0x4];                         // +0x1d8 list<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(549, sizeof(CTCHeroStats) == 0x1dc);
FABLE_STATIC_ASSERT_AT(550, offsetof(CTCHeroStats, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(551, offsetof(CTCHeroStats, OutstandingCrimes) == 0xc);
FABLE_STATIC_ASSERT_AT(552, offsetof(CTCHeroStats, ExperienceSpentOn) == 0x18);
FABLE_STATIC_ASSERT_AT(553, offsetof(CTCHeroStats, NumberOfDecapitations) == 0x24);
FABLE_STATIC_ASSERT_AT(554, offsetof(CTCHeroStats, Morality) == 0x28);
FABLE_STATIC_ASSERT_AT(555, offsetof(CTCHeroStats, MoralityChangingEnabled) == 0x2c);
FABLE_STATIC_ASSERT_AT(556, offsetof(CTCHeroStats, Age) == 0x30);
FABLE_STATIC_ASSERT_AT(557, offsetof(CTCHeroStats, SunTan) == 0x34);
FABLE_STATIC_ASSERT_AT(558, offsetof(CTCHeroStats, Fatness) == 0x38);
FABLE_STATIC_ASSERT_AT(559, offsetof(CTCHeroStats, Money) == 0x3c);
FABLE_STATIC_ASSERT_AT(560, offsetof(CTCHeroStats, MaxMoney) == 0x40);
FABLE_STATIC_ASSERT_AT(561, offsetof(CTCHeroStats, ConfiscatedMoney) == 0x44);
FABLE_STATIC_ASSERT_AT(562, offsetof(CTCHeroStats, TotalMoneyEarned) == 0x48);
FABLE_STATIC_ASSERT_AT(563, offsetof(CTCHeroStats, TotalMoneySpent) == 0x4c);
FABLE_STATIC_ASSERT_AT(564, offsetof(CTCHeroStats, RewardMoney) == 0x50);
FABLE_STATIC_ASSERT_AT(565, offsetof(CTCHeroStats, RentMoney) == 0x54);
FABLE_STATIC_ASSERT_AT(566, offsetof(CTCHeroStats, Stamina) == 0x58);
FABLE_STATIC_ASSERT_AT(567, offsetof(CTCHeroStats, MaxStamina) == 0x5c);
FABLE_STATIC_ASSERT_AT(568, offsetof(CTCHeroStats, FramesUntilStaminaRefills) == 0x60);
FABLE_STATIC_ASSERT_AT(569, offsetof(CTCHeroStats, FramesUntilHealthRefills) == 0x64);
FABLE_STATIC_ASSERT_AT(570, offsetof(CTCHeroStats, ScriptOverridePreventStaminaRefill) == 0x68);
FABLE_STATIC_ASSERT_AT(571, offsetof(CTCHeroStats, FramesUntilFatnessReduces) == 0x6c);
FABLE_STATIC_ASSERT_AT(572, offsetof(CTCHeroStats, RenownLevel) == 0x70);
FABLE_STATIC_ASSERT_AT(573, offsetof(CTCHeroStats, RenownMaxLevel) == 0x74);
FABLE_STATIC_ASSERT_AT(574, offsetof(CTCHeroStats, RenownPointsInLevel) == 0x78);
FABLE_STATIC_ASSERT_AT(575, offsetof(CTCHeroStats, RenownTotal) == 0x7c);
FABLE_STATIC_ASSERT_AT(576, offsetof(CTCHeroStats, Agreeableness) == 0x80);
FABLE_STATIC_ASSERT_AT(577, offsetof(CTCHeroStats, InherentAttractiveness) == 0x84);
FABLE_STATIC_ASSERT_AT(578, offsetof(CTCHeroStats, InherentScariness) == 0x88);
FABLE_STATIC_ASSERT_AT(579, offsetof(CTCHeroStats, ExtraMaxHealth) == 0x8c);
FABLE_STATIC_ASSERT_AT(580, offsetof(CTCHeroStats, ExtraMaxStamina) == 0x90);
FABLE_STATIC_ASSERT_AT(581, offsetof(CTCHeroStats, FramesLeftForSpeedMultiplier) == 0x94);
FABLE_STATIC_ASSERT_AT(582, offsetof(CTCHeroStats, MaximumMultiplier) == 0x98);
FABLE_STATIC_ASSERT_AT(583, offsetof(CTCHeroStats, Attractiveness) == 0x9c);
FABLE_STATIC_ASSERT_AT(584, offsetof(CTCHeroStats, Scariness) == 0xa0);
FABLE_STATIC_ASSERT_AT(585, offsetof(CTCHeroStats, AppearanceGoodstrength) == 0xa4);
FABLE_STATIC_ASSERT_AT(586, offsetof(CTCHeroStats, SoundRadiusMultiplier) == 0xa8);
FABLE_STATIC_ASSERT_AT(587, offsetof(CTCHeroStats, VisibilityMultiplier) == 0xac);
FABLE_STATIC_ASSERT_AT(588, offsetof(CTCHeroStats, FrameUpdatedClothingStats) == 0xb0);
FABLE_STATIC_ASSERT_AT(589, offsetof(CTCHeroStats, FrameUpdatedTattooStats) == 0xb4);
FABLE_STATIC_ASSERT_AT(590, offsetof(CTCHeroStats, NumFailedAQuest) == 0xb8);
FABLE_STATIC_ASSERT_AT(591, offsetof(CTCHeroStats, NumCompletedAQuest) == 0xbc);
FABLE_STATIC_ASSERT_AT(592, offsetof(CTCHeroStats, NumCompletedACoreQuest) == 0xc0);
FABLE_STATIC_ASSERT_AT(593, offsetof(CTCHeroStats, NumCompletedAFeatQuest) == 0xc4);
FABLE_STATIC_ASSERT_AT(594, offsetof(CTCHeroStats, NumCompletedAnOptionalQuest) == 0xc8);
FABLE_STATIC_ASSERT_AT(595, offsetof(CTCHeroStats, NumBoastsTaken) == 0xcc);
FABLE_STATIC_ASSERT_AT(596, offsetof(CTCHeroStats, NumBoastsCompleted) == 0xd0);
FABLE_STATIC_ASSERT_AT(597, offsetof(CTCHeroStats, NumMountedTrophies) == 0xd4);
FABLE_STATIC_ASSERT_AT(598, offsetof(CTCHeroStats, NumTrophyGamePlayed) == 0xd8);
FABLE_STATIC_ASSERT_AT(599, offsetof(CTCHeroStats, BestTrophyWitnesses) == 0xdc);
FABLE_STATIC_ASSERT_AT(600, offsetof(CTCHeroStats, NumberOfShopsOwned) == 0xe0);
FABLE_STATIC_ASSERT_AT(601, offsetof(CTCHeroStats, NumberOfHousesOwned) == 0xe4);
FABLE_STATIC_ASSERT_AT(602, offsetof(CTCHeroStats, NumberOfBuildingsRented) == 0xe8);
FABLE_STATIC_ASSERT_AT(603, offsetof(CTCHeroStats, NumberOfVomits) == 0xec);
FABLE_STATIC_ASSERT_AT(604, offsetof(CTCHeroStats, NumberOfDrinks) == 0xf0);
FABLE_STATIC_ASSERT_AT(605, offsetof(CTCHeroStats, NumberOfCrimes) == 0xf4);
FABLE_STATIC_ASSERT_AT(606, offsetof(CTCHeroStats, NumberOfCrimesOfMurder) == 0xf8);
FABLE_STATIC_ASSERT_AT(607, offsetof(CTCHeroStats, TotalFine) == 0xfc);
FABLE_STATIC_ASSERT_AT(608, offsetof(CTCHeroStats, FinePaid) == 0x100);
FABLE_STATIC_ASSERT_AT(609, offsetof(CTCHeroStats, TimesThrownOut) == 0x104);
FABLE_STATIC_ASSERT_AT(610, offsetof(CTCHeroStats, TotalBribeAmount) == 0x108);
FABLE_STATIC_ASSERT_AT(611, offsetof(CTCHeroStats, NumTeleports) == 0x10c);
FABLE_STATIC_ASSERT_AT(612, offsetof(CTCHeroStats, MaxChickenThrow) == 0x110);
FABLE_STATIC_ASSERT_AT(613, offsetof(CTCHeroStats, TrainingStatus) == 0x114);
FABLE_STATIC_ASSERT_AT(614, offsetof(CTCHeroStats, HeroStatsExperience) == 0x118);
FABLE_STATIC_ASSERT_AT(615, offsetof(CTCHeroStats, RegionTime) == 0x124);
FABLE_STATIC_ASSERT_AT(616, offsetof(CTCHeroStats, NumberOfTimesUsedAbility) == 0x130);
FABLE_STATIC_ASSERT_AT(617, offsetof(CTCHeroStats, TotalSpentExperience) == 0x13c);
FABLE_STATIC_ASSERT_AT(618, offsetof(CTCHeroStats, NumLoversAcquired) == 0x140);
FABLE_STATIC_ASSERT_AT(619, offsetof(CTCHeroStats, NumWeddings) == 0x144);
FABLE_STATIC_ASSERT_AT(620, offsetof(CTCHeroStats, NumSpousesMurdered) == 0x148);
FABLE_STATIC_ASSERT_AT(621, offsetof(CTCHeroStats, NumSpousesDied) == 0x14c);
FABLE_STATIC_ASSERT_AT(622, offsetof(CTCHeroStats, NumDivorces) == 0x150);
FABLE_STATIC_ASSERT_AT(623, offsetof(CTCHeroStats, NumSex) == 0x154);
FABLE_STATIC_ASSERT_AT(624, offsetof(CTCHeroStats, HadScriptedSex) == 0x158);
FABLE_STATIC_ASSERT_AT(625, offsetof(CTCHeroStats, HadScriptedGaySex) == 0x159);
FABLE_STATIC_ASSERT_AT(626, offsetof(CTCHeroStats, BiggestFish) == 0x15c);
FABLE_STATIC_ASSERT_AT(627, offsetof(CTCHeroStats, HousePositions) == 0x160);
FABLE_STATIC_ASSERT_AT(628, offsetof(CTCHeroStats, InfiniteStamina) == 0x16c);
FABLE_STATIC_ASSERT_AT(629, offsetof(CTCHeroStats, DeathRecoveryMarkerName_PStringData) == 0x170);
FABLE_STATIC_ASSERT_AT(630, offsetof(CTCHeroStats, FailQuestOnDeath) == 0x174);
FABLE_STATIC_ASSERT_AT(631, offsetof(CTCHeroStats, BestTimePairs) == 0x178);
FABLE_STATIC_ASSERT_AT(632, offsetof(CTCHeroStats, BestTimeSorting) == 0x17c);
FABLE_STATIC_ASSERT_AT(633, offsetof(CTCHeroStats, BestScoreBlackjack) == 0x180);
FABLE_STATIC_ASSERT_AT(634, offsetof(CTCHeroStats, BestScoreCoinGolfOakVale) == 0x184);
FABLE_STATIC_ASSERT_AT(635, offsetof(CTCHeroStats, BestScoreCoinGolfSnowSpire) == 0x188);
FABLE_STATIC_ASSERT_AT(636, offsetof(CTCHeroStats, BestScoreShoveHaPenny) == 0x18c);
FABLE_STATIC_ASSERT_AT(637, offsetof(CTCHeroStats, BestTimeGuessTheAddition) == 0x190);
FABLE_STATIC_ASSERT_AT(638, offsetof(CTCHeroStats, PrizeGivenPairs) == 0x194);
FABLE_STATIC_ASSERT_AT(639, offsetof(CTCHeroStats, PrizeGivenSorting) == 0x195);
FABLE_STATIC_ASSERT_AT(640, offsetof(CTCHeroStats, PrizeGivenBlackjack) == 0x196);
FABLE_STATIC_ASSERT_AT(641, offsetof(CTCHeroStats, PrizeGivenCoinGolfOakVale) == 0x197);
FABLE_STATIC_ASSERT_AT(642, offsetof(CTCHeroStats, PrizeGivenCoinGolfSnowSpire) == 0x198);
FABLE_STATIC_ASSERT_AT(643, offsetof(CTCHeroStats, PrizeGivenShoveHaPenny) == 0x199);
FABLE_STATIC_ASSERT_AT(644, offsetof(CTCHeroStats, PrizeGivenGuessTheAddition) == 0x19a);
FABLE_STATIC_ASSERT_AT(645, offsetof(CTCHeroStats, Drunkenness) == 0x19c);
FABLE_STATIC_ASSERT_AT(646, offsetof(CTCHeroStats, BeerReservoir) == 0x1a0);
FABLE_STATIC_ASSERT_AT(647, offsetof(CTCHeroStats, DrinkAffectMultiplier) == 0x1a4);
FABLE_STATIC_ASSERT_AT(648, offsetof(CTCHeroStats, DrinkAffectMultiplierMultiplier) == 0x1a8);
FABLE_STATIC_ASSERT_AT(649, offsetof(CTCHeroStats, FrameToVomit) == 0x1ac);
FABLE_STATIC_ASSERT_AT(650, offsetof(CTCHeroStats, DeedHandle) == 0x1b0);
FABLE_STATIC_ASSERT_AT(651, offsetof(CTCHeroStats, FrameEndVomit) == 0x1b4);
FABLE_STATIC_ASSERT_AT(652, offsetof(CTCHeroStats, UpdateDrunkenness) == 0x1b8);
FABLE_STATIC_ASSERT_AT(653, offsetof(CTCHeroStats, PDef_Object) == 0x1bc);
FABLE_STATIC_ASSERT_AT(654, offsetof(CTCHeroStats, PrimitiveHandle) == 0x1c0);
FABLE_STATIC_ASSERT_AT(655, offsetof(CTCHeroStats, UCounter) == 0x1c8);
FABLE_STATIC_ASSERT_AT(656, offsetof(CTCHeroStats, VCounter) == 0x1cc);
FABLE_STATIC_ASSERT_AT(657, offsetof(CTCHeroStats, RotCounter) == 0x1d0);
FABLE_STATIC_ASSERT_AT(658, offsetof(CTCHeroStats, IsInCutSceneFirstTime) == 0x1d4);
FABLE_STATIC_ASSERT_AT(659, offsetof(CTCHeroStats, Marriages) == 0x1d8);

#endif // FABLE_ENGINE_CTCHEROSTATS_H
#endif
