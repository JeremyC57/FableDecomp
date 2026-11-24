# NF395 hand-back (Account A naming agent, 2026-11-24): re-bin under R1 / R10 / R11 of the held NF392 rows and the NF390 binders

No repo writes, no git. Work dir $S/w22/agentNF395. About 75 VC7.1 compiles:
- b2: vland pair 4, definer census 9;
- b1: 3 probes, vland pair 12 (3 rows), census 18 (cen_a1 11 + cen_a2 7);
- callee owners 5;
- b3: two vland pairs 8 (without and with /Gr), definer census 9, a /Gr probe 1, owners 6.

Locks 00728a40 / 00728b10 / 00728b50 / 009b77b0 / 00762440 were claimed as NF395 and released at the end. I stayed off NF388's ??_C@ literal rows and
NF394's rows.

| batch | rows | verdict |
|---|---|---|
| b1 | 00728a40 00728b10 00728b50 (one unit) | ACCEPT; the build entries need CompilerFlags '/O2 /Oy /W3 /Gr' before the landing recheck |
| b2 | 009b77b0 (binder, `; target 0045bc09`) | ACCEPT |
| b3 | 00762440 | HOLD (T3 nf395 + RULINGS_NEEDED Q1) |

## Tools (tools/)
- bin395.py: bins every NEW COMDAT of a HEAD/new object pair. For each one it reports:
  - R11 candidates;
  - tier-1 definers (byte-and-reloc EQ in given objects);
  - tier-2 reloc-masked retail matches anchored at function starts (catalog start, or 16-aligned with 0xCC before it), plus the subset
    whose reloc targets resolve to the table's binding.
- samesec.py: the same comparison for any section, e.g. a .rdata vtable.
- vtcheck.py: compares a vtable COMDAT with the retail vtable slot by slot.
- Copied and renamed from NF393: nf395.py, probe395.py, census395*.py, comdats.py (now importable), samedef.py, pk374.py, runpair.sh and
  others. runpair.sh now passes a row's `// Flags:` line into the N payload when the build entry has none.
- Run the tools with `$S/wine/env.sh` sourced: pk_all's leaf lookup fails with FileNotFoundError without it.

## b1: 00728a40 / 00728b10 / 00728b50 (NF392 held rows), ACCEPT as one unit
- **The /Gr question (from the brief).**
  - Retail's unreferenced `_Construct<CMeshMorphSetParts>` at 0x00728A20 is __fastcall (`test ecx,ecx ; je ; mov eax,[edx] ; ... ; ret`).
    STLport's `_Construct` has no calling-convention keyword, so the convention must come from the TU's flags.
  - NF392's u3 probe used an added `__fastcall` keyword. I dropped every `__fastcall` keyword from the STLport templates and compiled with
    `// Flags: /Gr` (tools/gen_a.py builds the rows from NF392's held rows).
  - Result: all three selves grade RELOCATION_MATCH and `_Construct` comes out YI, 21 B.
  - /Gr renames no other free function into a new COMDAT. Placement new is now <new>'s explicit `__cdecl` `??2@YAPAXIPAX@Z`, EQ v HEAD.
    The model's own `inline operator new` would have become YI.
- **The landing needs the build entry.** recheck.py / land_scripted read flags only from the build entry. The coordinator must set
  CompilerFlags '/O2 /Oy /W3 /Gr' on 00728a40, 00728b10 and 00728b50 before land_scripted, or the recheck restores the rows. My vland N
  payload carried flags ['/Gr'], which is exactly what recheck builds from that entry.
- **vland at one path** (wd_b1h / wd_b1n): H and N both RELOCATION_MATCH PASS.
  - N warns SHARED_HEADER_AVAILABLE for CArray and CTCAppearanceMorphBase. This is the landed 00728d10 model (R8 debt, T3).
  - HEAD's RELOC_NAME warning for ??0CSecond_005bbdf1 is gone.
- **objsec under the renames** (self; ??0CSecond_005bbdf1 -> STLport vector copy; ??0SPairLongCSecond_005bbdf1 -> ??0CMeshMorphSetParts,
  which is R11):
  - every HEAD section is EQ, and .drectve is EQ;
  - the selves go NoDuplicates -> Any, because they are template instances.
- **Selves.** Each self is byte-and-reloc identical to the same-named pick-any copy that landed 00728d10 / 00728df0 already emit (samedef_*).
  No other census object defines them.
- **Bins** (evidence/bins_*.tsv):
  - `??1?$CArray@VCEngineTextureMorph@@@@QAE@XZ` and `??1CEngineTextureMorphSet@@QAE@XZ` (5 B jmp thunks): R1 tier 1.
    - They are identical to the copies of landed 00722270 / 007222b0 / 007280c0, which come from the shared headers (no local model).
    - The retail copies 0x005BC0F6 / 0x005BC1AB are not anchored function starts (/Os, right after a ret), so tier 2 would not cover them.
  - `??0?$CArray@VCEngineTextureMorph@@@@QAE@ABV0@@Z` and `??0CEngineTextureMorphSet@@QAE@ABV0@@Z` (19 B forwarders): R8.
    - The only definers are local models: 00728d10, 00728df0 and 00707b10.
    - Retail has 168 masked matches at function starts, of which 2 are reloc-bound (0x00709A30 / 0x00709BB0), so no address is recorded (R9).
    - pk_all --addrs reads EQ with NF392's addrs_728.tsv. That file is one of two byte-equal assignments of the twin forwarders / thunks,
      not a recorded address; the swap reads EQ too.
  - `??$_Construct<CMeshMorphSetParts>` YI (21 B): R1 tier 2.
    - It is new to the build; the siblings emit only the cdecl YA copy.
    - It has 24 masked hits at function starts and only one reloc-bound hit, 0x00728A20. It is therefore non-unique, and no address is recorded.
  - 00728b50 only:
    - `??0__false_type` 3 B: tier 1.
    - the pick-any `??$__uninitialized_fill_n<CMeshMorphSetParts*>` YI: not new. It is identical to landed 00728d10's copy and to row
      00728a40's graded self, and plain pk_all reads it EQ at 0x00728A40.
- **pk_all + pvr HEAD v new**: no NEQ, reach flips 0. The R5 condition holds: no new NEQ.
- **apply_table --dry**: delete `??0CSecond_005bbdf1@@QAE@ABU0@@Z -> 005bbdf1` (witness 00728a40). 0 misses, multi-target 1 -> 1.
- **/FORCE link** of the 3 rows + 005bbdf1 005bc0d3 + 00728d10 00728df0 + 00722270 007222b0 007280c0:
  - duplicates 0 -> 0;
  - unresolved 21 -> 20 (the stand-in callee is gone);
  - nothing HEAD-resolved is left unresolved.
- **Census by header reach**: 30 rows.
  - The set: the includers of CTCAppearanceMorphBase.h (through CTCRandomAppearanceMorph.h / CTCHeroMorph.h), every row that names
    CMeshMorphSetParts, and the 00728 family.
  - Objects: 18 fresh compiles, plus 12 NF392 VC7.1 objects from today. Those 12 were checked by mtime: they are newer than their sources and
    than every header, except CPSCSplineBase.h, which none of them includes.
  - Nothing references the old selves, ??0CSecond_005bbdf1 or ??0SPairLongCSecond_005bbdf1. No header ever declared them.
- **Tests** are unchanged (standalone).

## b2: 009b77b0, ACCEPT (binder)
- **Row.** NF387's candidate: engine/CArray.h + FABLE_RETAIL_STLP_VECTOR_ASSIGN replace the namespace-local CArray. The comment is updated;
  the self is unchanged.
- **vland**: H and N RELOCATION_MATCH PASS. N has no WARN.
- **objsec**: every HEAD section is EQ under the ??4 rename. ~CArray pairs under R11. .drectve gains libcp, as 009b7870 / 00b217d0 do.
- **10 new COMDATs, all R1 tier 1.**
  - This row is /Gr, so its free helpers mangle YI (`_Destroy` / `__destroy` / `__destroy_aux` <unsigned long*>). 00ad9980's copies are YA,
    so they do not cover them.
  - The YI copies are covered by the /Gr shared-header siblings of the same CDefinitionManager slice: 009b80c0, 009b8630 and 009b8820.
    - Each includes only engine/CArray.h.
    - Their own namespace std blocks hold just the row's algorithm and its callee declarations. I read all three.
  - The member copies are also identical in 00ad9980 / 00b217d0.
  - The ??4 forwarder is the only reloc-bound retail match, 0x009B5A60 (R4).
- **pk / pvr**: no NEQ, reach flips 0.
- **apply_table --dry**: delete `??4?$CArray@K@N009b77b0@@QAEAAU01@ABU01@@Z -> 0045bc09`. 0 misses, multi-target 1 -> 1.
- **Link** of the row + 0045bc09 0099efb0 0099eae0 + the 6 chain definers: duplicates 0 -> 0, unresolved 19 -> 18.
- **Census**: N009b77b0 exists only in this row's source (no header, no includer). The VC7.1 objects of the self witness 009b8220 and the 5 definers
  do not reference it.
- **Not done here.** The self respell to the table's ??$__push_heap spelling (witness 009b8220) is in NAME_FIXES and T3.

## b3: 00762440, HOLD
- **Row.** NF390's held self respell, plus `// Flags: /Gr`.
  - Why /Gr: without it, 3 bodies of the CCountedPointer<CSubdivisionTriangle> destroy chain have no definer and no retail match: ~vector
    42 B, `_Destroy(p)` 55 B, `__destroy_aux` 74 B (evidence/bins_00762440_nogr.tsv; that vland pair is kept as *_nogr).
  - The NShape TU is /Gr, by three pieces of evidence:
    - the PDB name ?StatOnSerialise@CShape@NShape@@SIX..., a static member that is __fastcall by default;
    - the shared-header siblings 007634e0 / 00764590 / 00764ee0 compile /Gr;
    - retail __destroy_aux<CCountedPointer*> = 0x00764EE0 is __fastcall.
  - With /Gr, ~vector<CCountedPointer> is reloc-bound at 0x00765660 / 0x007657B0 and __destroy_aux equals 00764ee0's graded self.
- **vland**: H and N RELOCATION_MATCH PASS. N has no WARN. HEAD warned SHARED_HEADER_AVAILABLE C3DVector and RELOC_NAME CRetailVector.
- **Bins of the 46 new COMDATs**: 0 residue.
  - 35 are tier 1 (007634e0 / 007621c0 / 00764ee0, shared headers only; 007634e0's one std block is an unrelated C3DVector specialisation).
  - 10 are tier 2 (masked, non-unique).
  - The CShape ctor pairs under R11 with HEAD's stand-in CShape ctor. It is also reloc-bound to retail 0x00764F30 (not catalogued).
- **What holds the row.**
  1. **Link order.** With the row after 007634e0, duplicates go 1 -> 2.
     - 007634e0 emits the YI __destroy_aux<CCountedPointer*> with selection NoDuplicates. It already clashes with 00764ee0 at HEAD.
     - This is a pre-existing 007634e0 defect (T3 nf395 0x00762440).
     - With the row first, duplicates stay at 1.
  2. **HEAD sections that are not extras, so none of the four bins covers them** (RULINGS_NEEDED Q1):
     - ??_7CShape / ??_7CBasicShape grow from 1-slot stand-ins (4 B) to 6-slot vtables (24 B). Both match retail slot by slot (evidence/vtables.txt).
     - ??_7CShape is identical to 007634e0's shared-header copy. 00764fc0 and 00762480 already define other stand-in bodies under both names.
     - The two stand-in CRetailVector default ctors vanish.
- **Not run** (the link gate failed first): the 13-row CRetailVector census and the table delete. Both are listed in batches/b3/TABLE_CHANGES_PROPOSED.tsv.
- **New unresolved names** in the link: ??1CBasicShape@NShape@@UAE@XZ and the const ?IsPosInShape. Their landed owners 00762480 / 007611c0 carry
  other spellings (NAME_FIXES).

## Final checks (after the advisor's review)
- **Definers.**
  - No DEF name of the b1 / b2 new objects is another landed row's self (self_symbols.tsv). Each object's own HEAD self is the only hit.
  - No census object (51 rows) defines one of the watched names with selection NoDuplicates.
  - b2's definer census adds the other /Gr CArray<unsigned long> rows 009b9030 / 009b9290 / 009b9320: every copy is Any (census_b2e.tsv).
  - Re-linked with the rows last: b1 0 -> 0 and b2 0 -> 0 duplicates (link_b1last / link_b2last).
- **Includers.** No landed row or test `#include`s any of the four changed row files, so land_scripted has no includers to regrade.
- **006cb390 / 006cb470** (`??0__false_type` tier-1 definers) include only engine/CEngineTextureMorph.h; neither has a namespace std block.
- **GOTCHAS lines 449 / 499** say flags come only from build_candidates CompilerFlags; a `// Flags:` line does not reach the grade.
- **Open question, 0x00765B10.**
  - Ego_r.pdb (00774e40) and FableWin.pdb (025d84e0) name ??_ECBasicShape@NShape@@UAEPAXI@Z. The b3 vtable's slot 0 relocates to ??_E,
    which VC7.1 resolves to its emitted ??_GCBasicShape.
  - That 30-byte ??_G body matches retail 0x00765B10 exactly (findwrap: rel -> 0x00762480 ~CBasicShape, 0x00BFE9BC operator delete).
  - The landed self is ?ScalarDeletingDestructor@CBasicShape@@QAEPAXI@Z. The new name is ??_E or ??_G: coordinator to decide. It is not in
    NAME_FIXES.

## Run-dir notes for the coordinator
- **Units and flags.** b1 is one unit (UNITS.tsv u1). Set the three build entries' CompilerFlags first, as above.
- **b2** carries `; target 0045bc09` in its reason.
- **T3 queue.** T3_PORT_QUEUE has 0x00728A40 (tag nf392) and 0x00728B10 (tag nf393) in its address column. land_scripted holds those rows,
  and through the unit all of b1, unless it runs with `--release-t3 nf392,nf393` or the lines are closed by hand. 009b77b0 and 00762440 have
  no address-column line.
- **No header edits**, so there is no HEADERS.tsv and no pin_asserts.
- **Open claim line.** COORDINATION_CLAIMS.tsv has an open NF395 claim line that names 009b77b0 / 00762440 in its text. land_scripted holds
  any accepted address that an open claim mentions, so close that line (done / released) before landing b2.
- **T3 line 0x009B77B0.** My T3.txt has a line keyed 0x009B77B0 (the __push_heap self respell). Append it to the queue only after b2 lands,
  or land with `--release-t3 nf395`; otherwise the queue reserves the row.
