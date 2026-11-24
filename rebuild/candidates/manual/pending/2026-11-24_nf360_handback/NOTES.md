# NF360 (Account A naming queue, 2026-11-24): two PRIORITY in-place self respells (NAME_FIXES nf356 lines)

Run dirs (land_scripted form, one row each so one ruling does not hold the other):
- `batches/b1/` 0x0042AF3C CDefinitionManager::GetDef<CThingBaseDef>: RESULTS **RULING** (R1, R2).
- `batches/b2/` 0x00428B10 CCountedPointer<NUISystem::CComponent>::DeleteData: RESULTS **RULING** (R3).

Every per-row gate the brief lists passes for both rows (grade, objsec, both checkers, drop test). Both are held on the link question the
brief asked to report: each strong self adds LNK2005 duplicates, and some are against copies with a DIFFERENT body. After a ruling, flip
RESULTS to ACCEPT (or swap in the variant) and land with land_scripted. No header change, no test change, no reloc_bindings change.

## 0x0042AF3C (b1)
- **Text.** Respelled in place on the landed local model. `GetDef_0042af3c(long, CDefPointer<const CDefClassBase>&)` became the member
  template `template <class T> bool GetDef(long, CDefPointer<const T>*) const`. The row defines it with a strong
  `template <> bool CDefinitionManager::GetDef<CThingBaseDef>(...) const` specialization, the form 0x0042B040 uses at HEAD.
  - New `// Leaf:` line: the exact PDB symbol.
  - Identity comment: FableWin.pdb, plus the reloc_bindings line witnessed by 0x007CBDC0.
  - The model is otherwise unchanged: CRefCounted, CDefBase, CDefPointer<T> with its one same-type operator=.
  - The PDB out type is `CDefPointer<const CThingBaseDef>*`, but the model's only assignment is CDefPointer<const CDefClassBase>'s. The body
    therefore assigns through `reinterpret_cast<CDefPointer<const CDefClassBase>&>(*out) = def;` (ruling R2).
- **Grade.** vland `--recheck-landed` gives RELOCATION_MATCH PASS for HEAD and for new (logs/). New loses the STANDIN_LEAF and "leaf matches
  no symbol" warnings. Both sides keep SHARED_HEADER_AVAILABLE (CDefPointer, CDefinitionManager), because the row keeps its local model.
- **objsec** (`evidence/objsec/0042af3c.txt`): OTHER_SAME, symdelta 0.
  - The leaf's bytes, relocations and characteristics are equal under the rename. Selection is 1 on both sides.
  - .drectve and the DEF / UND sets are equal apart from the self name. The 6 NOADDR model inline COMDATs are unchanged.
- **Pick-any gate.** pk_all and pickany_vs_retail with `--leaf` per side (HEAD stand-in, new PDB): SAME, GATE PASS, 0 reach flips.
  - Reversed control: PASS.
  - Planted control (a leaf-called NEQ copy added to N): FAIL, as it must.
- **Drop test.** Only the stand-in self is dropped. It has 0 reloc_bindings lines.
  - Scratch scan of 76,571 objects (`evidence/symscan_summary.txt`): 0 UND and no other DEF of the stand-in.
  - Landed binders of the stand-in: none. Three CTCShop sources (0x007743B0, 0x007742C0, 0x007721A0) name it only in historical "was"
    comments.
- **Link** (/FORCE; `evidence/link/`):
  - Owner + UND binder 0x004CA010: HEAD leaves the PDB name unresolved; new resolves it; 0 duplicates.
  - Owner + 0x004CA010 + pick-any emitter 0x00744ED0: HEAD 0 duplicates, new 1 (owner v 0x00744ED0; the bodies are EQUAL).
  - Plus 0x007CBDC0 (pick-any) and 0x00659164 (strong):
    - HEAD has 1 duplicate (the pre-existing 0x00659164 v 0x00744ED0).
    - New has 3: owner v 0x00744ED0 (EQUAL), owner v 0x007CBDC0 (DIFF, 51 B), owner v 0x00659164 (DIFF, 31 B, strong v strong).
    - cmpcomdat: `evidence/cmpcomdat_getdef.txt`. pk_all on the HEAD controls: both DIFF copies are NEQ and leaf-called
      (`evidence/pkall_ctl_*.tsv`).
  - The answer to the brief's question: yes, it collides like 0x0042B040, and also against different-body copies.
    - Equal pick-any: 0x00744ED0. Different pick-any: 0x007CBDC0.
    - Strong same-TU copies with stand-in bodies in ELEVEN landed binders, found by a multiline search of rebuild/src for a non-inline
      `template <>` definition with a body (`evidence/src_getdef_specdefs.txt`):
      - 0x005B43A2, 0x005B4795, 0x005B4EBA, 0x005B5BE6, 0x00659164, 0x0071DA70, 0x007744A0, 0x00801A50, 0x00811470, 0x008115A0, 0x008ED1F0.
      - 0x00659164 is object-verified at HEAD. Six more (0x005B43A2, 0x005B4EBA, 0x007744A0, 0x00801A50, 0x00811470, 0x008115A0) are strong
        in scratch objects; the rest rest on their source lines.
      - 0x0074EC50 defines it `inline` (pick-any).
    - No other landed self defines the PDB symbol (self_symbols.tsv: 0 lines; no `// Leaf:` line names it), so the identity-claims ruling
      does not apply.
    - Scratch census (stale objects included, not HEAD): 96 addresses emit the symbol; 45 EQUAL / 51 DIFF.
- **Variant V2** (`variants/0042af3c_v2.cpp`, evidence only): the template's operator= takes `CDefPointer<const CDefClassBase>` (0x0042B040's
  converting form) and the body is `*out = def`. Leaf equal, RELOCATION_MATCH. It renames one NOADDR inline COMDAT,
  ??4?$CDefPointer@$$CBVCDefClassBase... -> ??4?$CDefPointer@$$CBVCThingBaseDef@@@@QAEAAV0@ABV?$CDefPointer@$$CBVCDefClassBase... (a made-up
  stand-in name), so it fails "only the self name changes" (NF307 D1).

## 0x00428B10 (b2)
- **Text.** The landed `extern "C" void __fastcall DeleteIfSet(Obj*)` became an explicit specialization,
  `template <> void FABLE_FASTCALL CCountedPointer<NUISystem::CComponent>::DeleteData(void* data)`. It keeps HEAD's body and local slot-0
  `Obj` model (`static_cast<Obj*>(data)->Destroy(1)`).
  - It includes engine/CCountedPointer.h, which forward-declares NUISystem::CComponent, under the landed
    `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` opt-in (106 landed rows define it).
  - Measured: with the header's in-class body, VC7.1 emitted NO symbol for the specialization (`variants/00428b10_v1a_inclassbody.cpp`; the
    object holds only .drectve / .debug$S). Under the decl-only member the specialization is a strong definition, as the extern "C"
    function was.
  - Leaf line: the PDB symbol.
- **Names** (two sources):
  - FableWin.pdb name set (exact; the Ego_r name set lacks it).
  - Retail stores the address as the count block's DeleteFunc, `mov dword ptr [eax+4], 0x00428B10`, in Reset 0x00429C6F (instruction at
    +0x1A) and the (T*) constructor 0x00429C15 (+0x15).
  - reloc_bindings line 33029 (witness 0x005331A0, which declares this exact explicit specialization) already carries the PDB symbol.
- **Grade.** vland: MATCH PASS for HEAD and for new; new loses STANDIN_LEAF. objsec: OTHER_SAME, symdelta 0. Selection is 1 on both sides, and
  .drectve and UND are equal. pk: SAME / GATE PASS, 0 reach flips. Drop test: clean.
- **Binders of @DeleteIfSet@4.** There are none.
  - No landed source declares it without defining it.
  - 0 UND in the scratch scan.
  - 11 other landed gapscan rows DEFINE the same stand-in (NAME_FIXES.tsv line 3, not touched).
- **Link:**
  - Owner + UND binder 0x005331A0: PDB name unresolved at HEAD, resolved new, 0 duplicates.
  - Plus pick-any emitters 0x00626750 (EQUAL, 11 B) and 0x005820E7 (DIFF, 8 B plain operator delete; pk_all NEQ, reach y): HEAD 0
    duplicates, new 2.
  - Scratch census: 106 emitting addresses, 88 EQUAL / 18 DIFF (all 8 B). None of the 18 HEAD sources defines DECL_ONLY.
  - No other landed row defines the symbol strongly: a multiline search of rebuild/src finds no `template <>` DeleteData<CComponent>
    definition with a body (one declaration: 0x005331A0). No other self or `// Leaf:` line names it.
  - The links used the object compiled before the final comment edit. The final object differs from it only in the 2 timestamp bytes
    (objsec OTHER_SAME).
- **Pick-any variant v2** (`variants/00428b10_v2.cpp`): explicit instantiation of the header's default `delete static_cast<T*>(data)` over a
  local `namespace NUISystem { class CComponent { public: virtual ~CComponent(); }; }`. This is the family form of the landed DeleteData
  owners (e.g. 0x00428AE7, 0x00B8E690), which use the class header instead.
  - Grade: MATCH PASS. vland new shows no warning, but that only means the check did not fire: engine/NUISystem_CComponent.h exists, so v2
    is still a local model of a class that has a shared header.
  - objsec: OTHER_SAME except selection 1 -> 2. The leaf bytes are EQUAL to the strong text's.
  - pk: PASS. Link with the same four objects: 0 duplicates. A /FORCE link of pick-any copies does not report a body mismatch, so the
    0x005820E7 DIFF copy stays a pick-any race (pre-existing debt).
  - Including engine/NUISystem_CComponent.h instead adds `/DEFAULTLIB:"libcp"` (NF356), so the local model keeps .drectve equal.

## Rulings needed
- **R1 (b1, b2): a strong in-place self that adds LNK2005s against different-body copies.**
  - The standing link proof says "no new duplicate definition with a different body". NF281 D2 / NF283 D3 make a self that duplicates
    another row's strong same-TU copy T3.
  - For landing: the owner's body is the retail one. Every DIFF copy is a pre-existing NEQ stand-in (debt, QUEUE_PROPOSALS.tsv). The eleven
    strong GetDef copiers already collide with each other and with all pick-any copies at HEAD. 0x0042B040 landed in the same strong form
    with EQUAL-body LNK2005s.
  - Options:
    - (a) Accept both strong rows as is.
    - (b) For 0x00428B10, land variants/00428b10_v2.cpp instead: pick-any, 0 duplicates. The cost is selection 1 -> 2 and a local
      CComponent model. This is NF310 D1's "queue the self for the pick-any form".
    - (c) Hold both until the different-body copies are fixed.
  - 0x0042AF3C: a pick-any form was NOT measured here (outside the brief's strong form). NF356's C2945 concerns an explicit instantiation
    after the header's `template<>` specialization; on the local model a primary-template body plus an explicit instantiation (the shape
    0x007CBDC0 already uses for its copy) is a different text and remains open.
- **R2 (b1): the `reinterpret_cast` in the 0x0042AF3C body.** It is the only measured text that keeps every COMDAT name; V2 renames a local
  inline COMDAT onto another made-up name. Is it acceptable source? The other route is the header port NF356 held.
- **R3 (b2):** the same question as R1 for 0x00428B10, with the v2 alternative above. If v2 is chosen, swap `rows/00428b10.cpp` for
  `variants/00428b10_v2.cpp`; its evidence is in `evidence/variant_v2/` and `evidence/objsec/00428b10_v2.txt`.

## Queue / coordinator follow-ups
- TABLE_CHANGES: none. Each address has one reloc_bindings line, already in the PDB spelling, and its witness is unchanged.
- self_symbols.tsv lines 327 (0x0042AF3C) and 270 (0x00428B10) follow at the next self_symbols.py pass (RENAMES.tsv).
- RENAMES file decisions, proposed and NOT applied:
  - 0x0042AF3C `CDefinitionManager_GetDefT_0042af3c` -> `CDefinitionManager_GetDefCThingBaseDef_0042af3c`. engine/CDefinitionManager.h line
    242 cites the old path.
  - 0x00428B10 `global_DeleteData_00428b10` (Module `_global`) -> `CCountedPointerNUISystemCComponent_DeleteData_00428b10` (Module
    `CCountedPointer<NUISystem::CComponent>`), following the sibling rows.
- T3 lines that mention either address (land_scripted holds an address while any is unreleased):
  - line 3590 nf349 0x0042AF3C (column 2, the header port): stays open; the in-place respell does not close it. b1 needs a coordinator
    decision (`--release-t3 nf349` or keep it).
  - line 3591 nf349 0x0042B040 (names 0x0042AF3C in its reason).
  - line 3482 nf340 0x00429F43 (names 0x00428B10 in its reason: NF347's DECL_ONLY route for 0x00429C6F was rejected because this owner
    defined a stand-in).
- 0x00429C6F (NAME_FIXES nf356 line 2, "what blocks a declaration-only DeleteData route"): with b2 (either form) the owner defines the exact
  symbol at MATCH, so a DECL_ONLY reference from 0x00429C6F would now resolve to it. The 8-byte different-body copies remain: an LNK2005
  under the strong form, a pick-any race under v2.
- The 0x0042AF3C row still carries its older sentence "the def type is opaque", which the new Leaf line contradicts. Not edited (a comment
  change would need a recompile); a later text pass may drop it.
- QUEUE_PROPOSALS.tsv: three debt lines.
  - The eleven strong GetDef<CThingBaseDef> copiers (NF307 D5).
  - The GetDef different-body pick-any copies (0x007CBDC0 and others).
  - The 18 DeleteData<CComponent> 8-byte copies (0x005820E7 and others).
  - Not touched.
- NAME_FIXES.tsv line 3: 11 gapscan rows define @DeleteIfSet@4 (one stand-in name, many addresses).

## Process
- **Locks.** Claimed: 0x0042AF3C, 0x00428B10 (rows) and 0x007CBDC0, 0x00744ED0, 0x005331A0, 0x00626750, 0x004CA010, 0x00659164, 0x005820E7
  (HEAD controls, compiled once each, never edited). All released before hand-back (`evidence/locks_final.txt`).
- **Skipped (held by NF358, not compiled):** 0x00626110 and 0x00605570. 0x00429C6F (T3 nf352 column 2) was not compiled.
- **Prechecks.** Each landed test was prechecked (clang PASS) before its lock was claimed, and each source after it. The HEAD row sources
  were first prechecked before the locks and then re-run after claiming them, before any compile. Each new text was prechecked in an earlier
  call than its compile (`work/pre.log`).
- **Comment edit.** The final 0x00428B10 text changed one comment line after the first compile. It was recompiled (r3), objsec v the
  earlier object was OTHER_SAME (2 timestamp bytes differ), and the pk gate and vland HEAD / new were rerun on the final texts.
- **Grade budget.** pass.py:
  - HEAD: 2 rows + 7 controls.
  - New: 0x0042AF3C x2 (V1, V2) and 0x00428B10 x4 (in-class probe, decl-only, v2, final).
  - Plus vland chains b1 x2 and v2 x1.
- Tools: `tools/` (NF356's set retargeted to NF360, plus symscan360.py, copycensus360.py, mkbatch360.py).
- No oracle or retail bytes are in the hand-back; the vland oracles stay in work/.
