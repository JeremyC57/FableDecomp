// CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion() const -- an opinion enemy always attacks; otherwise the creature attacks unless its attitude to the hero is one of the friendly / neutral set (2, 4-6, 12-17)
// Leaf: ?IsWillingToAttackHeroOnOpinion@CTCCreatureOpinionOfHero@@QBE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006817F0, 84 bytes (with the 2-entry jump table and 16-byte index table).
// Callees: CTCBase::PeekWorld() const 0x00686A80 (protected), CWorld::PeekOpinionReactionManager() const (inline: the counted
//   pointer's Data at +0x74; header overlay), COpinionReactionManager::GetThingAttitudeToPlayer(const CThing&) const 0x00701840
//   (Ego_r.pdb, returns EOpinionAttitudeType).
// Shape: the parent thing is bound first (esi, loaded before PeekWorld); the switch prologue `sub eax,2` (not `add eax,-2`)
//   comes only from a named unsigned index with its own range test (GOTCHAS chunk 713); the ten listed indices (attitudes 2,
//   4-6, 12-17) give the byte index table and return false, everything else true.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureOpinionOfHero.h"
class COpinionReactionManager {                          // no shared header
public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840
};

bool CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion() const
{
    if (HeroOpinionEnemy)
        return true;
    const CThing& thing = *ParentThing;
    unsigned long index = PeekWorld().PeekOpinionReactionManager().GetThingAttitudeToPlayer(thing) - 2;
    if (index > 15)
        return true;
    switch (index) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return false;
    default:
        return true;
    }
}