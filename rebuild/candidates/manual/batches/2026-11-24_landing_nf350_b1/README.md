# Landing nf350 b1 + b2 (Account A, 2026-11-24): the map<CCharString,STB> owners take the 4-argument STLport symbols; the two held pilots land

Agent NF350's hand-back is in `pending/2026-11-24_nf350_handback/`. The rulings below are advisor decisions (owner delegated).

## b1: one unit, 0x00CD3D2E map::operator[] + 0x00CD3CAE map::insert(iterator, const value_type&)
- **The respell:** both owners now define the 4-argument STLport spellings (`...V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResource
  ObjectScriptedThingBase@@@std@@@4@...`), the symbols the repo's STLport headers emit. Before, they defined 3-argument stand-ins. No
  landed binder used the old spellings: NF350 scanned 246 HEAD objects, with the two owners as positive control. So the unit is the two
  owners (NF302 Q5).
- **Grade:** RELOCATION_MATCH PASS before and after, and from the repo after the catalog edit (`logs/recheck_after_catalog.log`).
- **0x00CD3D2E stays a local model (ruling 1).**
  - objsec is ALL_EQ under 9 renames: the self, two callees, and six inline helpers.
  - The shared-header route keeps the leaf but emits leaf-called NEQ copies, which the NF340 gate holds. That measurement is on the T3
    line.
- **Six renamed helpers (ruling 4).** The coordinator ran pk_all on its own new object under the changed table
  (`logs/coordinator_pk_all_00cd3d2e_newtable.txt`).
  - All six renamed helpers are NOADDR: the two iterator ctors, `??D`, and map end / key_comp / lower_bound.
  - The three NEQ copies keep their HEAD names and were NEQ at HEAD: `??M` 0x009E6530, `??8` 0x0041C820, and the leaf-called `less`
    0x0042AAB3. They are unchanged debt.
- **0x00CD3CAE moves onto stlport/fable_map.h under the landed switch FABLE_RETAIL_STLP_MAP (ruling 2).** The coordinator checked NF318 Q1
  on its own HEAD and new objects:
  - every other section is equal (`.debug$S` carries the names);
  - `.drectve` = HEAD's tokens plus `/DEFAULTLIB:"libcp"`;
  - the UND sets differ only by the intended callee respell to the hinted `_Rb_tree::insert_unique` that 0x00CD3AB6 defines;
  - the grade is PASS.
- **Table:** 3 replaces (`TABLE_CHANGES.tsv`), all applied:
  - map `_M_lower_bound` becomes the `_Rb_tree` spelling 0x00CD28BC defines;
  - the 4-argument insert;
  - the stale `CForwarder_00cd3cae` line becomes `insert_unique`.

  `apply_table`: 0 misses, nothing new multi-target. The link resolves the 4-argument `??A`, `_M_lower_bound` and `insert_unique`, all
  unresolved at HEAD, with no duplicates.
- **Catalog:** 0x00CD3CAE's catalog name, manifest and Module now spell the 4-argument map. The file names are unchanged.
- **T3:** the lines nf311 / nf326 / nf340 0x00CD3D2E stay open. They were parked during the run and restored verbatim (sorted diff
  checked).
- **NAME_FIXES:** the two nf343 lines are closed (`NAME_FIXES_CLOSED.tsv`). Lines 534 / 571 and T3 nf311 stay open: the
  `CCutsceneCastMap` stand-in binders of 0x00CD3D2E are still there.

## b2: 0x00EA59E0 and 0x00E8EF80 with FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY (the NF347 pilots, held until b1)
- **Resolve test:** each row's new `??A` reference now resolves to the respelled owner. NF350's /FORCE link: unresolved with the HEAD owner,
  resolved with the new one.
- **Grade:** RELOCATION_MATCH PASS before and after, and from the repo after landing.
- **objsec:** no section changed. Each row carries `// Fidelity: scaffolding, not retail`.
- **Pick-any:** the coordinator ran both checkers on its own objects. The NEQ copies left are not leaf-called HEAD debt: the pair dtor,
  `_M_erase`, `clear`, and ~CSpawnedFuncBase or `CBasicString ==`.
- **Drop test** (NF350 droptest / drop_census): 23 COMDATs dropped per row. Only two dropped copies are EQ with a retail address, the STB
  copy ctor 0x00905330 and `??8` 0x0041C820, and both keep 69+ definers elsewhere. No table line is witnessed by either row, and nothing
  is left undefined.

## Not landed here
- **0x00626750 (NF350 b3, approved):** it lands once, from agent NF352's batch, which builds its size-body change on top of this text.
- **The ~68-row family port:** the next free slot. The T3 nf340 0x00CD3D2E line has the gate: a drop test counting definers outside the
  batch, and the `??8` 0x0041C820 risk.
- **Newly visible debt (ruling 3):** the operator[] emitters' 4-argument insert copies now read NEQ against 0x00CD3CAE (T3 nf350).
