# NF414 hand-back (2026-11-25): the 25 void*-cast baked strings purity missed, and the StatGetClassTypeName re-form of 27 rows

## Result
- **28 rows, 28 ACCEPT, 0 HOLD.** All 28 locks kept as NF414 (none released). No RULINGS_NEEDED.
- The worklist is NF413's typedef_cast_scan_classified.tsv `STRING?` rows (25) plus the T3 `nf413 0x004D4369 self` trio (3).

| batch | rows | what |
|---|---|---|
| b1 | 0x00452B4D | literal only: `SomeGlobalFunc((const void*)0x122d70e)` -> `SomeGlobalFunc("")`; test made linked |
| b2 | 0x004D2A91, 0x004D2B04..0x004D2D13 (24) | literal + self respell + callee respell + in-class pick-any re-form + `// Leaf:`; linked tests |
| b3 | 0x004D4369, 0x004D5FF2, 0x004D66C3 | self respell + callee respell + in-class re-form + `// Leaf:` (literal already spelled by NF413) |

- One unit per row (UNITS.tsv); any batch can land alone. Each batch: rows.txt, rows/, tests/, orig/rows + orig/tests (HEAD copies;
  byte-equal to the repo at hand-back), LITERALS.tsv, RESULTS.tsv, CHECKS.tsv, UNITS.tsv, SELF_SYMBOLS.tsv, TABLE_CHANGES.tsv, logs/,
  evidence/, head/ new/ mut/ (payload.json + oracle.tsv: lane input, oracle holds retail bytes, NOT for the repo).
- Top-level LITERALS.tsv: all 28 rows (VA, width, spelling, retail bytes to the terminator, preceding byte, old form).

## Step 1: the literals (tools/lit414.py -> evidence/lit_all.tsv)
For each row, read from Fable.exe: the cast VA lies in .rdata; the byte before is NUL (string start); the bytes up to the NUL are printable
narrow ASCII (no wide string); for the 27 StatGetClassTypeName rows the retail entry is `56 6A FF 68 <imm32> 8B F1 E8 <rel32> 8B C6 5E C3`,
imm32 == the cast VA, the call goes to 0x0099EBF0, and the text equals the class name exactly. 28/28 OK; nothing needs escaping.
- 0x00452B4D: VA 0x0122D70E, a lone NUL = retail's pooled `""` (also NF388 / NF413 b2). A lone NUL preceded by a NUL cannot by itself tell
  an empty string from padding; the evidence is (a) vland's literal_check pairs the new `??_C@_00CNPNBAHC@?$AA@` relocation with
  0x0122D70E as equal (gates g2 `ok`), (b) NF413 b2 proved the same VA as the pooled "" for six data symbols, (c) the callee 0x00404500
  is also bound as `?CheckCRC@CPersistContext@@IAEXPBD@Z` (a `const char*` parameter; 0x00431020 / 0x004E59E1 spell `CheckCRC("")`).
  Only the argument changed; `SomeGlobalFunc(const void*)` and every name stay as at HEAD (the mis-model is T3).

## Step 2: the re-form (tools/gen414.py)
None of the 27 was in class, none had a `// Leaf:`, none had its real name: the 24 were `void* __fastcall <Class>_StatGetClassTypeName(GAN<addr>*)`
free stand-ins calling `?m@GAN<addr>@@QAEPAXPAXH@Z`; the 3 CTCD rows were `?FableStatGetClassTypeName_5@@YI...`, `?StatGetClassTypeName_8@CCharString@@QAE...`,
`@StatGetClassTypeName_9@4` calling `?Init@CCharString`, `?ctorB@CCharString`, `?Do@Helper`. Each row now has the NF406 0x004D31C0 form:
- `// Leaf: ?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ` (confirmed defined in each new object, objdump -t; COMDAT selection 1 -> 2);
- `#define FABLE_RETAIL_CCHARSTRING` + `fable_string.h` (the PDB ctor `??0CCharString@@QAE@PBDJ@Z`, 0x0099EBF0, no local CCharString);
- `static CCharString __fastcall StatGetClassTypeName() { return CCharString("<Class>", -1); }` in the class body + the anonymous-namespace
  `ReferenceStatGetClassTypeName` helper (`Fidelity: scaffolding, not retail`);
- a header comment: Retail bytes, Identity (two+ sources), Literal, Respell (old self + old callee), Callee, Header, Form.

### Names (each from >= 2 sources; evidence/xref.tsv, lit_all.tsv)
- FableWin.pdb name set has `?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ` for all 27 (Ego_r name set has none of them);
- the retail literal pushed is the class name (27/27);
- the class's RTTI vftable slot 22 (GetClassTypeName) calls the row's address for 25 of 27 (CTCActionUseBuyOrSteal 0x0072A710 and
  CTCDExperienceOrb 0x0077AB70 expand it instead);
- RegisterTCs 0x004EE23F calls 18 of them (0x004EF826 on) and reloc_bindings.tsv already binds those 18 real spellings (witness 004ee23f);
  the other 9 (0x004D2A91 2B04 2B7A 2B8D 2BA0 2BB3 2BC6 2BD9 2BEC) are among RegisterTCs' first 41 expanded registrations.
- Class keys: ghidra_out/pdb_class_keys.tsv has 8 of the 27 classes, all `V`; 19 have no entry. All 27 are written `class` (the binder's
  key; RTTI .?AV for every class with a vftable). The static's mangled name carries only CCharString's key, and every leaf is confirmed by
  objdump -t.

### Header form per row (working choice, on evidence: layout-rename only where the default branch compiles and adds no section)
- layout-rename (`#define X XLayout` / include / `#undef`): CTCTargeted (0x004D2A91), CTCRotate (0x004D2B17). Their new objects have the
  same section set as the local-form rows (leaf, literal, helper, .debug$F x2).
- local registration-interface class (the binder's own form), header debt:
  - CTCActionUseWindow (0x004D2C42), CTCDExperienceOrb (0x004D4369): their headers include engine/CDefPointer.h, which `#error`s without
    FABLE_RETAIL_DEF_CLASS_BASE. The pilot compiled 0x004D4369 in layout-rename form and Wine cl hung (GOTCHAS NF402); I killed my own cl /
    vland PIDs, nothing else. A g++ -fsyntax-only probe confirmed the `#error` for both headers.
  - CTCActionUseReadable (0x004D2B8D), CTCActionUseBed (0x004D2C7B), CTCDExplosiveTrail (0x004D66C3): the header declares the class only
    under an opt-in (PASS24 / PASS28 / PASS28); no default branch.
  - the other 20 classes have no engine header.
- WARN SHARED_HEADER_AVAILABLE appears on those 7 rows (2 layout-rename: the known `_DEF_RE` false positive NF406 recorded; 5 header debt).
  It is the only WARN any row gains; every row loses its HEAD RELOC_NAME, and the 3 CTCD rows also lose STANDIN_LEAF.

## Gates (tools/gates414.py for b2/b3, NF413 tools/gates.py for b1; strict, anything else HOLDs)
- **vland** HEAD / new / mutant at one LANDVERIFY_WORK path per batch (lv/batches_<b>_one renamed _h/_n/_m): 28/28 RELOCATION_MATCH PASS ->
  RELOCATION_MATCH PASS; new logs 0 LITERAL_DIFF / LITERAL_UNCHECKED / LITERAL_CHECK_ERROR; reloc_identity.literal_check on each new object
  returns the row's VA equal (positive proof). WARN sets per row in CHECKS.tsv (warn_head / warn_new) and evidence/leaf_<a>.txt.
- **Sections** (b2/b3, evidence/leaf_<a>.txt): the HEAD leaf (old self) pairs with the new leaf (`// Leaf:`): 19 B both sides, bytes equal
  with +4 and +11 masked; +4 HEAD = baked VA with no relocation (b2) or the same `??_C@` (b3), new = DIR32 to a `??_C@` whose bytes equal
  retail at the VA, terminator included, addend 0; +11 REL32 old stand-in callee -> `??0CCharString@@QAE@PBDJ@Z`, both bound to 0099ebf0;
  no other relocation; selection 1 -> 2. `.drectve` and the leaf's `.debug$F` are EQ; the only added sections are the `??_C@` literal (b2),
  the helper and the helper's `.debug$F`; nothing GONE. b1 (NF413 gates): the only new section is `??_C@_00CNPNBAHC@?$AA@`, the leaf is
  equal once the literal field is masked (HEAD held 0x0122D70E there).
- **purity / widened scan** (evidence/purity_widened.tsv): `baked_string_addresses` [] and the widened scan (comment- and literal-stripped
  code, every 0x constant that is a retail .rdata address, any cast) [] on all 28 rows and 28 tests. Control: the same scan finds the baked
  VA in 50 of the 56 HEAD files (all 25 rows and their 25 self-contained tests; the 6 b3 files were already spelled):
  evidence/purity_widened_HEAD_control.tsv. No `__asm` / `naked` / `_emit` / `volatile`.
- **Tests assert the text**: every new test links the row object (static / function declared, never defined) and checks the text with strcmp
  (`""` for 0x00452B4D: `s[0] == '\0'`), length -1, one constructor call, the hidden result slot; pass patterns kept from the catalog.
  Mutant run (row literal's first character changed, new test): 28/28 FAIL, and each mutant object raises WARN LITERAL_DIFF.
  HEAD's 25 tests were self-contained copies that compared the address (`gStr != (void*)0x...`), never the text.
- **r10cmp v the binder** (fresh HEAD build of 0x004EE23F in lv/binder; source md5 unchanged at hand-back): 27/27 EQUAL (byte- and
  reloc-identical pick-any copy, R1 tier 1), including the 9 RegisterTCs expands (the binder object still emits their pick-any copies).
  Control: the mutant objects give DIFF (evidence/r10_mutant_control.txt).
- **Other landed copies**: three landed rows emit pick-any copies of two of these statics from engine headers (0x00839940 and 0x00808E40:
  CTCActionUseWindow; 0x00802E00: CTCActionUseReadable); r10cmp: all EQUAL to the row copies (evidence/census_copies.txt).
- **/FORCE link** (link377.py, response file; binder + those 3 census objects + batch, HEAD v new; evidence/link_<b>.txt):
  b1 0 -> 0, b2 0 -> 0, b3 0 -> 0, all 28 together 0 -> 0 duplicates; NEW_UNRESOLVED none. RESOLVED_TO_GONE = only the 27 old stand-in
  selves (no landed object or table line references them: fixed-string census of rebuild/src, tests, include, integration, oracles);
  GONE_UNRESOLVED = only the 27 old stand-in callee spellings. **Control** (evidence/link_control_oot.txt): 0x004D2C55 in the out-of-line
  (NoDuplicates) form links 0 -> 1 duplicate (LNK4006) against the binder, so the gate is live.
- **Tables**: SELF_SYMBOLS.tsv b2 24 + b3 3 lines (old -> new; all 27 old spellings are in rebuild/oracles/self_symbols.tsv);
  TABLE_CHANGES.tsv b2 24 + b3 3 deletes of the stand-in callee lines at 0099ebf0 (each line's only witness is its row; source census: no
  other row spells them); apply_table.py --dry: b2 {'delete': 24} / b3 {'delete': 3}, misses 0, multi-target 1 -> 1, new [] (the repo table
  md5 unchanged by the dry runs). b1 needs no table or self change.
- **reserved_check** (RESERVED_REFS="", agent lanes run no git: rerun with the default refs at landing): reserved 0, mentions 0, all batches.
- **HEAD drift**: every orig/ row and test is byte-equal to the repo at hand-back; the binder source md5 is unchanged; `find -newer` of the
  marker written before the first build (evidence/binder_src.md5) finds nothing in rebuild/include, rebuild/oracles, rebuild/src/compiled/00/4d
  and 00/45, or tools/ (so no __pycache__ write either).

## Notes for the coordinator
- The catalog keeps leaf `StatGetClassTypeName` / module = class for the 27; set the leaf from the `// Leaf:` lines at landing (payload
  `leaf` was taken from them in the new runs). 0x00452B4D keeps its catalog name `PersistTransfer_00452b4d` and leaf `ChangeState`.
- After landing, the 25 rows that cast through void* leave the count NF413's scan found; the dashboard's `baked_string` grade does not see
  them (purity gap, NF413 RULINGS q1), so the count will not move: regenerate, do not hand-edit.
- Pilot: 5 rows (0x004D2A91 2B17 2C55 2C7B 4369) ran first; the 4369 layout-rename hang moved CTCDExperienceOrb and CTCActionUseWindow to
  the local form before the batch runs (pilot logs in pilot/p1, not used for any verdict).

## Budget / hygiene
- vland runs: HEAD x3 batches + census + binder; new + mutant x3 batches; pilot (killed mid-run); one control compile. No git, no
  `wineserver -k`, no repo writes (every python run with -B; the apply_table runs were --dry; reloc_bindings.tsv md5 checked before/after).
- Tools (tools/): lit414.py, xref414.py, gen414.py, gates414.py, binder414.sh, linkgate414.sh (new); runbatch.sh, mkpayload.py, gates.py
  (NF413), r10cmp.py, link377.py, nf.py, nf387.py, objsec356.py, cmpcomdat.py (NF411/NF408), repointed to agentNF414.
