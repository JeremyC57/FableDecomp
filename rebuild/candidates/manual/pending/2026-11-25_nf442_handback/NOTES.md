# NF442 hand-back (2026-11-25): T1_A caller rebinds, continued from NF438

## Result
- **98 rows ACCEPT** in 7 batches, 104 callee rebinds onto 43 distinct T1_A targets. No header changes (HEADERS.tsv is empty everywhere).
- **Land in this order:** p1, b1, q2b, b2, b3, q2c, b4, then apply every CROSSBATCH.tsv, then `STALE_TABLE.tsv` (table-only; see below).
- **tplalt_hold/** has 29 more rows. They are ready (every gate passed) but held for ruling q442a. If it is granted, land them as a batch after b4.
- **Table (batches + CROSSBATCH + STALE):**
  - 96 delete (44 batch + 52 stale), 10 rewitness, 2 add;
  - apply_table --dry in that order: 0 misses, no new multi-target symbol (1 before, 1 after);
  - `evidence/ALL_TABLE.tsv` holds the combined file; `evidence/ALL_TABLE_with_tplalt.tsv` adds tplalt_hold (also 0 misses).
- **Targets:** 88 T1_A targets touched; 47 close fully (`evidence/targets_closed_full.txt`), 34 partly. With tplalt_hold: 64 full, 35 partly.

| batch | rows | rebinds | targets | closed | TABLE_CHANGES | CROSSBATCH | rulings | what |
|---|---:|---:|---:|---:|---|---:|---|---|
| p1 | 7 | 8 | 7 | 1 | 8 delete | 0 | r3, q2 | NF438's test-LINK_FAIL holds; test stubs respelt |
| b1 | 46 | 50 | 23 | 13 | 26 delete, 1 rewitness | 5 | r3, q1 | q1 protected / private rows plus newly unblocked member rows |
| q2b | 2 | 2 | 2 | 0 | 1 delete | 1 | r3, q1, q2 | b1 rows whose tests needed a stub respell |
| b2 | 6 | 6 | 3 | 2 | 2 delete | 0 | r3, q1 | member rows (holds the whole GetAnimFromName unit) |
| b3 | 1 | 2 | 2 | 2 | 2 delete | 0 | r2, r3 | DTOR mode (stand-in -> qualified destructor call) |
| q2c | 1 | 1 | 1 | 1 | 1 delete | 0 | r2, r3, q2 | DTOR row whose test needed a stub respell |
| b4 | 35 | 35 | 7 | 2 | 4 delete, 2 add | 3 | r3, q1 | reference-type adaptation, FREE prototypes |

- **Deletes** name in column 4 the batch row that removes the last binding.
- **Rewitness lines** whose new witness is outside the batch (all of them here) are in that batch's `CROSSBATCH.tsv`. No CROSSBATCH key (symbol, target) is touched by a later batch.
- **Units:** UNITS.tsv was measured over all eight folders together, and no unit crosses a batch. The one span found was the old `GetAnimFromName` spelling, shared by b1's 0x008D70A0 and three b2 rows. That row was moved into b2 and both batches were re-gated.

## Gates (every ACCEPT row)
- **vland** HEAD and new at one LANDVERIFY_WORK path: the grade is unchanged (RELOCATION_MATCH), tests PASS, 0 LITERAL. The only new WARN kind is SHARED_HEADER_AVAILABLE (r3 header debt, T3.txt).
- **objgate:** ALL_EQ for all 98. The UND delta is exactly old -> new, DEF sets are equal, `.drectve` is equal.
- **name_gate --test:** NO_WORSE for all 98. On q2 rows the new side runs the respelt test.
- **pk:** 0 new NEQ.
- **/FORCE link with the definers** (target self rows compiled from HEAD): duplicates never go up, NEW_UNRESOLVED is 0, every new name is DEFINED, and the control fires (HEAD unresolved stand-ins > 0).
  - b1 keeps `?GetPlayer@CPlayerManager@@QAEPAVCPlayer@@J@Z`, still bound by 3 b1 rows through their own model.
  - Definer 0x00859EF0 fails the clang precheck but compiles under VC7.1, so predef.sh now always compiles.
- **Names:** all 104 new names (and the 29 in tplalt_hold) are in both the Ego_r and the FableWin PDB name sets, and each is DEF in its target's HEAD self object.
  - The final re-check found 6 DTOR rows whose name is FableWin-only. They were moved to b3_hold.
  - Re-screened at hand-back against the current descriptive registry (184 lines), the nf441 GameCODA ctors and NF444's rows: 0 clashes, so REGISTRY_CORRECTIONS.tsv is empty.
- **xrcheck:** every renamed callee resolves to its target in the retail bytes. xr2_438 re-proved 0064499c, 00665510 and the 3 TransferVectorOfSubComponentsLoadText rows in b4, where nf.py picks the wrong leaf.
- **Delete cross-check** (`tools/delcheck.py`): every landed row outside the batches that names all identifiers of a deleted spelling and references its target was checked by libclang. 0 suspects (tplalt included).
- **reserved_check** (RESERVED_REFS=""): 0 for every batch, rows and targets.
- **Screens:**
  - all 98 rows are locked NF442;
  - no newest claim is Account B (0x00859EF0's newest line is Account A's release; it is a target, not edited);
  - T3 col 2/3 rows were skipped, and none is in T1_B;
  - the generator excludes descriptive-registry targets, the nf441 ctors and 0x009BEF80.
- **orig/:** re-diffed against the repo last: 0 drift (rows and tests). **Mutation control:** callee-only, leaf bytes equal to HEAD, so NF419 r1 applies.
- **SELF_SYMBOLS.tsv** (`address, symbol, status, leaf_given, current_line`). A row with a committed self_symbols line carries that line unchanged (the graded status equals the committed one for every row).
  - 19 ACCEPT rows have no committed line yet. Their new line has symbol = leaf_given = the row's `// Leaf:`, status = the batch grade, and an empty current_line. vland raised no leaf-mismatch warning for any of them.
  - Those 19 rows: 00664a50, 006a23e0, 00836260, 0083cc10 (b1); 0085baf0 (q2b); 008d70a0 (b2); 00454b4c (b3); 005c2a8e (q2c); 00855500, 008c2810, 008cf610, 008cf700, 008cf7d0, 00d1cac0, 00d246c0, 00ddd030, 00def760, 00e292c0, 00ee2db0 (b4).

## Rulings used
- **q2** (p1, q2b, q2c: 10 rows). The test's stub of the callee is respelt to the PDB name over a test-side bodiless declaration, callee only. Each test carries a `// Naming (NF442, ruling q2)` line, and RESULTS says "test respelt with the row".
  - 3 rows stay held: the respelt test adds a test-side SHARED_HEADER_AVAILABLE, so name_gate gets worse by 1. They are 0089a070 and 0089a0e0 (CTCHeroStats::SetMorality) and 00497f80 (private CProgressDisplay::WordWrapText).
- **q1.** The bodiless declaration declares the PDB member under its own access (IAE / AAE / IBE ...) and adds a friend line:
  - `friend class/struct [::ns::]C;` plus a forward declaration of C, for a caller method;
  - `[template <..>] friend <head>;` plus a forward declaration when it is not declared earlier, for a free function or function template;
  - no friend when the caller is the class itself.
- **r2** (b3, q2c). A stand-in that runs a destructor becomes `reinterpret_cast<C*>(x)->C::~C()` on `class C { public: [virtual] ~C(); };`. The stand-in may be a free `X_dtor(T*)` or a member `X::dtor()`. Rows that model or name C are residue.

## Holds (released)
| folder | rows | why |
|---|---:|---|
| `tplalt_hold/` | 29 | Member-template callees (GetSubDef<T>, GetComponent<T>, GFSerialiseMap*). Gates pass, except that `.drectve` gains VC7.1's own `/alternatename:<??$ new>=<old mangling>` token. Ruling q442a; the landed 0x006CA4E0 carries the same token. |
| `b3_hold/` | 8 | 6 DTOR rows whose new name is FableWin.pdb-only (one PDB source). 2 rows whose two targets 0x004E0D7E / 0x004E424B both carry `??1CPatchTesselationEdgeStrip@@UAE@XZ` (a new multi-target symbol). |
| `p1_hold/`, `q2b_hold/` | 2 + 1 | q2 test-side header debt makes name_gate worse; each needs its method in the engine header. |

## STALE_TABLE.tsv (52 table-only deletes; outside the batch layout)
- NF438's 255 "no caller found" lines were re-censused with compiled objects (`tools/census442.py`, after NF439's census439).
- Every candidate row was compiled from HEAD with VC7.1: the table witness, plus every landed row that names all identifiers of the spelling and whose retail bytes reference the target.
- For these 52 lines, no candidate object has UND or DEF of the spelling (e.g. `_deflate`: the witness 0x00A3A7F0 now binds `_z_deflate`). They close 27 targets outright.
- Column 4 is the current table witness. No row lands with these lines, so the lander will not apply them on its own; apply them with apply_table.py by hand, after the batches.
- **Excluded:**
  - 1 `?A0x` (path-hashed anonymous namespace) line, which an object compiled at another path cannot measure;
  - 202 lines whose objects do bind the spelling through code libclang does not see. They are in RESIDUE as OBJCENSUS lines.

## Generator (tools/rebind442.py: NF438's rebind438 plus the following)
- **Access:** q1 friend lines, as above.
- **Member templates:** a generic `template <class T1..> RET Name(PARAMS with T substituted) [const];` declaration, with the call written `->Name<T>(...)`.
- **DTOR mode**, as above.
- **Argument and return casts:**
  - reference to pointer: `reinterpret_cast<T*>(&(x))`;
  - reference to reference: `*reinterpret_cast<T*>(&(x))`;
  - pointer-to-pointer: a C-style cast;
  - `const_cast` of a `reinterpret_cast` wherever the PDB type is const and the old type is not.
- **Types:** a libclang known-type map with offsets. A type declared after the insertion point gets a forward declaration. A class template that is unknown, or declared later, is residue. `>>` is written `> >`.
- **Pipeline:** `genpre442.sh` (generate, clang precheck, automatic CONSTAWAY retry), `mkb438.py`, `runb442.sh`, `post442.sh` / `finalize442.py` (deletes witnessed in-batch, CROSSBATCH, forcehold, SELF_SYMBOLS layout), `ssfix442.py`, `delcheck.py`, `census442.py`.
- **gate438.py** now reports NOOBJ and ALL_EQ_TPLALT.

## Residue (RESIDUE.tsv: 1,576 lines; T1_A only)
| reason | lines |
|---|---:|
| special member / operator (STL ctor / operator= on stand-in vector typedefs, operator[], ctor via Construct): model edits | 445 |
| row screens: T3 col 2/3 (209), locks, parse errors. The T3 lines are binder-respell lines, not the row's own self, so r1 does not open them | 274 |
| OBJCENSUS: bound in objects, no locatable call site | 202 |
| template scope: CCountedPointer<T>, std containers; CBasicString<char>::Compare (64 lines: protected static, needs the FABLE_RETAIL_CCHARSTRING opt-in) | 164 |
| stand-in has a body in the row (ruling q442b) | 107 |
| CThingSearchTools header lacks the protected `PeekClassList() const` (51 lines). The callers derive from CThingSearchTools, so one `_hdr` line opens them | 51 |
| call-site shapes, types, static v non-static, parameter count, own-model virtuals and overloads, DTOR model clashes | about 290 |
| holds (NF442 and NF438) | 43 |

## Next
1. If q442a is granted, land tplalt_hold.
2. Add a `_hdr` batch: CThingSearchTools.h gets `protected: const std::list<CClass*>& PeekClassList() const;`, then regenerate (51 lines). Do the same for SetMorality / WordWrapText (3 holds).
3. If q442b is granted, drop the stand-in bodies (107 lines).
4. Scratch: `$S/w22/agentNF442`. Regenerate with `bash tools/genpre442.sh gN`, skipping the rows in work/skip_rows.txt.

## Caveats
- **b1 repair.** mkb438.py reuses an existing `batches/<prefix>1`, and once wrote b2's rows into the finalized b1. b1 was rebuilt (rows.txt = its ACCEPT list, renames.tsv from the plan), and a re-finalize reproduced RESULTS, TABLE_CHANGES, CROSSBATCH, SELF_SYMBOLS, CHECKS and UNITS byte for byte. New batches now go under a fresh prefix and are renamed.
- **Table moved during the session.** An outside landing changed reloc_bindings.tsv at 19:00, and one finalize that read it mid-write gave a wrong table. Every batch was then re-finalized in landing order against the settled table; the files match the earlier ones.
- **Compile outputs deleted.** The `work/wd_*` directories were deleted to save disk, so `post442.sh` cannot rerun xrcheck for p1, b1, q2b, q2c and b4; their xrcheck evidence is in each batch.
