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
