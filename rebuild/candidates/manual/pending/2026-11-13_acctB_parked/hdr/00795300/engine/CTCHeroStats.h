#ifdef FABLE_RETAIL_CTCHEROSTATS
#ifndef FABLE_ENGINE_CTCHEROSTATS_H
#define FABLE_ENGINE_CTCHEROSTATS_H
// Retail opt-in (chunk 735, CTCInventoryExperience::AddAbilityToList(PC) 0x00570332 / 0x00571280): the thing component on the
// polymorphic CTCBase (define FABLE_RETAIL_CTCBASE), interface type TCI_HERO_STATS = 4 (the rows' inline CThing::GetTC key),
// sizeof 0x1DC (the generated retail layout below; members opaque). ?GetMoralityPercentage@CTCHeroStats@@QBEMXZ (Ego_r.pdb,
// out of line at 0x0057A690).
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#ifdef FABLE_RETAIL_CTCHEROSTATS_SERIALISE
// Nested opt-in (chunk 849, row 0x0057E2F1 CTCHeroStats::OnSerialise): the typed members the serialiser transfers (map, vectors,
// CCharString, list), so the includes they need come in only under the macro.
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CHeroMarriageInfo.h"
#include "engine/EHeroTrainingStatus.h"
class CPersistContext;
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
class CTCHeroStats : public CTCBase {
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:void InformOfEvent(const NHeroStatModifierEvent::CBase&);
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HERO_STATS; }
    float GetMoralityPercentage() const;
#ifdef FABLE_RETAIL_CTCHEROSTATS_CRIME_REPORT
    // Both original PDB types and native0057d9fd/0057daf5/0057dc22.
    void UpdateOutstandingCrimes(const CCharString&);
    void GetOutstandingFineAndFramesLeft(const CCharString&,long&,long&);
    void GetCrimeReport(const CCharString&,CWideString&,bool);
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
    float GetMaxDrunkenness() const;                                      // 0x0057A89D
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_ONHEALTHMODIFIED
    // Retail opt-in (agentNF8, row 0x0057A753): Ego_r.pdb + FableWin.pdb ?OnHealthModified@CTCHeroStats@@QAEXXZ (public), out of line
    // in retail at 0x0057A753 (Ego_r donor order GetMoralityMax 0x0056DA4B / OnHealthModified 0x0056DB3E / IsDrunk 0x0056DE4A =
    // retail 0x0057A67E / 0x0057A753 / 0x0057A874; CThingPhysical::InternalModifyHealth calls it on TC 4 after a health change;
    // the body resets FramesUntilHealthRefills +0x64). Declaration only.
    void OnHealthModified();                                              // 0x0057A753
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_MODIFYSTAMINA
    // Retail opt-in (row 0x00795300 CTCUnholyPower::FrameUpdate): FableWin.pdb ?ModifyStaminaBy@CTCHeroStats@@QAEXJ@Z (out of line,
    // 0x0057B1DB) and the inline ?GetStamina@CTCHeroStats@@QBEJXZ (`mov eax,[esi+58h]` in place). No layout change.
    void ModifyStaminaBy(long amount);                                   // 0x0057B1DB
    long GetStamina() const { return Stamina; }
#endif
#ifdef FABLE_RETAIL_CTCHEROSTATS_STAMINA
    // Retail opt-in (Codex pass 17): exact public float method in both donors.
    void ForceStaminaTo(float);
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
#ifdef FABLE_RETAIL_CTCHEROSTATS_MORALITY
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
    unsigned char _pad_0x40[0x114 - 0x40];
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
    unsigned char    PDef[0x4];                                          // +0x1bc CDefPointer<const CSpecialAbilitiesDrunkennessDef>
    unsigned char    PrimitiveHandle[0x8];                               // +0x1c0 CEnginePrimitiveHandle
    float            UCounter;                                           // +0x1c8
    float            VCounter;                                           // +0x1cc
    float            RotCounter;                                         // +0x1d0
    bool             IsInCutSceneFirstTime;                              // +0x1d4
    std::list<CHeroMarriageInfo> Marriages;                              // +0x1d8
#elif defined(FABLE_RETAIL_CTCHEROSTATS_STAMINA)
    // Retail opt-in (Codex pass 17): original fields at native58/5C/60; all remaining bytes retained.
private:unsigned char _before_stamina[0x58-sizeof(CTCBase)];
    long Stamina,MaxStamina,FramesUntilStaminaRefills;
    unsigned char _after_stamina[0x1dc-0x64];
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
};
FABLE_STATIC_ASSERT(sizeof(CTCHeroStats) == 0x1dc);
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

FABLE_STATIC_ASSERT(sizeof(CTCHeroStats) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, OutstandingCrimes) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ExperienceSpentOn) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfDecapitations) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Morality) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MoralityChangingEnabled) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Age) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, SunTan) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Fatness) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Money) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaxMoney) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ConfiscatedMoney) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalMoneyEarned) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalMoneySpent) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RewardMoney) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RentMoney) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Stamina) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaxStamina) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesUntilStaminaRefills) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesUntilHealthRefills) == 0x64);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ScriptOverridePreventStaminaRefill) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesUntilFatnessReduces) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownLevel) == 0x70);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownMaxLevel) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownPointsInLevel) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RenownTotal) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Agreeableness) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, InherentAttractiveness) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, InherentScariness) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ExtraMaxHealth) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, ExtraMaxStamina) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FramesLeftForSpeedMultiplier) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaximumMultiplier) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Attractiveness) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Scariness) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, AppearanceGoodstrength) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, SoundRadiusMultiplier) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, VisibilityMultiplier) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameUpdatedClothingStats) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameUpdatedTattooStats) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumFailedAQuest) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedAQuest) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedACoreQuest) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedAFeatQuest) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumCompletedAnOptionalQuest) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumBoastsTaken) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumBoastsCompleted) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumMountedTrophies) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumTrophyGamePlayed) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTrophyWitnesses) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfShopsOwned) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfHousesOwned) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfBuildingsRented) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfVomits) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfDrinks) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfCrimes) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfCrimesOfMurder) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalFine) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FinePaid) == 0x100);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TimesThrownOut) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalBribeAmount) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumTeleports) == 0x10c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, MaxChickenThrow) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TrainingStatus) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HeroStatsExperience) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RegionTime) == 0x124);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumberOfTimesUsedAbility) == 0x130);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, TotalSpentExperience) == 0x13c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumLoversAcquired) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumWeddings) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumSpousesMurdered) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumSpousesDied) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumDivorces) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, NumSex) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HadScriptedSex) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HadScriptedGaySex) == 0x159);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BiggestFish) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, HousePositions) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, InfiniteStamina) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DeathRecoveryMarkerName_PStringData) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FailQuestOnDeath) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTimePairs) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTimeSorting) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreBlackjack) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreCoinGolfOakVale) == 0x184);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreCoinGolfSnowSpire) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestScoreShoveHaPenny) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BestTimeGuessTheAddition) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenPairs) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenSorting) == 0x195);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenBlackjack) == 0x196);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenCoinGolfOakVale) == 0x197);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenCoinGolfSnowSpire) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenShoveHaPenny) == 0x199);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrizeGivenGuessTheAddition) == 0x19a);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Drunkenness) == 0x19c);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, BeerReservoir) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DrinkAffectMultiplier) == 0x1a4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DrinkAffectMultiplierMultiplier) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameToVomit) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, DeedHandle) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, FrameEndVomit) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, UpdateDrunkenness) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PDef_Object) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, PrimitiveHandle) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, UCounter) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, VCounter) == 0x1cc);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, RotCounter) == 0x1d0);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, IsInCutSceneFirstTime) == 0x1d4);
FABLE_STATIC_ASSERT(offsetof(CTCHeroStats, Marriages) == 0x1d8);

#endif // FABLE_ENGINE_CTCHEROSTATS_H
#endif
