# C2T2 LOG (2026-11-22)

Tools: the repo copy `tools/decomp_pipeline/linux_lane/lane_tools/c2probe/lk/` (run in place, `source env.sh` with S exported;
PYTHONDONTWRITEBYTECODE=1). Outputs only under `$S/w22/c2t2/`. Scripts of mine: `scripts/`.

## Step 0: control (final lka)
- `lka.py c2t1/logs/p8_all.json c2t1/logs/p8b.json --func UpdateShadowSpace` -> aligned 34291, unaligned 0, **FLIPS 0** (`out/lka_p8all_p8b.txt`).
  Self-compare p8b v p8b also 0. Sanity: p8b v p9 with the final lka reproduces C2T1's committed readout (FLIPS 132 sorts in 13 trees;
  identical text apart from the --shifts line) (`out/lka_p8b_p9.txt`). Control passes; work continues.

## Step 1: 0x00B4D370 p9 (no new run)
- Inputs: c2t1/logs/p9.json (= auth20 x/p9.cpp = best_evidence.cpp, cmp equal), listing auth20 x/p9.log split into
  `out/b4_retail.lst` / `out/b4_p9.lst`. Full comparator dump: `out/p9_show_all.txt` (lk.py show). Tree tracer: `scripts/tree.py`.
- Method: per residue site, find the commutative tree (final sort before codegen), read its operand order, then read the emitted
  order of the same operands in p9's listing by stack / memory operand, then retail's. Operand identities fixed by consistency
  across sibling trees (one bijection for all columns / rows), not by assumption; where a label is inferred it says so.
- Field-symbol model used for labels (INFERRED, supported): a scalar-replaced struct local keeps its own early record number for
  its offset-0 field; other fields get late numbers (allocated in pairs at 0x10769FE4, second of pair used). Support: L226 trees,
  new_shadow_transform at esp+0x34: s1166 = +0x00, then n = 1990 + (off-0x38)/2 for every one of the 11 other elements; SetInverse:
  site 141's incoming list (source order of GetDeterminant) pairs s602 (base) with the E11 cofactor, s1825 / s1826 = E21 / E31.
- Result table in REPORT.md. Every one of the 11 sum trees: tool order = emitted order (sum-operand order). No disagreement, so
  the wfA6g "tool says nothing" case did not arise at a sum level.
- ESCALATE discrepancy: AUTH20 writes "retail adds the Data[i][0]*m[0][j] product last"; the listing shows retail computes it FIRST,
  and only in column 0 (both expansions); columns 1..3 equal ours.

## Step 2: 0x00BD3070 c6
- Inputs copied: in/c6.cpp sha256 585d40f6... (= auth21 00bd3070.cpp = c6.cpp), in/hdr/ (12 headers, sha256 list in/hdr.sha256 = auth21 hdr/).
- Run r1: `lk.py run in/c6.cpp logs/c6.json --func RenderMeshPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase --ov in/hdr --syms
  --obj logs/c6.obj` (stock flags: none), started 08:16 UTC (first attempt at 08:15 died at once: S not exported).
- r1 done 08:48: 1967 s, events S 27475 / C 31296 / Y 33368, complete (exit seen, cl rc 0, PIPE, obj, no hook error).
  Gates: `lk.py check` 56925 operand keys ok, 0 wrong, 0 undecodable, 0 sort exceptions; objcheck logs/c6.obj v auth21 x/c6.log
  (base 00bd3070): 11288 B, 3174 instructions SAME (unmasked); fsolve zero-shift replay 0..27474 `-` -> F=0 holds (all 27475 sorts;
  flagged APPROX: some depth-4 trees keep logged hashes).
- Bookmark trees (`scripts/bm.py` -> out/c6_bm.txt): 39 SetRenderState expansions, each one AND (op 27d) + one OR (op 27e) tree,
  operands IND(temp => base + 0x2814) = C (CurrentBookmarkMask) and IND(temp => base + 8) = M (state.BookmarkMask); AND and OR
  always carry the same keys and order. Bases are kind-5 inline locals allocated before the first sort (0x1076F08C), so
  --late-after splits are empty here; the only split is by number (--min / --late-min = insertion points).
- Listing sites (`scripts/testor.py`): 38 `test r1,r2 ; jne ; or r2,r1 ; mov [st+8],r2`. Tree -> site map by render-state offset
  (lea [..+0x28xx]) and line: L3/L4 = SetSpecularEnable/SetFogEnable (sites 1-2); L25 = SetupAlphaBlendingStatesFromRenderFlags,
  16 trees in source order = sites 3-18 (states ZWrite 2854, AlphaBlend 28b8, AlphaTest 28cc, AlphaRef 28e0, ColourWrite 2840,
  SrcBlend 291c, DestBlend 2930 -- every lea matches the source order); L160 = Set2DAlphaMaskTextureMode(1) 19-23; L164 24;
  L175..L180 25-29; L187 30; L197..L202 31-35; L206 36; L216 37; L227 38. L165 SetFogEnable(false) has no own copy (3 SetFogEnable(false)
  calls, 2 sites with state 0x2944 after L4: one is cross-jumped); its tree is CM like L187's, so the reading does not depend on it.
  Table: out/c6_bm_map.tsv.
- Emitted v tool (convention: or destination = OR tree's first operand; test lists the AND's operands reversed): 30 of 38 agree,
  8 disagree (sites 1, 2, 10, 19, 34, 35, 36, 37). The opposite convention gives 8 of 38. No single convention fits all 38, so the
  test/or orientation is not a pure function of the comparator bit (register allocation also decides at those 8).
  Residue sites: 3, 11-15, 20-22 agree (tool addressable); 34 (0x00BD4C30) disagrees (tool says nothing there).
- Tier 2: x87 0x00BD32F4 = tree 17174 `s1191 (sphere.Radius, [ebp+28h]) * s1008 (scale, [ebp+30h])` (scale identified by tree 17259
  `.../distance * s1008` = the later `fmul [ebp+30h]`); tool Radius first, emitted `fld Radius ; fmul scale`: agree. SIB 0x00BD5712
  = tree 26727 `t10305 + s32 GPRenderManager`, tool IV first, emitted `[esi+ecx]` (esi = IV, ecx = GPRenderManager): agree; its
  sibling 26722 `t10322 + t10305` = `[eax+esi]` (same in retail): agree, and the shared temp t10305 = esi in both fixes the labels.
- psolve (scripts/psolve.py, fsolve's model per site, unlisted sites free; validated = fsolve on c2t1 p8b 32091=0231):
  out/ps_bm_strict_F.txt (min 0, F -2048..2047): 9 flips + 21 keeps -> EMPTY. Flips alone (9) -> EMPTY (p3 needs F 78..715,
  p20-22 need -1060..-425 / 988..1623). Spec variants all EMPTY: with tier 2, with the 7 disagreeing non-residue sites kept too.
  Single insertion point at each of the 79 base numbers (scripts/mscan.py, out/mscan1_strict.txt): EMPTY everywhere, best 52/60
  trees (26/30 sites). Two insertion points (N1 < N2 from 9 boundaries, F, T in -1024..1023): out/mscan2_strict.txt.
- At the best single shift (min 0, F = -180) 26 of 30 sites hold: p11-p15 flip as retail needs, every keep holds; p3 and p20-p22
  fail. p3 v p20-22 is a flip-v-flip conflict (no common F). Inside L25, p3 flip + p4 keep leave only F 713..715, while p11-15 flip +
  p16-18 keep need -180..-178 or 1868..1870: no common point.
- Step 3 not run: no uniform shift exists, so there is nothing to force. A staircase of 3+ insertion points might fit; that would be
  fitting record counts, which the ruling forbids. The two-point scan (out/mscan2_strict.txt, PID in out/mscan2.pid) was still
  running at handback.
- REPORT.md: the harness refused to write report files from this agent; the report went in the handback message.
- Two-point scan stopped after 9 of 36 (N1, N2) pairs with no error line (process gone; presumably killed): all 9 (N1=0 with N2 in {1328,1335,1570,2364,2475,3008,3328,3712}, and 1328/1335) have 0 solutions, best 52/60. The remaining 27 pairs were not scanned.
