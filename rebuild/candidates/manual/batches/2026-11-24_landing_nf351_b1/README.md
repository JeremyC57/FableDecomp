# Landing nf351 b1 + b2 (Account A, 2026-11-24): NF348's follow-ups

Agent NF351's hand-back is in `pending/2026-11-24_nf351_handback/`. The rulings below are the coordinator's.

## b1: the D2 unit, 0x00437990 `CPlayerGui::AddHitByMelee` + 0x00643D8C `NPlayerGui::CDrawCombatMultiplier::AddHitByMelee`
- **Fix for NF348's hold:** the forwarder now uses the local spelling, `PDrawCombatMultiplier.Data->AddHitByMelee(...)` (NF326 Q3), so
  the CCountedPointer `operator->` accessor COMDAT that held NF348's draft is gone. No COMDAT is added or dropped.
- **Grade:** RELOCATION_MATCH PASS before and after, and from the repo after the catalog renames.
- **objsec:** 0x00643D8C is ALL_EQ. 0x00437990's only delta is `.drectve` + libcp. NF318 Q1's conditions are checked one by one in
  `logs/nf318_q1.txt`:
  - the switch FABLE_RETAIL_CPLAYERGUI_MEMBERS is defined by 24 landed rows;
  - every other section is equal;
  - the `.drectve` tokens are HEAD's plus libcp;
  - the UND sets differ only by the unit's callee rename, which has its own second source.
- **Link:** with OnHit 0x00667D30 everything resolves, with 0 duplicates.
- **Pick-any:** the new forwarder object has no copies.
- **Table:** one add (0x00643D8C, witness 0x00437990).
- **Catalog:** files, Module, name and manifest renamed per RENAMES.tsv.

## b2: 0x00E1A990 `?Main@CQ_WizardBattleScript@NScript@@UAEXXZ`, now virtual, in place (form B)
- **The change:** the row's local CScriptBase model declares the five PDB virtuals in retail slot order (the NF299 Q3 / NF303 Q1 form).
  The grade now lands on the named leaf instead of falling back to the largest function.
- **Gates:** objsec is ALL_EQ under the one rename, the COMDAT sets are the same, and no vftable, RTTI or `??_G` is emitted.
- **Ruling R2:** form B lands. Form A, the shared-header route, drops a made-up pick-any COMDAT in a move (NF318 Q5) and adds a helper
  section, so it is queued (T3 nf351).

## Rulings and queue
- **R3:** rejected. 0x0045C5F6 does not move to an interim stand-in, because its real name `??1CThingNoiseDef@@UAE@XZ` is known (names right
  the first time). 0x007FC800 waits for the class port, T3 nf334. NF351's measurement is on the T3 nf348 line.
- **Closed:** the T3 lines nf348 0x00437990 and nf348 0x00E1A990 (released at landing), and the NAME_FIXES lines nf345 0x00437990 / 0x00643D8C
  and nf348 0x00E1A990 (`NAME_FIXES_CLOSED.tsv`).
- **New NAME_FIXES nf351:** the ctor 0x00E1A800 emits `??_7CQ_WizardBattleScript` with base-class stand-ins. Retail's slots are 0x00E1DA20 /
  0x00E1A8D0 / 0x00E1A990 / 0x00E1A870.
- **Cleanup:** removed NF351's stray empty `tools/decomp_pipeline/D:\Documents\FableTLC/` tree (GOTCHAS 2026-11-24).
