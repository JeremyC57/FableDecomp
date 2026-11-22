# HANDOVER (early return, not an escalation) 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (2073 B), AUTH9 2026-11-22
AUTH9 returned early for context after 5 grades. No stall clause is met: the bounded hunt below is untried and breaks no rule.
History: AUTH1 `pending/2026-11-21_auth1/008e18d0/ESCALATE.md`, Account B p25N / p25R (FINDINGS 2026-11-20/21).

## Next step for the holder
1. Bounded inl.py hunt for the loop-2 frequency input: an inline helper the loops reach (GetTC on `LightningBeam*ParticleEmitters[n]`,
   CArray/vector operator[], IS_THING_ALIVE on an element v on the PSkyBeam member) whose branch gives loop 2 the extra x0.5 the sky
   lacks; check each form against Ego_d's line sizes (3316 / 3344 / 3356 GetTC lines).
2. Opt-ins the landed sibling 0x007A1380 (same upParams block, Codex p26) has and b0 lacks, untested here: CVECTORMAP_INSERT_FORCEINLINE,
   CTHING_ORIGINAL_VTABLE, CINTELLIGENTPOINTER_ASSIGN/_DEREFERENCE, C3DVECTOR_PACKED, STLP_VECTOR_OVERFLOW_OUTOFLINE, STLP_OUTOFLINE_UNINIT_*.
   Not applicable from retail's call list: UPDATE_DECL_PASS26 (0x008E18D0 never calls 0x0076C750) and CTOR_NOINLINE (never calls 0x006E24D0).


## Best sources at HEAD (re-measured)
- `$S/w22/auth9/008e18d0/b0.cpp` (= AUTH1 r1) + overlay `hdr/` (IMPLICIT_DTOR, re-based on HEAD's header which gained Codex p26's
  VECTORKEY / UPDATE_DECL opt-ins; `hdr.diff` applies to HEAD): DIFFER(2045v2073) default flags, 753 diff lines.
- Same with /Gr /d2inlT65: DIFFER(2149v2073), 660 diff lines (p25R's base).

## New this pass (negative)
- The original key wrapper NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE (FableWin.pdb name, 0x025F1360; landed sibling 0x007A1380
  Activate uses it for the same LIGHTNING_START / LIGHTNING_END pairs) in place of CCRC::Calc(0, s, strlen(s)) is byte-neutral: b1 =
  b0 at default T, and T60 / T65 / T70 under /Gr = 1928 / 2149 / 2277, the numbers p25N / p25R measured for the Calc spelling. It is
  still the better-attested spelling for the final text (two sources: FableWin name + the sibling row), so keep it if the row matches.

## The residue (unchanged; FINDINGS 2026-11-20/21 p25N, p25R)
- LowerBound (GetTC) in loop 2 v the sky block: identical c2 records (sz 60, c1 52, f 6.25, score 63); retail calls the first
  (0x0040F020) and expands the second.
- ~CParticleUpdateParameters loop 1 (retail CALL 0x006E2500) v sky (EXPAND); the dtor COMDAT's inner ~vector<pair<ulong,ParamDesc>>
  scores 65 in every shared-header form; 0x006E2500 lies outside creature_modes.obj's range (its COMDAT came from another object).
- Frame: at /Gr /d2inlT65 this text's frame is 0xB0 like retail (p25N's 0xA0 was /Gr T64 on another base; T64 not regraded here).

## If the hunt fails
Then escalate with AUTH1's question (frequency model / Windows request), citing the hunt's measured forms.
