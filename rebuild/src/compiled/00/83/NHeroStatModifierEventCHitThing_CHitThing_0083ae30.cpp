// NHeroStatModifierEvent::CHitThing::CHitThing(const CCreatureStatsDef& stats_def) -- a hero-stat event for hitting a creature: keep the hit creature's stats def, not unarmed, strength experience by default, increment on, amount 1.0
// Leaf: ??0CHitThing@NHeroStatModifierEvent@@QAE@ABVCCreatureStatsDef@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083AE30, length 37 bytes, thiscall `ret 4`.
// Names (2026-11-10, chunk 897 queue land1011j): Ego_r.pdb + FableWin.pdb ??0CHitThing@NHeroStatModifierEvent@@QAE@ABVCCreatureStatsDef@@@Z
//   takes `const CCreatureStatsDef&` (was modelled as a CThing* in an invented non-polymorphic base). Layout from Ego_r.pdb CHitThing
//   (0x18): the NHeroStatModifierEvent::CBase vptr (vtable 0x01272FA0 for CHitThing; CBase's ~CBase 0x0083AD90 is landed),
//   StatsDefOfHitThing +4, Unarmed +8, StatToIncrement +0xC, DoStatIncrement +0x10, IncrementAmount +0x14.
// Shape: `mov eax,ecx` first (the constructor returns `this`); VC7.1 schedules the stats-def store ahead of the vptr store.
#include "engine/EHeroExperienceStatCategory.h"
class CTCHeroStats;
class CCreatureStatsDef;
namespace NHeroStatModifierEvent {
class CBase {
public:
    ~CBase();
    virtual void ModifyHeroStats(CTCHeroStats& heroStats) const = 0;
};

class CHitThing : public CBase {
public:
    CHitThing(const CCreatureStatsDef& stats_def);
    virtual void ModifyHeroStats(CTCHeroStats& heroStats) const;   // 0x00815DF0-family slot 0
    const CCreatureStatsDef*    StatsDefOfHitThing;    // +0x04
    bool                        Unarmed;               // +0x08
    EHeroExperienceStatCategory StatToIncrement;       // +0x0C
    bool                        DoStatIncrement;       // +0x10
    float                       IncrementAmount;       // +0x14
};

CHitThing::CHitThing(const CCreatureStatsDef& stats_def)
    : StatsDefOfHitThing(&stats_def), Unarmed(false), StatToIncrement(HERO_STAT_STRENGTH), DoStatIncrement(true),
      IncrementAmount(1.0f)
{
}
}
