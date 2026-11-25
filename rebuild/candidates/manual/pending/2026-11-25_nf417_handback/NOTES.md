# NF417 hand-back (2026-11-25; tree as seen at the end of the run): stand-in statics (RULINGS r1), their forwarders, and the 188 expanded GetClassTypeName rows

## Result
- **195 rows in 7 batches. 195 ACCEPT, 0 HOLD.** One unit per row (UNITS.tsv). There are no RULINGS_NEEDED and no NAME_FIXES (every proven
  fix is done in the batches). No header edits and no FLAGS changes.
- Locks: all 195 are held as NF417. 0x00B1AE00's lock is released: that row is already real and needs no change.
- **The tree changed during the run.** The coordinator reported HEAD c3bec33437 after the container restart; I cannot see commits. What I
  observed on disk:
  - after my census: rebuild/include/stlport/fable_containers.h and tools/decomp_pipeline/crawl/purity.py (05:13 / 05:25);
  - rebuild/oracles/reloc_bindings.tsv and self_symbols.tsv (05:25, and reloc_bindings again at 06:20);
  - a new tools/decomp_pipeline/crawl/baked_symbols.py (06:22).
- **None of my sources changed.** No row, test or support source of this hand-back changed (binder, census emitters, the 154 static rows),
  every orig/ file equals the repo, and every SELF_SYMBOLS current_line equals the repo's self_symbols line. The census, worklist and
  static census, regenerated, are identical.
- **Everything was rerun after those changes:** binder, census emitters, all vland HEAD/new/mutant, gates, links, pk and apply_table
  (work/rerun417.out, work/rerun417b.out, then s1/c1 re-gated after 06:20).

| batch | rows | what | table |
|---|---|---|---|
| s1 | 5 | stand-in statics -> `?StatGetClassTypeName@<C>@@SI?AVCCharString@@XZ`, in the NF414 in-class form: 0x004D3EF9 CTCDNone, 0x004D41DF CTCDNavigationSeed, 0x0049D450 CTCTargetingPlayer (brief), 0x004D3DFC CTCDParticleEmitter (the third holder of `??0CThingComponentDefNameTag@@QAE@XZ`), 0x004D4DF4 CTCAssassinRush (`??0CTCAssassinRush@@QAE@XZ`, same family) | none |
| c1 | 3 | their forwarders 0x004D3F30 / 0x004D4212 / 0x004E230A -> `?GetClassTypeName@<C>@@UBE?AVCCharString@@XZ` (NF412 gen412 form). **Requires s1** (UNITS.tsv `requires`) | add CTCTargetingPlayer static -> 0049d450 |
| b1 | 50 | expanded-body rows 0x004C5D50 .. 0x007312C0 | delete `?kActionStr@@3PADA` |
| b2 | 50 | 0x007319D0 .. 0x00785CD0 | delete `?kActionNameLiteral@@3QBDB` |
| b3 | 50 | 0x007874A0 .. 0x007D4D70 | none |
| b4 | 28 | 0x007D5EF0 .. 0x007E7E80 | delete `_g_actionName` |
| p1 | 9 | 0x00AFE440 .. 0x00B1FF10, NParticleEngine::CPSC* rows (CPSCBase slot 3) | none |


c1/UNITS.tsv has a third column, `requires` (the s1 static each caller needs). Each batch holds: rows.txt, rows/, tests/, orig/rows + orig/tests (HEAD copies), RESULTS.tsv, CHECKS.tsv, UNITS.tsv, SELF_SYMBOLS.tsv (NF412
columns: address symbol status leaf_given current_line), TABLE_CHANGES.tsv, LITERALS.tsv, logs/ and evidence/. head/, new/ and mut/ hold the
lane payloads; their oracle.tsv contains retail bytes and must **not** go into the repo.

## Task 1: the stand-in statics (s1) and their callers (c1)

### s1 (tools/gen417s.py, gates: tools/s414/gates414.py)
- **The stand-ins.** The HEAD selves were constructor names for code that is no constructor: a local one-member class built from the
  literal. `??0CThingComponentDefNameTag@@QAE@XZ` sat on 0x004D3DFC, 0x004D3EF9 and 0x004D41DF, so one name was on three addresses. On the
  HEAD /FORCE link this gives 2 LNK4006 duplicates; the new link gives 0. Neither PDB name set nor RTTI has a CThingComponentDefNameTag
  class.
- **Census of all 313 landed static rows** (evidence/static_census.tsv). These 5 are the only static rows whose self is not
  `?StatGetClassTypeName@...`. The other 308 already use the in-class form with the scaffolding helper.
- **Names**, from two or more sources per row:
  - the FableWin.pdb name set (Ego_r has none of these);
  - the retail static shape `56 6A FF 68 imm32 8B F1 E8 rel32 8B C6 5E C3`, pushing the class name (batches/s1/LITERALS.tsv, read from the
    exe), with the call going to 0x0099EBF0;
  - the class's vftable slot 22: it calls the static for 0x004D3F30, 0x004D4212 and 0x004E230A, and expands it for 0x006E1D10 and
    0x007AB5B0;
  - the RegisterTCs 0x004EE23F calls: 0x004F1F6F, 0x004F27F5, 0x004F26DC and 0x004F5451.
- **0x0049D450 is a special case.** RegisterTCs does not call it. It expands it (`push 0x01238B98` at 0x004EEA3A), and so do 0x004A3E39,
  0x004A50F8 and 0x004ACBAB. The landed binder and 0x004A3740 (CWorld::UpdateRegionLoad, via the header's FORCENOTARGET in-class static)
  both emit the pick-any copy, and r10cmp is EQUAL against both.
- **Form.** CTCDNavigationSeed, CTCDParticleEmitter and CTCTargetingPlayer use the layout-rename over their engine header (default branch).
  CTCDNone and CTCAssassinRush have no header and use a local class. The section sets are equal across both forms.
- **Gates** (gates414 strict, CHECKS.tsv):
  - vland RELOCATION_MATCH PASS -> PASS. The HEAD STANDIN_LEAF warning is gone. The only new WARN is SHARED_HEADER_AVAILABLE, on the 3
    layout-rename rows.
  - literal_check ok. The leaf is 19 B and equal once the relocation fields are masked. The COMDAT selection goes 1 -> 2. The only added
    sections are the helper and its .debug$F; the literal was already spelled at HEAD.
  - r10cmp EQUAL against the fresh binder build. Mutant (literal changed): test FAIL plus LITERAL_DIFF. purity and the widened scan give [].
  - pk_all / pvr: 0 NEQ, 0 reach flips. The only copy is the helper (NOADDR).
- **Control** (evidence/link_control_oot.txt). 0x004D3EF9 written out of line (NoDuplicates) and linked against the binder goes from 0 to 1
  duplicate, which shows the gate is live.

### c1 (tools/gen412c.py, gates412c.py, mut412.py)
- **The HEAD callees were made-up names:** `?Init_004d3ef9@T@@QAEXXZ`, `?Init_004d41df@T@@QAEXXZ` and `?Init@T@@QAEXXZ`, read from the HEAD
  objects' relocations. No table line binds them.
- **Names.** Both PDB name sets spell all three, and each row is its class's slot 22.
- **Form.** 0x004D3F30 is a local class over CTCDriverBase.h. 0x004D4212 is a layout-rename over CTCDNavigationSeed.h. 0x004E230A is a
  layout-rename over CTCTargetingPlayer.h with the opt-in `FABLE_RETAIL_CTCTARGETINGPLAYER` (the default branch is the flat generated layout,
  and its polymorphic check fails); it gains `/DEFAULTLIB:"libcp"`.
- **Gates:** vland PASS -> PASS. The HEAD RELOC_NAME and GENERIC_STRUCT warnings are gone; the only new WARN is SHARED_HEADER_AVAILABLE on
  the two layout rows. objsec and relocs ok. pk shows 0 copies. Mutation (static called twice) FAILs on 3/3.
- **/FORCE link** with s1 + binder + 0x004A3740: duplicates 2 -> 0 (the 2 are the s1 constructor clash). The same holds without the binder.
  NEW_UNRESOLVED is 0 and every callee is DEFINED. c1 alone, with no s1 and no binder, leaves the 3 statics unresolved
  (link_c1only_nobinder.txt). **That is why c1 requires s1.**

## Task 2: the 188 expanded-body rows

### Census (tools/wl417.py -> evidence/worklist.tsv)
- All 188 have the retail shape `56 8B742408 6AFF 68<lit> 8BCE E8<0x0099EBF0> 8BC6 5E C20400`. The literal is .rdata, NUL-preceded,
  printable, and equals the class name (188/188).
- **178 are CTCBase classes (b1..b4):**
  - each row is its class's vftable slot 22;
  - both name sets spell `?GetClassTypeName@<C>@@UBE?AVCCharString@@XZ`;
  - FableWin spells `?StatGetClassTypeName@<C>@@SI...` for all 178 (Ego_r for none).
- **10 are NParticleEngine::CPSC*:**
  - vftable slot 3 = CPSCBase's pure `GetClassTypeName() const` (engine/CPSCBase.h);
  - both name sets spell the UBE virtual, and neither has a static;
  - 0x00B1AE00 CPSCLight is already real (NF385), so 9 rows are in p1.
- **No row is a real GetTypeAsString.** None of the 188 classes has a GetTypeAsString in either name set.
- **HEAD selves:** 148 `?GetClassTypeName@<C>_Methods@@QAE...`, 39 non-virtual `<C>::GetClassTypeName` QAE/QBE (one returned a made-up
  CWideString), and 0x00B1AE00. The decorated old selves appear nowhere else in rebuild/ except self_symbols.tsv (evidence/oldself_grep.txt).
  The undecorated `_Methods` classes are still used by other rows (T3 debt).

### Form (tools/gen417.py)
- **CTCBase rows:**
  - `static CCharString __fastcall StatGetClassTypeName() { return CCharString("<C>", -1); }` sits in the class body, and
    `virtual CCharString GetClassTypeName() const;` is defined out of class as `return StatGetClassTypeName();`.
  - VC7.1 /O2 inlines the static and produces retail's 25 bytes. Tested first in work/exp/t1.cpp; every row is then graded by vland.
  - VC7.1 also emits the static's pick-any COMDAT copy (19 B, selection 2).
  - No pragma and no Flags line, as at HEAD.
- **CPSC rows:** a direct `return CCharString("<C>", -1);`, the landed 0x00B1AE00 form, with the class over engine/CPSCBase.h, or a
  layout-rename where the CPSC header compiles.
- **Header choice, in order:**
  1. layout-rename over the class's own header;
  2. a local class over the nearest RTTI ancestor with a header;
  3. a local class directly over engine/CTCBase.h.
  - Each choice is tried with the prelude ladder P0..P3 (gen412's).
  - It is accepted only if the polymorphic check TU compiles: `static_cast<Base*>`, `&Base::GetClassTypeName`, and
    `sizeof(class) == sizeof(parent)` (evidence/chk/<a>_chk.cpp).
- **Opt-in retry.** One bounded pass (35 s per compile) covered every row whose own header failed P0..P3: 61 rows. Each was retried
  with the header's own opt-ins, i.e. every FABLE_RETAIL_* guard within 15 lines above a `class <C> : public` definition in it
  (tools/optin417.py, tools/optretry417.sh, evidence/header_optin_retry.tsv).
  - 26 now compile as layout-rename over their own header with those opt-ins.
  - 35 stay local. Each one's `// Header:` line names the opt-ins tried, or says that none guards a polymorphic definition. These are header
    debt, listed in T3.
- **What each batch ended up with** (work/gen17_<b>.tsv):
  - layout / local: b1 24 / 26, b2 30 / 20, b3 26 / 24, b4 15 / 13, p1 3 / 6 (98 layout-rename, 89 local).
  - 77 rows sit directly on engine/CTCBase.h: their RTTI direct base is CTCBase, or the fallback applied.
  - 14 rows have ancestor headers that do not reach the polymorphic CTCBase (CTCPhysicsBase.h, CTCAppearanceMorphBase.h and others), so
    they use the CTCBase.h fallback: 0x00722360 23420 24B80 25E40 270E0 276B0 284C0 0x0075BF20 0x007D2220 D4D70 D5EF0 D7980 D9EB0 DC970.
- **WARN SHARED_HEADER_AVAILABLE** shows up in two cases. On layout-rename rows it is the queued verifier false positive (T3 `nf402
  tools`). On the 35 local rows whose header exists it is real header debt, not a false positive.
- **Polymorphic check TU:** every one was recompiled on the current tree in gen17 (the generated rows). Negative control
  (evidence/polymorphic_control.txt): 0x007AB5B0's check without FABLE_RETAIL_CTCBASE fails (C2039). 0x004C5D50's header defines the macro
  itself, so that control is moot.
- **Tests (generated):**
  - each test links the row object (the virtual and the static's copy come from it) and defines a recording `CCharString(const char*, long)`;
  - it calls through a const reference and asserts one constructor call, the hidden slot == &result, length -1, and strcmp of the text;
  - the pass patterns are kept from the catalog.

### Gates (tools/gates417.py; strict; batch logs and CHECKS.tsv)
- **vland**, HEAD / new / mutant at one LANDVERIFY_WORK path per batch (lv/batches_<b>_h|_n|_m):
  - 187/187 PASS -> PASS with the same grade.
  - The new logs have no LITERAL_*, RELOC_NAME, STANDIN_LEAF or GENERIC_STRUCT. The only new WARN is SHARED_HEADER_AVAILABLE (header
    debt / the queued false positive), listed per row.
  - HEAD 0x007E2490's RELOC_NAME (the CWideString ctor) is gone.
- **literal_check:** ok on 187.
- **Leaf:** 25 B, relocations at exactly +8 (DIR32 to a ??_C@ whose bytes equal retail at the VA) and +15 (REL32 to
  `??0CCharString@@QAE@PBDJ@Z`, bound only to 0099ebf0). The leaf is equal once those fields are masked.
- **Sections:** the only added ones are the static's .text (checked to be the 19-byte static shape with the same literal and ctor
  relocations, selection 2), its .debug$F, and a ??_C@ where HEAD had none. `.drectve +/DEFAULTLIB:"libcp"` is recorded on 72 rows; it comes
  from STLport behind an engine header (NF412 precedent). Two GONE kinds are allowed, and each is recorded:
  - 0x007276B0's HEAD-defined stand-in body of `??0CCharString@@QAE@PBDJ@Z`. pk_all graded it NEQ against retail 0x0099EBF0 and it was
    reached; the new row calls the real one.
  - 0x007DF2C0's HEAD data array `_g_actionName`, which holds the same text.
- **Static copy, r10cmp EQUAL** against every other object that defines it: the binder; the class's landed static row (lv/batches_<b>_st,
  154 static rows built at HEAD); the s1 new objects; and the census emitters 0x004A3740, 0x00518DC0 and 0x0064DD9A.
- **pk_all / pvr** (evidence/pk_summary.txt):
  - HEAD: 1 reached NEQ (0x007276B0's fake ctor). New: 0 NEQ, 0 reached NEQ, 0 pvr NEQ.
  - Each CTCBase row has one copy, the static: EQ where retail has an out-of-line copy, NOADDR (unreached) where retail only expands it.
  - pkcmp346 GATE PASS and 0 reach flips in every batch.
- **Mutant** (literal's first character changed): test FAIL on 187/187, plus LITERAL_DIFF.
- **purity** `baked_string_addresses` (the current purity.py) [] and the widened scan [] on every row and test. No asm, naked, _emit or
  volatile.
- **/FORCE link** (link377.py), per batch, census-free as briefed, with and without the binder: the binder + the batch's landed static
  rows + s1 new + the rows.
  - Duplicates 0 -> 0, NEW_UNRESOLVED 0, and nothing unresolved in the new set.
  - All old selves become RESOLVED_TO_GONE, and the kActionStr and CWideString unresolveds disappear.
  - Extra run with the census emitters added: duplicates 2 -> 2. Both are the emitters' own CVectorMap::LowerBound pair; neither involves
    these rows.
  - All 195 rows together (evidence/link_all_*.txt, linkgate_all.txt): census-free 2 -> 0 with and without the binder (the 2 are the s1
    constructor-name clash); with the census 4 -> 2. NEW_UNRESOLVED 0.
- **NF418 baked_symbols** (source and object passes; evidence/baked_symbols.txt): 0 hits on the 195 new rows. The HEAD objects hit 3
  times, which are exactly the three TABLE_CHANGES deletes.
- **apply_table --dry:** per batch and all together, 1 add + 3 deletes, 0 misses, multi-target 1 -> 1, nothing new. The repo table's md5 is
  unchanged.
- **reserved_check** (RESERVED_REFS empty, so no git): reserved 0 and mentions 0 in every batch (logs/reserved_check.txt). apply_table dry logs are in logs/apply_table_dry.txt.

### Table (TABLE_CHANGES.tsv)
- The deletes come from the HEAD leaf relocations that the new object no longer references, where the line's witness is the row itself:
  `?kActionStr@@3PADA` 0123de20 (006285f0), `?kActionNameLiteral@@3QBDB` 0123b4b4 (00771100), `_g_actionName` 0123d724 (007df2c0).
- No other row or test spells those names, or `??0CWideString@@QAE@PBDH@Z`; that ctor spelling has no table line.
- The new statics' spellings need no line: the rows do not call them.

## Not done / out of scope (T3.txt)
- **13 more slot-22 GetClassTypeName rows** use the 23-byte /Os form (the 0x0066122E shape). They are landed as
  `?GetTypeAsString@<C>_Methods`. Both name sets spell GetClassTypeName for all 13 and no GetTypeAsString. The 340 CTCBase slot-22 functions
  break down as 149 forwarders + 178 + these 13.

## Process
- Pilot: 10 rows covering layout-rename, local, NGraphicAppearance, kActionStr, CWideString, no retail static copy, 0x006E1D10, ScriptedHook
  and CPSC went through every gate first. The pilot is kept in work/pilot_batch and is not handed back.
- **NOFORM rows, fixed in gen417:**
  - the CTCBase.h fallback, used by the 14 rows above;
  - the right-hand side of the namespaced size check, which affected 4 rows. After the fix those rows were regenerated and re-gated.
- **Gate allowances** (both recorded per row): the GONE of a HEAD fake ctor body, and the GONE of a HEAD data array with the same text.
- **Restart:** the container restart cut b4's mutant stage. Everything was rebuilt at the new HEAD.
- No git, no repo writes, no `wineserver -k`. Only my own processes were killed: my batch driver, once.
- **Tools** (tools/): wl417.py, gen417.py, gen417s.py, mkbatch17.py, gates417.py, coff417.py, st417.sh, linkgate417.sh, pk417.py,
  batch417b.sh, rerun417.sh, fin_sc.py, gen412c.py, gates412c.py, optin417.py, optretry417.sh; plus NF412 and NF414 copies repointed to agentNF417.
