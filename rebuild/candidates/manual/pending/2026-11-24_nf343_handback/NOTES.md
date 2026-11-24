# NF343 hand-back: pick-any debt copy groups (Account A naming queue)

Result: **no group landed**.

## Decisions needed (in order)
1. **0x00ED8680 CTB_Bard::Main (Account B's staged row, held for this group): HOLD or land with debt.** Its ~MovieBase copy is
   NEQ 25 and leaf-called (pk_all: sources a,b, leaf=y). The NF340 gate as written in BRIEF_NF_BATCH ("New row: any leaf-called
   NEQ copy holds it") makes that a HOLD. B's ESCALATE.md argues debt: the copy is byte-identical to the one landed 0x00D62480
   already emits, and the symbol is bound at HEAD. **No non-forcing fix exists** (group 2 below): both measured text fixes make the
   copy EQ but change B's leaf. "Held until the fix lands" therefore means held indefinitely unless the coordinator rules otherwise.
2. Group 1 (0x00CD3D2E) items (a) to (c) at the end of that section. Every group either needs a ruling (group 1) or a forcing device / leaf change (groups 2, 3),
or belongs to another lane (resize, flag-caused rows). No header or repo file was changed; no land_scripted run dir
(batches/ is empty on purpose). All numbers below are measured; files under evidence/.

Tools: tools/pk.py (compile one row at one work path as vland does, grade it, run both pick-any checkers ->
evidence/pk_<kind>_<addr>.txt), objsec.py (section-by-section HEAD v probe by symbol label), pkdiff.py, odis.py, nf.py.
Probe sources: evidence/probes/. Kinds: H = HEAD source + repo headers; P* = my probes; B/M/D = Account B's staged row.

## Group 1: 0x00CD3D2E map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[] (11 rows): DECISION NEEDED

HEAD: all 11 rows and the owner row are RELOCATION_MATCH; their copy is 141..315 bytes, NEQ, leaf-called (evidence/pk_H_*.txt).
Retail copy is 118 bytes: compiled for size (ebp frame), every callee called out of line (_M_lower_bound 0xCD28BC,
less 0x42AAB3, STB ctor 0x7E72A0, pair ctor 0xCD27C7, map::insert 0xCD3CAE, ~STB 0x7E74D0, ~CCharString 0x99EAE0).

Isolation probes on the owner row (p1a..p1e): optimize("s") alone gives the frame; shared headers reach 115v118. The only
difference left is STLport's `__default_constructed((T*)0)` against `T()`. `__default_constructed` is declared only under
FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ.

Design that reaches EQ (probe PG, evidence/probes/declF.txt + tailG.txt; rows 00ea59e0, 00d58490, 00e8ef80):
- defines FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE, CCHARSTRING_LESS_INLINE, CCHARSTRING_FORCEINLINE_LESS, STLP_LESS_CCHARSTRING_SIZE_BODY;
- after the last include: a guarded `__default_constructed` primary template plus a *declaration* of the operator[] specialization;
- at row end: an unreferenced anonymous-namespace inline that copy-constructs STB, then a `#pragma optimize("s",on)` region holding
  explicit specializations of the pair ctor, map::insert(it,v), `__default_constructed<STB>` and operator[], then `optimize("",on)`.
Results:
- 00ea59e0: leaf section identical (objsec_H_v_PG_00ea59e0.txt); operator[] EQ 118; pair ctor, ~pair, less and map::insert EQ;
  the STB copy ctor stays EQ.
- 00d58490: operator[] EQ, grade unchanged. The second map<CCharString,CCharString>::operator[] (0x9AC700) goes 159->150 but stays
  NEQ (it was already debt).
- 00e8ef80 (/d2inlT72): operator[] EQ 118, map::insert now EQ 0xCD3CAE, leaf section identical (objsec_H_v_PG_00e8ef80.txt).
  A T-threshold flag does not block the route.
- 00d13580 and 00d2d930 (both /d2inlS15): operator[] 145, still NEQ, leaf identical (objsec_H_v_PG_00d13580 / 00d2d930).
  /d2inlS15 acts inside the pragma region.
- Flags per row (from the pk_H_* headers): unflagged 00ea59e0 / 00d58490; T72 00e8ef80; S15 00d13580 00d19bd0 00d1a740 00d1b200
  00d22f30 00d2d930; T89690 00ce7670; T53 00d45060.
  Measured: 3 rows reach EQ (00ea59e0, 00d58490, 00e8ef80) and 2 S15 rows do not. Not probed: the other 4 S15 rows (likely the
  same as the 2 measured S15 rows), 00ce7670 (T89690) and 00d45060 (T53, its HEAD copy is a different 141-byte form).
Why it is a decision and not a landing (advisor ruling):
1. Several COMDATs change, not just the fixed copy: 6 tree helper COMDATs are dropped; `_Destroy<pair>` goes 9->80 and `??_G pair`
   30->99 (both NOADDR); tree/map lower_bound go 55->21 / 22->21 (NOADDR); a `__default_constructed` COMDAT is added. The
   rule's "only that one COMDAT changes" condition fails.
2. A new unreferenced ??M (operator< on CBasicString, 39 v 40, NEQ) comes from LESS_INLINE. That is the NF320 Q2 case.
3. The anonymous-namespace copy trigger is an unruled scaffolding device. Without it the STB copy ctor turns EQ->NEQ, 45->39
  (probe PF), which is a HOLD.
4. The size region and the specializations must sit at row end in every row, so this is tail text after the leaf. Defining them
  before the leaf changes the leaf (probes PA/PB DIFFER 6844v6780).
5. BOUNDS_OUTOFLINE and LESS_INLINE apply to the whole TU and change other trees' COMDATs (the CCharString map in 00d58490).
6. Table/name work would be needed. The 4-arg `map::insert` spelling has no reloc_bindings line, and the owner 0xCD3CAE leaf uses a
   3-arg local-model spelling. `_M_lower_bound` for this tree becomes a new UND (= the 0x00CD28BC leaf; no table line). The owner
   rows 0x00CD3D2E / 0x00CD3CAE use 3-arg stand-in spellings (NAME_FIXES candidates).
Alternative: the existing FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY (probe PD) removes the copy and leaves the leaf identical, but drops
23 COMDATs, some of them EQ.
Scope: census evidence/census_g1.tsv finds about 74 landed rows that likely emit this copy; only the 11 in the brief were audited.
Decisions needed: (a) accept multi-COMDAT collateral and the trigger device, or use DECL_ONLY; (b) table + NAME_FIXES adds;
(c) whether the S15 rows stay debt.

## Group 2: 0x006E7B80 ~CScriptGameResourceObjectMovieBase: SKIPPED
Rows: the six from DEBT_BY_COPY.tsv (00d58490 00de8370 00e2ff00 00e3bb70 00ed17d0 00efa4f0), plus landed 0x00D62480 (not on that
list, but the coordinator named it) and B's staged 0x00ED8680.

Cause: the row carries a local decl-only `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();`.
The implicit ~MovieBase copy then calls the PImp release 0x6E7AB0 (25 bytes) instead of expanding it as retail does (66 bytes).
evidence/census_g2_decl_only_rows.txt lists 134 landed sources with that declaration. 00de8370 and 00efa4f0 are flag-caused (NF344's
domain). The EQ rows 00e17840 and 00f00c90 do not have the declaration.
Measured fixes, both of which change the leaf:
- 00d58490, probe PM (inline declaration + header body at row end): ~MovieBase EQ 66, new ~CCountedPointer<MovieBase> EQ 53 at
  0x6E7AB0, but the leaf DIFFERs 9580v9560 (+20; evidence/pk_PM_00d58490.txt). This leaf change was not examined. The -16
  explanation below was shown only on 0x00ED8680.
- 00e2ff00, 00e3bb70, 00ed17d0: not compiled. They carry the same declaration; NEQ 25 is DEBT_BY_COPY's figure.
- **0x00ED8680 CTB_Bard::Main (Account B's staged row, compiled read-only from pending/2026-11-24_acctB_b24 with its overlay
  hdr/00ed8680, PK_LEN=3652, lock NF343).** The rows/00ed8680.cpp I compiled is byte-identical to payload.json's `source_cpp`
  (13302 chars, checked). payload.json carries no headers; B's STATUS records the hdr/00ed8680 overlay with 0 stale headers.** Prechecks: clang PASS (evidence/pre_ed8680_clang.txt) and cl /Zs ZS_PASS
  (work/pre_zs.log, tag b24_ed8680).
  - B as staged: RELOCATION_MATCH; ~MovieBase copy NEQ 25, leaf-called (evidence/pk_B_00ed8680.txt). It is the same debt as landed
    0x00D62480 (pk_H_00d62480.txt: RELOCATION_MATCH, 006e7b80 NEQ 25).
  - M (inline declaration + body at row end) and D (declaration removed): ~MovieBase EQ 66 and 0x6E7AB0 EQ 53 in both, but the
    leaf DIFFERs 3636v3652 in both, and M and D produce identical leaves (evidence/pk_M_00ed8680.txt, pk_D_00ed8680.txt).
  - B's ESCALATE.md also measured FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR: 3636, DIFFER (the same length
    as M and D).
  - Mechanism (evidence/ed8680_leaf_B_v_D_calls.txt): retail's leaf expands ~MovieBase at +0x52F (calls the PImp release
    0x6E7AB0, then ~CBaseObject) while retail's ~MovieBase COMDAT expands the PImp release. When the PImp body is visible anywhere
    in the TU, the leaf calls ~MovieBase there instead (-16 bytes). No text found that gives both results without a forcing
    device.
  - Hold or debt: not mine to rule. See "Decisions needed" item 1. The NEQ 25 form is the one landed 0x00D62480 emits, and 134
    landed sources carry the declaration that causes it. The written NEQ gate for new rows says HOLD. B's grade is unchanged.

## Group 3: 0x0040FE60 CPersistTraits<long>::TransferBinaryIn: SKIPPED (near-miss)

The EQ row 00632830 uses LONG_BINARY_BODY + /d2inlT1. The NEQ rows 00463d18 and 004e9393 have a local strong specialization inside
the row's size region (99 bytes); 00472896 uses BINARYIN_BODY.
Probe PO on 00463d18 (O2 around the local specialization): the leaf is identical (objsec_H_v_PO_00463d18.txt) and the copy reaches
129 bytes (the retail size), but it is still NEQ. Only two jump displacements differ (jb/ja at +0x14/+0x22): the two failure tails
are ordered differently from retail. Most likely source: the local model's Read range check
`if (newPosition < 0 || newPosition > 0x7fffffff)` against the header CDataInputStream / CMemoryDataInputStream Read. Not closed.
The copy kind is "strong", not pick-any.

## Other groups
- 0x00AFE910 resize: every emitter is /d2inl-caused, which is NF344's lane. Skipped.
- 0x00B439C0 CEngineMap::PeekLandscapeHeight: the EQ row 00b41770 (T400) uses CENGINEWORLDMAP_GETMAP_ORIGINAL_PASS26 without
  PEEK_BODIES; the NEQ rows use PEEK_BODIES + GPENGINEWORLDMAP. 00b41770 is NEQ without its flag, so this is flag-sensitive.
  Lead only; not compiled.
- CCountedPointer<NUISystem::CComponent> family and 0x005949C3: not reached.

## Rule slips (self-reported)
- Prechecks (clang, /Zs) for group 1 ran before locking. I locked, then reran them (pre.log / pre_zs.log show the post-lock runs).
- 00d62480 got only the cl /Zs precheck (no clang). The probe texts ed8680_M/D were not prechecked on their own: their base
  (B's row) passed clang and /Zs, and each differs by a few lines. The d2d9_G / e8ef_G probes passed /Zs before lock and compile.
- One `rm -f pids_zs.txt` on a literal path inside my folder, and one no-op `sleep 0`. clean.py was used everywhere else.
