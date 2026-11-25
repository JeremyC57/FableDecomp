# NF421 hand-back (2026-11-25): the 13 /Os GetClassTypeName rows, and the 35 header-debt rows onto their own headers

## Result
- **48 rows in 2 batches, 48 ACCEPT, 0 HOLD.** No header edits, no table changes, no NAME_FIXES, no RULINGS_NEEDED. One unit per row.
- **t1 needs its RENAMES.tsv applied** (catalog Module / Name / Source / TestSource; the 7 columns of the existing RENAMES.tsv format, then
  old_source / new_source / new_test_source). It passes "no new STANDIN_LEAF" only once the catalog name is GetClassTypeName.
- `batches/t1` (13): T3 `nf417 0x00486172 ... identity`. Selves respelt `?GetTypeAsString@<C>_Methods@@QAE...` ->
  `?GetClassTypeName@<C>@@UBE?AVCCharString@@XZ`, plus a catalog rename (RENAMES.tsv: Module / Name / Source / TestSource).
- `batches/p1` (35): T3 `nf417 headers debt`. The same text and self, moved from a local class onto the class's own engine header.
  Every header already had a polymorphic branch. The missing piece was the prelude (opt-ins), not a header fix.
- Locks: all 48 held as NF421 (ACCEPT). None released, since there are no HOLD rows.
- No git, no repo writes, no `wineserver -k`, no process killed.

## Batch t1: the 23-byte /Os GetClassTypeName rows

### Retail shape
`8b4c2404 6aff 68<lit> e8<0x0099EBF0> 8b442404 c20400` (tools/wl421.py, evidence/worklist_t1.tsv). That is
`mov ecx,[esp+4]; push -1; push lit; call; mov eax,[esp+4]; ret 4`. All 13 match: the literal is .rdata, NUL-preceded, and equals the class
name; the call goes to 0x0099EBF0; each row is its class's vftable slot 22. Both name sets spell the UBE virtual; FableWin also spells each
class's static.

### Form: the NF417 "virtual calls the in-class static" form does NOT give these bytes (measured)
batches/t1/evidence/os_inline_measure.txt, stock cl 13.10.3077, CTCVillage:
- `#pragma optimize("s",on)` at the top, or before the virtual only, gives `mov ecx,[esp+4]; call StatGetClassTypeName; mov eax,[esp+4];
  ret 4` (14 B). The static is not expanded.
- Command-line /O1, /O1 /Ob2 and /Og /Os /Oy /Ob1 give the same 14-byte call.
- /Os /Oy without /Og gives a frame (`push ebp`); /Os /Ob2 /Oy expands it, but inside a frame. Neither is retail.
- Only the direct `return CCharString("<C>", -1);` under `optimize("s")` gives retail's 23 bytes. So the rows spell the literal in the
  virtual, do not name the static, and emit no static copy. The HEAD pragma is kept, now placed after the comment block (line 1 is the
  usual `//` description line).
- This is one compiler measurement, so the row comments record the measurement and make no claim about Lionhead's source. A
  `__forceinline` static would also expand here, but RegisterTCs 0x004EE23F expands the static at its first 41 registrations and then
  calls the out-of-line copy (0x004D31C0's comment), which is inline-budget behaviour, not `__forceinline`. So a literal spelt in the
  virtual is the likelier retail form. T3 `nf421 GetClassTypeName form` records what this means for the 210 /O2 CTCBase rows
  (NF417 178 + p1 32). No ruling is requested: nothing here depends on it.

### Header: polymorphic proof for all 13
- First pass (gen421 = gen417's P0..P3 ladder): only 2 compiled on their own header. The other 11 fell back to local classes.
- `tools/psearch.py` then searched preludes: the header's own opt-ins (any distance above the class, not only 15 lines), and the
  preprocessor preludes of every landed row that includes the header, smallest first. The first prelude that compiles the check TU is
  then minimised line by line. Check TU: static_cast to CTCBase, `&CTCBase::GetClassTypeName`, sizeof(class) == sizeof(layout).
  One log per compile is in work/ps_t1*/<addr>/.
- Result: 11 rows are layout-renames over their own header (9 with a found prelude, 2 with the ladder).
  - 0x00789397 CTCProjectileTrap and 0x007BE9D5 NThingShot::CTCShotDefinitionQueenHornetShot have no own header. They are local
    classes over their RTTI direct base header, CTCTrapBase.h and NThingShot_CTCShotDefinitionSpreadShotProjectile.h. Each uses the
    prelude found for its sibling.
- The `// Header:` line names the prelude.

### Gates (tools/gates421.py; CHECKS.tsv / RESULTS.tsv)
- vland HEAD / new / mutant at one LANDVERIFY_WORK path (lv/batches_t1_one, renamed _h / _n / _m):
  - HEAD and new are both 13 RELOCATION_MATCH PASS.
  - New-only WARN: SHARED_HEADER_AVAILABLE on 10 layout rows. This is the queued false positive, T3 `nf402 tools`.
  - No LITERAL_*, RELOC_NAME, STANDIN_LEAF or GENERIC_STRUCT.
  - STANDIN_LEAF disappears once the payload and oracle carry the renamed catalog name. s414/mkpayload.py applies RENAMES.tsv, so the
    lander must apply RENAMES.tsv too.
- Mutant (first character of the literal changed): 13 FAIL, plus LITERAL_DIFF.
- literal_check ok on 13.
- Leaf: 23 B on both sides, relocations exactly +7 DIR32 (??_C@, bytes equal retail at the VA) and +12 REL32 (`??0CCharString@@QAE@PBDJ@Z`,
  bound only to 0099ebf0), equal once masked.
- Every HEAD section pairs EQ: the literal COMDAT is the same symbol, and so is the leaf's .debug$F.
  - The only change is `.drectve +/DEFAULTLIB:"libcp"` on the layout rows (the NF412 precedent).
  - **No function COMDAT besides the leaf** (no static copy). UNDEF is the ctor only.
- pk_all / pvr: 0 copies, 0 NEQ, 0 reach flips (pkcmp GATE PASS).
- purity and baked_symbols (source and object passes): [] on 13 rows and tests. No asm, naked, _emit or volatile.
- /FORCE link, HEAD v new (evidence/linkgate.txt), duplicates 0 -> 0 in all three runs; NEW_UNRESOLVED 0; RESOLVED_TO_GONE 13 (the old
  `_Methods` selves). The runs:
  - binder 0x004EE23F (fresh build) + the 7 landed statics of these classes;
  - the statics alone;
  - the rows alone.
- Census:
  - the new names occur in no repo file and no table line;
  - no landed source or header defines `<C>::GetClassTypeName` for these 13 classes;
  - the old selves appear only in self_symbols.tsv (evidence/name_census13.txt).
  - The `_Methods` classes are still used by other rows: CTCInGameMenu_Methods 0x0047D5F2, CTCInventoryExperience_Methods
    0x00572689 / 0x00576C0B / 0x00572672, CTCHeroStats_Methods 0x0057DE14, CTCVillage_Methods 0x00658561. That is the existing
    `nf417 _Methods` debt.
- apply_table --dry: no changes, 0 misses. reserved_check (RESERVED_REFS=""): 0.

### Catalog (RENAMES.tsv)
- Name changes from GetTypeAsString to GetClassTypeName, so the Source and TestSource files are renamed, e.g.
  `00/66/CTCVillage_GetClassTypeName_0066122e.cpp`.
- The two NThingShot rows also take the namespaced Module `NThingShot::CTCShotDefinition...`, with the file stem
  `NThingShotCTCShotDefinition..._GetClassTypeName_<a>` (the convention of the landed NGraphicAppearance / NParticleEngine rows).

## Batch p1: the 35 header-debt rows (no header edit needed)
- psearch found a compiling polymorphic prelude for all 35 own headers; work/port35/PORT.tsv lists each prelude.
- NF417's opt-in retry missed them for two reasons:
  - it looked only 15 lines above the class (CTCBoastUI.h's guard is 21 lines up);
  - many headers need a companion opt-in or an include. Examples: CTCFollowed needs FABLE_RETAIL_STLP_LIST for `list::size`;
    CTCQuickAccessMenu needs only `engine/CCountedPointer.h`; CTCBoastUI needs FABLE_RETAIL_CQUESTMANAGER.
- tools/port35.py keeps the HEAD row text verbatim (in-class static + virtual for the 32 CTCBase rows, direct return for the 3 CPSC rows).
  It changes only:
  - the include block (prelude + layout-rename);
  - the class base (`<C>Layout`);
  - the `// Header:` paragraph.
- Tests are the HEAD tests, unchanged (orig/tests == tests/).
- Gates (tools/gates421p.py = gates417 strict):
  - vland HEAD / new / mutant at one path: 35 RELOCATION_MATCH PASS -> PASS, and the mutant FAILs on 35.
  - WARN sets are unchanged. SHARED_HEADER_AVAILABLE was on 34 rows at HEAD and is still there, now as the queued layout-rename false
    positive.
  - Leaf 25 B equal masked. Every HEAD section pairs EQ, including the static's pick-any copy (19 B, selection 2). The only difference is
    `.drectve +libcp` on 24 rows.
  - The static copy is r10cmp EQUAL against the fresh binder and against the class's landed static row (30 built at HEAD).
  - pk_all / pvr: 0 NEQ, 0 reach flips.
  - /FORCE link with and without the binder: duplicates 0 -> 0, NEW_UNRESOLVED 0.
  - purity and baked_symbols: []. apply_table --dry: no change. reserved_check: 0.
- No header changed, so no includer recheck is needed. Every existing includer is untouched by construction.

## Rerun after tree drift
The tree changed during the run: the C3D* headers, reloc_bindings / self_symbols, naming_status.py and 278 landed sources. None of them
is one of the 48 rows: every orig/ file equals the repo, and every SELF_SYMBOLS current_line equals the repo's line. The following were
all rerun on the final tree, after the t1 comment edit: head / new / mut vland, gates, pk, links, baked_symbols, reserved_check and
apply_table --dry. The results are unchanged. The p1 check TUs are now the ported row text plus the check block (35 compile), and so
are t1's (13 compile).
- Includers: `recheck.py --with-includers` finds none for the 48.
- The old t1 file stems (`_GetTypeAsString_<a>`) appear only in rebuild/build_candidates.ps1, which the rename covers.
- No source or header defines GetClassTypeName in class for any of the 13 classes.

## Not done / caveats
- Census emitters: NF417's lv/census (0x004A3740, 0x00518DC0, 0x0064DD9A, the CTCActionUseScriptedHook includers) was not rebuilt.
  - The t1 rows emit no static copy.
  - The p1 static copies are section-equal to HEAD.
  - So no census relation can change. The "census" link runs in p1 logs equal the census-free runs.
- Strict headers: under LANDVERIFY_STRICT_HEADERS=1 the layout-rename rows would be rejected, like the nf402 / nf406 / nf417 siblings
  (the verifier's false positive, T3 `nf402 tools`).
- The minimised preludes are compile-minimal, not semantic. Some keep a companion opt-in only because the header needs it to compile,
  e.g. FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE for CTCInventoryTrade.h. The bytes are unaffected: every HEAD section is EQ.

## Tools (tools/)
- New: wl421.py, gen421.py, psearch.py, port35.py, mkbatch21.py, mkbatchp.py, gates421.py, gates421p.py.
- NF417 / NF414 copies, repointed to agentNF421: s414/runbatch.sh, s414/mkpayload.py (now applies RENAMES.tsv to the payload and oracle
  names), s414/binder414.sh, st417.sh, linkgate417.sh, pk417.py, link377.py.
- head/, new/ and mut/ oracle.tsv files carry retail bytes: do not commit them.
