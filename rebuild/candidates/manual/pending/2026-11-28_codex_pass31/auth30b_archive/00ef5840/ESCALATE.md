# 0x00EF5840 CPersonalScript_EmptyScript::CBanditWalker::Main -- ESCALATE (AUTH30, 2026-11-27)
- c0 (Codex draft, overlay): DIFFER(736v727). Uses row devices (row-local explicit specialisations of both CPredicate_And destructors under
  `#pragma inline_depth(0)/(1)`, an `optimize("gt",on)` region) and a STRONG `??1CIsThingAlive@NScript@@UAE@XZ` (new LNK2005 against
  landed 0x00E241B0). Its residue: both CPredicateBase vptr resets compile as calls to ??1CPredicateBase.
- g2 (best legal base, no row devices): header opt-in FABLE_RETAIL_CPREDICATE_AND_INLINE_DTOR (existing, repo) + `inline` ~CIsThingAlive as 89
  landed callers spell it. DIFFER(768v727). All base vptr stores are inline (ebx hoisted, as retail).
  - The one residue: inside the expanded outer `CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>` destructor, retail
    CALLS the 33-byte inner `??1?$CPredicate_And@VCIsThingAlive@NScript@@V12@@NScript@@UAE@XZ` (0x00EF5B50, at 0x00EF5937); we expand it.
    The same inner destructor IS expanded in retail for the separate And<A,A> temporary. ~CPredicateBase is expanded at the same depth.
  - Pick-any: 5 EQ (pickany_vs_retail), 8 EQ (pk_all), no NEQ; vftables 0x012E1B70 3/3 and 0x012F6628 3/3. The ??_G copies reached only through
    vftables differ (??_GCIsThingAlive 30 v 45, ??_GCScriptGameResourceObjectScriptedThingBase 30, outer ??_G 75); not decided (row is DIFFER).
- g3 (implicit, compiler-generated And destructor; a removed overlay opt-in): DIFFER(727v727), refuted. Equal length by coincidence: both
  derived vptr stores vanish and the inner destructor is expanded. Retail's stores show a user-declared destructor; Codex's 727-byte
  "implicit" probes are the same coincidence.
- Question for the owner: is there an accepted form for "expand at depth 1, call at depth 2" for one template destructor? Codex's depth,
  size-budget, forceinline and definition-order probes (its PROBES.md lists them) did not reach it, and I have no shared-header opt-in that
  does. Grades: c0 (736v727), g1 (768), g2 (768, best legal base), g3 (727, refuted) = 4. Even with a leaf match, ??_GCIsThingAlive in this
  object is 30 B v retail 45 (the HEAD siblings emit 39), the same blocker as 00d2d1a0 / 00da71e0.
