# NF345 hand-back: leaf v table conflicts (143 rows), 2026-11-24

Folder: `$S/w22/agentNF345/`. No git command was run, and nothing was written into /home/user/FableDecomp.
- A check for `*.pyc` / `__pycache__` newer than BRIEF.md in the repo found none.
- The repo files that are newer than BRIEF.md come from other agents' landings, not from this run.
- `work/shadow` is a symlink farm into the repo with one real file, a copied reloc_bindings.tsv that has the changes applied. Do not copy it with `-L`, and do not write through it.

## Result (STATUS.tsv, 143 rows)

| STATUS | rows | which |
|---|---|---|
| table-fixed | 4 | 0040b050, 006403d0 (batches/b1); 0047dd9d, 00e1a870 (held/b2, decision D1) |
| respelled | 5 | 0063b890 / 0063b920 / 0063b9b0 / 0063ba30 (batches/b1 U1); 0042b587 (batches/b3) |
| skipped-port | 23 | the fix is a port, or the row / A / witness is in T3_PORT_QUEUE column 2 or 3 |
| research | 111 | 84 are not conflicts (below); 27 are open questions, 00531ec0 included (NF281 D4 hold, not compiled) |

The 84 rows that are not conflicts:
- 45 rows are `_$E2` / `_$E3` statics. These are COFF storage class 3, one per TU. `evidence/estatic.txt` proves `_$E3` from the 0121f7a0 object compiled for this check. A table line for one of these names describes only its witness's own TU.
- 39 rows are EH funclets. A funclet row grades its parent's symbol, and the table binds that symbol to the parent itself, which is right.

`tools/census_leaf.py` (NF344's tool) is patched to separate both kinds: `_$E` names are counted as `tu_local_static_$E` and skipped, and funclet rows get the class `FUNCLET_*`. A rerun gives 59 `_$E` rows and 329 FUNCLET rows (`work/census_new_summary.txt`). The next census should drop or label them.

## Batches

Gates passed by every changed row:
- objsec ALL_EQ under the rename (`logs/objsec_proof.txt`);
- vland `--recheck-landed`, HEAD and new: RELOCATION_MATCH PASS, grades unchanged (`logs/vland_*.log`, `recheck_final.log`);
- link HEAD v new: no new duplicates or unresolved symbols (`evidence/link_u*.txt`).

NF340 pick-any gate, run with both checkers:
- **pk_all, HEAD v new** (`evidence/pk_all_b123.txt`): verdicts are identical apart from the renamed symbols.
- **pickany_vs_retail.py** (the codex pass-28 copy in tools/), HEAD and new, under both the repo table and the applied table (`evidence/pickany_vs_retail*.txt`): no new NEQ, and no leaf-referenced NEQ on any changed row. The changes remove existing NEQs:
  - Each U1 HEAD object defined its own leaf under a name that the table binds to a sibling (an NEQ copy at the sibling's address); new removes it.
  - The 0042b587 HEAD object defined the J `_M_create_node` that the table binds to 0046ece7 (NEQ); new removes it.
- **Old debt, NEQ in both HEAD and new:**
  - 0059697a: the IsCameraYZInverted copy (005485b0);
  - 00e1a990: the ~CSpawnedFuncBase copy;
  - 0041e6d3: 31 copies, which include leaf-referenced `_List_iterator_base` / `GFStringsAreEqual` / `CPixelFormat` / `CCountedPointer<CTexture>::ReleaseData` copies.

**COORDINATOR: tests.** land_scripted copies only `rows/<a>.cpp`, and its recheck compiles the landed test from `rebuild/tests/`. Install `tests/<a>_test.cpp` over the repo test listed in each batch's `TESTS.tsv` BEFORE land_scripted. The old tests name the old spellings, so with them the recheck fails and the rows are restored. The RESULTS reasons say the same.

### batches/b1 (ready)

`UNITS.tsv`: each unit lands whole, together with its TABLE_CHANGES lines.
- **U1**: 0063b890, 0063b920, 0063b9b0 and 0063ba30 are a four-way self swap of `CWorldMap::PointIsWithinCell{TR,BL,TL,BR}`.
  - Evidence: ego_r FindGround/FindTheme call order, Ego_r object order and sizes, and the fadd pattern.
  - There is no table change: binders 0063d0a0 and 0063d4a0 already bind these names at these addresses.
  - The catalog labels and the source / test file names still carry the old letters (NAME_FIXES).
- **U2**: binder 0059697a is respelled to `SetCameraBowYZInverted`.
  - Table: delete (YZ, 0040d2f0); add (YZ, 0040b050, witness 0041e6d3, its only retail caller).
  - Under the applied table, 0040cca0's pick-any copy of SetCameraYZInverted goes from NEQ to EQ (`evidence/pk_table_effect.txt`).
- **U3**: binders 004fede0 and 004feec0 are respelled to `SetMapAsUnloaded`.
  - Table: replace RemoveMap with SetMapAsUnloaded at 00640430; add RemoveMap at 006403d0 (witness 004ff8b0).
  - ref_scan found three other RemoveMap referencers (004ff8b0, 005022f0, 00506940). All three bind 006403d0, both in their objects (xr) and in retail (xscan), so nothing else references RemoveMap at 00640430 (`evidence/table_targets.txt`).

### batches/b3 (ready)

The self 0042b587 is respelled from `long` to `unsigned long` (map<CCharString, unsigned long>).
- Table: replace the pair copy-ctor J with K at 004297db. The K copy is EQ at 004297db under the applied table.
- ref_scan cannot scan the J copy-ctor (TOO_MANY, "no identifier"). This is safe anyway: retail calls 004297db only from 0042b587 and from the unlanded 0042a911 (xscan), so no landed row's relocation depends on the J line. J pick-any copies in other objects become NOADDR; they were never compared with the right body.

### held/b2 (decision D1 first; held/ so it cannot land before the ruling)

Binders 0041e6d3 (PCSetToLoad → LoadGame) and 00e1a990 (Init → InitialiseVariables).
- The PDB names and the evidence are in RESULTS.tsv.
- Each owner still defines a stand-in self: 0047d008 `?Set@T@@QAEXH@Z`; 00e1ad30 `?Init@C@@QAEXXZ`.
- Its TABLE_CHANGES are separate and pass the dry run on their own and together with b1 + b3 (0 misses, nothing new; `evidence/apply_table_dry.txt`).

Top-level `TABLE_CHANGES.tsv` = b1 + b3 (`apply_table --dry`: 0 misses, multi-target 1 → 1, nothing new).

## Decisions needed

- **D1** (held/b2): may a binder be respelled onto the PDB name while its owner still defines a stand-in? The two cases differ from NF302 Q2:
  - **NF302 Q2**: the reference is unresolved before the change and after it.
  - **These two**: before the change, each reference resolves to the WRONG landed body (R 0047dd9d PCSetToLoad; R 00e1a870 Init). After it, each stays unresolved until the owner port (NAME_FIXES 0047d008 / 00e1ad30).
- **D2**: should 00643d8c and 00437990 land together as one unit?
  - The 00643d8c self respell is to `NPlayerGui::CDrawCombatMultiplier::AddHitByMelee`.
  - 00437990 needs a port onto engine/CPlayerGui.h: `CPlayerGui::AddHitByMelee`, a forwarder that jmps to 00643d8c.
  - Landing only one breaks CTCHero::OnHit's resolved reference.
- **D3**: these identity pairs have two landed selves defining one symbol (full symbols compared), so each is held as one unit. Please route them to the identity lane:
  - 00433cec/0040e290, 004345a0/00625350, 005d85de/00514160, 00699970/00699a50, 006aeb10/004b7980, 006c4560/006a75d0, 0076d850/0076d6f0, 0081a590/004708ef, 0081a9e0/00470d77;
  - 006a3c70/00cd3e7b, which is also T3.
  - 0085e200 is NOT one of them (its __false_type scope differs). It and 00b3e390 are an element-type / __false_type port; see STATUS.

## Locks

- **Kept until landing** (NF309): 0063b890, 0063b920, 0063b9b0, 0063ba30, 0059697a, 004fede0, 004feec0, 0042b587, 0041e6d3, 00e1a990.
- **Released** (owner files read back as `released`): the controls 0063d0a0, 0063d4a0, 004ff8b0, 0040d2f0, 0040b050, 00640430, 006403d0, 0042bcb5, and 0121f7a0.

## Tools (tools/)

- **Repointed copies:**
  - nf.py: grade, hgrade, proof, link, xr, syms, pay. It uses one work dir per address.
  - Others: rdis.py, xscan.py, refs.py, cmpcomdat.py, pre.sh, zs.py, zsrun.sh, objfn.py, pickany_vs_retail.py.
- **Written here:**
  - triage.py
  - donor.py: reads only the ego_r / FableWin call lists and names; nothing from the donor builds is copied.
  - assemble.py
  - status_rest.py
  - census_leaf.py: the patch above.
