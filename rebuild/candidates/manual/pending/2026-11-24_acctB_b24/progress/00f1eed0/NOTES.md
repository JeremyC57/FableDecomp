# 00f1eed0 CQ_ArenaScript::PlayWave -- B24B notes
## Lessons that apply (read before new ideas)
- TOOL FIX: Wine aborted every program with 'free(): invalid pointer' -> /run/user/0 missing on fresh machine (GOTCHAS 2026-11-18):
  `mkdir -p /run/user/0; chmod 700 /run/user/0`. Never pipe a first `wine` call into tail (wineserver -p keeps the pipe open).
- fg.py hunks whose R and O bytes are identical (`mov [x],0`) are display noise (norm() turns literal 0 into REL for ours only).
- SIB base/index order (REGISTER_SWAP_SUMMARY round 10, 007fcef0): follows c2 candidate rank, not spelling; change WHICH values
  are variables / which symbols exist, not how the sum is written.
- Codex OPERAND_BACKTRACE: key(local)=(ord<<5|1)&0x1ffff (0xb2e->0x165c1), key(temp)=(ord<<6)&0x1ffff (0x1d29->0x14a40); higher key
  goes first (base). Retail needs temp key > local key: +N symbols before the HUD statement with 220<N<727, or +M after it
  (temps only shift) with 110<M<727 (no wrap). So the cause is a repeated construct (~100+ symbols), not one-statement spelling.
- Codex dead ends (do not retry): STLport typedef/__n spellings (neutral), direct-start subscript (128 hunks), postincrement forms,
  PCH reconstructions, TU neighbours (AnimateCrowd/GivePrizeFund), explicit first-round compare (9525 bytes).
- Inline-budget devices (EXTRA_RULES 2026-11-08) allowed, smallest scope, marked `// Inline-budget:`.
## Progress (B24B)
- grades 1-10: best DIFFER(9262v9262) 1 SIB byte (unchanged). s2 = draft minus the two `CScriptThing NextCreature` inline_depth(0) regions
  (byte-neutral on current headers -> dropped). Neutral: ref-local spellings, include order (arena first), +60 Return() probe. Device-free
  source (no pragmas, no FORCEINLINE) = 9312v9262 161 hunks; /d2inlT 60/90 worse.
- fsolve on lk/s2.json (HUD tree = sites 10775..10782 pass a + 10788 pass b, all must flip; everything else kept):
  temps>=4544 only: T in {112..124 even} or {-912..-900}; all numbers >=2863: F in {112,120} (or -912..-900);
  all >=1200: F in {224,232,240,248} or -814..-768. So retail = ours + ~112..124 symbols allocated after the HUD import
  (or ~+232 before it). Narrow windows -> the construct must be counted, not guessed. Hypothesis: retail's sites that our
  inline_depth(0) pragmas block were imported then rejected by c2's inliner (symbols allocated, call emitted).
- KEY FINDINGS (lk readouts): (1) temp numbers (kind 15, >=4395/4544) never move when symbols are added -> temp-shift windows
  unreachable. (2) Target: HUD leaf `_this` (now s2862, hash 0x65c1) must land at 2048..2641 (221..814 fewer slots in the
  inliner range >=1825) with identical code; fsolve confirms every other sort survives F=-814..-221 (temps fixed).
  (3) Added inline calls shift the leaf +64 whether placed before OR after the HUD loop (allocation is position-independent;
  slots recycle). Additive probes always push the wrong way. (4) Removing one pre-HUD exit (x1) -> leaf 3102 and code changes.
- Pre-registered sibling opt-in set (landed CQ_ArenaScript rows 00/f1, 00/cf, e.g. 0x00F17C70 CArenaCellDoorGuard::Main):
  CSCRIPTTHING_IMPLICIT_COPY (21 rows), CSCRIPTTHING_IMPLICIT_ASSIGNMENT (3), CLEVELSCRIPTBASE (22, vs Codex's ORIGINAL_MEMBERS donor
  choice -- conflict noted), CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR (1), STLP_VECTOR_ASSIGN (1), CWIDESTRING (1).
- Device cleanup (all byte-neutral, 9262v9262 6 hunks): dropped inline_depth(0) at both `CScriptThing NextCreature`, at the final
  controller-wait cold return, at `CWideScreenMagicPauseEntities` ctor; GetPos/GetTime FORCEINLINE -> inline. Still needed: gift
  cold return region (drop -> 10 hunks, alignment), CreatedObject ctor region (drop -> 9313), CTimer::operator= FORCEINLINE (138),
  CRGBColour ctor3 FORCEINLINE (11). None of these moved the HUD leaf (lk: s2862 in s2, o3, o4, r23).
- Neutral & leaf unchanged: sibling opt-ins IMPLICIT_COPY/ASSIGNMENT/MOVIEBASE_OUTOFLINE_DTOR (o3), Game pointer + CLEVELSCRIPTBASE (o4).
- Symbol numbering model (s2 --syms): one shared counter for new symbols (kinds 3/4/5; Y events also log re-uses of existing
  numbers). c1 symbols of the WHOLE function come first (user locals up to ~1832: `_spawn_pos` 1825, `_PauseMyEntitiesPlease`
  1832 -- the gift block, after the HUD in source), then c2 inliner imports in rough source order (~1850..2990; HUD `_this` 2862).
  That is why +60 Return() calls anywhere shift the leaf +64. Kind 15 (strength-reduction temps, incl. t7465) has its own fixed
  base (4544) and never moves. So retail created 221..814 FEWER symbols (c1 anywhere in the function, or inliner imports
  before the HUD) than this source, with identical code.
## State for the next agent (B24B ending on row 1, rows 2-6 untouched)
Best: differ/00f1eed0.cpp (= r23.cpp) DIFFER(9262v9262), 6 fg hunks of which 5 are display noise; the one real byte is the SIB.
Next ideas (each: grade; if 9262 / 6 hunks, one `lk.py run` without --syms, read `grep ' s[0-9]* | c30$'` leaf, want 2048..2641):
 1. find c1-level constructs that create many symbols without code (string-literal CCharString temporaries, by-value CScriptThing
    returns, macro-expanded statements AR_START/AR_COUNT/AR_KEEPER) and their retail-plausible alternatives;
 2. instrument c2's inline expansion (GOTCHAS: expand bp 0x107723E7) to count imports per callee before the HUD import;
 3. STLport destroy/construct layer opt-ins (fable_containers.h) that change import counts but not code.
Do NOT retry: additive probes, include order, sibling CScriptThing opt-ins, Game pointer vs reference, for-macro spellings,
ref-local spellings, GETTER_CHAIN toggle (all leaf-neutral or code-changing).
