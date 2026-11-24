# 0x00975BB0 CAIStateGroup_JackOfBladesStage1::Update -- escalation (B24A7, 2026-11-24)

## 1. Inferred device: nine dead release ASSERTs (needed for the match)
The source is differ/00975bb0.cpp and payload.json. It defines FABLE_RETAIL_ASSERT_DEAD_ARM and includes fable_assert.h, the shared header
that 72 landed rows already use. It places FABLE_RETAIL_ASSERT(p) after each unchecked pointer result: ptc_boss, ptc_combat, p_jack_def.Get(),
p_TC_hit_locations, p_hit_location_final, ptc_targeted, ptc_solid_to_thing, ptc_perceive_thing and ptcd_obstruction.
- Mechanism (measured with the IL tools, il2.sh): in the combat-loop GetMagnitude / Normalise (0x00976483..0x009764E1), c2 sorts the sum of the
  three CSE product temps by key = (temp number << 6) & 0xFFFF. Retail stores XX->[78], ZZ->[20], YY->[48] and sums XX+YY+ZZ. That order needs
  the fresh ZZ / YY temps past a 1024 boundary while the free-list XX temp is not. Without the dead arms the keys are XX 779, ZZ 923 and YY 927,
  so the sum comes out YY+ZZ+XX.
- Counts: 7 and 8 asserts give +96 (ZZ key 1019, still wrong); 9 and 10 asserts match. A probe of 24 dead float statements also matched,
  which shows the lever is the temp count, not the wording.
- Evidence that asserts exist: the FableWin.pdb copy of this function is 1952i / 8620b at /Od, against 1218i / 4902b for this text with /EHsc.
  That is +734i, and fwsize flags it "assert?". One debug-form ASSERT costs about 42i at /Od /EHsc (fw/acal.cpp), so the debug build holds
  roughly 10 to 17 ASSERTs. Their conditions are not recoverable, because release drops them; the count is what is evidenced. Precedent:
  0x00B10B10 (11 dead asserts, landed).
- Evidenced alternatives tried, none of which reaches the window:
  - GetTC<T> chain: +32.
  - IsDead direct: -32.
  - IsStillRunnable: -1.
  - GetMagnitude named local: -2.
  - ADD/SCALE as named results: +43.
  - SUB as a named result: breaks site 1.

## 2. Pick-any copies: all 13 EQ (pickany.log). Kept fixes:
- CINTELLIGENTPOINTER_CTOR_OUTOFLINE (existing opt-in). Retail calls 0x0065E133 out of line at 0x00975F08.
- CJACKOFBLADESBATTLEDEF_TYPENAME_PASS26 (existing opt-in). Landed sibling 0x00974AA0 uses the same text.
- NEW overlay opt-in FABLE_RETAIL_CTHING_GETTCDRIVER_PHYSOBSTRUCTION_SIZE_OWNER, size-pragma scaffolding under the 2026-11-20 rule.
  It is an explicit specialisation `template<> inline __declspec(noinline)` under optimize("s"), the pattern of the landed
  CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26. The body is the landed owner 0x007890EA's text (`UsingComponents.test(id)`; the owner sets
  optimize("s", on)).
  - Condition 1 holds: the row grades RELOCATION_MATCH with and without it, and only that COMDAT changes (12 EQ + 1 NEQ becomes 13 EQ).
  - It is header scaffolding, not row text. Retail's own object had no such device; it is there only so the pick-any copy equals the owner.
  - Tried and NOT kept, because none of them changed the copy: in-class and out-of-class size pragmas, inline_depth(0), and an explicit
    instantiation. A HasTC body under the specialisation gave a 36-byte copy (HasTC called out of line).

## 3. Assert-count window (graded): 7 and 8 asserts DIFFER; 9, 10, 11, 12 and 14 RELOCATION_MATCH. 13 was not graded but is inside the window.
The upper bound comes from site 1, whose YY key 822 wraps at about +202. This count depends on helper texts that still differ from FableWin
sizes (GetMagnitude 24i v 26i, the vector operators 28i v 34i, GFSqr 6i v 10i, Normalise 43i v 48i). If those are retexted, the window moves.
