// CTCCreatureOpinionOfHero::IsFactionEnemyToHeroOverriddenOnOpinion() const -- never while the hero is this creature's opinion enemy; otherwise the creature's attitude to the player decides: attitudes 6 and 12..17 override faction enmity, 7..11 and anything outside 6..17 do not
// Leaf: ?IsFactionEnemyToHeroOverriddenOnOpinion@CTCCreatureOpinionOfHero@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681850, 80 bytes = 59 bytes of code, a `nop`, the two-entry jump table 0x0068188C
//   (0x00681885 `mov al,1`, 0x00681888 `xor al,al`) and the 12-entry byte index table 0x00681894 (00 01 01 01 01 01 00 00 00 00 00
//   00); the next function (IsWillingToStartFollowingHero 0x006818A0) starts right after it.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb ?IsFactionEnemyToHeroOverriddenOnOpinion@CTCCreatureOpinionOfHero@@QBE_NXZ;
//   the Ego_r donor order TransformOpinionFromHeroStats, IsWillingToAttackHeroOnOpinion, IsFactionEnemyToHeroOverriddenOnOpinion,
//   IsWillingToStartFollowingHero (0x0058F870 / 0x0058F8B0 / 0x0058F900 / 0x0058F950) is retail's 0x006817C0 / 0x006817F0 /
//   0x00681850 / 0x006818A0 (all landed or bound under those names); the only caller 0x00864C84 is the landed
//   NCreatureAI IsOverridingFactionEnemyOnTarget 0x00864B30, whose binding for this address already has this spelling, and it calls
//   it on the thing's interface 0x4F component (GetInterfaceType slot 21 of vtable 0x0125BC3C returns 0x4F). The catalog label
//   "IsWillingToAttackHeroOnOpinion" belongs to the sibling 0x006817F0 (same shape, cases 2..17).
// Callees: ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (protected, both PDBs), then the world's POpinionReactionManager (+0x74)
//   ?GetThingAttitudeToPlayer@COpinionReactionManager@@QBE?AW4EOpinionAttitudeType@@ABVCThing@@@Z 0x00701840 (both PDBs: a const
//   member taking `const CThing&`; the landed callers' pointer / long / non-const spellings are wrong: NAME_FIXES.tsv).
// Attitude values (the retail CCharString attitude-name table at 0x013BABD0, filled in index order by the dynamic initializer
//   around 0x0121E482: NONE, RESPECT, AWE, DISDAIN, FEAR, FRIENDLY, WORSHIP, RIDICULOUS, OFFENSIVE, AGREEABLE, UGLY, ATTRACTED,
//   LOVE, LOVE, LIKE, NEUTRAL, DISLIKE, HATE; the OPINION_REACTION_TYPE_ATTITUDE_* strings give the same order with 13..17 the
//   WIFE_LOVE .. WIFE_HATE attitudes): the enumerator spellings are in neither PDB dump, so the
//   cases are numeric, as in the landed siblings 0x006818A0 / 0x006818F0.
// Shape: retail's `sub eax,6 ; cmp eax,0Bh ; ja` is the only `sub`-normalised switch in all of Fable.exe (with the sibling
//   0x006817F0's `sub eax,2`); a plain `switch (attitude)` over 6..17 compiles to `add eax,-6` under both 3077 and 4035, with or
//   without /GL or any /O sub-flag. VC7.1 keeps the `sub` only for a named unsigned index whose range test comes first and merges
//   with the switch's own bound check (tested in probe/00681850.log): the 0-based table then needs no rebase. HeroOpinionEnemy is
//   tested before ParentThing is read; ParentThing and the manager are named locals so the PeekWorld call precedes the argument
//   push. Arms in source order give the true arm first (table entry 0), as retail.
// Layout: engine/CTCCreatureOpinionOfHero.h retail opt-in FABLE_RETAIL_CTCCREATUREOPINIONOFHERO (hdr/ overlay, chunk 713:
//   : CTCBase, HeroOpinionEnemy +0x342), engine/CTCBase.h's FABLE_RETAIL_CTCBASE branch with the PDB's protected PeekWorld (hdr/
//   overlay, chunk 713), engine/CWorld.h retail opt-in FABLE_RETAIL_CWORLD (POpinionReactionManager flattened to its Data word
//   +0x74 there).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#include "engine/CTCCreatureOpinionOfHero.h"
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"

enum EOpinionAttitudeType {                                 // values 0..17 (see above); spellings not recovered
    OPINION_ATTITUDE_NONE = 0
};

class COpinionReactionManager {
public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(const CThing& thing) const;   // 0x00701840
};

bool CTCCreatureOpinionOfHero::IsFactionEnemyToHeroOverriddenOnOpinion() const
{
    if (HeroOpinionEnemy)
        return false;
    const CThing& thing = *ParentThing;
    const COpinionReactionManager* manager = PeekWorld().POpinionReactionManager_Data;
    unsigned long index = manager->GetThingAttitudeToPlayer(thing) - 6;
    if (index > 11)
        return false;
    switch (index) {
    case 0:                                                 // 6 WORSHIP
    case 6:                                                 // 12 LOVE
    case 7:                                                 // 13 wife LOVE
    case 8:                                                 // 14 wife LIKE
    case 9:                                                 // 15 wife NEUTRAL
    case 10:                                                // 16 wife DISLIKE
    case 11:                                                // 17 wife HATE
        return true;
    case 1:                                                 // 7 RIDICULOUS
    case 2:                                                 // 8 OFFENSIVE
    case 3:                                                 // 9 AGREEABLE
    case 4:                                                 // 10 UGLY
    case 5:                                                 // 11 ATTRACTED
        return false;
    }
    return false;
}