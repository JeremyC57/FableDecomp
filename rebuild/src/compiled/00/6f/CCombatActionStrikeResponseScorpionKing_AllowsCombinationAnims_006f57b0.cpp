// CCombatAction_StrikeResponseScorpionKing::AllowsCombinationAnims() const -- the Scorpion King's hit response allows combination animations
// Leaf: ?AllowsCombinationAnims@CCombatAction_StrikeResponseScorpionKing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F57B0, 3 bytes: `mov al,1 ; ret` (thiscall, this unused).
// Identity: FableWin.pdb 0x023F2520 ?AllowsCombinationAnims@CCombatAction_StrikeResponseScorpionKing@@UBE_NXZ (public virtual const; no Ego_r /
//   Ego_d record) + retail CCombatAction_StrikeResponseScorpionKing vftable 0x01262874 slot 66 (+0x108) = 0x006F57B0 (every other
//   CCombatActionBase-derived table has CCombatActionBase's 0x0062EA10 there). One PDB + the retail slot + the body (nf228 ruling).
// NF264 (2026-11-22, q12750): re-spelled from the free stand-in `bool __fastcall CCombatAction_StrikeResponseScorpionKing_AllowsCombinationAnims()`
//   (no Leaf line) onto the shared engine/CCombatAction_StrikeResponseScorpionKing.h (the PROTOTYPE class landed 0x006F5C10 builds, plus the
//   new declaration-only opt-in FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_ALLOWSCOMBINATIONANIMS), with 0x006F5C10's defines.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEBASE_DEFAULTCTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_PROTOTYPE
#define FABLE_RETAIL_CCOMBATACTION_STRIKERESPONSESCORPIONKING_ALLOWSCOMBINATIONANIMS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCombatAction_GenericStrikeResponseBase.h"
#include "engine/CCombatAction_StrikeResponseScorpionKing.h"

bool CCombatAction_StrikeResponseScorpionKing::AllowsCombinationAnims() const
{
    return true;
}
