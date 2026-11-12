# Hand chunk 3014 (CEngineSkyRenderer): 1 of 1 byte-exact -- 0x00B65A20 CEngineSkyRenderer::RenderOuterSky RELOCATION_MATCH (g02)

## What matched (1)
- CEngineSkyRenderer::RenderOuterSky 0x00B65A20 (1893 bytes): src/00b65a20.cpp + test/00b65a20.cpp. Shape lesson: the two theme colours
  are copied straight from `params.ThemeChannels[0].Colours[12]` / `[13]` into named locals WITH the existing repo opt-in
  FABLE_RETAIL_CARRAY (CArray::operator[] through begin()). That one change fixed all three residues of the c2031/pass-1 source.

## Pass 2 (continuation agent): how it was found
- New free probe tools/walk2.sh (hook tools/sched/t20.py at c2.dll 0x10784f6f: every tuple the rename walk passes, opcode, flag bytes
  +8..+B, source line; decoder tools/rr2.py; region summary tools/w2s.sh <probe>). The walk visits ALL tuples of the region in reverse;
  the round-robin pick (0x10785179) runs only for opcodes with class flag 0x400 in the table 0x10870dc0 (cmp, push, add, or, inc, dec,
  op 0x51 ...) and tuple flag byte +9 bit 0. Opcode 18 (the 3-address address form, emitted as `lea` or `add r,imm`, e.g. ApplyPixelShader
  `add esi,128h`) is NOT visited.
- Simulator (scratch sim.py, same pick rule + measured busy sets): with v0's busy sets retail's ebx is reachable by (i) deleting BOTH colour
  address `add` tuples (L23/L25), (ii) deleting `push &colour1`, (iii) two extra visited tuples between the `or` and the colour-12 add,
  or (iv) ebx not live at `push 1`. (i) is the natural one: in v0 the colour addresses are visited `add` (opcode 0x2d) tuples.
- Measured opcode of the colour-address tuple (tools/w2s.sh): v0 named refs -> add; v1 direct copies -> add; w7 assignment -> add;
  c1 `C4DVector x(params...Colours[12])` -> add; c3 `const CRGBFloatColour* colours = ...Colours` and c4 channel reference -> opcode 18
  (but one [params+54h] load, FIT 0.72); c5 direct copies + FABLE_RETAIL_CARRAY -> opcode 18 for both, two [params+54h] loads:
  tools/try.sh c5: FIT 1.0000, 67 ops, T1 OK, T2 OK, T3 ebx. Graded g02 = RELOCATION_MATCH; gall.sh, tall.sh, tall.py PASS; read-only
  vland RELOCATION_MATCH + behaviour PASS, no RELOC_IDENTITY (lv/ deleted). finish.py run last.
- Interpretation (measured, not decoded in c2): begin()'s inline result is a register variable, so `begin() + 0` then `.Colours[12]` is a
  register-variable + constant address (opcode 18, like c3/c4), while `_M_start[i]` leaves an expression temporary that gets a two-address
  `add`. Opcode 18 also gives a HARD edge to colour 13's first load (T2) and removes Y's extra successor (T1).

## Pass 1 (predecessor) record follows


## Setup
- hdr/ = copy of prior/2031/hdr; tools/ = copy of prior/2031/tools with paths repointed to agent3014 (never writes to agent2031).
- g01 = prior differ source regraded with this overlay: DIFFER(1897v1893), 597/602 (baseline reproduced).

## Measurements (free probes)
- tools/try.sh <src> : scheduler DAG dump (t16 hook: tuple operands decoded) + tools/ev.py verdicts:
  T1 = blend Y dead store after the GPShaderRenderManager load (retail), T2 = colour-13 `add eax,0D0h` before its first load
  (retail), T3 = register of the split `UpdateFlags |= 2` (retail ebx). Baseline v0: T1 BAD, T2 BAD, T3 esi.
- (b) FOUND THE MECHANISM: the flags temp register is NOT chosen by the allocator or 0x10775ACC. It is a round-robin
  "rename" pass that runs per scheduling region, walking the region's tuples in REVERSE (block end first): c2.dll 0x10785179
  picks the next register after the global index [0x108b022c] in the table 0x108a9738 (eax ecx edx esi edi ebx ebp 0), skipping
  busy ones (bit set arg); the index is reset to 7 at each region start (reverse). Tuples visited: cmp, push, add reg,imm, or,
  inc/dec/test (the RMW / memory-operand candidates). Caller 0x107852a0; operand creation 0x10789304 (writes REGDESC+0x60*reg).
  Hook: tools/sched/t18.py, decoder tools/rr.py. Baseline walk (tail block, reverse): cmp L29 ->edi(4); push L28 ->eax(0);
  push L28 ->edx(2); push L27 ->esi(3); push L27 ->edi(4); add L25 (col13) ->ebx(5); add L23 (col12) ->edx(2); or L22 (flags)
  busy {eax,ebp} entry idx 2 -> esi. Retail's ebx needs entry idx 4 at the `or` (two more steps, or 5 fewer, mod 7).
- Free sweeps (tools/try.sh + ev.py FIT = pick-sequence similarity to the retail target, 1.0 = exact): every local spelling of the
  blend vector (temp, named, C3DVector+w, member-wise, copy-init) gives the IDENTICAL DAG (y1..y5); named references / pointers /
  block-scoped references for the colour copies identical to v0 (n3, n5); a dead-name sweep (enums k1..k8, unused const refs r1..r5)
  changes nothing (not a record-id / name-count key); FABLE_RETAIL_CARRAY (begin()-based operator[]) changes nothing. Colour spellings
  that change the structure (direct copies v1/w9/w10, channel ref n6/v2, refs first n7/w8, copies before the pixel constant v3/v4, array
  v6, assignment w7) all lose the retail register shape (FIT <= 0.88) although several flip T3 to ebx.
- Diagnostic d1 (pixel-constant inline with `UpdateFlags |= 2` FIRST, free function, never deliverable): T1 OK, T3 edi, 68 ops.
- c2 scheduler readiness: a node is ready when c20 (remaining preds) == c24 (remaining "soft" preds: RAW from an `add reg,imm` the
  scheduler can fold as a displacement; c2.dll 0x107b52bd / 0x10745d14). Colour 12's first load waits for the Z dead store (WAR edx,
  16->31); colour 13's first load (41) has only the soft add (40) left once 39 issues, and its load bonus (+32 prio, fl&4) beats the
  add -> `mov ecx,[eax+0D0h]` before `add eax,0D0h`. Retail needs a hard pred of the colour-13 first load outstanding past 39.
- T1: Y store prio 80 = h70 + 10 succ (WAR edges to every later ecx toucher incl. table stores 21..23); W 80 = h72 + 8; tie -> lower
  index (Y). One successor less on Y (or one more on W) gives retail's X, W, GP load, Y order.
- Region-end diagnostics (e1 drop the three blend-mode setters, e2 move them after PrepareForRender, e3 reorder them): T1 / T2 stay BAD
  (only T3 moves) -> T1 / T2 are decided by the local vector / table-copy / colour-copy IL, not by the region end.
- Inline-body diagnostics through a free FABLE_FORCEINLINE twin of SetPixelShaderConstant (d2 = control, reproduces v0 exactly):
  b2 named `C4DVector&` slot, b5 memcpy, b6 `PSConstTable[index]` + `UpdateFlags = UpdateFlags | 2`, b7 copy of value first: all
  identical to v0; b4 member-wise slot stores: 61 ops (dead temp stores gone, not retail); d1 flags first: T1 OK but 68 ops / edi.
- c2 edge creation (watchpoint on the Y store node's succ list, tools/sched/t17.py): the Y -> 19..53 edges are all made at
  c2.dll 0x107b41c8 under the ecx register descriptor (0x108a9864) path 0x107b46f5 / 0x107b47d6, i.e. register-tracking edges on
  ecx; Y (the only blend store whose value register is ecx) collects edges to the three table stores 21..23 (they read ecx as
  base) on top of the real WAR edges (19, 20, 28, 41, 45, 52, 53). Why 21..23 but not 24 is not decoded yet.
- Uncalled COMDATs after the sky TU (0x00B66B90 / 0x00B66BF0 counted-pointer releases, 0x00B66C10 GetTexture): no shader-constant
  helper copy exists; byte search: the only out-of-line `or [r+3D8h],2` is 0x0098B40F (the CCharString overload 0x0098B390), the
  split `or r,2` form occurs at 8 sites (0x00B65E64 this row, 0x00B707A4, 0x00B7C3F4/5B7/76C, 0x00B7F28A, 0x00B7FB33, 0x00BBAFE0).
- Same-shape sibling (UNLANDED, never attempted): CEngineWaterRenderer::RenderEnvironmentMap 0x00B70A90 copies the same two colours
  (`params.ThemeChannels[0].Colours[12]` / `[13]`, params reloaded from [esp+18h] into ecx, both `add eax,0C0h|0D0h ; mov edx,[eax]`)
  into adjacent locals [esp+48h] / [esp+58h] right after ApplyVertexShader and passes them to 0x00989B50 (index 0 / 1, `mov ebx,1`)
  -- a pair candidate: author it and compare its source shape (where the colour locals are declared relative to the pixel constants).

## Grades (pass 1)
| grade | source | result |
|---|---|---|
| g01 | prior differ/00b65a20.cpp, overlay hdr/ (copy of prior/2031/hdr) | DIFFER(1897v1893), 597/602 -- baseline reproduced |
Probe compiles on top (free, never diffed against retail): ~55 scheduler-DAG dumps (tools/try.sh: v0..v6, w1..w10, y1..y5, k1..k8,
r1..r5, ca1, n3..n7, d1, d2, b2..b7, e1..e3), 4 rename-walk dumps (tools/walk.sh / t18), 3 watchpoint runs (t17), 1 rotation trace.
No source variant improved on v0 by the measured verdicts, so no further grade was spent (every grade would have been v0 again or a
measured-worse shape).

## Residue (unchanged from c2031; retail addresses)
(a) 0x00B65E42 `mov [esp+28h],ecx` (blend Y dead store) after `mov eax,[GPShaderRenderManager]` in retail, ours right after X
    (DAG tie Y 80 / W 80, lower tuple index wins);
(b) 0x00B65E5A..0x00B65E67 flags RMW register retail ebx, ours esi (rename walk entry index 2, retail needs 4);
(c) 0x00B65E97 retail `add eax,0D0h ; mov ecx,[eax]`, ours `mov ecx,[eax+0D0h] ; add eax,0D0h` (soft add edge, load bonus).

## Ranked untried ideas
1. Decode c2's register-edge builder (0x107b418c called from 0x107b47d1 / 0x107b46f5) to learn why the Y store gets edges to table
   stores 21..23 (base-register readers of the NEW ecx): that rule tells which IL shape gives Y one successor less (retail T1).
2. Decode the soft-edge criterion (c24 increments in the DAG builder) for `add reg,imm` -> load: retail's colour-13 add is hard.
3. Author the sibling CEngineWaterRenderer::RenderEnvironmentMap 0x00B70A90 (same colour copies, same callee) and carry its matched
   statement shape back here.
4. Rename walk (T3) is a pure function of the visited tuples (cmp / push / add reg,imm / or / inc / dec) and their busy sets after
   the `or` in the region; once (1)/(2) give retail's IL it should follow; tools/walk.sh checks it for free.
5. Last resort per brief: (r3004)/(r3005) sweeps -- measured here as NO effect (k1..k8 enums, r1..r5 references).

## Names
No new names proven; all names and evidence as in prior/2031/NOTES.md + LABELS.tsv / NAME_FIXES.tsv (0x00989B50 CRGBFloatColour
overload, 0x006B7350 PeekInterpolatedParams, etc.). Descriptive: the local variable names (as c2031); diagnostic-only DiagSetPSC,
EDiagN / pdN (probe files, never delivered).

## Header changes
None beyond the c2031 overlay (hdr/ is an unchanged copy of prior/2031/hdr; hdr/README.md copied). FABLE_RETAIL_CARRAY is an
existing opt-in of the repo header rebuild/include/engine/CArray.h (default text unchanged, no overlay copy).

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Label corrections
None new (c2031 NAME_FIXES.tsv / LABELS.tsv stand: 0x00989B50 is the CRGBFloatColour overload).
