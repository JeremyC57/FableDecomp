# TRACE -- 0x006096E0 CTCInventoryMap::ConstructLogbookListPC, playbook session (2026-11-09)

## Step 1: regime
- tools/c2/nl.sh (copy of hand886 cnt.py probe, gate c2.dll 0x1077BE49): base (= differ/ p12, /d2inlT71) **5571 IL tuples** -> regime 0
  (global optimizer on; block weights w = 1 << loop depth; regime-0 inline rule).
- Baseline grade: DIFFER(12832v12832); cmpx.py: exact 86.76% (relocs normalised, crude), regs 86.94%, regs+frame 99.00%;
  frame 0x124 v 0x11C; 16 differing call segments (frame-masked): 0 (frame), 279/280 (count prologue), 332-349 (books icon
  branch), 374/376/378 (books loop exit / title), 491 (COMBAT ring release), 537 / 577 / 590 (pads + tail loop jmp).

## Step 1b: inline trace (tools/inl, hand890 inl.py) at /d2inlT71
- 900 decisions, 54 rejected. The count loop's `++it` (`??E?$_Rb_tree_iterator...`, site 636): sz 62 pen 33 c1 89 f 5 **sc 72**
  -> T71/T72 call it (retail), T73/T74 expand it. Group-4 CAction sites: sc 107-111 (budget). CCountedPointer<CComponent>(T*) sites
  sc 76-81 (called, as retail).

## Step 2: allocator, first wrong decisions (c2prio --trace --blocks on base; tools/obj.py object-level histories)
- Books icon branch (retail 0x0060B58D..0x0060B718): candidate #448 (`subtitleText.Info`, lines 384-385,395) prio 34; the
  manager piece #195 (u794; refs in the two CreateComponent blocks, K 14 / 16) prio 54, allowed `b` only -> ebx; then #448 is
  split and Info lives in memory ([esp+0xC8]); retail: Info in ebx, manager read from its home twice, sprite spilled.
  Inequality: prio(#448) > prio(manager piece #195) = 54.
- **Diagnostic (tools/wi, force-by-id what-if): #448 = 60 -> frame 0x11C (= retail), exact 87.1% -> 99.03%, differing
  segments 16 -> 8** (icon branch, frame and seg 577 tail-loop fixed). Lever proven.
- Count prologue: the const-0 piece covering the count block (rel lines 189-197) is u797 (prio -12 at re-sort) -> loses ebx to
  const-0 piece #569 (prio 0), is split; the count-block remainder u1000 (prio 42, allowed acdb, not across the clear() call)
  -> eax. Retail: a const-0 piece in ebx across the clear() call.
- **Second diagnostic (tools/c2pf, force-by-creation-ordinal C2_FORCEUID): manager piece u794 = 30 -> identical result to
  #448 = 60** (frame 0x11C, exact 99.03%, 8 segments: 279, 280, 374, 376, 378, 491, 537, 590). The lever is the pair
  inequality prio(subtitleText.Info) > prio(then-arm manager piece); threshold measured: #448 = 55 works, 53 does not.
- Piece shape: #209 (mesh.Data, lines 356..368, allowed b only, prio 324) takes ebx at ev5646; the split pass at ev5647 cuts the
  books-loop manager piece u632 into u793 (after the loop, dropped), u794 (then-arm B1 275-278 K7 cost 2 = +28, B2 278 K3
  live-through = -6, B3 278-283 K8 cost 2 = +32 -> 54) and u807 (else-arm, 1 ref, dropped); the entry/model creates get the
  parent-None piece #564 (prio 64, ebx, = retail `mov ebx,[esp+98h]; mov ecx,ebx`). Info: +32 +12 (B1, B2) -18 (B3 live)
  -6 -6 -6 -2 (icon release blocks) +8 +8 +12 (own release) = 34.
- Arithmetic of natural levers: one candidate added to B2 or removed from B3 moves the gap by 6; the gap is 21 -> 4 candidates,
  which no code-identical spelling provides. Variants graded (all no gain): e1 icon in a nested scope, e3 `CManager* const`,
  e7 subtitleText declared before the frame call (moves the zero init before the call: code change), e11 counted CSprite (code
  change); earlier a1-a8, d1-d6, h1/h2 (LEADERBOARD.tsv).

## Step 6b: frequency rule re-check for the /d2inlT71 flag
- w1: `it = begin(); if (it != end()) do { switch; ++it; } while (it != end());`, no flag: ++it is EXPANDED (worse) and the loop
  loses retail's `jmp cond; lea esp,[esp]; nop` rotation (retail is a `for`). w1t (same + T71): 312 segments. The `for` stays,
  and so does the flag (owner ladder step 1). A plain `while` is 9x (playbook) and cannot help.

# Session 2 (2026-11-09, family push session 2)

## S2 step 0: instruments
- tools/srch/cf.py + cf.sh: compile-only fitness (no grade, never touches $S/wine/q): cl with BASE_FLAGS + // Flags: and the hdr/
  overlay into tools/srch/obj/, then cmpx shares + frame-masked call segments; ~4-5 s per variant. tools/srch/gen.py: one variant
  per named edit (edits files tools/srch/e_*.py), '+'-joined combos, logs tools/srch/SEARCH.tsv. Base check: s2base = differ ->
  fr 0x124, ex 87.09%, rf 99.36%, 16 segs (identical to the lb.sh grade).
- c2pf c2prio copy: PIECE_SITES += 0x107A9B63 (return of the parent-None piece call 0x107A9B5E; parent in ebp, the piece copies
  [ebp+0x44] and links [edi+0x14] = ebp). Run on s2base: tools/c2p/s2base.{trace,json} (189 s in gdb).
- tools/pc/pblk.py <json> <ids> [--after EV]: per-pass block contributions (blkend refl / live lists) of chosen candidate ids.

## S2 step 1: then-arm surface spellings (e_then.py, 18 variants, compile-only): all code-identical, no fitness change
T1 subtitle.Data, T2 ShareData spelled out, T3 .Get()->SetValue, T4 entry.Get()->AddChild, T5 icon.Get()->GetType, T6 unnamed
sprite cast, T10 `if (BankIndex)`, T12 (*p).SetValue, T13 (*icon).GetType, T17 explicit SetStaticCast<CComponent>, T18 swapped
compare, T19 C cast, T20 icon.Data, T21 icon.Data in AddPositionChild, T22/T23 copy-init counted pointers: all 16 segs / 87.09%.
T15/T16 (named CEngineGraphic): code change (worse). => the then-arm's IL is pinned by retail's code.

## S2 step 2: where the piece's 54 comes from (s2base.json, pblk.py)
- The books-loop manager piece (#553, made by the first split at ev4001 when list takes edi; spans rel lines 241-303 = the books loop
  + title) is split at ev5647 (mesh.Data #209 takes ebx) into #190 (loop head + title), #564 (entry/model, prio 64 -> ebx, = retail),
  #56 (then-arm, prio 54 -> ebx, retail: memory) and #266 (else-arm, 1 ref); the details create (rel 265) gets no piece.
- #56 = B341 (rel 275-278) w2 K7 ref cost 2 (+28) - B342 (278) w2 K3 live (-6) + B343 (278-283) w2 K8 ref cost 2 (+32) = 54.
- K at a re-sort = the initial K minus candidates dropped/forwarded since (coloured ones still count): B341 8 -> 7 (string const #61's
  piece dropped), B343 9 -> 8 (#62 dropped). The B341/B343 K' sets at the split: {frame (esi), #56, const -1 piece #513, const 0 piece
  #210, info temp #449, Info #448, subtitleText.Data #215} and {frame, #56, #513, const 1 piece #561, #210, icon.Data #216, entry.Data
  #206, #215}. Info's own 34 is from the initial pass (K 8/3/9 ... release blocks +8 +8 +12) and is never recomputed.
- So with the then-arm IL pinned, prio(#56) = 4*(K341'+K343') - 2*K342' only falls if candidates referenced in B341/B343 are dropped
  or forwarded before the second split (the three const pieces = -20 -> 34, a tie that Info's larger key 2748 v the manager key
  would win), or if the piece's region changes. Region formation = the iterative bit-vector pass at c2.dll 0x1078B51C..0x1078B625
  (per block +0x24/+0x28/+0x2C/+0x30/+0x34/+0x38/+0x3C sets, piece bits OR-ed over predecessors) -- not yet modelled.

## S2 step 3: coordinator family (drop/forward the B341/B343 const pieces) + broad code-identical screen
- What the pieces are (s2base.json, events 5647..5867): the second split (mesh.Data #209 takes ebx) cuts const -1 piece #49,
  const 1 piece #512, const 0 piece #523 and the manager piece #553 at the same time. In the K' sets: #513 (const -1, 2 refs:
  the `push -1` of the SUBTITLE and ICON CCharString temporaries), #561 (const 1, 2 refs: SetValue's `push 1 ; push 1`), #210
  (const 0, 39 refs). A new piece leaves K only when it is dropped (refs 1 and spill <= 0, e.g. #560, #190, #266); a 2-ref
  piece is only checked for copy forwarding and stays listed (#513 prio -6, #561 0, #210 -6). So the -20 needs the three regions
  cut to one reference each, which retail's code (two `push -1`, two `push 1`, one region of `push 0`) does not allow.
- c2's region pass (0x1078B51C) read: pass 1 is backward liveness over successors (OUT +0x3C = OR of succ IN +0x38;
  IN = GEN +0x30 | OUT & ~KILL +0x34), then IN/OUT are saved to +0x24/+0x28 and pass 2 is forward reach over predecessors
  (IN = OR pred OUT & live-in; OUT = (IN & ~KILL | GEN2 +0x2C) & live-out): a piece's region = blocks where it is live AND reached
  from one of its references, one bit per piece (32 per round). Not yet modelled: which blocks seed each bit.
- tools/srch/e_wide.py: 125 code-identical edits over the whole body (one per occurrence: `if (p)` -> `!= 0`, `false` -> `0`,
  `true, true` -> `1, 1`, copy-init counted pointers, `.Get()` -> `.Data`, `x->` -> `x.Get()->`, `++it` -> `it++`, `const long
  index`, swapped compare) + the coordinator's then-arm family (K1 decl first, K1e/K1b else-arm/both, K3 AddChild first, K4 cast
  before the frame call, K5 subtitleText in its own scope, K6 cast before AddPositionChild). **All 125 code-identical edits:
  byte-identical objects (87.09%, 16 segs).** The K family changes code (K5 frame 0x11C but releases subtitleText early, 24 segs;
  K3/K6 15 segs by reordering calls retail does not reorder; K1/K1e/K1b 21-247 segs). Family fails; nothing full-graded.
