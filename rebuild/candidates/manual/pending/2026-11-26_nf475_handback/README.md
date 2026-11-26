# NF475 hand-back (2026-11-26): five more CAIStateGroup vftables at full size

`README_agent.md` is the agent's own report.

## Landed: b0_hdr (6 headers) + bAa (15 rows, 5 units), one commit after equiv_check
- **Units:** BirdGroundWander, KidHurtSelfRunToMother, Drunk, WifeGiveYouGift and KidWanderNearKids. Each lands its ctor (full 14-slot, 56-byte `??_7`, equal to retail), its `??_G` thin row, and its slot row respelt onto the PDB virtual.
- **T3 closed (`REL=nf324,alias1,nf318,nf320,nf319`):** lines 2950, 2150, 2330, 2429 and 2384.
- **Table:** the `?h1_9143c0` stand-in line is deleted. Its only emitter was 0x009143C0, and the PDB PeekWorld line is already bound.
- **Coordinator, after landing:**
  - 5 relabels and 15 SELF_SYMBOLS lines;
  - bindings_multi 1; funclets 1,749, 0 broken; s units 204, 0 failed; descriptive_check 2,030, 0 FAIL.
  - **Tracker:** +10, rtti_identified → agree_pdb, nothing worse.

## Held at landing
Both units passed every gate, but each has a row named in a T3 line the batch does not fully resolve:
- **MinionReactToAlert** (0x008FD50D, 0x008FF19C, 0x0090F0D0): nf374 line 3451 (0x008FCEC0) is only partly resolved; binders 0x00902045 / 0x0090F690 / 0x009612E0 are untouched. Its `rewitness` table line was not applied.
- **IdleAtHome** (0x008FDB12, 0x008FFEBC, 0x00917330): nf305 line 1339 (0x00666EF0) asks for the AutoPlaceNearBuilding class port of binders 0x00917330 / 0x0090FC50.
  - So r475a (the `// Inferred:` local) and r475b (the existing `reinterpret_cast`) are not needed now. They come back with that port.
  - The IdleAtHome header opt-in landed in b0_hdr: add-only, inactive by default.

## Held by the agent (bH_hold, 4 units, r467b)
MinionReactToNoise (0x0091CDC0), MinionKidGroupTag (0x00934920), MinionLeaveRegion (0x0091E4B0) and AvoidRegionExit (0x0092BCB0).

## Rulings (coordinator)
- **r474d: answered.** `return PTCGroupTag != 0 && PTCGroupTag->GetTagger() != 0;` with the PDB bool return gives retail's bytes, so the PDB spelling stands (0x00934100, in bH_hold).
- **r475a / r475b: deferred** to the nf305 port (see above).
- **r475c: accepted.** Locks of a finished agent may be taken over. The four held ctors are COORD_HOLD_NF475.
- **NF429's family claim** on CAIStateGroup_* in COORDINATION_CLAIMS.tsv is stale (no NF429 locks are left). The coordinator will close it with the next claims update.
