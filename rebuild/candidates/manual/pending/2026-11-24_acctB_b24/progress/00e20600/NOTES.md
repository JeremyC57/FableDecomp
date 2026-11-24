# 0x00E20600 CTraderToEscort::Main -- agent B24A notes

## Tool fix (B24A, 2026-11-24)
Wine (wineserver and every cl.exe run) aborted with `free(): invalid pointer` (rc -6 / -11) whenever TMPDIR is set in the
environment (bisected: TMPDIR=/tmp alone triggers it). Fix: `unset TMPDIR TMP` after sourcing env.sh in every call (fg.py,
q.py, vt.py, addmatch.py, vland all start Wine).

## Lessons that apply (read before new ideas)
1. Codex pass 25: all of the residue is the prefix predicate cleanup (two out-of-line ~CPredicateBase calls where retail stores
   0x012C2FF4 inline); the rest (pop ebp placement, alignment nops) is fallout from ebp not holding that constant.
2. Probe t2: `#pragma inline_depth` around a TEMPLATE member definition has no effect (instantiation is elsewhere); around an
   explicit `template<>` specialization or a plain inline function it does.
3. Probe t4/t5: inline_depth on ~CIsThingAlive (with or without __forceinline base dtor) cannot give retail's split (at the same
   nesting level the big ~CScriptThing is called, the tiny ~CPredicateBase expanded): that split is a size decision.
4. Probe t3: plain inline everywhere, ~CScriptThing body visible, gives retail's And half (~CIsThingAlive members called out of line
   inside the expanded And dtor, And base store inline) through size alone.
5. Inline-budget residues may use /d2inlS<N> / /d2inlT<N> (EXTRA_RULES 2026-11-08, preference 2), marked `// Inline-budget:`.

## B24A grades so far (hdrA = pass25 overlay minus the two inline_depth(0) wraps, plus ESC_* knobs)
- baseline v0 (pass25 overlay): DIFFER(14526v14510) 45 hunks; everything outside the prefix is ebp-push fallout + padding.
- natural (no wraps): And half right (members ~CIsThingAlive called, base store inline), temps call ~CIsThingAlive (e241b0) -> 14494.
- budget inliner (`#pragma optimize("g",on)` + /Og-, as 0x00D17A80): prefix EXACTLY retail shape, but the rest expands
  ~CScriptThing / MovieBase and calls tiny helpers -> 15225..15358; /d2vol2 14361. Not this TU's inliner.
- /d2inlT55..70 on natural: non-monotonic, whole function moves.
- implicit ~CIsThingAlive (FableWin has ??_G and implicit copy ctor but NO ??1CIsThingAlive; also no ??1CScriptThing): I1
  14638/91 hunks: And exactly right (same offsets 0x294/0x280/0x284, push ebp at top), temps expand ~CScriptThing fully (retail calls
  0x4AA840). Rest = frame-slot fallout of the temps.
- implicit ~CIsThingAlive + ~CScriptThing defined under inline_depth(2): 14542/46; (1): 14478/71; (0): 14446/82.
- implicit ~CIsThingAlive AND implicit ~CScriptThing: 14574/63, And dtor out of line -> worse.
- statement-scoped inline_depth(1)/(2) around SetPredicate: no effect; (0): operator&& called.

## B24A key findings (c2 inline trace, $W/inl/run.sh = agent3009's tool, ~6 min on Main, seconds on $W/sz/base.cpp)
- Heuristic inliner (default /O2): c2 compiles callees first (bottom-up) and Main INHERITS ~CIsThingAlive's own-pipe body. ~CIsThingAlive
  own pipe expands ~CScriptThing (sc 63 < T74) -> sz 84; temps' site c1 = 84-6 = 78 >= 74 -> called. IL size is spelling-insensitive
  (four ReleaseData spellings all sz 53). So no heuristic-path form gives retail's temps (~CIsThingAlive expanded, ~CScriptThing called,
  base inline) WITHOUT changing ~CIsThingAlive's own-pipe body.
- PICK-ANY: Codex's baseline device (inline_depth(0) on ~CIsThingAlive) emits ??1CIsThingAlive = `vtbl; call ~CScriptThing; jmp ~CPredicateBase`,
  which differs from the landed owner 0x00E241B0 (full expansion) and Main binds it (And members) -> that device can never land (held).
  Implicit ~CIsThingAlive is refuted the same way (its copy lacks the own-vtable store that 0x00E241B0 has).
- BUDGET inliner (`#pragma optimize("g", on)` + `/Og- /d2vol20`, the landed 0x00D17A80 quest-family device): predicate prefix EXACTLY
  retail AND ??1CIsThingAlive copy = 0x00E241B0 (needs /d2vol>=20; default/10 calls ~CCountedPointer inside it). The rest of Main then
  differs: 15225 v 14510 (tiny TRetVoid::Return / CDefPointer-> called at 8/13 sites, ~CScriptThing expanded 13x, MovieBase split).
  This is the only mechanism found that explains the prefix and the pick-any copy together; next: fit the rest under the budget path.
- hdrA/ hdrB/ are probe-only overlays (ESC_* knobs); hdr/ is the landable pass-25 overlay (hdr/README.md).
