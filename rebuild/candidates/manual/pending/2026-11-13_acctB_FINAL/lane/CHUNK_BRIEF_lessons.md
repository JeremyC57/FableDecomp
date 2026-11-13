# Chunk 2050 -- 1 never-attempted rows of the acctB reservation (this brief OVERRIDES PROMPT.md where they differ)

You are an expert in reverse engineering and in decompiling retail x86 game code into clean, portable C++. You recover the
C++ a Lionhead programmer wrote and you know how VC7.1 RTM (cl 13.10.3077) turns each spelling into machine code.

## Rows (catalog labels are guesses; prove every name)
- 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate (2974 bytes)

These are large, never-authored rows. Lengths come from a recursive-descent extent scan; confirm the first and last
instruction yourself (a row may be a fragment, may hold several functions, or may own cold code after its ret). If the true
length differs, write it in SPLITS.tsv (`address<TAB>true_length<TAB>evidence`) and tell the coordinator in your report; the
oracle `$S/batch/chunk2050_oracle.tsv` is what q.py grades against, so a wrong length must be reported, not worked around.
It is fine to hand back fewer rows exactly rather than many rows nearly. Start with the row you judge most tractable.

## Environment (every Bash call starts with these two lines; shell state does not persist)
    export S=/opt/fable_lane
    source $S/wine/env.sh; export S=/opt/fable_lane; cd /home/user/FableDecomp
- Your directory: `/opt/fable_lane/w22/agent2050` (src/, test/, differ/, probe/, hdr/, tmp/, bin/, scripts g.sh / gall.sh / tall.sh / tall.py /
  finish.py, dis_all.txt = the retail disassembly of your rows). Write ONLY there. The repo is read-only for you.
- NO git at all (not even read-only commands). Never write under /home/user/FableDecomp. Never kill a process you did
  not start (kill a hung cl.exe of yours by explicit PID only; never pkill / killall / wineserver -k): one other agent
  shares this Wine lane. Wrap every compile in `timeout 200`.
- Tools: `python3 $S/w17/xdis.py <8hex-va> <len-hex>` (disassemble anything in Fable.exe), `python3 $S/w23/xref.py <hexva>`
  (callers / references), `python3 $S/tools/rstr.py 0x<va>` (string at an address), `python3 $S/tools/iat.py 0x<slot>`
  (import name), `python3 $S/tools/fun.py 0x<va>` (EH handler / unwind states / funclets of an EH-framed body),
  `python3 $S/tools/q.py <addr8> <src.cpp> [leaf] [--flags ...]` (grade), `python3 $S/w18/udiff.py <(bash /opt/fable_lane/w22/agent2050/g.sh <addr>)`
  (only the differing lines). The retail exe: `$FABLE_EXE`. The raw PDBs are NOT on this machine; the PDB-derived name sets
  and layouts are in the repo (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv, pdb_class_keys.tsv,
  struct_layouts_egor.tsv, fable_types.h; rebuild/include/engine/<Class>.h).
- Preflight every new source with `g++ -E -D_MSC_VER=1310 -I /opt/fable_lane/w22/agent2050/hdr -I rebuild/include <src> > /dev/null` before grading:
  cl under Wine spins until the timeout on a missing include or an `#error`.

## Read first, in this order (large files in parts)
1. /home/user/FableDecomp/CLAUDE.md
2. docs/pipeline/ASM_RULES.md (canonical asm / volatile rules)
3. tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md (ALL of it; later sections override earlier ones)
4. docs/pipeline/MATCHING_PLAYBOOK.md (how register / slot / scheduling residues were closed; read before parking anything)
5. docs/pipeline/GOTCHAS.md -- at least the last 300 lines, and grep it for your classes and callee addresses
6. /opt/fable_lane/w22/agent2050/PROMPT.md (tool and file-layout details; its "40 rows", "4 grades", "16 agents" numbers are stale -- this brief wins)
7. Landed relatives of your classes: `grep -rl "<Class>" rebuild/src/compiled | head`, and the newest large-row hand-backs
   for how such rows are closed: rebuild/candidates/manual/pending/2026-11-11_linux_handQ2/ (NOTES.md, LESSONS.md) and
   2026-11-11_linux_handH40i/.

## Rules (the coordinator re-verifies every one and rejects the row otherwise)
- A row counts only if it grades MATCH or RELOCATION_MATCH under VC7.1 RTM for the exact file in src/, AND has a behaviour
  test in test/<addr>.cpp that prints exactly `<addr>_TEST PASS` under both g++ (tall.sh) and VC7.1 (tall.py).
- Grade budget (owner, this session: "give them large budgets to ensure we match as many rows as possible"): up to THREE
  passes of 10 grades per row (30 in all). A grade = a source change + a q.py / g.sh run on the row's real source. Between
  passes, stop grading and re-analyse: measure the first wrong compiler decision (MATCHING_PLAYBOOK), write down the lever,
  then start the next pass. Probe compiles of fragments to learn codegen do not count and are not capped; use as many as the
  row needs, and take the time and reading each row needs -- matching the row matters more than speed. Keep the per-row,
  per-pass count in STATUS.tsv. Only after the third pass park the row: best source in differ/<addr>.cpp (+
  differ/<addr>.test.cpp if you have a test) with line 2 `// Grade: DIFFER(<ours>v<retail>) -- <residue>`, and write the EXACT
  remaining diff (the differing instructions with retail addresses) plus a concrete next idea in STATUS.tsv and NOTES.md.
- No `__asm`, `naked`, `_emit` or the `register` keyword in any source or test. `volatile` only where retail used it
  (ASM_RULES `volatile` section, with the `// Retail-volatile:` evidence line, declared in the shared header). Go past plain
  C++ only as ASM_RULES.md allows (intrinsic, then a shared-header helper with a FABLE_PORTABLE twin) and only with byte
  evidence that the original used asm; never for a compiler residue.
- Model `this` and every other class with the shared `rebuild/include/engine/<Class>.h` headers, never a local padded
  struct. When a header is missing or wrong for retail, write / correct it under `/opt/fable_lane/w22/agent2050/hdr/engine/<Class>.h` (copy the repo
  file first; prefer an opt-in `#ifdef FABLE_RETAIL_<CLASS>` branch so landed includers keep their bytes) and grade with
  `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2050/hdr`. Document each in hdr/README.md (what changed, evidence from two sources, opt-in or
  default path). Never edit a landed row or an existing header in the repo.
- Names right the first time: the row's own function, every callee, global, class, member -- exact PDB spelling (class key,
  namespace, const, access, calling convention), confirmed by two sources. One retail address, one name: grep
  rebuild/oracles/reloc_bindings.tsv for each callee / global address before declaring it, and use a correct bound spelling
  exactly. Made-up (descriptive) names only when the original is found nowhere; list them as descriptive in NOTES.md. Wide
  text is `unsigned short` (never wchar_t / `/Zc:wchar_t`). FABLE_FORCEINLINE, never a bare `__forceinline`.
- Line 1 of every source: `// <Module>::<Name>(<params>) -- <what it does>`; a `// Leaf:` line naming the exact object symbol
  (check with `objdump -t $S/wine/q/<addr>/<addr>.obj`); exactly one `// Flags:` line when flags are needed.
- Check every call target and global against retail before finishing (q.py masks relocations, the lander does not): run the
  read-only verifier
  `LANDVERIFY_WORK=/opt/fable_lane/w22/agent2050/lv FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2050/hdr timeout 3000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py /opt/fable_lane/w22/agent2050/payload.json /opt/fable_lane/w22/agent2050/oracle.tsv > /opt/fable_lane/w22/agent2050/tmp/vland.log 2>&1`
  (NEVER with --land), fix every RELOC_IDENTITY / real DIFFER, then delete /opt/fable_lane/w22/agent2050/lv. Leave no background job running.

## Lessons from the acctB chunks already handed back (2001-2012)+2015
- g.sh / gall.sh / tall.py do not set the header overlay: `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2050/hdr` yourself before every grade.
- Check first that each row starts at a real function start and where it really ends (one reserved row began 16 bytes
  inside its function; several rows have cold code, funclets or a second function between the scanned extent and the next
  catalog start). Record a wrong start / length in SPLITS.tsv; a fragment is SKIP.
- An inline PDB accessor used as a call argument (`GetParentThing()`, `GetGameState()`) changes evaluation order and register
  colouring versus reading the member directly. A named local for an earlier argument restores retail's order ahead of an
  implicit `""` CCharString temporary. An implicit `const char*` -> CCharString conversion re-reads the temporary's frame slot;
  `CCharString("X")` uses the constructor's eax.
- A local whose address goes to a same-TU callee that retail compiled earlier stays enregistered only when that callee's body
  is visible in the TU. A same-TU noinline body of a LANDED callee is fine. If the needed callee is itself unlanded, prove it
  with a diagnostic probe and report a pair-landing case instead of spending grades.
- `C3DVector x; x = a - b;` keeps retail's temporary-then-copy; copy-initialisation builds in place. A 3-argument constructor
  evaluates Y first; member-by-member stores keep X first.
- With FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND VC7.1 reproduces retail's expand-in-loop / call-at-entry LowerBound split.
- Bare `fistp dword` sites are the shared `engine/GFFloatToLongNear.h` helper (ASM_RULES; cite the retail address).
- When operand orders shift with unrelated edits, a small generator search over neutral spellings (scored by the
  relocation-masked instruction diff) is the practical tool. c2 measuring tools (c2prio etc.): read-only references in
  rebuild/candidates/manual/pending/2026-11-09_linux_hand892/tools/ and /opt/fable_lane/w22/agent2002/c2p (copy what you
  need into your own directory).
- (2003 / 2004) A same-TU callee body that is visible but NOT inlined decides whether a local whose address it receives stays
  in a register and whether a struct copy is scheduled load-ahead (e.g. STLport `_M_push_back_aux_v`, a search-tools body).
  A named local for a call made on a temporary destroys the temporary before the test. A named `n = count - 1` local stops
  VC7.1 reusing count's register. A loop with no aligned loop head is a down-counting `for`. Assigning a flag in both branches
  raises its c2 priority. `if (!a && b)` after an if/else moves the else arm out of line. Arm-block locals share frame words
  with earlier temporaries; at function scope they get their own slot.
- Quest scripts: a `const CScriptThing&` keeps retail's virtual IsAlive; a same-TU noinline CTimer constructor;
  `for (;; NewScriptFrame()) { if (T) return; ... }`; inline-split control measured with the c2 inline trace
  (/opt/fable_lane/w22/agent2003/tools/inl.sh, pk.sh -- read-only reference). Size TUs use `#pragma optimize("s", on)`.
- A row that reaches an existing shared asm helper (GFFloatToLongCeil / Near / Floor) needs a `// Retail-asm:` evidence line
  citing the retail address and instruction, or purity.py misgrades it; include the per-site attempts note ASM_RULES asks for.
- Handy read-only references: /opt/fable_lane/w22/agent2004/md.py (relocation-masked instruction diff) and rel.py
  (call-target identity check). Hand-backs _c2003/ and _c2004/ also carry hdr/ opt-ins not yet in rebuild/include.
- Write `const unsigned short*` in comments too, never wchar_t.
- (2005 / 2006) CMatrix3x4 products: VC7.1 orders the terms of each matrix sum itself; what decides it is how the helpers
  are NESTED (e.g. `PreMultiply(m) { CMatrix3x4 copy(*this); SetMultiply(m, copy); }`), not the written term order.
  x87 commutative operand order follows c2 candidate ids: trace the ids (c2prio) rather than enumerating spellings.
  A visible inline callee (ConvertTimeToFrames) fixes how c2 reuses dead parameter slots. Loop bodies weigh ~5x for
  inlining. A tiny inline store helper splits a long scheduling region and keeps stores in source order. Generated script
  returns are `TRetVoid _Fail_Val; return _Fail_Val.Return();`. Jump-table switch cases: write them in retail's layout order.
- (2007) `-(a*b)+c` needs a named float for retail's fchs/fadd (else fsubr). List loops with `next` in memory and `end` in a
  register: `for (it = begin(), end = end(), next = it; it != end; it = next) { ++next; ... }`. A later branch's locals at
  the enclosing scope cost frame words; move them into the branch. `!(a > b)` compiles to `je`, `a <= b` to `jp`. A const vs
  non-const component pointer changes which LowerBound overload is called: q.py cannot see it, so check every call target of
  a matched row with /opt/fable_lane/w22/agent2004/rel.py (copy it) or the read-only vland. An inline site that retail expands
  but whose trace score is just over the threshold can take a per-row `// Flags: /d2inlT<N>` (smallest N, `// Inline-budget:`
  line with the trace numbers) only after control-flow spellings fail.
- (2008 / 2009) Out-of-line same-TU definitions of small members (getters / setters) reproduce retail's split between
  expanded constant-argument sites and called variable-argument sites by themselves. Visible STLport reserve / search-tools
  bodies keep a local vector's end() in a register. The frame packer gives a named 1-byte local a 4-byte node while a 1-byte
  temporary keeps size 1 and sorts first; an address-taken record at function scope conflicts with every slot. STLport
  std::min / max select through a reference; retail's branch arms are value ternaries. `if (!Pred()) { Terminate(); return; }`
  with a visible predicate decides which exit copy survives cross-jumping. A named `float` local declared before an `if`
  reproduces a seemingly dead frame store. The c2 frame-packer hook: /opt/fable_lane/w22/agent2009/tmp/c2/pk.py (read-only
  reference; copy it).
- If you are running out of working budget, stop cleanly: finish and verify the rows you have, mark the rest NOT_ATTEMPTED in
  STATUS.tsv with a decoded outline in NOTES.md, and run finish.py. Never hand back an unverified row as matched.
- (2010 / 2015) A visible but out-of-line template body (CPersistContext::Transfer<T>; opt-in in
  /opt/fable_lane/w22/agent2010/hdr) lets VC7.1 hoist loop-invariant default stores out of a loop. Locals in scope at an
  inline-asm helper (GFFloatToLong*) get +2 frame-packer weight, so block scoping around the helper call changes the frame.
  A `bool done` loop avoids loop rotation and the peeled first compare. A retry loop nested in another loop is a do-while
  with a post-check (`for(;;)` gets rotated); do-while keeps an aligned loop head. An explicit `(float)(...)` cast shifts
  store scheduling where the implicit conversion matches. `X += expr` spills the address before a call; a named float first
  matches. A plain `GetTC` statement with its result unused gives a materialised HasTC byte. Last-resort inline split when
  neither control flow nor one `/d2inlT<N>` fits (prove with the c2 inline trace): a scoped `#pragma inline_depth(0)` ...
  `#pragma inline_depth()` with an `// Inline-budget:` line.
- Always `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2050/hdr` before grading: an overlay-only include without it hangs cl until the timeout.
- (2011 / 2012) A top-tested short loop means the bound test sits inside an inline predicate (`for (i = 0; HasLOD(i); ++i)`).
  Two consecutive `for` loops give the second loop's body and the code after it the same static frequency: an inline split
  between them needs the second loop behind `if (it != end)`. FableWin COMDAT order after a function names its inline
  helpers. `s.find(x) == s.end()` evaluates end() first; a named iterator gives the other order. `(x & 0x80) ? 7 : 6`
  compiles to and / or / shr; an if / else-if chain does not. `and [m],7FFFFFFFh` is the inline GFFloatAbs, not fabs.
  GETDEF_GLOBAL_BODY / GETSUBDEF_BODY opt-ins are needed for `dec [reg+4]` releases.
- (2013 / 2020) A `lea` reading a value reloaded after a call means that value had two references there: write the sum at
  every use, not into one named local. In a size TU the CIVCountedPointer destructor is called out of line unless forced
  inline (`_DTOR_DIRECT_CLEAR` opt-in, c2013 hdr). Test a FindChild result as `CComponent*` and cast only at the call. A large
  same-TU callee with NULL default arguments can be expanded at a constant-argument call site and called at another (check
  whether half of your row is a neighbouring function expanded). A GetTC out pointer left uninitialised gives retail's
  zero-at-entry / reload-on-miss pattern (`= 0` folds to xor and shifts registers). A 4-way result picked from a loop index is
  an if / else-if chain, not a switch. A c2 priority tie can be broken by one extra named local in a block.
- (2016 / 2018) When a landed callee's address-taking parameter makes a local escape, make the callee's real body visible in
  the TU without inlining (prove it first with a trivial noinline body). `colours[i].Initialise(...)` stores straight into the
  buffer; assigning a temporary does not. A temporary built before a call's argument pushes is a named local; a flag stored
  before a call was assigned before it in the source. A walk that keeps the next element in a second register has a separate
  `next` iterator. Force helper cascades inline from the bottom up (forcing only the outer helper leaves the inner one
  called), with the c2 trace scores in the `// Inline-budget:` line. Table indices as `GFGetTopBit(x) - GFGetTopBit(32)`.
  A repo header that includes another header relative to rebuild/include beats the -I overlay: include your overlay copy
  first in the row. After ANY overlay header change regrade every row you already matched.
- (2017) A same-TU callee that only READS a local's address must still be visible (the call stays out of line) or the local
  is treated as escaping. `+ C3DVector(0, 0, 8)` folds the zero terms and stores the intermediate Z to the frame.
  `if (GetDef(...)) { body }` instead of an early return lets the failure path share the normal exit block. The visible GetDef
  body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY) changes the release form and the register choices. A
  one-expression 2D distance `GFFastSqrt(v.X*v.X + v.Y*v.Y)` gives a different x87 order from a two-statement GetMagnitude2D.
- (2014 / 2022 / 2026) A TU's run of unreferenced inline COMDATs lists the accessors its rows really call. c2prio's ref
  events give x87 sum order for free. A member string set to one of two literals is an if / else of two assignments, not a
  ternary. A local `bool` cleared at some exits of a wait loop is threaded so each exit jumps to its own path. Chained member
  stores (`a.X = b.X = 0.0f;`) give interleaved store order. `(long)((a / b) * fps)` gives fimul; without the parentheses it is
  reassociated. A named difference vector plus a copied direction gives the X, Z, Y squaring order with a stored reciprocal.
  A visible same-TU body of a callee that takes a local's address can turn on reuse of dead parameter homes for the whole
  frame; the frame packer places each value in the most recently created slot that fits (trace it with the pk tools before
  grading a frame-only residue). Compile-only layout-marker probes test block order without spending grades.
- (c2023) An inline by-value accessor (`p->GetRadius()`) is evaluated ahead of out-of-line calls in the same expression; retail's
  "pointer early, float late" (`mov ebx,[eax+60h]` ... calls ... `fadd dword ptr [ebx+34h]`) is a direct member read. `a >= F()` gives
  `cmp [mem],eax ; jl`, `F() <= a` gives `cmp eax,[mem] ; jg`. Declaring a list iterator before the `for` (`const_iterator it; for (it =
  l.begin(); ...)`) versus inside it changes register colouring of the whole function start, frame slot order and loop-head alignment.
- (c2023) Static frequency after a loop = entry x (normal exits / all exit edges); each `||` operand of an in-loop `if (...) return` is
  its own exit edge. A `bool waiting` flag loop plus a trailing copy of the refusal block made every identical in-loop
  `{ ...; return; }` cross-jump into the function's last block. A by-value CIntelligentPointer argument copied from a member needs the
  user copy constructor opt-in (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY). Hand-back: rebuild/candidates/manual/pending/2026-11-11_acctB_c2023/.
- (c2021) Inline header bodies: reading a global directly in the body versus copying it into a local changes how the first expanded
  copy is allocated (reload versus spill) -- c2prio shows it without a grade. Nested ifs with one `return x` give a shared exit, flat
  early returns a second epilogue. One named local per repeated step gives each its own frame slot. Grouping parentheses
  `(a*b)*k` can fix a one-store scheduling swap. When another TU calls the out-of-line copy of an inline in a loop, the inline is
  plain (smallest per-row `/d2inlT<N>` + `// Inline-budget:` line), not forced. Hand-back: pending/2026-11-11_acctB_c2021/.
- (r3002, frame layout) c2's frame packer is deterministic and replayable offline: /opt/fable_lane/w22/agent3002/tools/my/ (sim.py replays
  the packer from one trace; fmap.py maps the frame words of two instruction-identical builds; copy, never modify in place) and
  pending/2026-11-11_acctB_r3002/LESSONS.md + NOTES.md. Facts measured: dead parameter homes are reused only when the packed frame is
  under 0x70 bytes; a temporary whose address escapes dies at its call, the same value as a named block local lives to the block end
  (opens extra slots); `v /= d; C3DVector w = v;` is one forwarded node, copy-then-divide is two conflicting ones; a callee body
  visible in the TU changes escape analysis; equal-weight nodes keep the order in which they reached that weight (scheduled code
  order). When a row is "all instructions equal, frame differs", derive the slot membership retail implies first, then the spelling.
- (c2024) A constant colour on a CRGBColour member is `member.Initialise(255,255,255,255)`, not an assigned temporary. A member
  store that sits between two calls in retail is written between them in the source. Retail's unreferenced out-of-line copies of
  inline functions show how helpers nest (the copy returning `this` in eax is the constructor, the same body without it a void
  member); compiling a candidate inline body out of line against its retail copy is a free check. The word `register` must not
  appear in a source or test even inside a comment (the purity grep is textual). Hand-back: pending/2026-11-11_acctB_c2024/.
- (c2025) Look for a LANDED TWIN first (a sibling method of the same class with the same shape): 0x00600B60 matched at its first
  grade from the landed ConstructRegionDescriptionShops. Two retail copies of one search loop = two source loops in if/else arms. A
  `while` search loop drops the static frequency after it back to the entry's; a `for` with `break` keeps the body's. A visible
  same-TU body of an STLport helper (`__find`) under `#pragma auto_inline(off)` changes where its tag temporary lives. Measured
  inline score: c1*(1/f+2)/3 for f >= 1 (sqrt(f) in place of f below 1), expanded iff below the threshold (default 74).
- (c2027) Generated quest scripts: if/else arms holding only the terminate check are merged and leave retail's dead loads / dead `cmp`
  behind; do not copy the real statements into both arms. `bool in_range = d2 <= r*r;` gives `test ah,1 ; je` with a late flag
  store. An implicit `T*` -> CIntelligentPointer conversion re-reads the temporary's slot. A patched c2prio that prints spill / refs /
  key at every choose and skip is at /opt/fable_lane/w22/agent2027/tools/c2p/ (copy, never modify in place).
- (r3004, per-function key) The operand / term order c2 picks for commutative x87 sums over memory operands in an expanded inline
  helper (and possibly other "canonical order" ties such as SIB base/index order) can be keyed on the OUTER function's own
  named-symbol count, repeating every 8: 0x00B4FC40 matched by putting five accessor results into named, forwarded locals
  (references / enums: no instructions, no frame slots). Diagnose with a throwaway sweep of k = 1..7 dead locals in the outer body
  (free: read the order from the trace object, never deliver dead locals), then add that many natural forwarded names. Named class
  locals are not neutral (frame slots). Locals of inline bodies, argument temporaries and other functions in the TU do not count.
  The lever does nothing for sums over a local matrix's own fields. Tools: /opt/fable_lane/w22/agent3004/tools/ (c2h/, tord.sh,
  ordobj.py, oolcmp.py, fr.sh, frag.py; copy, never modify in place); LESSONS.md + NOTES.md there.
- (c2029) FABLE_RETAIL_CCOUNTEDPOINTER (the constructor tests the stored member) lets c2 thread a counted pointer's null path into
  the else arm of the following test. The construction order of locals can be read off the destructor run at the end of retail. c2
  hoists a by-value vector's `_M_start` load into a loop's test block when every path after the loop reads it; an else arm before
  the loops that falls into the shared final return stops this. Identical `{ free; return false; }` copies are cross-jumped whole
  only when their rotation register equals the survivor's; each explicit copy consumes one rotation step where it is laid out.
- (c2028) A copied vector's dead Z store is dropped only when Z is read first (a `const C3DVector&` overload forwarding to the
  float overload evaluates right to left). An inlined `a || b` keeps `cmp ; jbe`, an if-return chain materialises `seta al`.
  `bool alpha = x; if (c) alpha = true;` gives an in-register select. A `while` search keeps block frequencies, for+break changes
  them. Declaring the copy constructor keeps a by-value copy out of line. When c2 inline scores split (80..534 expanded v 76 / 91
  called) no single `/d2inlT` fits: per-helper FABLE_FORCEINLINE bottom-up with an `// Inline-budget:` line. Always write the
  behaviour test before running out of context, also for a parked row. Hand-back: pending/2026-11-12_acctB_c2028/.
- (r3005, c2 record ids) Some per-function tie-breaks (SIB base/index choice, x87 operand order) are decided by c2 symbol-record
  ids read mod 1024 / mod 256; records come from 32-record chunks numbered at creation and reused through a free list, so what
  counts is how many records are allocated and WHEN (before inlining, during an inline expansion, before loop strength reduction).
  The outer-name sweep (r3004) is only one way to move them; temporaries inside an expanded inline helper are another. Frame-packer
  ties do NOT move with outer names. Free tools: /opt/fable_lane/w22/agent3005/tools/c2h/ (cands.py, sib.py, recs.py, watch.py) and
  tools/my/ (k.py, sweep scripts); NOTES.md there has the full sweep tables for 0x00A9EB90 and 0x00550DC0.
- (c2030) A member CMatrix3x4 filled from 12 values is `M.Set(...)`; assigning a `CMatrix3x4(...)` temporary copies with
  `rep movsd`, and twelve plain assignments from an address-taken local chain the stores. Reading through an inline constructor's
  `const T&` parameter turns earlier member stores into barriers; reading the member directly does not. No-code grouping
  (`((new - old) * t) + old`, a `(float)(...)` return cast) moves c2's 81-tuple scheduling cut. Constant-register colouring follows
  the order the defaults appear in the source. A retail_asm row needs the per-site attempts table ASM_RULES asks for (30+10), not
  only the helper header's proof. Hand-back: pending/2026-11-12_acctB_c2030/.
- (r3013) A 1-byte frame-node residue can be pure ALIGNMENT: a byte parameter of an inlined helper fed straight from a raw member
  load gets alignment 4 (low byte of the word), fed through an inline accessor or a computed bool it gets alignment 1 (retail's
  [esp+1Bh]); read it from node byte +0x2C in the frame-packer trace (/opt/fable_lane/w22/agent3013/tools/al.py). Prefer the PDB's
  inline accessors (IsX() / GetX(), names in FableWin.pdb) over raw member reads in general. In the allocator, a forced spill of 0 on
  a split piece is recomputed: the live-range split pass decides (c2prio_b.py there prints each piece's first / last block).
- (c2031) Pair-landing check: when the only residue is how inline copies next to out-of-line calls are scheduled (load-ahead versus
  interleaved), test with a diagnostic TU that makes the callees' bodies visible (non-capturing stubs that keep the callee's memory
  side effects); if that grades RELOCATION_MATCH the row needs the real callee bodies in its TU. Taking a member function's address at
  file scope emits its out-of-line copy, so an inline header body can be graded free against an uncalled retail COMDAT. Two inline
  decisions at one cost in the same function prove a forced inline. Copying through a named const reference makes the source address
  an allocator candidate. Scheduler replay: /opt/fable_lane/w22/agent2031/tools/sched.sh + skyfit.py.
- (c2032) An `a != NULL && ...` guard puts the guarded loop at static frequency 0.25, which decides which callees inside it are
  expanded; take the iterator inside the guard. When two layers of an inline must expand at one site while cheaper callees (scores
  76 / 77) stay calls, no `/d2inlT` fits: header opt-ins that force the two layers, with an `// Inline-budget:` line. A visible
  same-TU STLport body (`insert_unique`) changes the frame. A squared-distance written as a running sum (`d = x*x; d += z*z; ...`)
  gives a non-source term order. VC7.1 moves a single-use named float local late (past the next statement).
- (c2033) A temporary passed by reference to an STLport map insert stays non-escaping only when the WHOLE insert chain is visible
  (FABLE_RETAIL_STLP_MAP + _MAP_ORIGINAL_INSERT_BODY; insert_unique alone does nothing). A local whose address goes to two callees
  shares the dead parameter word only when both callee bodies are visible (same-TU body of the preceding out-of-line member under
  `#pragma auto_inline(off)`). `1.0f / 3.0f` gives 0x3EAAAAAB, the literal `0.333333f` 0x3EAAAA9F. A chained `A = B = x;` stores B first.
- (r3011, exits / frame ties) Which failure paths share the function's final `return false` block decides where c2 hoists a
  by-value vector's `_M_start` load (to the immediate dominator of the shared block), hence how that live range blocks other values
  and how it splits: vary which failure frees fall into the final return, which sit inside an enclosing `if`, and `else if (x)
  free;` forms. c2's frame-slot sort (c2.dll 0x107AE0C4) is a K&R quicksort by density (weight*1000/size) where equal densities
  compare 0: ONE extra IL reference breaks a tie, and an explicit `return` copy that later cross-jumps away still counts (it adds a
  destructor reference). Replay: /opt/fable_lane/w22/agent3011/tools/fs/qs.py (`--w` weight what-ifs); also vt.sh, ch.py, blk.py,
  nb.sh (neighbour-penalty hook) there. Matched 0x00915B60 this way after two agent sessions.
- (r3016) "Every instruction right, frame slots swapped" was fixed for 0x00550DC0 (after four failed attempts at adding references)
  by REMOVING a frame-packer node: write the block the way the landed sibling does (CComponent::UpdateColour 0x0052F900) -- named
  locals constructed in the sibling's order, and the loop REUSING an earlier named variable (`colour = ... * render_colour;`) instead
  of declaring a new one, so a temporary and the loop variable become one node. Always read the closest landed sibling's statement
  shape for the same computation before inventing spellings. Packer replay with slot sort: /opt/fable_lane/w22/agent3016/tools/sim2.py
  (reproduces a baseline exactly; what-ifs), fixed qs.py in tools/fs/ (parameter homes excluded). c2 normalises operand order inside
  an inlined `operator*`: swapping operands changes nothing.
- (c2037) c2 has a hard plain-inline cost cap of 450 (c2.dll 0x1078D248 `cmp esi,1C2h`) that no `/d2inlT` lifts: a costlier
  helper retail expands is FABLE_FORCEINLINE. Retail's UNCALLED COMDAT copies of inline helpers grade an inline header body for
  free (/opt/fable_lane/w22/agent2037/tools/qx.py). An inlined `return a && b...` materialises `mov eax,1` / `xor eax,eax`; a
  `setne` pattern comes from an if-return body. The last of N identical expanded blocks can be allocated differently from the
  others (what is live AFTER it decides). A shared inline body must be ONE text for all rows: never a row-specific opt-in that
  changes a shared inline's body. A row whose TU carries a helper-calling callee body needs a tagged `// Retail-asm:` line.
- (r3015) PROVE INLINE BODIES AGAINST RETAIL'S UNREFERENCED OUT-OF-LINE COPIES before using them: retail keeps uncalled COMDAT
  copies of inline helpers in some TU (often uncatalogued; e.g. 0x00B9C210 GetAppropriateLOD, 0x00B9C280 CalcDistanceFromCamera);
  compile the candidate body out of line (take its address at file scope) and compare -- free. Two bodies with identical
  out-of-line bytes can still EXPAND differently (ternary clamp versus if-clamp moved an x87 chain). A reload of a member pointer in
  retail (where ours reuses a cached load) can come from a named pointer local (`T* info = p.GetRenderInfo();`) used for later
  stores: a store through another base pointer kills c2's load reuse. Judge x87 term order only on full-file compiles.
- (script oracles) Aeon's Lua ports of 20 retail quest scripts ARE in the repo: refs/script_recovery/seed_corpus/sources/<Package>/
  (Gameflow, WaspBoss, MeetSister, TrophyDealer, DragonBossFight, Fisherman, ...), with per-script native call lists in
  refs/script_recovery/native_operation_ir/ and native_clusters/ (large JSON: query with python, never print). For an NScript row,
  check for a port first: for Gameflow 308 of 309 unique retail string literals occur in the Lua and each Lua stage function maps
  to one contiguous retail range. The Lua gives operations and order, not the C++ statement shape; retail's bytes decide.
- (c2037 continuation) When only the LAST of N identical inlined copies differs, read that copy's c2prio priorities first
  (equal-priority copies are coloured last copy first) and look in the HEADER inline, not the row: passing named locals of an
  inline body on to a nested call doubles their references in the expansion; passing the argument expressions again lets c2 merge
  them. A residue in a row that expands header inlines can be the header's fault. Always pass the row's `// Flags:` to g.sh.
- (c2034) A forced-priority what-if (hook tools: /opt/fable_lane/w22/agent2034/tools/fw.sh, pf.sh, c2pf/) proves whether ONE
  allocator decision is the whole residue: do it early. Frame-packer weights are counted AFTER register allocation, so a
  frame-only residue can follow from one register decision. The register chooser never refuses a register on cost: colouring
  ORDER (priority, then key) decides. Binding a call result to a `const T&` changes the x87 term order of a following dot
  product. Frame-masked instruction counts hide x87 operand-order residues: check the unmasked diff too.
- (c2035) x87 operand orders inside an inlined Normalise depend on the order in which c2 creates the vector fields' records
  relative to the helper's locals: referring to a field directly (or reading the element through a named `const C3DVector& pos =
  *it;`) changes it. One shared call after an `if` (`if (blocked) new_pos = ...; MoveTo(new_pos);`) gives the two duplicated exits
  retail has. Member `+=` easing versus an interpolation helper decides whether the member is re-read. A same-file non-inlined body
  of a private helper whose only caller is the row fixes frame sharing. A probe that checks ONE instruction can mislead: check all
  residue sites before grading.
- (r3014) CONTAINER ACCESS FORM: the existing repo opt-in FABLE_RETAIL_CARRAY (CArray::operator[] through begin(), like
  STLport's vector) makes the element address a 3-address IL form instead of a two-address `add`; copying an element straight into
  a named local (`T x = a.b[12];`, no intermediate const reference) plus that opt-in fixed three scheduler / rename residues at
  once (0x00B65A20). Try the access-form opt-ins that already exist in repo headers on the plain direct spelling early. c2's
  scheduler renames RMW temporaries round-robin per region over the cmp / push / add / or / inc / dec tuples it visits
  (free probe: /opt/fable_lane/w22/agent3014/tools/walk2.sh); soft add-to-load edges plus a +32 load bonus reorder `add r,imm`.
- (c2036, 0x005841F9) A CDefPointer / smart pointer filled through a hidden return pointer is address-taken: `p->F(); p->G();`
  reloads it from the frame after every call. Retail's "load once into a callee-saved, test, use three times" is a named
  reference taken after the null test (`else if (!p.IsNull()) { const T& t = *p; ... }`); a pointer local gives the same
  instructions but one more c2 candidate in the block, which can cost another value its register.
- (c2036) An inline accessor returning a reference leaves an inline-return candidate in the caller's block even when the bytes
  equal a direct member read; that one extra candidate lowers every live-through candidate's priority by 1 and can flip a tie.
  Colouring ties go to the LARGER key: a parameter (small key) loses every tie against a block-defined value and needs a
  strictly higher priority. When a row's colouring is one point off, compare accessor v direct member read first (free).
- (c2036) One local re-assigned in N repeated statement blocks is ONE frame-packer node (weight 3N, placed first among the
  4-byte nodes, takes the first free slot such as a dead parameter home); N named locals are N weight-3 nodes placed after the
  blocks' temporaries. Repeated blocks each with their own named result is the Lionhead shape.
- (c2036) Reservation length estimates can include unrelated tiny COMDATs after the function's last `ret`; trust the extent scan.
- (f4001, 0x00CE7670, 32 KB matched) Very long generated functions (script Mains, Register* / Transfer tables): build a
  machine-made draft from the retail call sites (callee from the vtable slot or call target, strings from the string ctor
  slots), generate C++ from it, freeze the draft and fix it with a patch file keyed by draft line. Grade the WHOLE function
  every time (1 s compile); partial sources are in another optimizer regime and tell nothing. Tools to copy:
  /opt/fable_lane/w22/agent4001/tools/ (map.py, gen.py, build.py, pd.py masked diff, rel.py symbol pairing, mktest.py).
- (f4001) In a script Main `TRetVoid _Fail_Val; ... if (IsActiveThreadTerminating()) return _Fail_Val.Return();` is not
  cosmetic: plain `return;` lays the shared local-destroying exit out inline and changes the caller size c2 scores inlines with.
  A jump table that sends a value to a neighbour's label needs that enumerator as an extra `case` on the same label.
  if / else gives `mov ; jne ; mov`, a ternary neg/sbb/and; if / else-if gives `cmp 1 .. cmp 2`, a switch dec/je.
  `while (f() != false)` gives `setne`, `while (f())` the neg/sbb/inc form. A `while (true)` wait with the terminate check at
  the top and none after the bottom frame gives retail's rotated loop.
- (f4001) Inline split in a cold block: read the c2 trace scores first. If retail's expanded callees all score below its called
  ones, ONE `/d2inlT<N>` with the smallest N (prove N-1 fails) plus out-of-line header opt-ins for the low-score members retail
  calls anyway reproduces it; forced header opt-ins cannot expand an implicit destructor. An implicit base ctor nested in an
  expanded ctor is always expanded; when retail calls its COMDAT there, only an out-of-line declaration reproduces it.
- (f4001) A relocation-masked diff hides wrong call targets: after the instruction diff is empty, pair every relocation symbol
  with the retail target (rel.py or the read-only vland). A behaviour test for a long script can be generated from the retail
  disassembly's call order in address order (mktest.py), not from the source.
- (f4001) Lane: grading without FABLE_HDR_OVERLAY when the row needs overlay headers can leave cl.exe spinning under Wine;
  `timeout` kills only the Python parent. Always export the overlay; stop only your OWN cl.exe by PID if it happens.
- (c2038, 0x00BB8750) All instructions equal but frame words swapped is not always a sort tie: one live-range conflict can
  do it. The frame-packer trace showed two locals that share a word in retail overlapping in the source; moving one statement
  (computing `num_verts` before `v_scale`) past the other variable's last use removed the conflict and the packer produced
  retail's whole frame. Grid index code: name `row = width + 1` and write indices with `row` and `width` the way retail's
  `lea`s do; `vi += 1 + (expr % row == 0)` gives `neg ; sbb ; inc`. Identity + translation is `SetIdentity()` then
  `SetTranslateOnly()`. `CArray(n)` is called out of line for a non-POD element and expanded for `unsigned short`.
- (f4003, 0x00CD52D0, 24.7 KB matched at the FIRST whole-function compile) For a long run of near-identical statements: hash
  each statement's instruction shape first (map.py -> units.txt); only OPERAND differences are source, the shape deviations
  (temporary rotation eax/ecx/edx, stores ahead of pushes, constants kept in callee-saved) are the scheduler's and come out by
  themselves. FableWin.pdb COMDATs listed right after a function give its inline ctor signatures and helper names (the ctor
  parameter order equalled retail's store order: `: Name(name) { ID = id; ... }`). One-to-one relocation symbol pairing does
  not prove a symbol is the right NAME for its address: back names with the PDB class plus landed Leafs. Tools:
  /opt/fable_lane/w22/agent4003/tools/ (map.py with shape hashing, gen.py, rel.py that also pairs relocated `push 0`, mktest.py).
- (c2039, 0x006ACA30) Each early `return` past a local with a destructor adds a destructor reference to that local's
  frame-packer weight (five returns: `sound` 12 v the container pointer 7, wrong frame order); switch `break`s plus a result
  flag that c2 removes tie them and give retail's order. Swapping then / else arms moves the cold block out of a loop; a
  result flag gives ONE shared release where a `break` gives two; reading a getter at the call (`(float)stats->GetRentMoney()`)
  instead of through a named local lets c2 forward the just-stored value and fixed a callee-saved swap. Def lookups: make the
  shared GetDef / GetSubDef bodies visible (existing GETDEF_GLOBAL_BODY / GETSUBDEF_BODY opt-ins) when retail releases a def
  handle with a single `dec [reg+4]` and no reload. In a HasTC-style helper a nested `if` avoids the conditional-destruction
  flags an `&&` adds. An unreferenced retail COMDAT copy proves an inline BODY, not whether it was plain inline.
- (c2040, 0x005F6E40) In an `optimize("s")` TU c2 expands plain inlines only up to callee size 15 and refuses bigger ones
  WITHOUT a score (so no /d2inlT reaches them): where retail expands CCharString::operator==(const char*) or CVectorMap::Find
  there, use the existing shared opt-ins (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ, FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND)
  with an `// Inline-budget:` line quoting the trace. A by-value CCharString call result pushed by reference with retail's `lea`
  re-address is a named local in its own block (`{ CCharString name = def->GetInstantiationName().GetString(); f(name); }`).
  A same-TU body of a hidden-result callee (the landed row's body, proven to grade against its own address) lets c2 see the
  result is not captured: the caller's handle releases in place with no null test. if / else assignments, not a ternary, give
  two stores and the cold-block parking spot; reading a count into a named local before `resize` flips a scheduler tie.
- (c2019, 0x006B42F0) SIB base / index order follows c2 record ids: the record with the SMALLER id becomes the index. A
  loop induction variable that gets a fresh record above the per-store reload temporaries gives the wrong order; a folded
  conversion temporary before it frees a record the variable reuses. Natural levers are PDB-listed template instances with a
  different argument type (`GFLimit(x, 0.0f, 1)`, FableWin ??$GFLimit@MMH@), never dead code. A function-local static's
  mangled scope number (`?4?` v `?8?`) is set by the braces and the clamp form before it -- check it when the static's symbol
  differs. `FABLE_RETAIL_STLP_VECTOR_ASSIGN` is needed when a struct holding vectors is copied with its implicit `operator=`
  (otherwise the vectors are copied bitwise and the size is far off). `_$E<n>` RELOC_CONFLICT warnings are TU-local atexit
  stub names: expected, say so in the report.
- (f4004, 0x00F2D6E0, 23.7 KB matched) c2 has TWO inline rules: a callee already compiled in the TU is decided by the cost
  rule, a callee not yet compiled goes through a per-caller BUDGET rule. A long list of small inline calls that expands for
  the first K statements and is called from then on is that budget running out, not a source change at statement K. An inline
  virtual needed only by the vtable is emitted BEFORE the templates it first uses (retail's COMDAT order after the function
  tells which kind a row was). A template body that is accepted but never expands still costs budget, so it must be visible
  with the right c1 size; one unit of c1 size can decide a nested inline (48 v 49). Debug-info flags, the `inline` keyword alone
  and definition order do not move the split. Tool: /opt/fable_lane/w22/agent4004/tools/sim.py replays the budget arithmetic.
- (c2041, 0x006AB820, parked 652/656) An outer parenthesis around a float expression in an inline body
  (`(float)(cos(a)*(b*c))` v `(float)cos(a)*(b*c)`) leaves a no-code PAREN tuple between the `fmul` and its `fstp`; the `fstp`
  issues one cycle early and flips a scheduler choice in the CALLER. A one-swap scheduling residue around an expanded shared
  inline can be the header body's parenthesisation: test the alternative body as a free probe and, if it fixes the row, regrade
  every landed user of that inline before proposing it (one body for all rows). Also: `GetPhysicsTC()` at every physics call
  loads the component once; early arms ending `Member = 0; return;` give the constant 0 its own callee-saved; a found / null
  if-else sharing ONE tail keeps another arm's scheduling region whole; `direction.Initialise(x, y, 0.0f)` plus
  `(facing ^ direction).Z` fixed four x87 operand-order ties.
- (c2042, 0x007F2DC0, parked 1929v1942) FableWin.pdb's per-TU COMDAT list shows which inline helpers the source used
  (`vector::empty`, `GFDebugPDynamicCast<..>`): read it before writing the statement list. Spelling a test with extra `||`
  terms halves the static frequency of everything after it and flips inline decisions in later loops; an `||` erase test makes
  two identical erase blocks cross-jump into one (retail keeps two); `if (!v.empty())` with the iterator taken before an
  intervening call, then a do-while, gives an aligned loop head with no re-test (a `for` re-tests). A frame-only residue can be
  ONE missing local copy that c2 forwards: a member read that retail keeps in a frame word is probably a by-value argument of
  an expanded inline -- look for the helper in the PDBs before trying spellings of a plain local.
- (c2043, 0x0064ED4F) When retail shows inline-lookup loads (vector `operator[]`, def member reads, ftol) scheduled AFTER a
  call to a same-TU callee that is only DECLARED in your source, give that callee its real body in the TU (out of line; in a
  size TU a callee over size 15 stays a call): with the body visible VC7.1 knows it writes no memory and moves the reads past
  it. Do this before rewriting vector access as raw pointers. `def.V * t` on a def member puts `&def.V` in a register; building
  the vector member by member (`C2DVector(def.V.X * t, def.V.Y * t)`) gives `fmul [eax+off]`. A named `long d` v re-reading
  `it->X` decides reload-compare-multiply patterns and 8-bit arms; a named `float time` keeps a converted value on the x87 stack.
- (f4005, 0x00F2A250, 11.7 KB matched) An implicit operator= is matched with NO statement of it in the source: a header
  opt-in drops the declared operator= and the row file holds a piece of retail's TU that makes the compiler emit it (the
  inline Copy / Alloc virtuals and the ctor, laid out as retail lays them out). c2 has a THIRD inline gate after the cost and
  budget rules: a callee not yet compiled is refused when its statement count exceeds (arguments + 2) * 3 + 3. "Expanded here,
  called there" between sibling classes is their ORDER in the unit: the first user of a not-yet-compiled inline must call it,
  later users get the cost rule. Grade the unit's OTHER functions against retail as a free model check
  (/opt/fable_lane/w22/agent4005/tools/sib.py). Map tool for member-wise copies: tools/map2.py; test generator mktest2.py.
- (r3010, 0x00A4E300) x87 commutative ADD-CHAIN operand order is decided by c2's tree linearizer (0x1075FC1C): the operands of
  each n-ary add are merge-sorted by a key whose low 16 bits hash the operands' c2 record ids (a field-record leaf hashes as
  rotl16(id, 5); a product of two as 32*max + 64*min + 48 mod 65536), so the order depends on HOW MANY records the function
  allocated before the linearizer, i.e. on the amount of IL in the function -- including IL that is dead and removed later.
  Small spellings do not move it. A row whose only residue is such orders is missing (or has extra) IL: look for an inline
  helper whose real body does more work that later folds away (here `SetMultiplyNoTranslation` = `SetMultiply(a, b);` then
  clearing the translation row, equal out of line to the 9-sum body). Tools: /opt/fable_lane/w22/agent3010/tools/c2h7/model.py
  (predicts the orders from record ids), tools/rfi.sh (15 s screen of a candidate). FableWin debug COMDAT sizes compared with
  VC7.1 /Od instruction counts of a candidate body are a cheap second source for inline helper bodies (Transpose = three
  SWAP<float> calls) -- but FableWin is a later revision; say which way that evidence points.
- (r2041, 0x006AB820 matched) The c2041 residue was a FITTED shared body: `C3DVector::InitialiseFromAngles` is
  `float cosYZ=GFCos(angleYZ); X=GFSin(angleXY)*(cosYZ*length); Y=GFCos(angleXY)*(cosYZ*length); Z=GFSin(angleYZ)*length;`
  (implicit double -> float stores), not hand-expanded `(float)(sin(..)*..)` lines. When a residue shows up in only ONE user of
  a shared inline, suspect the shared body: the FableWin COMDAT listed right after the inline's debug copy names the helper
  the original body called (first instantiation). Regrade every user under the new body. Unreferenced retail copies of an
  inline can be found by scanning .text for its constant plus its opcodes and `ret` size (0x0068D4C0 was uncatalogued).
- (c2045, 0x00BB8EF0) Inline-budget by flag: read the c2 inline trace first; when retail expands helpers the default threshold
  (74) refuses and no if / return arrangement can raise the block frequency enough, the minimal `/d2inlT<N>` is the largest
  refused score + 1 (prove N-1 fails and that N flips nothing else). A NON-inline function always gets the threshold rule for
  its inline callees even if a callee is defined later in the file. `SetupAlphaBlendingStatesFromBlendMode` costs 693, over
  c2's hard 450 cap: FABLE_FORCEINLINE in every row that expands it (with `SetupBlendOpState`); the scoped shader helpers stay
  plain inline (retail has uncalled copies 0x009D8AD0 / 0x009D8B40). Where `CGraphicFrame frame;` is declared decides where
  its zero store is scheduled. A flag tested again right before a call with no else call is an else arm compiled out on PC.
- (r2042, 0x007F2DC0 matched) VC7.1 folds a single-use copy of a member straight into its use UNLESS a control-flow join lies
  between the copy and the use: `long timer = Timer; const bool has_life_time = PDef->LifeTime > 0.0f; if ((has_life_time &&
  timer > ...) || ...)` -- the materialised bool (branch + join) keeps `timer` in a frame word (`mov eax,[mem]` /
  `mov [esp+N],eax` ... `fild [esp+N]`), as a clamp's join does in the landed 0x006D6CB0. `const bool` v plain `bool` changed an
  x87 squaring order elsewhere (named-local COUNT was not the lever). A store into an address-taken temporary (one whose address
  goes to an out-of-line ctor) carries a may-alias edge to a later load written as `p->Member`: reading the member into a named
  value first (`float range = PDef->Range;`) removes the edge and the load issues BEFORE the store. So "retail hoists a load
  above a store" = read it through a named value / named pointer before the storing statement. Tool: scheduler hook with the
  `ridx` option in /opt/fable_lane/w22/agent2042/tools/sched/; landed-byte pattern scan tools/scan/fc2.py.
- (c2044, 0x00A9F540 matched / 0x00AA0090 parked) FableWin's COMDAT run after a function also names undecorated FILE-STATIC
  helpers; retail keeps uncalled LTCG copies of them (custom calling conventions, not gradeable) but expands them in the
  callers, which VC7.1 only does when they are FABLE_FORCEINLINE (Inline-budget line per row). A callee whose body holds an
  asm helper must still be VISIBLE in the TU, or its reference outputs escape and the caller reloads them. The 64-bit range
  check is `__int64 n = count * 5; if (n < 0 || n > 0x7fffffff) return;`. `fsub st(1) ; fadd st(1)` interpolation is the
  shared `GFGetInterpolatedValue`. `fabsf` (not `fabs`) in a float test fixed a dot-product term order. When a row reaches a
  bare `fistp dword` through the shared GFFloatToLong* helpers it grades retail_asm and MUST carry ASM_RULES' full per-site
  table (30 plain-C++ + 10 intrinsic spellings, each with what VC7.1 emitted): build it with the fragment harness as part of
  the row, not as an afterthought.
- (c2046, two script rows matched in 2 grades each) Write a script function in the generator's own pattern and the layout
  comes at the first compile: a terminate check after every loop and at the top of every for / if body, a `bool flag` loop for
  "wait until any". Read the c2 inline trace BEFORE the first grade: it shows the one or two call-v-expand decisions left
  (`vector::clear()` whose erase retail calls -> existing FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE; a pause ctor retail
  expands at sites where no single /d2inlT separates it from called ones -> existing ..._FORCEINLINE_CTOR opt-in; a counted
  pointer dtor retail calls -> declared out of line). A row that ends on a jump table can be followed by an uncatalogued
  function. Do not name a helper script dis.py (it hides the standard library module capstone needs).
- (c2047, 0x00A9E1E0) `AlignedSetMultiply`'s x87 fallback arm read through the `CMatrix4x4` element accessors (FableWin emits
  both `operator()` copies right after it: first user) made a transposed multiply loop match; x87 multiply-loop column orders
  also follow the outer function's named-symbol count mod 8 (a function-scope `unsigned long i;` and one named pointer
  `const C3DBone2* bone = mesh.GetBone(i);` supplied it, and the function-scope counter fixed the induction base).
  Forced 4x4 `CQuaternion::ToMatrix` / `ToMatrixTransposed` explain retail's out-of-line C3DKeyframe copies with no flag
  (0x00AA3AF0 is ToMatrixTransposed). FableWin signature instruction counts are plain /Od. An aligned frame (`and esp,-16`)
  comes from a `__declspec(align(16))` LOCAL; the class itself is not aligned. A cheap per-function feature screen (loop
  extraction + term orders) saves grades. For the shared SSE macro `GF_SSE_MATRIX_MULTIPLY` the per-site table harness with
  a positive control is in /opt/fable_lane/w22/agent2047/probe/asm/.
- ONE ROW AT A TIME (project owner, 2026-11-12): work a row until it grades MATCH or RELOCATION_MATCH, then write and run its
  behaviour test (g++ and VC7.1), then start the next row. Never park a row, never switch to another row early, never keep
  several rows open at once. Rows that share helper bodies with the current row may be regraded and adjusted as part of the same
  job. If your context runs low on an unmatched row, hand over in place on THAT row; do not start another one.
- WORKING STYLE (project owner, 2026-11-12, latest wording): RAPID-FIRE. You have as many grades as you need; there is no cap on
  full-row grades, regrades or free compiles, and any "at most N attempts" / "park after" wording elsewhere is void. The loop is
  author -> full-row grade against retail -> read the diff -> next change, at speed: do not ration grades and do not deliberate
  between candidates; write it, grade it, log one line (what changed, result, equal count, first difference), write the next.
  Batch hand-written candidates and grade them back to back. Get a complete first draft graded early. Traces, fwsize runs and
  fragment probes are allowed whenever they are the quickest way to choose the next candidates: one question, read the answer,
  straight back to authoring and grading. Dead-local diagnostics are allowed as a ruler for an "amount of intermediate code"
  residue, never as a hand-in. Do not build new compiler-hook tooling when an existing tool answers the question. Hand-written
  natural source only: no padding in the hand-in, no automated generator search over spellings. Run the behaviour test under
  g++ and VC7.1 as soon as a row matches. If your context runs low, hand over in place with the best source and the exact
  remaining difference.
- (r3009, 0x007C6E50) A register-allocation residue in ONE block that ~25 local spellings (declaration scope, named temps,
  reference/pointer aliases, helper bodies) could not move was decided by WHERE THE BLOCK SITS IN THE CONTROL FLOW. Two cheap
  grades diagnose it: (1) swap the if / else arms, (2) move the block behind a `bool` flag after the whole construct. If either
  reproduces the target block's prefix (even with the wrong layout), bring the block back inside its arm behind the flag:
  `bool give = false; if (..) ..; else if (..) { ..; if (--n == 0) { n = N; give = true; } } if (give) { lookup }`. VC7.1
  threads the flag away (no bytes) but the live-range split of locals used in the block changes. A partial c2 model from two
  agents' worth of tracing did NOT predict this; eleven hand-written graded candidates found it in twelve minutes.
- FABLEWIN DEBUG-SIZE ORACLE (coordinator, 2026-11-12; free, seconds): `python3 $S/tools/fwsize.py <src.cpp> [name-regex]` compiles
  the TU at plain /Od under your overlay and compares the row and EVERY inline helper it instantiates with the FableWin debug build
  (bytes + instruction count per mangled name). Plain VC7.1 /Od reproduces FableWin byte for byte when a body's TEXT is right
  (PrepareForRender 158i/565b, render-state setters 17i/49b, TouchResource 33i/92b, C4DVector(MMMM) 20i/51b ...). A helper a few
  instructions off has a different text in the original: C2DVector operator+ / - / *(float) are 26i/68b, 26i/68b, 24i/63b in
  FableWin = `C2DVector result(...); return result;` (ours `return C2DVector(...)` is 22i/54b). VC7.1 has no named-return
  optimisation, so such bodies add IL that folds away in release: this is where "the function needs more IL" residues (operand
  order of or / x87 chains decided by c2 record ids: r3010, c2044, c2048) come from. When a row has such a residue, run fwsize,
  refit each helper flagged DIFF until SAME (one free compile each), then grade the row. Gaps of +55 instructions and more are
  FableWin assert code (CArray::operator[], operator->, SetRenderState): ignore. FableWin is a later build: its size is a strong
  hint for which text to try, the retail grade is the judge. Changed shared bodies go in opt-in FABLE_RETAIL_* blocks; regrade users.
  FableWin was built with C++ exception handling: compare ROWS (and helpers with destructible locals) with `FW_EXTRA=/EHsc` in the
  environment (GetFrame 147i/464b v FableWin 146i/461b; plain /Od 136i/413b); leaf helpers at plain /Od. fwsize matches by mangled
  name, so a helper FableWin labels differently is missed. On chunk 2048 this route took RenderClouds from 721 to 731 of 734 with
  code-neutral helper texts (named-result C2DVector - and *, ~CGraphicFrame -> ReleaseData, direct-member accessors, Get(),
  GetR/GetG/GetB, C4DVector(C3DVector(..), w)). A newly layered plain-inline helper can be refused in OTHER rows of the TU:
  regrade every row after each helper refit.
- (c2048, 0x00B64550) Two destructible frames share one stack slot only when they sit in sibling scopes (put the tail's frame in its
  own `if (x != 0) { ... }` block); that layout also gives retail's per-exit restore epilogues. Unnamed
  `C2DVector(a, b).GetMagnitude()` temporaries with repeated subexpressions give retail's x87 stack-copy pattern. A `continue`
  ahead of a loop-body frame leaves the destructor's zero store live: use if / else around the frame.
- (f4006, 0x005D1F0C, 17,873 bytes) Four residues of a very long function and what closed each: (1) a loop-entry choice that 40
  loop spellings did not move: define the SAME-TU callee that receives the local's address (its real body, over the shared
  headers) ahead of the row; (2) ~340 frame-word displacements: a statement's escaped temporaries get +2 packer weight per
  inline-asm helper in the statement, so the frame words tell whether the helper call sits inside the statement or in its own
  `float x = Helper(...);` statement before it; (3) a wandering "load late" flake is the scheduler's 81-tuple region cut: a
  hand-expanded copy of an inline helper, or a missing `return (expr);` parenthesis in the helper, shifts the tuple count with no
  byte change; use the real helper at every site; (4) SIB base/index order follows record ids and only one more inline level on
  one operand swaps them (a sum helper or an inner inline getter). Grade q.py with the Leaf as the third argument when the
  catalog name is a vfunc placeholder.
- (r2044, 0x00AA0090 + twin 0x00A9F540) An x87 product-order residue "about 4 dead-float units short" closed with the TEXT of one
  tiny shared inline: GFFastSin written with named table values and the index step as two statements
  (`i -= 256; i &= 0x3ff; float a = g_CosTable[i]; float b = g_CosTable[i + 1]; return (1.0f - frac) * a + frac * b;`), found by
  comparing the helper AND ITS SIBLING (GFFastCos) with FableWin sizes: when a helper is off by the same small residual as its
  sibling, the difference between the two siblings still pins the text. Twins that share helper bodies are one job: do not
  freeze the already-matched one; fit one helper set and regrade both. Use dead locals as a ruler ("how many units short") to
  rank candidate texts, and confirm the final match is not on an edge (still matches with a few diagnostic units added).
- (c2048, 0x00B64FA0 RenderClouds, closed) The or-operand order of N expanded SetRenderState blocks had TWO flip points: the back
  one moved only with the spelling of nearby statements (pixel constants back to the matched sibling's `C4DVector(b, b, b, b)`),
  the front one with the NUMBER OF NAMED LOCALS in the function counted across every expanded inline (dead or live, anywhere).
  A dead-local ruler gave the count needed (about 12); the natural source was ONE named local inside the inline that is expanded
  12 times (`const unsigned long desired_state = state.DesiredState;` in SetRenderState, opt-in
  FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED, FITTED). When a ruler says "k more named symbols" and an inline is
  expanded k times in the row, try one named local in that inline first. Fit sibling rows' shared statements to the spelling of
  the already-matched sibling before anything else.
- (c2051, 0x0042EC7C Run, closed) A callee-saved register holding the "wrong" value across a long stretch was decided by ANOTHER
  candidate in that stretch: an unnamed argument temporary (`font->Preload(CWideString(L"..."), true)`) made VC7.1 load the
  object's vtable before the temporary's constructor and keep it in the register. Naming the temporary in its own block
  (`{ CWideString glyphs(L"..."); font->Preload(glyphs, true); }`) freed it. Look at what else competes for the register.
- (r2034, 0x0097BDA0, closed) Apply the arm-swap probe to the OTHER arms of the same construct too: 16 bytes of allocator
  difference in the near arm closed by testing the far arm's cases in the other order (`if (A || B) { tail } else { charge }`).
  Load order of a compare operand: parenthesise the subtraction `x <= (a - b)`. x87 multiply operand order when both factors are in
  memory: VC7.1 loads the factor whose field record has the larger id; naming the fields in the row body (a by-value copy
  `C3DVector facing = ...;` then `v.Dot(C3DVector(facing.Y, -facing.X, 0.0f))`) creates their records early and flips the order.
- fwsize writes its scratch next to the source file (`_fwsize/`); never run it on a file inside the repo.
- (c2052, CWorld save / load, closed) Frame 4 bytes short with identical code (107 displacements): retail gave two locals more
  packer weight; the source was an EARLY `if (flag) return true;` before the function's tail. VC7.1 merges the extra return into
  the final one (no code) but it adds destructor references to every local live there. Shared temporary slot for a late value:
  put the tail that computes it (`header_length` .. `Close()`) in its own block. A `double` member read before a call that may
  change it: copy it to a named local first and store the new total to the local before the member.
- (f4007, ConstructQuestList 12 KB, closed from 3324 / 3370) Order of work on a big row: (a) FULL direct-call comparison first
  (retail call targets v the object's DISP32 symbols; /opt/fable_lane/w22/agent4007/tools/cd.sh): every wrong inline decision is
  one line; (b) a BRANCH-TARGET check once close (agent4007/tools/bt.sh): the masked diff hides branch targets, and a statement
  block sitting in the wrong arm showed up as four unrelated "register residues"; (c) FableWin's signature list says which
  TEMPLATE INSTANTIATIONS and accessors the original used: a class with only the one-argument `GetTC(T**)` instantiation means
  the original wrote `GetTC(&tc)`; accessors that are one instruction longer than a plain member read go through a smart
  pointer's `operator*`; a 4-argument constructor of 18 instructions forwards to `Initialise`. Each such layer adds intermediate
  code with no byte of its own, and that amount decided BOTH the call-v-expand decisions at a later site and the last SIB
  base / index order. Measure the distance with a dead-statement ruler (diagnostic only), then find the layers by FableWin size.
  Also: `if (a || b) X else Y` places X after Y and makes X's inline decisions first; `if (n != 0)` in front of a `for` halves
  the loop body's static frequency with no code; an inlined bool helper written `if (x) return false; return true;` folds away
  where `return !x;` leaves a bool candidate that moves choices all over a large caller; a hinted STLport `insert_unique` body
  visible in the TU changed the frame size by 8.
- (r3017..r3021: five parked rows closed) Operand ORDER residues (x87 term order of a Dot, SIB base / index, which factor is
  loaded) follow hash keys over c2 record ids: they depend on how many named symbols / records exist before that point, and the
  ids are handed out in chunks of 32, so one record can flip an order far away. Procedure that closed all five: (1) fwsize every
  inline helper the row expands and take the text FableWin's size proves (a GetRight text of 43 instructions against FableWin's 15
  was the cause of 150 differing instructions); (2) measure the distance with a dead-local ruler (diagnostic only: a dead local
  is one unit anywhere in the function, a used forwarded name is less, dead constant initialisers are 0); (3) close the distance
  with NATURAL step-by-step source: name the intermediate values the logic already has (a `bool` for each test, a pointer / a
  reference for each object used twice such as `const CPlane& source_plane = planes[i]; CPlane plane = source_plane;`, a named
  local per vector component or dot), table helpers written with named table values (`float a = table[i]; float b = table[i + 1];`),
  corner / member reads through the inline accessors FableWin lists. Grade each piece alone first: it must leave the code
  unchanged apart from the record-keyed sites. Say in NOTES.md how wide the window is (how many dead locals break the match).
- (r3019) A callee that retail CALLS at every site while its plain inline body would be expanded at some, with inline scores that
  no /d2inlT<N> can separate from the sites retail does expand: make the body VISIBLE but not expandable (hdr opt-in with
  `__declspec(noinline)` under _MSC_VER, `// Inline-budget:` line, EXTRA_RULES preference 3). Visibility alone matters: with the
  body visible VC7.1 knows a by-reference key is only read and keeps it in a callee-saved across the next call. A noinline probe
  header separates "visibility" from "inline decision" in two grades. `find` assigned to a named iterator before `end()` is
  evaluated gives `call find ; cmp eax,[end]`.
- (r3017) A cross product helper with each component grouped `((Y*o.Z-Z*o.Y),(Z*o.X-X*o.Z),(X*o.Y-Y*o.X))` moves c2's scheduling
  cut (parentheses are tuples); pointers fetched before counts in the prologue.
- One text per helper across rows: when a helper text you change is also used (as a row-local copy or a header body) by another
  matched row of the same class, regrade that row with your text and say so; a text that breaks a matched twin is not finished.
- (c2058, c2054, c2055: inline decisions) VC7.1 expands a callee iff c1*(1/f+6)/7 < 74 with f the static frequency of the site.
  When retail CALLS many small callees (destructors, copies, find) that ours expands, and all sites sit behind one construct,
  the frequency in front of it is off by a factor of two: look for one more branch level in the original (a guard such as
  `size() > 0`, `if (n != 0)` ahead of a `for`, a pointer test the compiler can fold). A c2 inline trace gives the scores in one
  compile. Before forcing anything noinline, grade the TU's own out-of-line copy of the helper against the retail copy: a forced
  noinline `__copy` was a WRONG helper text (68 bytes against 97); the right text (existing opt-in FABLE_RETAIL_STLP_VECTOR_ASSIGN)
  was kept a call by the compiler itself. A helper that must be visible but called (`_M_insert_overflow`, `find`): visible body
  with the noinline macro in an hdr opt-in plus `// Inline-budget:` (r3019).
- (c2059) Render-state bookmark test / `or` order residues did not move with dead-local rulers at all: the fix was writing the
  states through the SAME inline helpers the sibling pass uses, with constant arguments (`SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
  SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);`), not direct setter calls.
- (u3022) Settling one text per helper across three rows: diff the helper blocks, then decide each helper by its out-of-line retail
  copy (compile the helper alone out of line and grade it against that address) and by FableWin size; the row that loses its
  match gets a natural named temporary (`CQuaternion slerp; slerp.SetSlerp(...); rotation = slerp;`, `C3DKeyframe& keyframe = keyframes[bone];`).
- Scripted compiles count as grades. Do not run generator searches over hundreds of full-row variants; measure the first
  wrong c2 decision and derive the spelling. Prefer spellings a Lionhead programmer would plausibly have written.
- Count every full-row compile you diff against retail as a grade (say how many probe compiles you ran on top).
- Use all three passes before parking unless you can PROVE the row cannot match in this TU.
- The two hand-backs, for conventions: rebuild/candidates/manual/pending/2026-11-11_acctB_c2001/ and _c2002/ (their hdr/
  holds headers and opt-ins not yet in rebuild/include: copy any you need into your own hdr/ rather than rewriting them).

## Hand-over (all inside /opt/fable_lane/w22/agent2050)
- src/<addr>.cpp + test/<addr>.cpp for matched rows; differ/ for parked rows; hdr/ (+ hdr/README.md).
- STATUS.tsv: header `address<TAB>grade<TAB>bytes<TAB>test<TAB>grades_used<TAB>notes`, one line per row of this chunk
  (grade MATCH | RELOCATION_MATCH | DIFFER | SRC_FAIL | SKIP; bytes `<ours>v<retail>`; notes = for a parked row the exact
  remaining diff and the next idea).
- NAME_FIXES.tsv (`address<TAB>correct_symbol<TAB>evidence<TAB>wrong_spellings_seen`) for landed spellings you prove wrong.
- LABELS.tsv (`address<TAB>name<TAB>evidence`) for every function / global name you proved (your rows and their callees).
- NOTES.md (names and evidence, descriptive names, header debt, parked rows), LESSONS.md (GOTCHAS-style one-liners with
  addresses + "Review at landing" items), SPLITS.tsv if any length was wrong.
- Run finish.py LAST (payload.json + oracle.tsv). Final message: matched (MATCH vs RELOCATION_MATCH) / parked / skipped
  counts, per parked row what still differs, grades used per row, names proven vs descriptive, header changes, lessons.
