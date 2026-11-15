# agentP6 best sources for the four parked Codex pass 22 rows (2026-11-15; work continuing)

Each compiles on the repo headers (Codex's private header profiles merged in landings gc and gd) and passes its behaviour test.
vland dry runs, strict headers:

| row | grade | measured reason |
|---|---|---|
| 0x00BF0B20 SetupOutlineStencilStates | DIFFER(1290v1290), 3 lines | scheduler tie: the B load and the G store both priority 0x17400, the lower index (G store) wins |
| 0x00F3E120 FindBestStartVertex | DIFFER(1411v1411), 2 lines | two immediate-only constant records (0x2aaaaaab, -1) set the reload order; the order that fixes the reloads also flips the prologue stores |
| 0x0064BFBA CDrawHighlightingTargetingCursor::Draw | DIFFER(2217v2223) | `vcall * Call().Y` evaluates max-first; only a const-ref binding gives dims-first, at the cost of a frame slot |
| 0x00675E20 CCreatureDef::Transfer | DIFFER(1912v1912), 177 lines | scratch-register rotation starts at the first inlined traits arm, where retail gives every stream push eax |

`LESSONS.md` has the measurements; `HEADERS.md` the header-profile merge.

## Session 3 (no new match)
- 0x00F3E120: c2.dll hooked (`tools/tfree.py`, `tools/tcnt.py`). After constant-candidate creation, c2 walks its 64-bucket constant hash
  (0x10882050, bucket = value & 0x3F) and frees every constant with use count 0 (0x10784D3B); the next root candidates reuse those
  records LIFO, so `this` takes -1's id and `vertex_link_map` takes 0x2aaaaaab's. The order is set by the constants' values. Retail's
  c,s,b source needs one more count-0 constant in buckets 44..63 walked after -1, most likely the -4 of the three `size() == 0` guards
  (`test reg,0xfffffffc`, bucket 60) in a spelling that leaves its use count 0.
- 0x00BF0B20: no source lever keeps retail's bytes (the B byte has no other consumer; the debug bodies fix the R,G,B,A order).
- 0x00675E20: a one-arm micro-TU rotates scratch registers from the first arm in every variant; retail's first arms look like allocator
  candidates. Next: hook c2's expression-temp routine 0x10775ACC.
- 0x0064BFBA: both PDBs give C2DExtentsI only constructors and Initialise, so no accessor spelling exists.

## Fable-model session (agentP6F; `fable_session/`)
0x00BF0B20 matched and landed (landing ge). Still open, with the causes narrowed further:
- 0x00F3E120: `best_start_vert = 5` in place of -1 leaves only the immediate differing, so the constant-record free order is the whole
  residue. c2 builds the constant table in tuple order (0x1077D560) and frees a constant used once outside a loop; retail needs a
  second byte-free use of -1, or a single-use non-loop constant in buckets 44..63 after it. The -4 guards are inside loops.
  Next: the c2 pass that deletes a constant-carrying tuple after that walk.
- 0x0064BFBA: micro TUs give the rule: an operand that is exactly `*(call)` is called first, `*(call + 4)` (`.Y`) evaluates the left call
  first unless that call's argument is constant; retail's argument is `[ebx+0x7d0]`. Next: trace c2's tree ordering on micro/o6.cpp.
- 0x00675E20: retail's first five stream temps (0x00675E43..0x00675E8D) are non-rotation assignments (eax), ours rotation temps; c2
  -ZI pseudo-tuples and /Zi /Z7 /Zd /GX ruled out. Next: hook 0x10775ACC on a class-type-arm micro TU.
Environment: the first wine cl.exe run in a fresh container can hang (wineserver inherits the capture pipe); stop it and rerun.

## Fable-model session 2
0x00675E20 matched and landed (landing gf: the existing NODEFAULT TransferVector opt-in). Still open:
- 0x00F3E120 (2 lines): no source construct found that adds a constant use which survives to c2's constant walk and then vanishes;
  across this TU the only constant that is freed and leaves no immediate is 0. Next: diff c2's tuple list between the walk
  (0x1077D560) and emission on a function from another TU where a non-zero constant vanishes, to name the pass.
- 0x0064BFBA (6 bytes short): `micro/o15.cpp` reproduces retail's exact sequence only with BOTH an inline accessor yielding the
  receiver AND an inline reference pass-through of the extents temporary; no PDB-named inline does either. Next: read the debug
  donor's call order at FableWin 0x020FEA1D / 0x020FEA77 (FableWin.exe is not in this container).

## Fable session 3 (2026-11-15, hand-authoring): 0x0064BFBA matched (landing gg); 0x00F3E120 still 2 lines
- 0x0064BFBA: HAND-5 of six whole-block rewrites, the left border graphic index named once by `const long&`; landed as landing gg.
- 0x00F3E120: 20 hand rewrites (`fable_session/hand/`, `ATTEMPTS_session3.log`). The chain behind the two reloads is measured:
  round-0 ids from the freed constants (0x2aaaaaab id 1, -1 id 2), two split passes that each reverse the order of `this` and
  the map. One flip anywhere fixes both reloads; HAND-15 (`return -1;` in phase 3) proves it (every byte to 0x00F3E695 equal)
  but adds its own `or eax,-1` return path. The coordinator's nine extra probes are at the end of the same log.

## Fable session 4 + coordinator probes (2026-11-15): 0x00F3E120 still not matched
- Two bases (`fable_session4/`): `base_pdb_order.cpp` (PDB declaration order; prologue right, both reload pairs swapped, 4 lines)
  and `base_u1.cpp` (`long vertex_count;` assigned after the two initialisers: reloads right, the three prologue stores in the wrong
  order, 3 lines). The prologue stores tie at scheduler priority 0xc00 and come out in IL order; retail's order also keeps the
  `this` spill first, so retail's IL order is count, score, vertex and only the reload pair is open.
- Measured chain: c2 numbers constants in forward tuple order; immediate-only constants (0x2aaaaaab, -1) are freed at round 0 in
  hash order and `this` / vertex_link_map take their records; split rounds pop in ascending record id; the phase-1 `start_vert`
  record (#12) goes to the lower of `this` / vertex_link_map, the other gets #72, and the reloads follow piece ids.
- 83 hand-written grades by agentP6G (region rewrites of the prologue and both reload sites tagged in ATTEMPTS.log) and about 40
  coordinator probes (`fable_session/ATTEMPTS_session3.log`): every byte-neutral spelling canonicalises to the same IL. Levers left:
  a use of -1 or 0x2aaaaaab the constant walk sees but that leaves no bytes, or an extra float/integer candidate without code. The
  debug build's body (FableWin.exe) would settle both; it is not in this container.
