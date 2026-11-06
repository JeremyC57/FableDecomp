# Chunk 813 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 813, 0x00F17C70 CArenaCellDoorGuard::Main) When retail expands the implicit ~seh_me in two places, the loop form decides
  the match.
  - The two places: (a) the natural exit, reached from the loop-top terminate check (0x00F1994C), and (b) the first
    StartScriptingEntity wait pad, which joins (a) (0x00F17EBC). Every other return cross-jumps to one shared [~movie; ~seh_me; ret]
    tail.
  - Spell the loop as `for (;; NSF) { if (T) return _Fail_Val.Return(); ... }`. The `for (; !T; NSF)` form expands only (a) and
    duplicates every return epilogue (193 lines -> RELOCATION_MATCH).
  - So check which pads the expanded ~seh_me serves before picking the loop form. The same form made CFisticuffsMember worse there,
    because retail calls ~seh_me at its loop-top pad.
- (chunk 813, 0x00E5B3B0 CBeggarBully::Main) A declared-only `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();`
  also fixes which site is expanded, not only the call itself.
  - Without it, VC7.1 called the implicit ~MovieBase at both cut-scene scopes and tail-merged them.
  - With it, VC7.1 calls it at the first scope and expands it (call 0x006E7AB0; GROBase vtable store; ~CBaseObject) at the second,
    as retail does.
- (chunk 813, 0x00E5B3B0) The same-TU `inline __declspec(noinline)` CBasicString<char>::Compare body (landed 0x00F063B0) does more
  than keep `expression`'s data pointer in esi across an `== "A"` / `== "B"` chain.
  - It also moved the locals' frame slots to retail's (bools 0x42/0x43, expression 0x44; ours were 0x46/0x47 and 0x3c).
  - So when an expression-compare row is off in slot order, add the Compare body before trying declaration order.
- (chunk 813, 0x00DB5330 CNOVI_BarrelMan::Main) When retail expands the movie ctor at one site and calls it at the others, drop
  FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR (the implicit inline ctor). That took 789 -> 426 masked lines, and
  317 together with the `for (;; NSF)` loop.
- (chunk 813) The CIsThingAlive / CIsThingConscious copy blocker (GOTCHAS chunk 810) still holds. Two more spellings are identical:
  - the predicate bound to a const reference temporary (`const CIsThingAlive& alive = CIsThingAlive(Me); SetPredicate(alive);`);
  - a named predicate local in its own block.
  - CBeggarBully is now a clean witness: every other byte matches, and the 138 lines are only the one by-value Me copy to
    SetIsPushableByHero (expanded in retail) and its knock-on register colouring.
- (chunk 813) Fisticuffs switch case order was proven from the temporary-dtor flag bits (GOTCHA 806): cases 0, 3, 2, 4, 5, 1.
  - VC7.1 places case 1 after the loop by itself either way.
  - A store written before a call is scheduled before it (`queued_to_fight = true;` before `SFighterOne = Me;`).
  - A `const CScriptDef* def = pGlobalScriptDef;` local reproduces the def pointer loaded before GetHealth (the CRingFighter model).

Review at landing:
- These rows needed more than 4 grades: 00E5B3B0 (14), 00E84B80 (10), 00DB97A0 (6), 00DB5330 (5). Attempts are logged
  in STATUS.tsv.
- NAME_FIXES.tsv lists two wrong bindings in already-landed rows. Row 00e0f4e0 binds FadeOutAndKillEntity as `_NM1`. Row 00e8b020
  binds the CPredicate_And<CIsThingAlive,CIsVariableFalse> ctor 0x00E7E020 as `??0CAndPredicate@@...CIsFlagFalse`.
- LABELS.tsv corrects the autoname labels of the landed CFisticuffsMember ctor 0x00E83920.
- Two new headers: engine/CV_BeggarAndChildScript.h and engine/CV_FisticuffsClubScript.h. The second includes the existing
  engine/NScript_CV_FisticuffsClubScript.h and defines its forward-declared nested entity.
- 0x00E84B80 declares a row-local `NScript::CIsVariableFalse`: vtable 0x012EA43C (bound as ??_7CIsVariableFalse@NScript@@6B@ by the
  landed row 00e7e020), with slots operator() 0x00E7DDD0, Clone 0x00E7DDE0, dtor 0x00E7DE10 (none bound yet). No engine header for it
  exists; it should move to a shared header when one is made.
