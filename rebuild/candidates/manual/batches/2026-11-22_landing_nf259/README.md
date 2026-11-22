# Landing nf259 (Account A, 2026-11-22): five CCombatActionBase / CCreatureActionBase vtable slot self rows on PDB members, six CCreatureAction rows off local models (11 rows)

Agent NF259 (q12730, q12731, q12732 done; q12659 open). Hand-back: `pending/2026-11-22_nf259_handback/`.
- **q12730 (slot self rows, both PDB name sets + retail vftable slots 78 / 79 at 0x01259E44 and 50 / 65 / 67 at 0x01259D24), first form each:**
  0x0062EAC0 GetIsToFaceTargetCone returns `1.0f / 9.0f` and 0x0062EAD0 GetIsToFaceTargetMinDist returns `0.5f` (both values in VC7.1's float
  literal pool with other readers, so the made-up externs are gone; the 1/9 wording `// Inferred:`); 0x00692670 IsPreventingLookTurning
  `return IsPreventingNormalMovement() ? true : false;` MATCH; 0x00692880 / 0x006928B0 call `PAnimEntry->GetFlags()` through
  engine/CCreatureActionBase.h / CAnimationEntry.h (0x005DC210 2 -> 1; the cat_probe table line deleted). Reached only through vtables;
  vtmap on 0x00955620 flips slots 50 / 65 / 67 / 78 / 79 DIFF -> SAME. Five new tests on the shared headers link the real row object (two old
  tests printed PASS without checking).
- **q12731 (five rows off local models, first form, every leaf / callee spelling unchanged):** 0x008B7A20 (L72 prelude + CSummonDef.h /
  CTCDParticleEmitter.h; def +0x44 read as the header's SummonEffect), 0x008C2AD0 (CTCHealLife.h + new opt-in, CCombatAction_WillHealInto.h),
  0x008C0560 (gains its Leaf line), 0x0096AE50 (the banned renaming-macro CastAssassinRush model gone; its pick-any `??1` EQ owner 0x0072D990),
  0x006F5C10 (object identical to HEAD). Header-less classes (CTCAISummon, CCombatAction_WillHealRelease, CCreatureAction_MidAttack360) keep
  their HEAD local declarations.
- **q12732 0x008F41D0:** the L92 prelude + DRAGONFIREBEAMINTO_INITIALISE / DEREFERENCE, first form.
- **Headers (add-only, declarations, both PDBs):** CTCHealLife.h `FABLE_RETAIL_CTCHEALLIFE_SETAICAST` (0 asserts pinned),
  CCreatureAction_CastAssassinRush.h `FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_TARGETCTOR` (16 pinned). Whole-tree equiv_check: 0 of
  57,125 differ (`logs/equiv.log`, DONE 0; the run finished after the agent handed back, coordinator confirmed).
- Link (11 new objects + HEAD controls): 0 duplicates (positive control LNK4006). pk: every new copy with a retail owner EQ; three NEQ
  copies (0x008C2AD0, 0x006F5C10) identical at HEAD. No new vtable emitted; no funclet / fold includes a changed file.
- **Recheck** (`logs/recheck.log`, landed tree): the 11 changed rows + 0x00955620, the header includers 0x0072D990 / 0x0072CAA0 / 0x007C3FB0 /
  0x007C4430 / 0x008C2550 / 0x008CD470 / 0x009735D0 and the GetFlags rows 0x005DC210 / 0x00663140 (21 rows): 1 MATCH + 20 RELOCATION_MATCH,
  all PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF259's questions
1. The two add-only declaration opt-ins: accepted.  2. The 1/9 literal spelled `1.0f / 9.0f` (value proven, wording inferred, marked):
   accepted.  3. 0x008B7A20 reading def +0x44 as the header's SummonEffect (the HEAD local's SummoneeEffect; same bytes): accepted.
4. NAME_FIXES queued: 0x0122F59C's two made-up data names for the pooled 0.5f; CCombatActionBase slot rows 0x0062EA40-0x0062EAB0 (slots
   71-77, FableWin + the slot); 0x00692890 AllowsCombinationAnims; the CastAssassinRush local models in 0x0072CAA0 / 0x008CD470.
5. Header tests for the six entry 2 / 3 rows: queued.  6. q12659 to the next agent.
Rule slips (two GOTCHAS extracts briefly outside the agent folder, deleted; an evidence line after a failed cmp, corrected;
COORDINATION.md read late): noted.
