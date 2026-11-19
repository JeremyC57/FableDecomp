// CTCTavernGameCardPairs::EndRound() -- card-base EndRound, reshuffle; in the timed game type, advance the card layout, then either start the next round (state 7) or finish: the score is the round counter and a new best time is flagged
// Leaf: ?EndRound@CTCTavernGameCardPairs@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D6150, 262 bytes (plain `ret`); CTCTavernGameCardPairs vtable 0x0126F6FC
// slot 33 (the EndRound slot: CTCTavernGameCardBase::EndRound 0x008F1720 is called first; the manifest says "vfunc_33").
// Callees: CTCTavernGameCardBase::EndRound() 0x008F1720 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; ShuffleCards (slot 60, `call [edx+0F0h]`);
//          CTCTavernGame::GetHeroStats(CTCHeroStats**) 0x005EB4B0; CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20;
//          the def's virtual slot 1 when its count reaches zero.
// Layout (retail): CTCTavernGame GameWon +0x21, NumRounds +0x24, GameFinished +0x28, Score +0x2c (PDB - 8), NewBestScore
//          +0xa0 (PDB +0xbc); CTCTavernGameCardBase Counter +0x154, GameType +0x300, CardOrder +0x304;
//          CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte records); CTCHeroStats::BestTimePairs +0x178 (PDB +0x190).
// Release: the def pointer's release is `if (--m_count == 0) Destroy();` (count stored before the copy that is tested;
//          DecRef() then GetRefCount() copies first).
// Shape: the game-type test is a one-case `switch` (`dec eax ; jne`); the layout index is incremented in place, then reduced modulo the signed layout count (`cdq ; idiv`); the
//        next-round arm returns through SetState(7) and the finish arm falls into the shared SetState(8), so the two
//        calls tail-merge on `push 7 / push 8`; `(GameWon && best == 0.0f) || Counter < best` is the fucompp/jnp then
//        fcomp/`test ah,5` pair; the stats pointer is zero-initialised.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
// Shared headers (NF96 q11339): engine/CTCTavernGameCardBase.h / CTCTavernGameCardPairs.h (the 0x007D6870 recipe; overlay opt-ins
//   FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ENDROUND / _ECARDGAMETYPE_VALUES and FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES),
//   CTavernGameCardBaseDef.h (CardOrders), CCardPositionsDef.h and CTCHeroStats.h included directly, with
//   CDefPointer<const CTavernGameCardBaseDef>, so GetCardDef 0x008F0B20 binds its PDB spelling (was a local CDefPointer struct, local
//   CTCTavernGameCardBase / def / vector models and CTCHeroStats under a renaming macro).
// Inferred: the (long) cast on CardOrders.size(): retail reduces the order index with a SIGNED modulo (`cdq ; idiv ecx` at 0x007D61C5);
//   STLport size() is unsigned (the Sorting twin 0x007D4F40 graded DIFFER 296v295 without it, `xor edx,edx ; div ecx`).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ENDROUND
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ECARDGAMETYPE_VALUES
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS
#include "stlport/fable_containers.h"
#include "fable_game.h"
#include "engine/CThingObject.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CCardPositionsDef.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardPairs.h"
void CTCTavernGameCardPairs::EndRound()
{
    CTCTavernGameCardBase::EndRound();
    CDefPointer<const CTavernGameCardBaseDef> def(NULL_DEF);
    GetCardDef(&def);
    ShuffleCards();
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    switch (GameType) {
    case ECGT_ROUND_LIMIT:
        ++CardOrder;
        CardOrder %= (long)def->CardOrders.size();
        if (--NumRounds > 0 && !GameFinished) {
            SetState(ETGS_NEW_ROUND);
            return;
        }
        Score = Counter;
        if ((GameWon && stats->BestTimePairs == 0.0f) || Counter < stats->BestTimePairs)
            NewBestScore = true;
        break;
    }
    SetState(ETGS_REACTION_AND_PAYOUT);
}
