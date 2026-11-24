# NF339 hand-back (Account A naming queue, 2026-11-24): the 227 T3_PORT_QUEUE "self respell" lines

Scope: `ENTRIES.tsv`, 227 lines over 207 addresses (access1 40, access2 36, access3 41, access4 7, alias1 93, binders1 6, nf334 3,
flatten4 1). Every line has a STATUS entry (`STATUS.tsv`); the scripted re-read is `TRIAGE.tsv`.

## Outcome
| status | lines | what |
|---|---|---|
| done-already | 138 | the obstacle is gone: a later landing already moved the row (list + evidence in `STALE_T3.tsv`) |
| done (b1) | 4 | binder respells, `batches/b1/` (5 rows: these 4 lines plus binder 0x008C6430 of the same unit) |
| done (b2) | 1 | 0x008800C0 Clone onto the now-landed StabbedToDeath model, `batches/b2/` |
| skipped-port | 60 | real ports, measured reason per line (all still hold on today's tree) |
| skipped-held | 8 | held by a ruling or by another reservation |
| not-attempted | 1 | 0x00692480: blocker partly gone, recipe in STATUS |
| rejected | 15 | return width: retail bytes win over the PDB return type (NF317 Q4) |

Vocabulary: `done-already` = no source change needed, the row already defines / binds the PDB spelling at HEAD (census) and the T3
line is stale; `done (bN)` = changed row in run dir `batches/bN/`; `not-attempted` = workable now, not done in this pass.

## Step 1: scripted re-read (no compiles), then a HEAD census
- `tools/triage.py` (facts: recorded old / PDB spelling, today's Leaf, self_symbols.tsv, the member's declaration in the row, alias class
  still defined, header include, stay-off flags, other T3 lines) and `tools/mktriage.py` (TRIAGE / STATUS).
- 137 lines read "Leaf = PDB". On advice, the Leaf alone was not taken as proof: `rebuild/oracles/self_symbols.tsv` (03:05) still says
  "old" for 111 of them (it is stale). So all 127 addresses were compiled at HEAD (`tools/census.py`, 4 vland chunks, prechecks first,
  locks claimed and released): **135 lines define the PDB symbol and it is the graded symbol, grade PASS (22 MATCH, 50 RELOCATION_MATCH
  per address)**. Every one of them is in the rows.txt of a later landing (nf317 / nf318 / nf321 / nf323 / nf326 / nf327; `STALE_T3.tsv`
  column landed_by). Those landings went in through apply_rows, not `land_scripted --release-t3`, so the T3 lines were never closed.
- Binder lines (binders1 / nf334): the census reads the binder's UND set. 0x00858130 / 0x00892F60 / 0x00789900 no longer reference the
  old callee spelling and bind the PDB one: done-already (3). 0x008C63C0 / 0x00731BF0 / 0x0058509F lines record the caller's own Leaf
  on both sides; their real callees were read from `reloc_bindings.tsv` (WhackGround / GetPLocalSeed / GetQuickAccessNextFreeSlot).
- The 86 remaining lines: 84 already carry a measured reason in a later T3 line (nf317 / nf318 / nf320 / nf321 / nf322 / nf323 /
  nF325 / nF327 / nf288 / nf290 / nf309 / nf311 / nf314); the newest untruncated reasons were read from the pending hand-backs' STATUS.tsv.
  Checked by script on today's tree: the named stand-ins of 23 const / mutable addresses (CStockMap, CTCInventoryBase_Retail,
  CCarrySlotMap, CRegionNodeMap, CVectorMap_00a84e40, BaseGetPMeshAnims, CCriticalSectionLock_Retail, CreateSaveGame, the local container
  operator[], no `mutable`) are all present; all 24 open alias1 rows still define their alias class (`evidence/tri2.tsv` alias_def);
  FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR still has no landed definer. Lines whose newest reason said a blocker was being lifted were
  re-checked: the StabbedToDeath model landed (0x008800C0 now done, b2); the CCreatureActionBase base model is now proven slot for slot
  (0x00692480 not-attempted); CVillageTaskBase is still unmodelled (0x00860300, 0x0065EDAC); 0x009680D0 / 0x00860290 stay held by
  nF327's measured NF320 Q2 / binder results; 0x00BAD670 still needs its derived model chain (nF325). The return-width and Clone
  reasons were not re-measured (the rows are unchanged since).

## Batch b1 (`batches/b1/`, land with `land_scripted.py <b1> <batch> <tag> --release-t3 nf334,binders1`)
Five binder rows, each a caller-only respell onto the PDB callee spelling its owner row already defines. Proof per row
(`tools/ver.py` + `tools/ver_run.sh`): HEAD and new compiled by vland at ONE work path, objsec ALL_EQ under the rename(s), external
symbol sets equal after the rename, own graded symbol unchanged, grade unchanged (RELOCATION_MATCH PASS both sides). Each address's
only T3 mention is its nf334 / binders1 line (0x008C6430 has none).
- 0x00AF59A0 Update: `DestroyDecals` -> `?DeleteAllDecals@CParticleSystem@NParticleEngine@@QAEXXZ` (Ego_r + FableWin; self 0x00AF5770
  defines it, census object DEF). Its test mirror respelled (the test copies the row verbatim).
- 0x0069A920 RemoveCombatCameraMode: `PopulateSwitchList` / `PopulateListFromData` -> private `InitManualCameraList` /
  `InitSpecialCameraList` (AAE, both PDBs; selves 0x0069A010 / 0x0069A1A0 carry those Leafs since NF334). The caller is a
  CGameCameraManager member, so `private:` needs no friend. The same-TU strong copy of SetCameraManagerDef (pre-existing debt) now
  references the same names as its owner 0x0069A850. Test mirror respelled.
- 0x004D297B CTCPhysicsStandard_Alloc: ctor `(CThing*)` -> `??0CTCPhysicsStandard@@QAE@AAVCThing@@@Z` (both PDBs; self 0x00723FD0 DEF),
  call site `*owner` (NF313 Q2). Its own flat self stays (out of scope). Retail-bytes comment row: `head/HEAD_FROM_REPO.txt` (D-f).
- 0x008C63C0 TrollTunnel::PerformAction + 0x008C6430 TrollProjectileAttack::PerformAction (unit `whackground`, UNITS.tsv):
  `WhackGround` QAE -> PDB private `AAE` (both PDBs; self 0x008C61C0 Leaf AAE, binder 0x008C6390 binds AAE). The derived class calls its
  base's private member, so each local base model gets `friend class <the real derived class>` (NF305 Q5).
- Table (`TABLE_CHANGES.tsv`, 5 deletes): each old spelling's only witness is a moved row and the PDB spelling already has a line.
  Census for the deletes (HEAD objects of every other landed row naming the identifier, `work/cen_c04`): no old reference left.
- Tests: land_scripted does not copy tests; `tests/` + `orig/tests/` hold the two changed mirrors (0x00AF59A0's landed and new test fail
  clang only on MSVC explicit-specialization syntax; both pass `cl /Zs`, NF305 Q2, `work/pre_zs.log`).
- Link: no new definitions (symbol sets equal under the rename); HEAD's old references had no definer; the new ones are defined by the
  owner rows (DEF checked on the census objects for 0x00AF5770 / 0x00723FD0, Leaf for 0x0069A010 / 0x0069A1A0 / 0x008C61C0). A /FORCE link
  was not run.
- Locks: the 5 changed rows stay locked (NF339) until the landing; controls released.

## Batch b2 (`batches/b2/`): 0x008800C0 CCreatureAction_StabbedToDeath::Clone
- The NF323 hold ("NF322 is porting the class model") is gone: `FABLE_RETAIL_CCREATUREACTION_STABBEDTODEATH_VIRTUAL_MODEL` is defined
  by landed 0x0087E830 / 0x0087FE50 (nf330 b2). The row drops its local stand-in models and defines the PDB member
  `?Clone@CCreatureAction_StabbedToDeath@@UBEPAVCCreatureActionBase@@XZ` (Ego_r + FableWin; RTTI slot 11) on the shared header, with
  FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR (the base copy ctor 0x0062EBC0, called out of line as retail does).
- Proof: HEAD and new RELOCATION_MATCH PASS at one work path; nf334's `cloneproof.py` OK (`batches/b2/cloneproof_008800c0.txt`): Clone
  COMDAT bytes + relocations equal under the self rename, no DEF lost, only `??_G` added, the emitted `??_7` equals retail 0x01277B74
  70 of 70 slots, `??_G` equals retail slot 0 (0x0087FE80), `.drectve` HEAD + libcp (NF318 Q1), UND growth only what `??_7` / `??_G`
  reference (NF323 Q6); the made-up `?v00@CCreatureAction_StabbedToDeath` reference is gone. No table line names any Clone spelling.
- Landing: the address has four T3 lines (access3, nf317, nf320, nf323), so `--release-t3` will hold it (D6).

## Decisions needed
- **D1 (138 stale lines).** Close the T3 lines in `STALE_T3.tsv` without a landing: each row already defines and grades the PDB
  symbol at HEAD (census, objects compiled today) and the line's fix came in with the landing named there. Alternatively I can build a
  no-change land_scripted run dir for them so `--release-t3` closes them, but 31 of these addresses (10 of them with two of my lines each) also have other-tag T3 lines (nf317 /
  nf318 / nf320 / nf321 / nf323) that the single-mention rule would not release; those other lines are equally stale where they describe
  the same self fix (please confirm before anyone deletes them). Three of the 138 are still open in the tracker for a
  different reason (`STALE_T3.tsv` column remaining): 0x007339E0's binder 0x00736AE0 (its own nf289 T3 line), 0x009F5D70's binder
  0x009F5E10 (a T1 binder follow-up: access + parameter class, in no queue or agent list), 0x00789900's own stand-in self.
- **D2 (release rule).** 10 still-open addresses carry two of my lines (access1 + access2) and most open addresses carry nf317 / nf318 / nf321 lines too;
  `--release-t3` only releases a single mention. Nothing in b1 is affected; future work on those rows needs a multi-tag release.
- **D3 (0x008F1720 EndRound, skipped-held).** In-place shaped (`virtual` on the local CTCTavernGame::EndRound and on the override,
  NF299 Q3 / NF303 Q1; callee QAE -> UAE, already bound by 0x008EF190), but the address is named in the nf288 unit line of 0x005E9AA0.
  May it go alone, or only with that unit?
- **D4 (0x00900E42 / 0x0091E970 / 0x00914EF0).** The members are declared only under `_PASS27` / `_PASS23` switches that landed rows do
  define; the NF322 Q1 route (widen the guard with `|| defined(FABLE_RETAIL_<CLASS>)`, ??_7 probe, equiv_check, planted control) would
  open all three. Not attempted here (a header unit); say if you want it as a follow-up batch.
- **D6 (0x008800C0, b2).** Four T3 lines name it; land_scripted's single-mention release holds it. Close all four with the landing?
- **D5 (rejected, 15 lines).** The return-width lines are recorded as `rejected` (PDB revision difference, NF317 Q4), not skipped-port.

## Stay-off record
- 0x008C61C0 (WhackGround self) is an NF338 entry: not compiled, not edited; NF338's triage marks it "holds (binder reserved)" on my line.
  My table delete of its QAE witness line resolves that split; please tell NF338 at landing.
- 0x00582027 (NF338 entry, owner of 0x0058509F's strong copy): not touched.
- No NF337 / NF338 locks, B claims, acctB_b24 rows or Codex 26-28 reserved rows were touched.

## Process slips (own)
- One unquoted heredoc (needed `$A` expansion) ran a backticked word as a command; the lost word was restored in
  `work/triage_override.tsv`. No other effect.
- `census.py read` first ran with cwd = the repo before I added `os.chdir(work)`; it failed at objdump before any write. The repo's
  `D:\Documents\FableTLC` directory predates this session (2026-11-19).
