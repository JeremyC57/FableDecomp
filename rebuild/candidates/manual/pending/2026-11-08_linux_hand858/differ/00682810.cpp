// CTCCreatureOpinionOfHero::InformOfDeed(EOpinionDeedType type, long handle, const COpinionDeedEffectsDef& def, bool flag) -- this creature learns of a hero deed: shop keepers and guards react to a witnessed crime (or guards raise the village's guard enmity), a non-guard notes a criminal hero; flirting is reported by a jealous partner or wife who sees it; violence upsets a wife and boasting crowds; then the deed joins the pending list by reaction priority, its transient opinion offsets are applied (rate limited per deed type) and an unknown-threat marker is moved to the hero
// Grade: DIFFER(3385v3442) -- 464 diff lines: frame-slot packing (crowd_group / the interface-lookup key temporaries sit 4-8 bytes off retail's), register allocation in the flirt section (retail keeps the hero in edi then spills it, me in ebp), the merged CDefPointer null tests in the guard-effects assignment, and the tail-duplicated region_def epilogue (ours at the criminal return, retail at the GetDeedFromHandle(handle)==0 return)
// Next idea: `goto finished` for both the bribed and the criminal return moves the duplicated epilogue to retail's spot (variant e21, normalised diff 401 vs 408) but costs frame packing; find the declaration that packs crowd_group at [esp+30h] (retail shares it with the key temporaries) and retry e21 on top
// Leaf: ?InformOfDeed@CTCCreatureOpinionOfHero@@QAEXW4EOpinionDeedType@@JABVCOpinionDeedEffectsDef@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00682810, 3442 bytes, `ret 10h`.
// Identity (two sources): Ego_r.pdb ?InformOfDeed@CTCCreatureOpinionOfHero@@QAEXW4EOpinionDeedType@@JABVCOpinionDeedEffectsDef@@_N@Z, and
//   the landed callers InformOfDeed(long, bool) 0x00683590 and CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed 0x006C6040 push
//   (flag, def, handle, type) to 0x00682810.
// Callees (Ego_r.pdb spellings): CTCBase GetPlayerManager 0x00686D40 / PeekWorldMap 0x00686D20 / GetWorld 0x00686A70; CPlayerManager::
//   GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0; CVectorMap LowerBound 0x0040F020 / const 0x004365B0 (the
//   GetTC / PeekTC bodies); CWorldMap::GetLoadedRegion 0x004FB150 / GetRegion const 0x004FC180; CTCVillageMember::GetPVillage 0x007EE540;
//   CTCHeroOpinionDeedLog GetDeedFromHandle 0x006C58F0 / PeekPDeedEffectsDef 0x006C5630 / PostDeed 0x006C6990; CTCShopKeeper::WitnessedCrime
//   0x007993F0; CTCGuard::WitnessedCrime 0x007B2FD0; CTCVillage IncreaseHeroEnemyOfGuardsUntil 0x006588DC / IsHeroCriminal 0x0065BFCE;
//   __ftol2; CWorld::GetFrame 0x0049D870; CBaseIntelligentPointer::GetPItem 0x00A01B50; CTCWife IsMarriedToThePlayer 0x006CEC80 /
//   OnHeroViolence 0x006CFDA0 / IsCourtableAndNotBlockingIt 0x006CF810; CThingCreatureBase GetSex 0x00661FF0 / GetCreatureType 0x00662000;
//   IS_THING_ALIVE 0x004A88F0 / 0x004CC340; CTCPerceiveThing::GetPerceivedThing 0x007446A0; CanCreatureSeeCreature 0x008645E0;
//   NCreatureAI::IsAwareOfThingIndirectly 0x00864700; list<const CThingAICreature*> ctor 0x006860C0 / dtor 0x00685BC0; CTCHeroStats
//   GetWivesInMarriageState 0x0057AE53 / PeekMarriageInfo 0x0057AC89; CTCBoastingCrowdControl GetCrowdGroup 0x00783730 /
//   OnHeroAttackCrowdMember 0x00783C80; CTCCreatureGroup::IsMember 0x00812F20; list<long>::insert 0x0042AB33; list<COpinionTransientOffset>
//   ::push_back 0x00479449; COpinionTransientOffset(const COpinionTransientOffset&, const COpinionPersonalityTraits&) 0x00681680;
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410;
//   CTCDInternalMarker::Create 0x00870BB0; CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760 / ~ 0x00A01C10;
//   COpinionReactionManager::AttachOpinionSource 0x00700780; CThing::GetPos 0x004C73D0.
// Status: parked after 51 graded variants (best md 464, variants e4 / e20); the local models below carry descriptive names where no shared model was used.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_THREAT
#define FABLE_RETAIL_COPINIONTRANSIENTOFFSET_TRAITS
#define FABLE_RETAIL_COPINIONTRANSIENTOFFSET_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_OUTOFLINE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"

class CThingAICreature : public CThing {
public:
    unsigned char _pad98[0x188 - 0x98];
    CIntelligentPointer<CThing> Partner;                                 // +0x188 (descriptive)
    unsigned char _pad190[0x1c9 - 0x190];
    unsigned char Flags1c9;                                              // +0x1c9 bit 3 (descriptive)
    long GetSex() const;                                                 // 0x00661FF0
    long GetCreatureType() const;                                        // 0x00662000
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
bool __fastcall IS_THING_ALIVE(CThingAICreature* const& creature);       // 0x004A88F0
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>& thing);   // 0x004CC340
bool __fastcall CanCreatureSeeCreature(CThing* viewer, CThing* target, bool flag);   // 0x008645E0
namespace NCreatureAI { bool __fastcall IsAwareOfThingIndirectly(const CThingAICreature* creature, const CThing* thing, bool flag); }   // 0x00864700

class CRegionDef : public CDefPointeeBase {
public:
    unsigned char _pad1c[0x9d - 0x1c];
    bool UseCrimes;                                                      // +0x9d
    bool DisplayCrimes;                                                  // +0x9e
    bool PermitCrimes;                                                   // +0x9f
};
class CRegion {
public:
    unsigned char _pad00[0x24];
    CDefPointer<const CRegionDef> PDef;                                  // +0x24
    CDefPointer<const CRegionDef> GetPRegionDef() const { return PDef; }   // FableWin.pdb ?GetPRegionDef@CRegion@@QBE?AV?$CDefPointer@$$CBVCRegionDef@@@@XZ (inline)
};
class CWorldMap {
public:
    long GetLoadedRegion() const;                                        // 0x004FB150
    const CRegion& GetRegion(long region) const;                         // 0x004FC180
};
class COpinionPersonalityTraits { public: float Values[9]; };
class COpinionPersonalityDef : public CDefPointeeBase {
public:
    unsigned char _pad1c[0x3c - 0x1c];
    COpinionPersonalityTraits* PersonalityTraits;                        // +0x3c
};
class COpinionOfHeroDef : public CDefPointeeBase {
public:
    unsigned char _pad1c[0x2c - 0x1c];
    bool WillAttackCriminals;                                            // +0x2c
};
class COpinionDeedEffectsDef : public CDefPointeeBase {
public:
    unsigned char _pad1c[0x44 - 0x1c];
    long MinimumSecondsBetweenEffectsForIndividual;                      // +0x44
    float PreReactionDelayIgnoreOnDelay;                                 // +0x48
    std::list<COpinionTransientOffset> Effects;                          // +0x4c
    long ReactionPriority;                                               // +0x50
    unsigned char _pad54[0x70 - 0x54];
    long CrimeSeverity;                                                  // +0x70
    long CrimeFineAmount;                                                // +0x74
    long Tutorial;                                                       // +0x78
    unsigned char Flags7c;                                               // +0x7c (bit 6 IsFlirting)
    bool IsFlirting() const { return (Flags7c & 0x40) != 0; }
};
class COpinionDeed {
public:
    EOpinionDeedType Type;                                               // +0x00
    unsigned char _pad04[0x18 - 0x04];
    CIntelligentPointer<CThing> Victim;                                  // +0x18 (descriptive)
    unsigned char _pad20[0x23 - 0x20];
    bool IsCrime;                                                        // +0x23
    long LinkedHandle;                                                   // +0x24
};
class CFaction;
class CTCHeroOpinionDeedLog : public CTCBase {
public:
    COpinionDeed* GetDeedFromHandle(long handle);                        // 0x006C58F0
    CDefPointer<const COpinionDeedEffectsDef> PeekPDeedEffectsDef(EOpinionDeedType type) const;   // 0x006C5630
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* thing, CThingAICreature* creature,
                  long postType, bool b1, bool b2, const CFaction* faction, bool b3, long l1);      // 0x006C6990
};
class CTCVillage {
public:
    unsigned char _pad00[0xa8];
    long FrameBribeEnds;                                                 // +0xa8 (descriptive)
    long GetFrameBribeEnds() const { return FrameBribeEnds; }
    unsigned char _padac[0xb3 - 0xac];
    bool CrimesSuspended;                                                // +0xb3 (descriptive)
    void IncreaseHeroEnemyOfGuardsUntil(long frames);                    // 0x006588DC
    bool IsHeroCriminal();                                               // 0x0065BFCE
};
class CTCVillageMember : public CTCBase { public: CTCVillage* GetPVillage() const; };   // 0x007EE540
class CTCShopKeeper : public CTCBase { public: void WitnessedCrime(const COpinionDeed& deed, long handle); };   // 0x007993F0
class CTCGuard : public CTCBase { public: void WitnessedCrime(const COpinionDeed& deed, long handle); };        // 0x007B2FD0
class CTCWife : public CTCBase {
public:
    bool IsMarriedToThePlayer() const;                                   // 0x006CEC80
    bool IsCourtableAndNotBlockingIt() const;                            // 0x006CF810
    void OnHeroViolence();                                               // 0x006CFDA0
};
class CTCPerceiveThing : public CTCBase {
public:
    unsigned char _pad0c[0x2f - 0x0c];
    bool LineOfSight;                                                    // +0x2f (descriptive)
    CThing* GetPerceivedThing() const;                                   // 0x007446A0
};
class CHeroMarriageInfo {
public:
    unsigned __int64 WifeUID;                                            // +0x00
    unsigned __int64 MaritalHomeUID;                                     // +0x08
    long State;                                                          // +0x10
    bool GayMarriage;                                                    // +0x14
    CHeroMarriageInfo() : WifeUID(0), MaritalHomeUID(0), State(0), GayMarriage(false) { }
};
class CTCHeroStats : public CTCBase {
public:
    void GetWivesInMarriageState(std::list<const CThingAICreature*>& wives, long state) const;   // 0x0057AE53
    bool PeekMarriageInfo(unsigned __int64 uid, CHeroMarriageInfo& info) const;                // 0x0057AC89
};
class CTCCreatureGroup { public: bool IsMember(const CThingAICreature* creature) const; };     // 0x00812F20
class CTCBoastingCrowdControl : public CTCBase {
public:
    bool Active;                                                         // +0x0c
    bool GetCrowdGroup(CThing& thing, CTCCreatureGroup** group) const;   // 0x00783730
    void OnHeroAttackCrowdMember();                                      // 0x00783C80
};
class CTCDInternalMarker { public: static CThing* __fastcall Create(long defIndex, const C3DVector& pos); };   // 0x00870BB0
class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& name); };    // 0x009AD410
class CTCOpinionSource;
class COpinionReactionManager { public: bool AttachOpinionSource(CThing& thing, const CCharString& name, CTCOpinionSource** source) const; };   // 0x00700780
class CPhysicsDriver { public: virtual void v00(); };
class CWorld {
public:
    unsigned char _pad00[0x10];
    CDefinitionManager* DefinitionManager;                               // +0x10
    unsigned char _pad14[0x74 - 0x14];
    COpinionReactionManager* OpinionReactionManager;                     // +0x74
    static long __fastcall GetFrame();                                   // 0x0049D870
};
extern long ConstantFPS;                                                 // 0x01375550
extern bool g_UnknownThreatMarkers;                                      // 0x01375733 (descriptive)
extern CCharString g_CCharString_013ba898;                               // 0x013BA898

void CTCCreatureOpinionOfHero::InformOfDeed(EOpinionDeedType type, long handle, const COpinionDeedEffectsDef& def, bool flag)
{
    CTCCreatureGroup* crowd_group = 0;
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (hero == 0)
        return;
    CTCHeroOpinionDeedLog* log;
    if (!hero->GetTC(TCI_HERO_OPINION_DEED_LOG, &log))
        return;
    if (log == 0)
        return;
    CDefPointer<const CRegionDef> region_def = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetPRegionDef();
    bool crimes_count;
    if (!region_def.IsNull() && (region_def->UseCrimes || region_def->PermitCrimes))
        crimes_count = true;
    else
        crimes_count = false;
    const CTCVillageMember* village_member = 0;
    if (ParentThing->PeekTC(TCI_VILLAGE_MEMBER, &village_member)) {
        CTCVillage* village = village_member->GetPVillage();
        if (village && village->CrimesSuspended)
            crimes_count = false;
    }
    if (ParentThing->HasTC(TCI_SHOP_KEEPER) && crimes_count) {
        long crime_handle;
        if (type > 0 && type < 10)
            crime_handle = handle;
        else if (type == 0x35) {
            COpinionDeed* deed = log->GetDeedFromHandle(handle);
            if (deed == 0)
                goto shop_done;
            crime_handle = deed->LinkedHandle;
        } else
            goto shop_done;
        if (crime_handle != -1) {
            COpinionDeed* crime = log->GetDeedFromHandle(crime_handle);
            if (crime && crime->IsCrime) {
                CTCShopKeeper* shop_keeper = 0;
                ParentThing->GetTC(TCI_SHOP_KEEPER, &shop_keeper);
                shop_keeper->WitnessedCrime(*crime, crime_handle);
            }
        }
    }
shop_done:
    if (ParentThing->HasTC(TCI_GUARD)) {
        if (!crimes_count) {
            COpinionDeed* deed = log->GetDeedFromHandle(handle);
            if (deed) {
                CDefPointer<const COpinionDeedEffectsDef> effects;
                if (deed->Type > 0 && deed->Type < 10)
                    effects = log->PeekPDeedEffectsDef(deed->Type);
                if (deed->Type == 0x35)
                    effects = log->PeekPDeedEffectsDef((EOpinionDeedType)8);
                if (!effects.IsNull()) {
                    if (effects->CrimeSeverity != 1) {
                        const CTCVillageMember* member;
                        if (ParentThing->PeekTC(TCI_VILLAGE_MEMBER, &member)) {
                            CTCVillage* village = member->GetPVillage();
                            if (village)
                                village->IncreaseHeroEnemyOfGuardsUntil((long)(effects->CrimeFineAmount * 0.2f));
                        }
                    }
                }
            }
        } else {
            long crime_handle;
            if (type > 0 && type < 10)
                crime_handle = handle;
            else if (type == 0x35)
                crime_handle = log->GetDeedFromHandle(handle)->LinkedHandle;
            else
                goto guard_done;
            if (crime_handle != -1) {
                COpinionDeed* crime = log->GetDeedFromHandle(crime_handle);
                if (crime && crime->IsCrime) {
                    bool bribed = false;
                    CTCVillageMember* member;
                    if (ParentThing->GetTC(TCI_VILLAGE_MEMBER, &member)) {
                        CTCVillage* village = member->GetPVillage();
                        if (village)
                            bribed = GetWorld().GetFrame() < village->GetFrameBribeEnds();
                    }
                    bool against_me = crime->Victim.Get() == ParentThing;
                    if (bribed && !against_me)
                        return;
                    CTCGuard* guard = 0;
                    ParentThing->GetTC(TCI_GUARD, &guard);
                    guard->WitnessedCrime(*crime, crime_handle);
                }
            }
        }
    } else {
        COpinionDeed* deed = log->GetDeedFromHandle(handle);
        if (deed && deed->IsCrime) {
            CDefPointer<const COpinionDeedEffectsDef> effects = log->PeekPDeedEffectsDef(deed->Type);
            if (effects->CrimeSeverity == 1) {
                if (village_member && village_member->GetPVillage() && village_member->GetPVillage()->IsHeroCriminal())
                    return;
            } else if (!PDef.IsNull() && PDef->WillAttackCriminals && effects->CrimeSeverity == 3) {
                HeroOpinionEnemy = true;
                ForcedAttitude = (EOpinionAttitudeType)3;
            }
        }
    }
guard_done:
    if (def.IsFlirting()) {
        CThing* hero_thing = log->ParentThing;
        if (hero_thing && !hero_thing->IsDead()) {
            CThingAICreature* me = static_cast<CThingAICreature*>(ParentThing);
            if (me->GetType() == 1) {
                const CTCWife* wife;
                if (me->PeekTC(TCI_WIFE, &wife) && wife->IsMarriedToThePlayer())
                    goto flirt_done;
                {
                    const CTCTargetingBase* targeting;
                    if (!hero_thing->PeekTC(TCI_TARGETING, &targeting))
                        goto flirt_done;
                    CThing* target = targeting->GetPTargetedThing();
                    if (target) {
                        const CTCWife* target_wife;
                        if (targeting->GetPTargetedThing()->PeekTC(TCI_WIFE, &target_wife) && target_wife->IsMarriedToThePlayer())
                            goto flirt_done;
                    }
                }
                if (me->GetSex() == 2) {
                    CThingAICreature* partner = static_cast<CThingAICreature*>(me->Partner.Get());
                    if (IS_THING_ALIVE(partner)) {
                        const CTCPerceiveThing* perceive;
                        if (me->Partner.Get()->PeekTC(TCI_PERCEIVE_THING, &perceive) && perceive->GetPerceivedThing() == hero_thing
                            && perceive->LineOfSight && CanCreatureSeeCreature(me->Partner.Get(), hero_thing, true))
                            log->PostDeed((EOpinionDeedType)0x4b, log->ParentThing->GetPos(), me,
                                          static_cast<CThingAICreature*>(me->Partner.Get()), 1, false, false, 0, false, 0);
                    }
                }
                if (me->Flags1c9 & 8) {
                    const CTCHeroStats* stats;
                    if (hero_thing->PeekTC(TCI_HERO_STATS, &stats)) {
                        std::list<const CThingAICreature*> wives;
                        stats->GetWivesInMarriageState(wives, 0);
                        for (std::list<const CThingAICreature*>::iterator it = wives.begin(); it != wives.end(); ++it) {
                            CThingAICreature* wife_creature = const_cast<CThingAICreature*>(*it);
                            if (!IS_THING_ALIVE(const_cast<CThingAICreature*>(*it)))
                                continue;
                            const CTCPerceiveThing* perceive;
                            if (!wife_creature->PeekTC(TCI_PERCEIVE_THING, &perceive))
                                continue;
                            if (perceive->GetPerceivedThing() != hero_thing || !perceive->LineOfSight)
                                continue;
                            if (!NCreatureAI::IsAwareOfThingIndirectly(wife_creature, hero_thing, true))
                                continue;
                            CHeroMarriageInfo info;
                            stats->PeekMarriageInfo((*it)->GetUniqueID(), info);
                            if (info.State != 5)
                                log->PostDeed((EOpinionDeedType)0x55, hero_thing->GetPos(), me, wife_creature, 1, false, false, 0, false, 0);
                        }
                    }
                }
            }
        }
    }
flirt_done:
    if (type == 7 || type == 8 || type == 9 || type == 0x35) {
        CThing* hero_thing = log->ParentThing;
        if (hero_thing && !hero_thing->IsDead() && ParentThing->GetType() == 1) {
            CTCWife* wife;
            if (ParentThing->GetTC(TCI_WIFE, &wife))
                wife->OnHeroViolence();
        }
        if (ParentThing->GetType() == 1) {
            CThing* hero_thing = log->ParentThing;
            CTCBoastingCrowdControl* crowd;
            if (hero_thing->GetTC(TCI_BOASTING_CROWD_CONTROL, &crowd) && crowd->Active
                && crowd->GetCrowdGroup(*hero_thing, &crowd_group)
                && crowd_group->IsMember(static_cast<CThingAICreature*>(ParentThing)))
                crowd->OnHeroAttackCrowdMember();
        }
    }
    if (log->GetDeedFromHandle(handle) == 0)
        return;
    bool apply_effects = true;
    if (def.MinimumSecondsBetweenEffectsForIndividual > 0) {
        long gap = def.MinimumSecondsBetweenEffectsForIndividual * ConstantFPS;
        long since = gap < CWorld::GetFrame() ? CWorld::GetFrame() - gap : 0;
        apply_effects = LastFrameAppliedDeedEffect[type] <= since;
        if (!apply_effects)
            goto effects_checked;
    }
    {
        bool courting_flirt;
        CDefPointer<const COpinionDeedEffectsDef> effects = log->PeekPDeedEffectsDef(type);
        CThingAICreature* me = static_cast<CThingAICreature*>(ParentThing);
        const CTCWife* wife;
        if (effects->IsFlirting() && me->GetType() == 1 && me->GetSex() == 2 && me->GetCreatureType() == 2
            && me->PeekTC(TCI_WIFE, &wife) && !wife->IsMarriedToThePlayer() && !wife->IsCourtableAndNotBlockingIt())
            courting_flirt = true;
        else
            courting_flirt = false;
        if (courting_flirt)
            apply_effects = false;
    }
effects_checked:
    long priority = log->PeekPDeedEffectsDef(type)->ReactionPriority;
    std::list<long>::iterator it = PendingDeeds.begin();
    while (it != PendingDeeds.end()) {
        COpinionDeed* pending = log->GetDeedFromHandle(*it);
        if (pending == 0) {
            it = PendingDeeds.erase(it);
            continue;
        }
        if (log->PeekPDeedEffectsDef(pending->Type)->ReactionPriority < priority) {
            PendingDeeds.insert(it, handle);
            goto inserted;
        }
        ++it;
    }
    PendingDeeds.push_back(handle);
inserted:
    if (apply_effects) {
        LastFrameAppliedDeedEffect[type] = GetWorld().GetFrame();
        for (std::list<COpinionTransientOffset>::const_iterator offset = def.Effects.begin(); offset != def.Effects.end(); ++offset) {
            if (!PPersonalityDef.IsNull())
                TransientOffsets.push_back(COpinionTransientOffset(*offset, PPersonalityDef->PersonalityTraits[offset->Opinion]));
            else
                TransientOffsets.push_back(COpinionTransientOffset(*offset));
        }
    }
    if (g_UnknownThreatMarkers && flag && log->GetDeedFromHandle(handle)) {
        if (!IS_THING_ALIVE(PUnknownThreatOpinionSource)) {
            long marker = GetWorld().DefinitionManager->GetDefGlobalIndexFromName(CCharString("INTERNAL_MARKER_LIGHT"));
            PUnknownThreatOpinionSource = CIntelligentPointer<CThing>(CTCDInternalMarker::Create(marker, ParentThing->GetPos()));
            if (IS_THING_ALIVE(PUnknownThreatOpinionSource))
                GetWorld().OpinionReactionManager->AttachOpinionSource(*PUnknownThreatOpinionSource.Get(), g_CCharString_013ba898, 0);
        }
        if (IS_THING_ALIVE(PUnknownThreatOpinionSource)) {
            PUnknownThreatOpinionSource.Get()->GetPhysicsTC()->ResetPosition(log->ParentThing->GetPos());
            FrameLastUpdatedUnknownThreat = GetWorld().GetFrame();
        }
    }
}
