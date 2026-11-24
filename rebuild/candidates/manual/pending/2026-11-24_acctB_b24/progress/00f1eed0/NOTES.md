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
- (B24B2) fg.py drops branch targets (`J jcc`): always also run rawcmp.py on the q.py log (r23 hid a wrong jne target).
- (B24B2) A SIB/operand order between an imported symbol L and a kind-15 SR temp T: BOTH move. Flip iff T mod 2048 in 1024..2047
  and 2*(T mod 1024) > L mod 2048; read both with leaf.sh. More inline expansions raise T a lot (d1b: +489).
- (B24B2) Inline-device questions are cost questions: B24A's decision trace (inl/run.sh) shows each region's removed decisions.
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
## B24B2 (2026-11-24)
- RAW CHECK: fg.py normalises jump targets away (`J jcc`). New $W/rawcmp.py on a q.py log compares raw bytes incl. branch
  displacements. r23 (B24B best) has a SECOND raw difference: jne at 0x00F20EF3 -> 0xf20ca1 (retail 0xf212cc), i.e. the dropped
  controller-wait inline_depth(0) region IS needed (Codex was right). r3 (that region kept) = SIB byte only -> new baseline b0.cpp
  (= differ/00f1eed0.cpp). Devices now: gift cold return region, CreatedObject ctor region, controller-wait cold return region,
  CTimer::operator= FORCEINLINE, CRGBColour ctor3 FORCEINLINE. Every screen from here: fg + rawcmp.
- fwsize (FW_EXTRA=/EHsc) b0: PlayWave ours 4220i/17998b v FableWin 4345i/18217b (FableWin = older revision with the gift loop
  + EntitySetCutsceneBehaviour, ~15i of the gap). CTimer ctor/dtor/operator=/GetTime each FableWin +1i/+2b (text of CTimer
  bodies differs slightly from ours). ego_r.exe.signatures: Ego_r release PlayWave 0x00CF07F0 = 11459 b / 2886 i (other revision).
- Allocator model (s2 --syms, read by B24B2): ONE counter; temps (kind 3, site 0x1075C331) take single numbers, locals (kind 4)
  take 32-number chunks reserved at the counter rounded up to 32 when the previous chunk is spent. So a number ~= count of all
  symbols allocated before it. c1-phase (IL read, whole function): ~1842 numbers (HUD-loop i = 1164: ~1164 from source before the
  HUD, ~680 after); inliner phase 1843..2862 (~1020) before the HUD import.
- Instrumented copy $W/lkx (lk_gdb.py + inline-decision hook 0x1078CF71: ebx = callee sym, edi = call node; Y events get names
  at PIPE time) to count inline imports per callee before the HUD import.
Pre-registered candidates (each: scr raw-clean? then leaf):
 A. donor gift loop + break (Codex main_arena_donor_loop_break, FableWin has the loop): more text, single site.
 B. plain `return;` for the exits (landed CQ_ArenaScript::Main / CArenaEnemy::Main spell `if (T) return;`; but Ego_d has _Fail_Val
    in PlayWave per Codex) -- slope ~1/exit, ~-75: alone not enough.
 C. CTimer inline bodies as FableWin-sized text (fwsize +1i each) -- per-site multiplier (3 operator=, 3 GetTime, ~exits x ~CTimer).
 D. header inline bodies imported at every site where retail calls out of line (CScriptThing ctor, ~vector, operator=).
- COST TABLE (b0y.json, lkx with temp-creator caller): counter = c1 IL read of the whole function 64..2213 (countdown ~74 per
  keeper+wait round; vectors..spawn-loop start 427..589; spawn loop 589..1459 (each spawn branch ~270); HUD..gift 1459..1770;
  gift ~90; end 2213), then a BURST of 470 temps 2214..2684 before any inline decision (caller 0x10747D50: one shadow temp per
  symbol node, [sym+0x28]), then the inline phase adds only ~146 new numbers before the HUD import (most imports reuse freed
  numbers: 1843/1844/2786/2812/2858 recycled). HUD `_this` = chunk 2848 + 14; needs chunk base <= 2624.
- Exit-cost probes: +10 `if (T) return _Fail_Val.Return();` at the top (3 live dtor objects) -> leaf +53 (pe1); inside the j loop
  (7 live) -> +128 (pe2): ~1.9 numbers per live destructible object per exit. Exits x live objects (~69 x ~6) is the biggest cost.
- Retail has 67 IsActiveThreadTerminating calls v our 69 `if (T)` in the body: 2 are c2-hoisted per-arm checks (e.g. 0x00F1F4F9 call
  hoisted above `cmp edi,1`, test al per arm) -- consistent with our per-arm source, not a text surplus.
- Screens: cB (plain `return;`, no TRetVoid) 9326v9262 (Return() sites feed the inline budget: keep); v2 (no for-scope macro)
  11061 (macro is semantic under VC7.1 /Zc:forScope-); v4 (spawn_pos folded into the call) 9279; v1 (donor gift loop + break)
  raw-clean, leaf 2863 (+1: single-site, neutral).
- CORRECTION to B24B's window: the SR temp is NOT fixed. Kind-15 numbers start at the counter after inlining rounded to 32
  (b0: max 4517 -> base 4544) and the HUD temp is base + 2921 = t7465. pe2 (+10 deep exits) gave t7571 (+106) with L 2990.
  Exact flip rule (keys: temp = (T*64)&0x1FFFF, local = 0x10000|((L&0x7ff)<<5)^(L>>11)): retail order iff T mod 2048 in
  1024..2047 AND 2*(T mod 1024) > (L mod 2048) (+ tiny xor term). Now T=7465 (mod1024 297, bit10 set), L=2862 (814):
  594 < 814. With L += a and T += b: need 2b - a > ~220, b <= 726, a >= -814. So it is NOT only "fewer symbols": a uniform
  +221..+726 (B24B's own fsolve: F in {224..248} for numbers >= 1200 keeps every other sort) or +111..+726 symbols allocated
  after the HUD import but before the SR pass (inline phase / CSE) work too. B24B's additive probes read only L, never T.
- ALIGNMENT DONE (all raw-clean, SIB only): b1 = Game pointer + FABLE_RETAIL_CLEVELSCRIPTBASE (landed siblings; was Codex's
  ORIGINAL_MEMBERS reference); b2 = Pass25 header bodies moved in-row as landed Main/DoorGuard do (getter chain = the 28-row
  landed GetPos spelling); b3 = clean overlay hdr2 (current CRGBColour.h + FABLE_RETAIL_CRGBCOLOUR_CTOR3_FORCEINLINE only).
  differ/00f1eed0.cpp = b3 (needs OV=$W/hdr2). 4-arg sibling CRGBColour(0,0,0,255) without the force: 9266 (keep the opt-in).
- PICK-ANY (b0 object): only NEQ = ??BCDefString 0x00415DD0 (17 v 25 b, not leaf-referenced) = the FABLE_RETAIL_CDEFSTRING_
  OUTOFLINE_GETSTRING copy that the 44 landed NScript rows with the same opt-in already emit (HEAD debt per EXTRA_RULES
  2026-11-20, not new). GetPos / GetDataString copies EQ.
- For-scope macro after the includes (STLport loops unwrapped): 9374 -- the macro must wrap STLport too (as Codex found).
- DEVICE TRACE (copied B24A's inline-decision kit to $W/inl; `INL_F=PlayWave inl/run.sh <src> <tag> $W/hdr2`, ~3 min;
  inl/b3.txt v inl/n3.txt = b3 without its three inline_depth(0) regions): the regions remove exactly these decisions --
  gift cold return: ~CTimer expanded (f=0.1339, c1 25, sc 48; retail calls), CreatedObject: ??0CScriptThing expanded (f=0.1339,
  c1 34, sc 65; retail calls 0x006E7B40), controller-wait return: ~CTimer expanded (f=0.1004, sc 56; retail calls). Late sites
  carry pen=11 for ~CTimer (c1 = sz+pen-sav = 25), early ones pen=0 (c1 14). A uniform late-site ~CTimer c1 in [38.5, 44.6) would
  call both cold sites and keep every other late decision (825 f=.1785 / 856 f=.2008 expanded, 872 f=.0502 called).
- ~CTimer text probes WITHOUT the two cold-return regions (CreatedObject region kept): current body n2 = 17 raw-differing lines;
  d1 `{ CGameScriptInterfaceBase* game = pDSTGame; game->DeregisterTimer(TimerIndex); }` (sz 19) = 92 raw lines after the resync
  fix (the old rawcmp stopped at the first address shift; '4' was wrong); d1 also EXPANDS the gift/controller cold sites 833/862
  (c1 23, sc 44/52), so it does not remove the devices; first differences at the countdown exits (retail 0xF1EF96 jne
  0xF1F059 = expanded Deregister tail; ours jne 0xF1F1DB). d1b (d1 + regions) = 53 raw lines after the resync fix. So the cold-region
  devices are a ~CTimer cost question (family root cause, B24A 00d09010/00d11930): the right ~CTimer text is still open;
  FableWin sizes say all four CTimer bodies have one more /Od instruction than ours (15i/39b etc.).
- KEY RESULT (L/T probes, OV=hdr2): n3 (b3 minus all 3 regions) L 2862 T 7483; d2b (b3 + `long index` local in ~CTimer)
  raw-clean (1 line, rechecked), L 2875 T 7497 (2b-a = 51, no flip); d1b (b3 + `CGameScriptInterfaceBase* game = pDSTGame;` in ~CTimer, sz 19 ->
  c1 13 < inlS+5 = 14, ALWAYS inline) L 2964 T 7954 -> RETAIL SIB ORDER (lea eax,[edi+eax+0x30]) but 53 raw-differing lines (resync rawcmp; the
  earlier '3' was the old compare stopping at the first shift); first difference at the countdown: retail keeps the expanded Deregister tail at 0xF1F059 (first post-wait exit) and sends the
  2nd/3rd-wait exits to the called tail 0xF212F0; d1b expands ~CTimer at every site and cross-jumps the countdown copies to the
  LAST one (0xF1F1DB). Whether the other extra expansions are hidden by cross-jumping is NOT established (53 lines differ). So: retail's
  extra symbols can come from ~CTimer (or similar) expansions that cross-jumping hides; but retail did NOT always-inline ~CTimer
  (its countdown sites 20..33 are called), so d1b is not retail. Diagnostic e1 (d1 + inline_depth(0) on three 2nd/3rd-wait
  returns, cold regions dropped): retail SIB, 87 raw lines (resync); e2 worse. These are fitted devices on an un-evidenced body:
  ESCALATE only, never land.
## State for the next agent (B24B2 ending, context spent; rows 2-6 untouched)
Best landable: differ/00f1eed0.cpp (= b3.cpp, OV=$W/hdr2): DIFFER(9262v9262), raw = the SIB byte only, devices: 3 inline_depth(0)
regions + forced CTimer::operator= (in-row, landed Main precedent) + CRGBColour ctor3 opt-in (hdr2). Pick-any: only HEAD-debt
??BCDefString NEQ. Tools: scr.sh (grade + raw), leaf.sh (L and T; LKDIR=$W/lkx for the instrumented copy), inl/run.sh (decisions).
Best next idea: find the retail CTimer text (FableWin: each of the 4 bodies is +1 /Od instruction, +2 bytes v ours; d1/d2 local
forms are 16i) whose ~CTimer keeps c1 >= 14 at the countdown sites (called, as retail) yet raises the late inline numbers; read
the effect with leaf.sh (flip iff 2b - a > ~220) and rawcmp. Also try the same for ~CScriptThing / ~vector / MovieBase forms.
- CORRECTIONS (B24B2, after advisor review): rawcmp.py now resyncs (difflib); recount: b0 1, b3 1, r23 2, d2b 1, n2 41, d1b 53,
  d1 92, e1 87. "The for-scope macro is semantic under /Zc:forScope-" is UNTESTED (only: no macro = 11061). "44 landed rows emit the
  same ??B copy" is PRESUMED (44 rows define the opt-in; copies not inspected). 00f1eed0_best_B24B.cpp = r23 text (wrong jne at
  0x00F20EF3): SUPERSEDED by b3. b3 re-verified at the end: rawcmp 1 line (0x00F1FFB8), pick-any only ??BCDefString NEQ.
## B24B3 (2026-11-24)
- LESSON (lane-wide): FableWin.exe.signatures.tsv strict_signature is an EXACT text oracle. Re-implemented Ghidra's canonical form
  (gsig/ghash.py, calibrated on FableWin bodies; probe TU builder ct3/mk.sh). All four CTimer bodies = the CURRENT landed text
  compiled by VC10 (which splits `call [vt+X]` into `mov eax,[vt+X]; call eax`: the fwsize +1i/+2b). CRGBColour(EEE) +3b = VC10
  movzx byte loads; text = ours. operator== = `return GetTime() == time;` (VC10 setz idiom). Local-variable ~CTimer forms (d1, d2)
  are not FableWin's text. => The CTimer text is NOT the lever for the SIB; ESCALATE open question 1 closed. SHARED_FORM.md written.
- Pick-any doubt settled: landed 0x00EB3D10 (defines OUTOFLINE_GETSTRING) compiled at HEAD emits the same NEQ
  ??BCDefString 0x00415DD0 (17 b, not leaf-referenced): HEAD debt confirmed, not this row's.
- /Zc:forScope doubt settled: b3 without the macro = 11061 under /Zc:forScope AND /Zc:forScope- (2987 raw lines): the VC6 macro
  is codegen (its `if (0) {} else` head), not a scoping switch.
- Family flag probe: b3 + /d2inlS15 or /d2inlS11 = 9262 but 48 raw lines; n3 (no regions) + S15/S11 = 9302 (220). Not a lever.
- B24A4's family recipe (/d2inlS15 + scoped inline_depth(0)) needs a ~CTimer of c1 >= 20; the original text measures c1 14 (sz ~20),
  so that branch is closed from lane B's side.
State: unchanged best b3 (SIB only); remaining lever is the a/b symbol-count inequality with no evidenced form. Moving to row 2.
