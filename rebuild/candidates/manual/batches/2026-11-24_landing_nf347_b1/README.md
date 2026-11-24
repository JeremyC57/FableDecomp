# Landing nf347 b1 (Account A, 2026-11-24): 0x00D58490 takes FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY (pilot; scaffolding)

Agent NF347 piloted the existing per-row opt-in `FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY` on three emitters of the NEQ
map<CCharString,STB>::operator[] copy 0x00CD3D2E (T3 nf340). Its hand-back is in `pending/2026-11-24_nf347_handback/`.
The row comment carries `// Fidelity: scaffolding, not retail` and gives the reason.

## Ruling (advisor decision, owner delegated)
**Only 0x00D58490 lands.** The test: after the opt-in, the row's new external reference must resolve to a landed row that defines
that exact symbol and is EQ.
- **0x00D58490 passes.** Its leaf-called NEQ map<CCharString,CCharString>::operator[] copy (159 v 149) goes. The reference resolves to
  landed owner 0x009AC700, which defines the exact 4-argument symbol, select-any, RELOCATION_MATCH.
- **0x00EA59E0 and 0x00E8EF80 are HELD.** The owner 0x00CD3D2E still defines a 3-argument stand-in, so their reference would land on
  another row's NEQ copy. They wait for the owner respell and then move with the ~74-row emitter family.

NF318 Q5 / NF320 Q2 gate moves, not copy repairs (the BRIEF_NF_BATCH NF340 scope line). The drop test is the gate here.

## Gates
- **Grade:** RELOCATION_MATCH PASS before and after (NF347), and from the repo after the landing (land_scripted recheck).
- **objsec:** no section changed. The only additions are the external `??A` references.
- **Pick-any, both checkers on A's own object** (`logs/coordinator_*.log`): the NEQ set is a strict subset of HEAD's. Gone:
  - `less` 0x0042AAB3;
  - the pair ctor 0x00CD27C7;
  - the two leaf-called operator[] copies 0x00CD3D2E and 0x009AC700.

  Left: the known debt ~MovieBase 0x006E7B80 (leaf-called, already NEQ at HEAD), plus five copies that are not leaf-called.
- **Drop test** (`evidence/droptest_00d58490.tsv`): 41 COMDATs dropped.
  - No reloc_bindings line witnessed by this row names a dropped symbol.
  - No landed object is left undefined.
  - Only two dropped copies are EQ with a retail address: the STB copy ctor 0x00905330 and `_Rb_tree_base_iterator::operator==`
    0x0041C820. Both keep EQ definers elsewhere.
  - The other 39 have no retail address or were NEQ.

## Also in this landing (coordinator housekeeping)
- **T3 closed:** acctB_b24B6 0x0066C340, fully repaired by nf341_r2, B's 0x00839940 and nf346 (`T3_CLOSED.tsv`).
- **T3 updated:**
  - the 0x00CD3D2E / 0x009AC700 emitter lists;
  - the nf341 D3 status;
  - NF347's measurements on 0x00429C15, 0x00429F43, 0x005949C3, 0x00B439C0, 0x00B43720, 0x00433530 and 0x005149F0.
- **NAME_FIXES:** the nf343 line for 0x00CD3CAE now names the STLport-header spelling, not Ego_r.pdb.
