# Chunk 713 header overlays (copy into rebuild/include/engine/ at landing)

All four files are full copies of the repo header (as of 2026-11-02, after chunk 712's CTCBase.h edit landed) with
a minimal edit. Grade with `FABLE_HDR_OVERLAY=$S/w22/agent713/hdr`.

| Header | Change | Path | Includers rechecked |
|---|---|---|---|
| engine/CTCQuestCard.h | new `#ifdef FABLE_RETAIL_CTCQUESTCARD` branch: `class CTCQuestCard : public CTCBase`, PDB ctor `(CThing&)`, virtuals OnSerialise (slot 1, public as FableWin UAE), GetInterfaceType/GetClassTypeName/IsComponentSerialisable (21/22/23); members CDefPointer<const CQuestCardDef> PDef +0xC, bools, FeatIndex, five CCharString +0x18..+0x28, Gold/RenownReward; sizeof 0x34 asserted. `#else` = untouched generated text. | opt-in | none needed (default path byte-identical); 0x00702A50 uses it |
| engine/CTCCreatureOpinionOfHero.h | new `#ifdef FABLE_RETAIL_CTCCREATUREOPINIONOFHERO` branch: `: public CTCBase`, virtual slots 1/6/8/9/14/21/22/23, the four IsWilling../IsFactionEnemy.. queries, real CDefPointer members, opaque `unsigned long X_[n]` at the retail offsets of the vectors / COpinionSet / lists / CIntelligentPointer (no member dropped), EOpinionAttitudeType ForcedAttitude +0x33C, HeroOpinionEnemy +0x342; sizeof 0x350 asserted. | opt-in | none needed; 0x00681850 uses it |
| engine/CLevelScriptBase.h | `virtual ~CLevelScriptBase();` wrapped in `#ifndef FABLE_RETAIL_CLEVELSCRIPTBASE` (retail's dtor is compiler-generated: every implicit level-script dtor tail-jumps to ??1CScriptBase 0x00CBD510; six `jmp ~CScriptBase` thunks 0x00CDD100..0x00CDD150; the Ego_r ??1 at 0x00CC2A70 is a debug-build out-of-line copy). | opt-in (default text unchanged for the 1,062 landed includers) | none needed; 0x00E32310 / 0x00E398A0 / 0x00E542B0 / 0x00ED39F0 use it |
| engine/CTCBase.h | inside the EXISTING `FABLE_RETAIL_CTCBASE` branch: `class CWorld;` forward declaration and `const CWorld& PeekWorld() const;` (protected, 0x00686A80, ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ; Ego_r 0x006B1E70 + FableWin 0x02190D60). Declarations only: no layout or vtable change. Rebased on chunk 712's RemoveFromGlobalTCs edit. | edit to an existing opt-in branch | all 39 landed includers (hdr/INCLUDERS_CTCBase.txt, files defining FABLE_RETAIL_CTCBASE or FABLE_RETAIL_CTCDOOR) rechecked with `vland.py --recheck-landed`: 2 MATCH + 37 RELOCATION_MATCH, all PASS (tmp/vland_recheck_CTCBase.log; 2 RELOC_NAME warnings pre-existing) |

Proof the overlay is the copy used (tmp/proof/overlay_proof.log, no `#error`): without FABLE_HDR_OVERLAY, 0x00702A50 and
0x00681850 are SRC_FAIL and 0x00E32310 tail-calls ??1?$CLevelScriptBase instead of ??1CScriptBase; with a member-shifted
copy of the overlay both rows go DIFFER (75v75 / 80v80); with the real overlay all MATCH.

Coordinator note: my first CTCBase.h copy predated chunk 712's RemoveFromGlobalTCs line, which turned 0x0055D590 into
SRC_FAIL in the first recheck. The copy was rebased onto the current repo header and the recheck rerun clean. If
another chunk edits CTCBase.h before landing, merge the two `PeekWorld`/`class CWorld;` lines instead of copying the
file over.
