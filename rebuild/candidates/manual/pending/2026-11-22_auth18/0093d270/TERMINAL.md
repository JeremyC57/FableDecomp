# 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable: terminal (A), 2026-11-22

Advisor ruling (2026-11-22), on AUTH18's ESCALATE.md: outcome (A), not a `const float&` opt-in. The reference form contradicts all three
PDBs' by-value signature (FableWin `?GetProximityForNavigation@CCombatManager@@QBEMXZ`, Ego_d line 55 = 76 B, Ego_r LF_MFUNCTION 0x25AAC
returns float), so it cannot land on advisor or coordinator authority. Only the owner can override.

## The one-grade oracle (pre-registered by the coordinator, graded once; `oracle_ref/`)
- Candidate: AUTH18's best rule-clean text `e1.cpp`, unchanged, plus one header line: CCombatManager.h:144
  `const float& GetProximityForNavigation() const` (`oracle_ref/CCombatManager.h.diff`). Stock flags.
- Control (header as landed): **DIFFER(305v295)**. Oracle: **RELOCATION_MATCH**.
- So the whole residue comes from this one inline's return form: no other text in the row is unexplained.

## Why it ends here
Retail's argument order (the manager pointer held across GetPItem, +0x28 read after it) is what VC7.1 3077 emits for a reference-
returning inline, and for no by-value form measured: a by-value float or long getter loads before the next opaque call (mini m3),
retail itself homes a by-value float getter's return (0x00AC361A in landed 0x00AC2BE0), and no flag, LTCG or c2 inline path moves it
(ESCALATE.md, LOG.md). The PDBs on the lane are a later VC10 revision; retail's TLC-era header probably differed here.

## The sibling: one item with 0x006AA720
0x006AA720 CThingPlayerCreature::SetPlayerCreatureActionForMelee contains the other retail +0x28 read (0x006AAA24), calls the same
getter, and is parked at DIFFER(1218v1212). Its 2026-11-03 draft hung the compiler (cl.exe stopped after 11 minutes), so the oracle is
not graded there. Treat the two rows as one item: whatever settles the getter's TLC form settles both.

## Reopens only on
- TLC-era evidence for this inline: a TLC header, PDB or map showing a reference-returning or public-member form (queued for the next
  Codex package, `pending/NEXT_CODEX_QUESTIONS.md`), or
- the owner choosing to land the reference form as an Inferred opt-in against the three PDBs (the oracle above is the evidence).

Best rule-clean text: `e1.cpp`, DIFFER(305v295). The 297v295 on record came from hand219's local struct model, not rule-clean.

## Owner override (2026-11-22): landed as Inferred
The owner chose "land as Inferred" for this row and 0x0066B370 (same mechanism, three retail sites in two classes). Landed:
`batches/2026-11-22_landing_auth18_isrunnable/README.md` (engine/CCombatManager.h `FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY_REF_INFERRED`).
