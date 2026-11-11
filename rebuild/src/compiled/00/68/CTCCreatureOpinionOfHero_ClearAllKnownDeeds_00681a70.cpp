// CTCCreatureOpinionOfHero::ClearAllKnownDeeds() -- empty the pending-deed list and each of the 92 per-deed-type known-deed lists
// Leaf: ?ClearAllKnownDeeds@CTCCreatureOpinionOfHero@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681A70, length 110 bytes (+2 int3 padding).
// Label correction: the catalog row says "CTCHero::ClearMarriageAndRelationshipData"; the body clears CTCCreatureOpinionOfHero
// lists (PendingDeeds and the KnownDeeds[92] array; it sits right before KnowsOfDeed 0x00681AE0 and is called from the
// CTCWife divorce code on the opinion component). The PDB name of the member that clears exactly these lists is
// ?ClearAllKnownDeeds@CTCCreatureOpinionOfHero@@AAEXXZ (private, Ego_r.pdb and FableWin.pdb); ClearPendingDeeds clears only
// the first list.
// Callees: free @ 0x00BFEA14 (STLport malloc allocator node release).
// Layout: the shared engine/CTCCreatureOpinionOfHero.h with retail 4-byte STLport lists (head node pointer): PendingDeeds +0x54,
// LastFrameAppliedDeedEffect long[92] +0x58, KnownDeeds list<long>[92] +0x1C8.
// Shape: the inline STLport list::clear (walk from the head's next, free each node, relink the head to itself, re-reading the
// head member after each free); the array loop counts down from 92 because the index is otherwise unused.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCCreatureOpinionOfHero.h"

void CTCCreatureOpinionOfHero::ClearAllKnownDeeds()
{
    PendingDeeds.clear();
    for (long i = 0; i < 92; ++i)
        KnownDeeds[i].clear();
}
