# Chunk 3009 (near-miss retry 3 of 0x007C6E50 CTCTurncoatSpell::FrameUpdate) -- working notes

**RESOLVED 2026-11-12 (continuation 2): RELOCATION_MATCH 1806v1806 with a `give_xp` flag (see "MATCHED (hand-over)" at the end).**

## State
- Start: prior/3013/differ source + test copied to src/ + test/ (and differ/), prior/2027/hdr copied to hdr/ (3013 had no hdr changes).
- Grade 1 (baseline regrade of the prior source, FABLE_HDR_OVERLAY=hdr): DIFFER(1804v1806), same residue as before (XP block,
  0x007C741C..0x007C749E).

## Measured this chunk (all free: c2prio traces / what-ifs, no source grades)
- WHAT-IF PROOF (tools/wi/whatif.py, sticky spill -5 on the XP piece #73 {id 73, prio 14, refs 2, spill 2}): the resulting object is
  1806 bytes and md.py reports 551/551 equal instructions (ratio 1.0000) -- the ONE decision "parent's XP piece is coloured (esi)" is the
  whole residue; with that piece left in memory every byte equals retail (incl. the temp rotation ecx -> edx -> eax).
- Why spill 0 alone did not work for 3013: c2's deferred-low-spill path re-costs (0x10789715: f(sym) + spill, f = 2 for a non-constant)
  and then 0x1075111c walks the piece's copy-preference list (+0x38, nodes {next, regdesc, count}); the XP piece has [esi, 1] (copy to the
  map #62 in esi), so it is coloured anyway. The absorb piece #75 has an empty list -> memory.
- Spill arithmetic (priority pass 0x10782473; operand cost via 0x107801c0 adds cost*w to +0x40, tuples of opcode 0x26A / 0x26B subtract
  2*w): priority-pass block entries (tools/pblk.py on a --blocks trace): XP piece = counter-store block (live, flag 0x10, cost 2) + map
  block (refl 0x18 cost 2) -> spill 2; absorb piece = absorb HasTC block (live 0x10) + sound block (live 0x11) + push block (refl 0x19)
  -> spill 0.
- Spellings measured (c2prio, XP piece still formed and coloured): XP lookup on word 0 (CSE'd HasTC address, probe w0); GetTC default /
  ORIGINAL_CHAIN bodies (p1, p2); visible same-TU bodies of IsTurncoated / ModifyTurncoatedness / SetTurncoatOriginator (lead E, vis);
  turncoat or hero_stats kept live after MakeNoise (lt, lh); a release flag joined before MakeNoise (fl, lead B); `this` live on the
  HasTC-false edge only (ef: piece forms and takes eax).
- `this` live to the END of the function (ls: StopTurncoatingEffects() after MakeNoise; 3013's MakeNoise on ParentThing) is the only
  thing seen so far that removes the XP piece -- but then the XP map moves to edi (not retail).

## Next
- Find which property of `this`-live-at-end prevents the XP piece (split pass 0x1078b2aa / component builder 0x1078b372, piece ctor
  call 0x1078b494) and look for the retail-natural source of it.

## c2 split-pass findings (free traces; tools/c2p/c2prio_c.py = c2prio_b + hooks; tools/sp.sh, tools/pblk.py)
- Piece benefit (+0x40 "spill") = sum over the piece's tuples: operand use +2*w (0x10782692; op 0x33 = HasTC test, 0x12 = map copy,
  0xd = push, 0x1 = other), minus 2*w for every 0x26A tuple (= a reload at an entry into the piece from memory, 0x1078281c) / 0x26B.
  Base: XP piece #73 = {0x26A -2 (entry, counter-store block), 0x33 +2 (HasTC), 0x12 +2 (map copy)} = 2 -> coloured esi.
  Absorb piece #75 = {0x26A -2, 0x33 +2 (HasTC), 0x26A -2 (re-entry at the sound join 7136), 0xd +2 (push)} = 0 -> memory.
  The absorb map copy is NOT in the piece: its block is marked +0x34 ("blocked") by 0x1078b346 because a 0x1c tuple in that block
  carries a live-across set containing parent; the XP map block has no 0x1c tuple at all (tools: on_tup / on_callpre hooks).
- Web builder 0x1078b372 = union-find over blocks using per-block bitsets +0x24 (live-in) / +0x28 (live-out) / +0x2c,+0x30 (refs) /
  +0x34 (blocked) for the split candidates' mask bits (tools/c2p on_web hook).
- So retail needs the XP map block blocked for parent (or a second entry into the XP piece): then the XP piece is {reload, HasTC} = 0
  and the map copy reads memory (= retail's `mov esi,edx`).
- Structural probes (learning only, code changes): MakeNoise duplicated into both arms (md) / s5 / s7 -> XP becomes retail-like but
  the function grows (1852 / 1897 / 1851 bytes, no cross-jumping) and the absorb piece gets coloured: not retail.
- 0x1c pseudo-tuples are made by 0x107967b2 (op 0x2c7, set at +0x18) at the start of each split pass: in the parent's split they
  exist at the calls of blocks 35108 / 36508 / 37908 / 56936.. / 84496 / 84696 / 84896 / 85096 (XP after LowerBound) / 85696 (absorb
  LowerBound) / 7136 / 7536 -- but NOT at the XP LowerBound call (block 84096), although parent is live to MakeNoise there.
  0x107967b2 is called with a candidate set (arg) and an insertion tuple; its callers decide which calls get one (unread).

## Hand-over state (context limit)
- Best source + passing test: differ/007c6e50.cpp (DIFFER 1804v1806, regraded after the comment edit) + differ/007c6e50.test.cpp
  (PASS g++ and VC7.1). src/ and test/ are empty (row not matched). hdr/ = chunk 2027 overlay, unchanged.
- Grades: 2 (baseline + comment regrade); passes 2-3 unused. Free probes: probe/*.cpp (w0, p1, p2, vis, lt, lh, ls, fl, ef, md, so,
  so2, s5, s7), traces in tmp/*.json, tools: tools/prio.sh (C2P=c2prio_c.py, BLK="--blocks parent"), tools/sp.sh <src> <tag>
  (per-piece benefit terms), tools/pblk.py, tools/wi/wi_host2.py (+ WIF=whatif.py sticky spill what-if), tools/odis.py <obj>.
- Next concrete step: hook the callers of 0x107967b2 to see which liveness set decides 0x1c creation at a call, compare the XP
  LowerBound call (84096) with the absorb one (85696), then derive the source spelling (likely something live across the XP
  LowerBound in retail that is not in ours, or the XP/absorb lookup IL shape).

## Continuation (agent resumed in place) -- the 0x1c marker rule, measured (tools/c2p/c2prio_d.py, tools/my/occ.py, free)
- The 0x1c tuples are REGION-BOUNDARY markers made by the region builder 0x10740a6e..0x10740bb6 / marker loop 0x107409c9..0x10740e10
  (callers 0x107409b7 / 0x10740cc0 / 0x10740cf6 / 0x10740dd1 / 0x10740dfa of 0x107967b2). Blocks are visited in layout order;
  T(block) = allocatable regs NOT occupied in the block (call clobbers + candidates already coloured before this split). A block JOINS
  a visited pred's (else succ's) region iff T(block) and the region's common free set intersect; else it starts a new region. Markers
  go on every region boundary edge.
- Ours: region 3 (7216..83896, lines 165..180) has common free set {edi} only (ebx = hero_stats to line 167, ebp = turncoat to line 176,
  esi = this). XP call block 84096 T = {ebx, ebp, edi} -> joins region 3 (no boundary -> map copy inside XP piece -> esi).
  Absorb call block 85696 T = {ebx} (esi this, ebp turncoat, edi absorb map) -> new region 5 -> boundary -> memory = retail shape.
- So retail needs edi (and ebx, ebp) unavailable in 84096 at parent's split, or a region-3 free set without edi. Const 0x68 (#18) is
  live in 84096 uncoloured (immediate); a what-if spill 4 on it changed other things (532/551) -- not the lever as such.
- Free what-ifs (tools/wi/whatif_prio.py + wi_host3.py: set candidate priorities before the sort): parent prio -300 / -60 -> unchanged
  (523/551); -10 / 3 -> far worse. Colouring ORDER of parent is not the lever. What-if const 0x68 (#18) spill 4 -> it takes edi, the
  XP region breaks and the XP prefix becomes retail's exactly (mov edx / mov esi,edx, temp rotation) but 0x68 stays in edi (532/551):
  confirms the region rule; retail must have edi (or ebx) unavailable in the XP call block 84096 at parent's split.
- Region arithmetic: any region holding 83696 (ModifyTurncoatedness call: free {ebx, edi}) keeps a common free set within {ebx, edi};
  so retail needs the member of that set occupied IN 84096 itself. Retail's final code has nothing in ebx / edi there (experience gets
  edi only after the call) -> the occupant is invisible in the final code or the IL of the XP block differs.

## Grade log (continuation; candidates in cand/, grader ./gr.sh <abs path>)
- g3 c0 baseline differ source: DIFFER 1804v1806, 523/551 (first diff 0x007C741C).
- g4 c1 experience declared at case scope: unchanged.  g5 c2 `bool found = parent->GetTC(&experience); if (found)`: unchanged.
- g6 c3 ability_def declared before the XP GetTC: unchanged.  g7 c4 lookup before the counter store: 1820v1806 (worse).
- g8 c6 `--Cur; if (Cur == 0)`: unchanged.  g9 c8 `if (--Cur != 0) {} else {...}`: unchanged.  g10 c9 `experience = 0`: unchanged.
- g11 c11 `CThing* thing = parent;` for the XP lookup: unchanged.  g12 c12 `CThing& thing = *parent;`: unchanged.
- g13 c13 default (non-EXPANDED) GetTC body for the whole TU: unchanged bytes.  g14 c14 `const bool give_xp = --Cur == 0;`:
  unchanged.  g15 c15 named stat / amount locals: SRC_FAIL (Stat is EHeroExperienceStatCategory; not retried, it is after the call).
- What-if spill +3 on const -1 (#7): did not match at the check (no hit), nothing learned.

## Hand-over (continuation, context budget)
- Best source unchanged: differ/007c6e50.cpp (DIFFER 1804v1806, 523/551) + differ/007c6e50.test.cpp (PASS g++ / VC7.1, unchanged).
  Grades in this directory: 15 (g1-g2 predecessor, g3-g15 this continuation; all candidates in cand/ c0..c15).
- Tools added: tools/c2p/c2prio_d.py (c2prio_c + region-builder hooks rgn / rT / rJ / rNew and mk2c with the inserting caller),
  tools/my/occ.py <json> [blk...] (per region block: free registers + live candidates with their registers at parent's split),
  tools/my/xr.py (c2.dll branch xrefs), tools/wi/whatif_prio.py + wi_host3.py (priority what-if), gr.sh (full-row grade + diff).
- Ranked next ideas:
  1. Find the occupant: in retail ebx or edi must be unavailable in the XP LowerBound block at parent's split (proof above). The only
     ways that leave the final code unchanged: (a) a constant coloured there and released at CONSTBACK 0x10785366 (check how that
     release decides: per register descriptor or per web -- edi also holds `experience`, so a per-register rule rules it out);
     (b) IL in the XP call block that is coloured and later deleted (a copy coalesced after allocation); (c) a different block
     LAYOUT order at split time (84096 visited before its pred 83896 -> new region). Test (c) first with compile-only layout probes.
  2. Structural spellings that change the XP block's IL but not its bytes: GetTC through an out pointer that is a reference /
     a CTCHeroExperience*& parameter of an inline helper; the lookup in a nested `{ }` scope with the def handle; PeekTC-style
     const map pointer -- grade each.
  3. If (1) proves no source can do it in this TU, record the row as a c2-history residue with the what-if proof (551/551).

## Continuation 2 (2026-11-12, "grade more, trace less"); grader ./gr.sh, candidates in cand/ (g16 = baseline regrade of c0)
- g17 c16 IsTurncoated arms swapped (`if (turncoat->IsTurncoated()) {absorb} else {...}`): 1787v1806, 442/551 (layout: absorb in
  line, not-turncoated arm moved after the ret) BUT the XP prefix becomes retail-shaped (`mov eax,[esp+14h]` hoisted above the counter
  store, `mov esi,eax ; add esi,44h`): parent pieces #75 (absorb, memory) and the XP piece dropped (refs 1). Layout/flow decides it.
- g18 c17 `if (alive && health > 0 && stamina > 0) { if (InRange) {...} } else release;`: unchanged 523/551.
- g19 c18 ability_def at case scope: 1803v1806 frame grows (0x68): worse.
- g20 c20 braces around the release statement: unchanged (no-op).  g21 c21 (+ g22 re-run for the full diff) named `CGameDefinitionManager& defs` first: Get() moves
  before the CCharString ctor (535/551, 1806 bytes, XP prefix unchanged).
- g23 c22 `parent = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0)` (CTCGhostSword sibling idiom): unchanged.
- g24 c23 `const bool turncoated = turncoat->IsTurncoated(); if (!turncoated)`: unchanged.  g25 c25 experience at function scope: unchanged.
- g26 c24 (+ g27 re-run) `bool give_xp` set in the counter block, lookup after the IsTurncoated if/else (before MakeNoise): 1805v1806, 487/551 --
  the XP prefix is retail-shaped (hoisted parent load + copy) but the absorb arm moves after the ret and the frame / temps shift.
- g28 c26 c24 + a `release` flag for both release statements: 1809v1806, 497/551 (turncoat loses ebp): worse.
- g29 c27 `bool give_xp = false;` at the top of the not-turncoated arm, set in the counter block, and the experience lookup in
  `if (give_xp) {...}` after the dead / InRange chain, still inside the not-turncoated arm: RELOCATION_MATCH 1806v1806, 551/551.
- g30 src/007c6e50.cpp (c27 + comment update): RELOCATION_MATCH 551/551. rel.py: every call target equals retail; vland (read-only):
  RELOCATION_MATCH + behaviour PASS, one RELOC_NAME warning (0x007C76D0 labelled _unattributed::AssignTarget_007c76d0 in the
  repo labels; LABELS.tsv here proves ?SetTurncoatOriginator@CTCTurncoat@@QAEXPAVCThing@@@Z).

## MATCHED (hand-over)
- src/007c6e50.cpp + test/007c6e50.cpp (PASS g++ / VC7.1). differ/ emptied (old parked source in prior/3009_differ/).
- FITTED: the `give_xp` flag (name descriptive; the bytes cannot show the flag, VC7.1 threads it away). Why it works (measured
  by the c16 / c24 / c27 grades, not traced in detail): moving the experience lookup out of the counter block and behind a joined
  flag test changes the c2 live-range split of `parent` so its XP piece no longer contains the map copy; c16 (arms swapped) and
  c24 (lookup after the whole if/else) showed the same XP prefix with the wrong layout, c27 keeps the lookup inside the
  not-turncoated arm so the layout stays retail's.
- PROVEN (unchanged from chunks 2027 / 3013): the function name and every callee (two sources each, see the source header);
  NAME_FIXES.tsv adds 0x006D8300 AddTrainableStatExperience(EHeroExperienceStatCategory, long) (Ego_r + FableWin only spelling;
  reloc_bindings has `JJ` from the landed 0x008F5C50).
- No header changes in this chunk (hdr/ is the chunk 2027 overlay, untouched), so no shared-inline regrades are needed.
