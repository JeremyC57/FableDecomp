// CTCTavernGameCardSorting::EndRound() -- card-base EndRound; in the timed game type, advance the card layout, then either start the next round (state 7) or finish: won when the counter is within the target time, the score is the counter, a new best time is flagged
// Leaf: ?EndRound@CTCTavernGameCardSorting@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D4F40, 295 bytes (plain `ret`); CTCTavernGameCardSorting vtable 0x0126F5B4
// slot 33 (the CardPairs override 0x007D6150 has the same skeleton).
// Callees: CTCTavernGameCardBase::EndRound() 0x008F1720 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; CTCTavernGame::GetHeroStats(CTCHeroStats**) 0x005EB4B0;
//          CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20; the def's virtual slot 1 when its count reaches zero.
// Layout (retail): CTCTavernGame GameWon +0x21, NumRounds +0x24, GameFinished +0x28, Score +0x2c, TargetTime +0x98,
//          NewBestScore +0xa0; CTCTavernGameCardBase Counter +0x154, GameType +0x300, CardOrder +0x304;
//          CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte records); CTCHeroStats::BestTimeSorting +0x17c (PDB +0x194).
// Shape: one-case `switch` on the game type (`dec eax ; jne`); the layout index is incremented in place then reduced
//        modulo the signed layout count; `GameWon = Counter <= TargetTime` is `test ah,41h ; jp`; the constant 1 lives in
//        edx for the round compare, the clamp and the bool stores; the GameFinished store before the best-time test
//        survives because the stats reads may alias it; the stats pointer is left uninitialised. The def release is
//        `if (--m_count == 0) Destroy();` (count stored before the copy that is tested; DecRef() then GetRefCount()
//        copies first).
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
// Shared headers (NF96 q11339): engine/card_sorting_types.h (the Ego_r.pdb CTCTavernGameCardBase / CTCTavernGameCardSorting, recipe of the
//   landed CardsMainGame 0x007D5070), CTavernGameCardBaseDef.h (CardOrders), CCardPositionsDef.h and CTCHeroStats.h included directly, with
//   CDefPointer<const CTavernGameCardBaseDef>, so GetCardDef 0x008F0B20 binds its PDB spelling (was a local CDefPointer struct, local
//   CTCTavernGameCardBase / CTavernGameCardBaseDef / CCardPositionsDef / vector models and CTCHeroStats included under a renaming macro).
// Inferred: the (long) cast on CardOrders.size(): retail reduces the order index with a SIGNED modulo (`cdq ; idiv ecx` at 0x007D4FA3);
//   STLport size() is unsigned (`xor edx,edx ; div ecx` without the cast, graded DIFFER 296v295). The old local model got the same bytes
//   from a vector whose size() returned long.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
void CTCTavernGameCardSorting::EndRound()
{
    CTCTavernGameCardBase::EndRound();
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    CTCHeroStats* stats;
    GetHeroStats(&stats);
    switch (GameType) {
    case ECGT_ROUND_LIMIT:
        ++CardOrder;
        CardOrder %= (long)def->CardOrders.size();
        if (NumRounds > 1 && !GameFinished) {
            --NumRounds;
            SetState(ETGS_NEW_ROUND);
            return;
        }
        if (NumRounds < 1)
            NumRounds = 1;
        if (!GameFinished)
            GameWon = Counter <= TargetTime;
        Score = Counter;
        GameFinished = true;
        if (GameWon && (stats->BestTimeSorting == 0.0f || Counter < stats->BestTimeSorting))
            NewBestScore = true;
        break;
    }
    GameFinished = false;
    SetState(ETGS_REACTION_AND_PAYOUT);
}
