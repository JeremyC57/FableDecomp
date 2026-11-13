// CThingFilter_IsEligibleWitness::operator()(const CThing& thing) const -- whether a thing can witness an opinion deed: a
// creature (CClass::Type 1) not sleeping in bed (creature mode 13), with an opinion-of-hero TC, the deed type enabled, optionally
// not the recipient, optionally allied with (reversed: an enemy of) the faction, and optionally perceiving the perpetrator (by
// hearing-or-indirect awareness, else IsPerceivingPerpetrator) with an ignore-FOV line of sight. An unknown-threat search inverts
// that: the witness must NOT see the perpetrator, but must be aware of the deed's (alive) target, or have its deed position inside
// its sight radius and in navigation line of sight.
// Leaf: ??RCThingFilter_IsEligibleWitness@@QBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C8E90, 804 bytes, thiscall `ret 4`, default /O2.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0, CTCCreatureModeManager::GetCurrentMode const
//   0x0073A6E0 (CModeBase vslot 1 GetType), CThingFilter_HasTC::operator() 0x006A80A0, CThingFilter_IsDeedTypeEnabled::operator()
//   0x006C8D40, CThingFilter_IsNot::operator() 0x006A76A0, CTCEnemy::IsEnemyOfFaction 0x0076C7D0 / IsAlliedWithFaction 0x0076C7F0,
//   NCreatureAI::IsAwareOfThingIndirectly 0x00864700 (__fastcall), CTCPerceiveThing::GetPerceivedThing 0x007446A0 /
//   GetSightRadius 0x007448F0, IS_THING_ALIVE<CIntelligentPointer<const CThing> > 0x0069CE20, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CThing::GetPos 0x004C73D0, CTCCreatureNavigation::IsPosNavLineOfSightToPos 0x006CB610.
// Shape: the three sub-filter operator()s (IsPerceivingThingAndIsThingPerceived / IsAlliedWithFaction / IsEnemyOfFaction) are
//   expanded in place (FABLE_FORCEINLINE). IsPerceivingThingAndIsThingPerceived and IsEnemyOfFaction have no out-of-line retail
//   copy (their operator() symbols exist only in FableWin.pdb); IsAlliedWithFaction does: the landed row 0x006C8C50
//   (rebuild/src/compiled/00/6c/CThingFilterIsAlliedWithFaction_operator_006c8c50.cpp), whose body the inline copy below
//   repeats, but retail inlined it here all the same. IsPerceivingPerpetrator's result starts false (`xor bl,bl` before the
//   GetPerceivedThing call); the whole body sits inside `if (GetType()==1)` (retail puts that exit at the very end); the mode /
//   HasOpinions / DeedTypeEnabled / IsNot chain is one && expression (its false falls into the esi-level `return result` exit);
//   the perceiving tail is a single-exit block (do/while(0) + break, one `ret`) and its unknown-threat awareness test is a second
//   nested single-exit block (`aware`) -- the shape of two inlined helpers: it gives retail's tail-duplicated `return true`
//   placed after the awareness test and the final false falling into the shared `return ret` exit.
class CTCBase;
class CBaseObject;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS
#include "engine/CTCPerceiveThing.h"
#define FABLE_RETAIL_CTCENEMY_QUERIES
#include "engine/CTCEnemy.h"
#include "engine/CTCCreatureModeManager.h"
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT
#include "engine/CTCCreatureNavigation.h"
#include "engine/COpinionDeed.h"
#include "engine/NCreatureMode.h"
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_IsNot.h"

class CThingAICreature;
class CFaction;
template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0069CE20 <CIntelligentPointer<const CThing> >
namespace NCreatureAI {
bool __fastcall IsAwareOfThingIndirectly(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight); // 0x00864700
}

class CThingFilter_IsDeedTypeEnabled : public std::unary_function<CThing,bool> {
public:
    EOpinionDeedType DeedType;
    bool operator()(const CThing& thing) const;     // 0x006C8D40
};
class CThingFilter_IsPerceivingThingAndIsThingPerceived : public std::unary_function<CThing,bool> {
public:
    const CThing* PerceivedThing;
    bool IncludeHearing;
    FABLE_FORCEINLINE bool operator()(const CThing& thing) const
    {
        const CTCPerceiveThing* perceive;
        thing.PeekTC(TCI_PERCEIVE_THING, &perceive);
        bool result = false;
        if (perceive->GetPerceivedThing() == PerceivedThing)
            result = (IncludeHearing && perceive->IsThingHeard()) || perceive->IsThingLineOfSight() || perceive->IsThingSmelt();
        return result;
    }
};
class CThingFilter_IsAlliedWithFaction : public std::unary_function<CThing,bool> {
public:
    const CFaction* PFaction;   // +0x00 (Ego_r: CFaction* PFaction; ctor ??0...@@QAE@PBVCFaction@@@Z)
    // Body = the landed out-of-line row 0x006C8C50 (CThingFilter_IsAlliedWithFaction::operator()), inlined here by retail.
    FABLE_FORCEINLINE bool operator()(const CThing& thing) const
    {
        const CTCEnemy* enemy;
        if (thing.PeekTC(TCI_ENEMY, &enemy))
            return enemy->IsAlliedWithFaction(PFaction);
        return false;
    }
};
class CThingFilter_IsEnemyOfFaction : public std::unary_function<CThing,bool> {
public:
    const CFaction* PFaction;
    FABLE_FORCEINLINE bool operator()(const CThing& thing) const
    {
        const CTCEnemy* enemy;
        if (thing.PeekTC(TCI_ENEMY, &enemy))
            return enemy->IsEnemyOfFaction(PFaction);
        return false;
    }
};

class CThingFilter_IsEligibleWitness : public std::unary_function<CThing,bool> {
public:
    const CThing*                                     Perpetrator;             // +0x00
    const COpinionDeed*                               PDeed;                   // +0x04
    CThingFilter_HasTC                                HasOpinions;             // +0x08
    CThingFilter_IsDeedTypeEnabled                    DeedTypeEnabled;         // +0x0c
    CThingFilter_IsPerceivingThingAndIsThingPerceived IsPerceivingPerpetrator; // +0x10
    CThingFilter_IsNot                                IsNotRecipient;          // +0x18
    CThingFilter_IsAlliedWithFaction                  IsAllied;                // +0x1c
    CThingFilter_IsEnemyOfFaction                     IsEnemy;                 // +0x20
    bool UnknownThreatSearch;                                                  // +0x24
    bool UseHearing;                                                           // +0x25
    bool UsePerceiving;                                                        // +0x26
    bool UseIsNot;                                                             // +0x27
    bool UseFaction;                                                           // +0x28
    bool ReverseFactionSearch;                                                 // +0x29
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsEligibleWitness::operator()(const CThing& thing) const
{
    if (thing.GetType() == 1) {
        const CTCCreatureModeManager* mode_manager;
        bool result = !(thing.PeekTC(TCI_ENTITY_MODE_MANAGER, &mode_manager) && mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_SLEEP_BED)
                      && HasOpinions(thing) && DeedTypeEnabled(thing) && (!UseIsNot || IsNotRecipient(thing));
        if (result && UseFaction)
            result = ReverseFactionSearch ? IsEnemy(thing) : IsAllied(thing);
        if (!result || !UsePerceiving)
            return result;
        bool ret;
        do {
            bool perceiving = UseHearing ? NCreatureAI::IsAwareOfThingIndirectly((const CThingAICreature*)&thing, Perpetrator, false)
                                         : IsPerceivingPerpetrator(thing);
            const CTCPerceiveThing* perceive;
            result = perceiving && thing.PeekTC(TCI_PERCEIVE_THING, &perceive)
                     && perceive->GetPerceivedThing() == Perpetrator && perceive->IsThingIgnoreFOVLineOfSight();
            if (!UnknownThreatSearch)
                { ret = result; break; }
            result = !result;
            if (!result)
                { ret = result; break; }
            bool aware;
            do {
                if (PDeed && IS_THING_ALIVE(PDeed->PTarget)
                    && NCreatureAI::IsAwareOfThingIndirectly((const CThingAICreature*)&thing, PDeed->PTarget.Get(), false))
                    { aware = true; break; }
                const CTCPerceiveThing* perceiver;
                if (PDeed && thing.PeekTC(TCI_PERCEIVE_THING, &perceiver)) {
                    const CTCCreatureNavigation* navigation;
                    if (thing.PeekTC(TCI_NAVIGATION, &navigation)) {
                        float radius = perceiver->GetSightRadius();
                        const C3DVector& deed_pos = PDeed->Position;
                        const C3DVector& pos = thing.GetPos();
                        float dx = pos.X - deed_pos.X;
                        float dy = pos.Y - deed_pos.Y;
                        if (dy * dy + dx * dx < radius * radius
                            && navigation->IsPosNavLineOfSightToPos(thing.GetPos(), PDeed->Position))
                            { aware = true; break; }
                    }
                }
                { aware = false; break; }
            } while (0);
            ret = aware;
        } while (0);
        return ret;
    }
    return false;
}