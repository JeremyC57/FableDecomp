# NF320 (2026-11-23): alias-class rows with stand-in parameter types (144) and the CCreatureAction Clone ports (23)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Identity per entry: the retail RTTI vftable
placement (ghidra_out/labels_rtti_port.tsv) names the ENTRIES pdb_spelling at the address (prefix). Four shared headers take an add-only
declaration opt-in (hdr/: engine/CEnemyDef.h, CExpressionSubDef.h, CGoldDef.h, CAIStateGroup_MinionReactToCriminal.h; one exact
equiv_check over every landed row and test: 0 differing, hdr/equiv_check.tsv; planted control flagged, hdr/equiv_planted_control.*).

## Result (STATUS.tsv, one line per entry, 167)
- **61 done**, ready to land (57 on existing switches + 4 through the add-only opt-ins) (rows.txt, rows/<addr>.cpp; no test changed: every landed test is a standalone stub or its own copy and
  passed unchanged in vland with the new row).
- **skipped-held** (decisions below): 1 Clone pilot + 8 Clone rows whose header declares Clone (Q1); 0x00B9D060, 0x004FC130,
  0x004FC150 (Q2: new pick-any COMDATs); 6 add-only-declaration rows whose PDB spelling is in FableWin.pdb only (Q3); 5 rows under a
  pass-specific switch or a switch no landed row defines (NF318 Q2: 0x004DE9F3, 0x006FEED0, 0x00933300, 0x00922530, 0x004DE916).
- **skipped-port**: 42 census rows (flat GENERATED header / no header: the virtual member cannot be declared without a new class model),
  14 Clone rows without a class model, 5 CScriptThing members with landed in-TU inline copies (NF318 unit), and body / callee ports
  (stand-in callees, local models built with `new`, casts onto stand-ins, a 12-parameter body written as a zero-argument call).
- **Add-only route (4 done, 1 held)**: 0x004DE33E CEnemyDef / 0x004DE881 CExpressionSubDef / 0x004DEBC4 CGoldDef ::Transfer(CPersistContext&)
  and 0x00930D70 CAIStateGroup_MinionReactToCriminal::InitStateGroup: the override (both PDB name sets; the base declares the identical
  virtual, no new slot) is declared under a new member-named macro, `_TRANSFER_DECL` / `_INITSTATEGROUP_DECL`, inserted just before
  the class's `};`; pin_asserts.py pinned the 8 asserts the insert moved. 0x004DEA03 (CFireballSpellLevelDef) is held: that header is
  in NF319's overlay. Three more candidates were settled by reading: 0x00BC2460 (no header declares the slot: port), 0x008FD892
  (made-up base callee: NF318 Q4 unit), 0x00BCEA40 (its class chain is 0x00B9D060's: dependent on Q2).

## Method (per row)
- Draft: `tools/mkdraft.py <idx> --defs <switches> | --prec <landed precedent>` starts from the HEAD source (orig/), keeps every HEAD
  line except the alias struct (when it only declared M) and the stale "Stand-in (owner rule ...)" comment, replaces the definition head
  by the header declaration's return / parameter types with HEAD's parameter names, adds the full `// Leaf:` and a provenance line.
  `tools/addpre.py` adds the prerequisite switches / includes a header needs (only switches landed rows already define; minimal sets
  instead of large precedent blocks), and the few call-site edits the new types force (`stream` -> `&stream`, `*cs` -> `cs`, NF313 Q2),
  enum returns as `(ERendered)0` (NF307 D3 / NF318 Q3).
- Bodies written onto the real model where the HEAD body read a flat-header stand-in (same bytes, checked by objsec):
  - 0x0049E1A0 CWorld::DrawGetWorldMap: HEAD read "DefinitionManager +0x10 then +4"; the real override runs on the CIDrawWorld
    subobject (+4), so `[ecx+10h]` is PWorldMap (+0x14) and `+4 / null test` is the upcast to CIDrawWorldMap: `return PWorldMap_ptr;`.
  - 0x004FB2E0 CWorldMap::IsMapChangeable: HEAD called a made-up slot-10 virtual; slot 10 of the retail vtable 0x01244AEC is
    0x004FC050 ?IsMapLoaded@CWorldMap@@UBE_NJ@Z (tools/vslot.py reads the exe, labels_rtti_port.tsv names it; the header's
    Ego_r.pdb slot order agrees): `return IsMapLoaded(mapIndex);` (a virtual call: no symbol binding changes).
  - CScriptThing (9 rows): forwarding through the private PImp (CCountedPointer<CScriptThing>) as the landed NScript in-TU copies spell
    it; the pointee's virtual slot is the header's (32, 34, 35, 37, 44, 55, 58, 59; GetName slot 1).
  - 0x009943B0 / 0x00994590 NeedsBufferedRead: `requested < BufferSize` (+0x20 / +0x1C, the header's members).
  - 0x009949F0: the local _Retail view and its CScopedPointer stand-in replaced by the header members (boost::scoped_ptr).
- Proof, per row (all from vland --recheck-landed objects; tools/runpair.sh compiles HEAD and new under the SAME LANDVERIFY_WORK path,
  renamed afterwards, so ?A0x hashes cannot differ, GOTCHAS 315):
  1. precheck: clang -fsyntax-only of the new row, its HEAD source and the landed test (evidence/pre.log; tests before the locks);
     (Steps 1-5 below cover the 57 rows on existing switches; the 4 add-only rows have the same proof plus the header proof in
   "Add-only route proof" below.)
2. vland HEAD (work/<t>h_payload.json, built by the repo's recheck.py) and new (same entries, rows/ source + Leaf): MATCH /
     RELOCATION_MATCH + test PASS on both sides for all 57 (evidence/recheck_b*{h,n}.log);
  3. tools/libcp.py on the two objects: exactly one DEF rename old -> the ENTRIES pdb_spelling (tools/recdone.py asserts it), UND sets
     equal, DEF sets equal under the rename, no ??_7 / ??_R, every non-debug section equal in bytes, flags and relocations, .drectve equal
     (39 rows) or HEAD + exactly /DEFAULTLIB:"libcp" (18 rows, NF318 Q1) (evidence/libcp_b{1..6}_vland.txt);
     0x009949F0 with the explicit two-name map (tools/libcpN.py, evidence/libcp_b5_vland_N.txt): NF304 Q1, see below;
  4. graded symbol = the PDB symbol (vland result lines name it);
  5. link: /FORCE link of all 57 HEAD objects v all 57 new objects (evidence/link_all57.txt): 0 duplicates in either set, the same 8
     unresolved externals in both (nothing newly unresolved).
- Binders: exe census of every done address (tools/nf.py census -> evidence/census.tsv): only 0x00CBFB7D (GetName / MsgOnCutSceneAnimEvent,
  whose reloc_bindings lines already carry the PDB spelling) and 0x0057FF92 (its retail body references 0x0042DA40, but its source names no RemoveSubDef; what it binds there is
  undetermined (object not compiled); its spelling is unchanged by this batch since the source is untouched). No landed row or table line binds any old alias spelling
  (evidence/rb_targets.txt). TABLE_CHANGES.tsv: none.
- Duplicates / new-name census: tools/defgrep.py (evidence/defgrep.tsv) + tools/inclass.py (evidence/inclass.txt): no other landed source defines
  or Leafs the PDB name of a done row (the CScriptThing members that do are skipped).
- Screens: none of my entries is in NF319's ENTRIES / witnesses or under another lock; the 23 Clone addresses are only `nf317` lines of
  T3_PORT_QUEUE (released to me by the prefix).
- vland prints WARN SHARED_HEADER_AVAILABLE on 6 rows (0x00A39BB0, 0x005DBA80/B60/CD0, 0x0088EC60, 0x00892EC0) on HEAD and new alike:
  their HEAD local models (CBankFile, CDataInputStream / CMemoryDataInputStream, CWorld, CQuestManager) are kept as the rule "keep every
  HEAD line" says; header-policy debt, not new.

## Flagged done row: 0x00BC3AF0
CEngineInternalPrimitiveParticleMesh::PeekRenderInfoMeshBase returns `(const CEngineRenderInfoMeshBase*)&RenderInfo_Lights`: the header
flattens the CEngineRenderInfoMeshBase member at +0x7C into its words, so the member's address is cast to the PDB return type. HEAD
already returned that address through a cast (to a local CEngineLocalLightArray*); both sides of the new cast are PDB types and the
address is the member's. Kept as done (objsec ALL_EQ), but flagged: 0x00891320 / 0x00B23B50 went to port because their casts would sit
on stand-in callees.

## Overlap with NF319 (read-only check of ../agentNF319/hdr/engine/)
- NF319 has opt-in copies of engine/CAIStateGroup_BalverineCornered.h and engine/CAIStateGroup_BalverineJumpAtFleeingEnemy.h, which my
  done rows 0x0094F080 / 0x0094F170 include (unchanged repo text here). Their additions are new `_RTTI_DECLS` macros my rows do not
  define, plus pinned asserts; if both batches land, recheck 0x0094F080 / 0x0094F170 on the merged headers (or confirm by equiv_check).
- NF319 also copies engine/CFireballSpellLevelDef.h and engine/CJoystickDX.h (my held 0x004DEA03 / 0x00AB7910 / 0x00AB7920 would need
  those headers' add-only declarations: leave them to NF319's opt-ins).

## Locks
Kept on the 61 done rows and on the 4 drafted held rows (0x0084C6C0, 0x00B9D060, 0x004FC130, 0x004FC150; drafts in evidence/drafts/).

## 0x009949F0 (NF304 Q1)
The move renames two definitions: the self, and the pick-any COMDAT of the scoped pointer's operator->
(??C?$CScopedPointer@VCCompressorZlib@@@@QBEPAVCCompressorZlib@@XZ -> ??C?$scoped_ptr@VCCompressorZlib@@@boost@@QBEPAVCCompressorZlib@@XZ,
the FableWin.pdb name). All sections are byte- and relocation-equal under that map; the new COMDAT is the header template's inline copy
that every includer emits; no landed row defines it strongly. Marked done under NF304 Q1 (flag it if you read Q1 more narrowly).

## Questions
- **Q1 (Clone, 9 rows held):** the pilot 0x0084C6C0 (CCreatureAction_DropGenericBox::Clone on engine/CCreatureAction_DropGenericBox.h)
  grades RELOCATION_MATCH PASS and its vftable relocation resolves to the retail table 0x012731E4. The TU's ??_7 grows from the stand-in's
  1 slot to 70 slots = retail's 70; 68 slot names equal the retail RTTI labels (tools/vtcmp.py, evidence/vtcmp_0084c6c0_new.txt), slot 0
  is ??_E (VC7.1's weak alias of ??_G), and slot 10 differs: retail overrides GetAnimExtraFlags in the derived class (0x0084A820), which
  the header does not declare. The TU now also defines ??_GCCreatureAction_DropGenericBox@@UAEPAXI@Z (it compiles the copy constructor,
  as the deleting-destructor ruling assigns). Is this the port NF317 Q3 means: land when the emitted table matches retail slot by slot
  (after an add-only override declaration in the class header), with ??_G accepted as a new pick-any COMDAT? Draft: evidence/drafts/0084c6c0.cpp.
- **Q2 (3 rows held):** byte-exact moves that also emit pick-any COMDATs HEAD did not: 0x00B9D060 (??_H / ??_I eh vector iterators from
  the base chain's inline constructors), 0x004FC130 / 0x004FC150 (CArray<CMapInfo>::operator[] and CCountedPointer<CMap>::operator*,
  unreferenced). NF318 Q5 holds a move that DROPS a COMDAT; is a move that ADDS unreferenced pick-any COMDATs held the same way? Drafts
  in evidence/drafts/.
- **Q3 (6 rows held):** the member is an override the class model does not declare, and its PDB spelling is only in FableWin.pdb's name
  set (Ego_r has none: ICF / inlined): 0x00A7D950, 0x00A7D960, 0x00A84E20, 0x00AB7910, 0x00AB7920, 0x00B4B6B0. The retail RTTI vftable
  placement names them. Does RTTI + FableWin satisfy "both PDB name sets agree" for an add-only declaration opt-in?

## Add-only route proof (4 rows)
- The four `_DECL` macros are the prefix's add-only declaration route (declarations only, both PDB name sets, new member-named macro),
  not NF318 Q2 class-model switches: their class-model switches (FABLE_RETAIL_CENEMYDEF, _CEXPRESSIONSUBDEF, _CGOLDDEF,
  _CAISTATEGROUP_MINIONREACTTOCRIMINAL) all have landed definers (evidence/row_switch_definers.tsv; the four _DECL macros show 0).
- Landing: apply hdr/<X>.diff onto the current header (not the full-file copy) and re-run pin_asserts.py on the merged header, then the
  4 rows. NF319's ENTRIES and hdr/ touch none of these four headers (checked at hand-back).
- Duplicate-definition grep with a positive control: evidence/dupgrep_control.txt (CScriptThing::GetPos 86 hits; the 4 members 0).
- hdr/engine/<4 headers> (full copies of today's headers plus the opt-in block and the pinned asserts), hdr/<X>.diff, orig/hdr_<X>.h.
- equiv_check.py repo-tree v new-tree over every landed source and test: hdr/equiv_check.tsv (0 lines = 0 differing rows).
- Planted control: hdr/equiv_planted_control.txt / .tsv.
- Rows: vland HEAD on the repo headers, new with FABLE_HDR_OVERLAY = the new tree (evidence/recheck_b8{h,n}.log: 3 RELOCATION_MATCH +
  1 MATCH, PASS both sides); objsec evidence/libcp_b8_vland.txt (one DEF rename, .drectve equal); link evidence/link_b8.txt (no
  duplicates, the same unresolved ??$Transfer@J in both sets); no landed caller (evidence/census_b8.tsv), no other definition.
- Each Transfer row keeps HEAD's local CPersistContext template model, so the ??$Transfer@J@CPersistContext@@QAEXAAJ@Z UND is unchanged
  (the member at +0x28 is now named: Faction / ExpressionDef / Gold).
- One foreground `tail --pid` wait on the background equiv_check (no sleep, but a foreground wait).

## Re-check after NF319's landing (coordinator note, 2026-11-23)
- The repo now carries the NF320 work: all 61 done rows are in rebuild/src/compiled and match rows/ byte for byte
  (work/landed_nf320.txt), and the four repo headers match hdr/engine/ (CEnemyDef, CExpressionSubDef, CGoldDef,
  CAIStateGroup_MinionReactToCriminal). NF319's 31 headers do not include these four. hdr/ is therefore based on the current repo copies,
  and the assert pins are unchanged.
- Old v current tree for my rows: equiv_check from work/tree_new (pre-NF319 + my 4) to work/tree_cur (current repo + my 4) over the 61
  done rows gives 0 differing lines (work/equiv_mine61.tsv). Two rows include NF319 headers: 0x0094F080 includes BalverineCornered.h and
  0x0094F170 includes BalverineJumpAtFleeingEnemy.h. Neither defines NF319's opt-in macros, so the compiler input is identical and no
  vland recheck is needed.
- Full check against the current tree: equiv_check of rebuild/include (current) v work/tree_cur over every landed row is written to
  hdr/equiv_check_cur.tsv and hdr/equiv_check_cur.out. This is an identity check, because the current tree already holds my headers.
- Planted control on the current tree: work/tree_ctl_cur is tree_cur plus the same token in CGoldDef.h's default branch
  (hdr/equiv_planted_control_cur.tsv). It flags 0x004E0FED (source and test) and none of the definers. 0x004DEBC4 is no longer flagged
  because its landed row now defines FABLE_RETAIL_CGOLDDEF, which is the expected outcome.
- Held rows: none of the 25 held rows touches an NF319 header, except 0x004DEA03. That row is still held on NF319's Fireball header
  (CFireballSpellLevelDef.h), which has now landed. NF319's opt-in is landed precedent, so 0x004DEA03 can be re-drafted on it in a
  later batch (not done here; it stays held).
- Slip: equiv_check over the 61 rows (a gcc cpp pass, no VC7.1) ran in the foreground with timeout 900. The full check and the control ran
  in the background with nohup and a recorded PID.
