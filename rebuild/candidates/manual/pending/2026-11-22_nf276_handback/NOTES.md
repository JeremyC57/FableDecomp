# NF276 notes (2026-11-22, round 32)

Entries 1 and 2 done; entry 3 (landscape host-model tests) not started (context). No header touched (hdr/ and orig/ empty, no
equiv_check needed). Seven rows in rows/ (no test changed: every row keeps its landed test). Two vland --recheck-landed runs
(evidence/e1_recheck.log, evidence/e2_recheck.log) plus recheck_final.log over all eight changed / witness rows (after the enum change).

## Entry 1: the CLandscapeLayerMesh unit -- done
Rows: 0x00BFDE20 (self rename), 0x00BFDDB0 (self rename, added: see below), 0x00BFDD70 / 0x00BFDE80 (base ports). Witness: 0x00BFE900 (??_G).
- **0x00BFDE20** `??1CLandscapeLayerMesh@@QAE@XZ` -> `??1CLandscapeLayerMesh@@UAE@XZ` (Ego_r.pdb 00a494c0 + FableWin.pdb 02ddd720; the body
  stores vtable 0x012A835C first; its only caller, ??_G 0x00BFE900, already binds UAE: table line `??1CLandscapeLayerMesh@@UAE@XZ 00bfde20
  00bfe900`). Port: the shared retail engine/CMovableResource.h (virtual dtor) + the retail branch of engine/CEngineLandscapeMemoryManager.h with
  the landed DELETEINDEXBUFFER / DELETEVERTEXBUFFER opt-ins (as 0x00BFDD40); the local CLandscapeLayerMesh (quarantined header) in 0x00BFDE80's
  Ego_r layout with both private virtual overrides (RelocateData EAE, CanRelocateData EBE_NJ) so the emitted vtable binds retail's slots; +0x3C is
  `Next` (Ego_r; was the stand-in struct `CLandscapeLayerMeshOwned_00bfde20* m_pOwned`); class delete inline from FableWin.pdb
  `??3CLandscapeLayerMesh@@SIXPAX@Z` (02ddd700; Ego_r has only the placement form, so inline in retail): FreeVirtual 0x00BD9B90.
  - Leaf (evidence/e1_00bfde20_leaf_cmpobj.txt): 93 = 93 bytes with relocation fields zeroed, relocation lists equal (HEAD QAE v new UAE);
    RELOCATION_MATCH (q.py and vland). sc.sh relocation diff empty.
  - New COMDATs in the TU: `??_GCLandscapeLayerMesh@@UAEPAXI@Z` 38 bytes, pk EQ 0x00BFE900 (calls ??1 out of line, then FreeVirtual), cmpobj
    SAME against the HEAD 0x00BFE900 object; `??_GCMovableResource` EQ 0x009D5110; `??1CMovableResource@@UAE@XZ` (7 bytes, no retail owner)
    SAME as 0x00BFE900's copy; `??_7CLandscapeLayerMesh@@6B@` / `??_7CMovableResource@@6B@`.
  - vtcheck / vtmap (evidence/e1_vtcheck.txt): ??_7CLandscapeLayerMesh 4 slots on retail 0x012A835C's targets: 0 ??_E (alias of ??_G) ->
    0x00BFE900, 1 ?RelocateData@CLandscapeLayerMesh@@EAEXJPAX0J@Z -> 0x00BFDE80 (SAME as its landed Leaf), 2
    ?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z -> 0x00BFDDB0 (its row now defines that leaf), 3 ?Verify@CMovableResource@@UBEKXZ -> 0x009D5100
    (no landed definer: nf272 NAME_FIXES, queued). ??_7CMovableResource: the shared header's table, as every other emitter (slot 2 / 3 owner-less,
    carried). NOLINE everywhere = no table line for vtable slots (as nf272).
  - Link (evidence/e1_link_order.txt, /OPT:NOREF /MAP, LINK unset): the emitters of ??_7CLandscapeLayerMesh are 0x00BFDE20 and 0x00BFE900 only
    (source grep: no other landed row defines a CLandscapeLayerMesh ctor / dtor or news one). 0 duplicates in every order. Slot 0: both ??_G copies
    SAME, so slot 0 is retail's body in every order. The kept table follows link order: new 0x00BFDE20 first -> its correct 4-slot table;
    0x00BFE900 first -> 0x00BFE900's HEAD 3-slot table (slot 1 ?Verify@CMovableResource, slot 2 ?CanRelocateData@CMovableResource). At HEAD the
    choice was 0x00BFE900's 3-slot table or 0x00BFDE20's 1-slot table (`?v00@CMovableResource@@UAEXXZ`, unresolved), and ??1...UAE was
    unresolved (HEAD defined QAE); now it resolves to 0x00BFDE20. No order is worse than HEAD (decision D1).
- **0x00BFDDB0 (added to the unit)** `?CanRelocateData@CLandscapeLayerMesh@@QAE_NH@Z` (stand-in) -> `?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z`
  (Ego_r.pdb 00a493d0 + FableWin.pdb 02dde0c0; RTTI vtable 0x012A835C slot 2 = 0x00BFDDB0). Why: the new 0x00BFDE20 table binds EBE_NJ at slot 2;
  without the self move that would be one more spelling at 0x00BFDDB0 and an unresolved slot. The exe scan finds no call site (vtable only), so this
  is a leaf-only self rename under the brief rule (both PDBs agree, no landed referencing row). Port: the same local CLandscapeLayerMesh model on the
  shared CMovableResource; `struct S` (the stand-in for the map) becomes a local CEngineLandscapeMap in the landed 0x00BF7F70's shape (its only
  header is quarantined: header-less for the local-model rule; `MemoryFreeingLocked` +0x5C); body `return Map->MemoryFreeingLocked == false;`.
  MATCH; objsec ALL_EQ with the leaf renamed (evidence/e1_00bfddb0.txt). Graded forms: 3 (FableWin's inline `IsLockedFromMemoryFreeing()`: MATCH
  but VC7.1 emitted a pick-any copy of it, a new owner-less name, so not used; `== false` with the accessor present: MATCH; final, accessor removed:
  MATCH). Its landed host-model test is unchanged (self-contained; entry 3 debt).
- **0x00BFDD70** (base port): the shared CMovableResource replaces the row-local one (placeholder `virtual void v00()`); the local
  CLandscapeLayerMesh takes 0x00BFDE80's Ego_r layout (the HEAD names had +0x34 / +0x38 the wrong way round, as NF269 found in 0x00BFDE20); the two
  unused local-manager declarations AppendFreeEdgeNode / FreeBlock respelled in place to DeleteIndexBuffer / DeleteVertexBuffer (no call, no symbol);
  the local manager model stays (RebindBlockOwner 0x00BD9AB0 has 7 spellings already; not touched). Full Leaf line added (pinned: the HEAD object
  defines `?OnPostRelocation@CLandscapeLayerMesh@@QAEXXZ`). objsec ALL_EQ against HEAD (2 sections).
- **0x00BFDE80** (base port): the shared CMovableResource replaces the row-local one. objsec ALL_EQ against HEAD (4 sections). Its local CIndexBuffer
  (SHARED_HEADER_AVAILABLE warning) is HEAD debt, unchanged.
- Recheck (evidence/e1_recheck.log): 0x00BFDE20 / 0x00BFDD70 / 0x00BFDE80 / 0x00BFE900 RELOCATION_MATCH PASS, 0x00BFDDB0 MATCH PASS. 0x00BFDE20 now
  has no warnings (its local CMovableResource / manager models are gone).

## Entry 2: D4 S3 with the owner -- done (measured; one landing set)
- **S3 regenerated** from the landed S1 rows 0x00BDA150 / 0x00BD9CE0: only `// Flags: /d2inlT114` and an `// Inline-budget:` line added after the
  Leaf line (rows/). q.py: both leaves RELOCATION_MATCH, relocation lists unchanged; objsec against the landed S1 objects NOT_EQ only in the five
  TU-mate COMDATs, cmpobj every other COMDAT SAME (evidence/e2_s3_grade_pk_objsec.txt). pk: FreeVirtual 68 -> 97 bytes, EQ 0x00BD9B90; Deallocate
  63 -> 92, EQ 0x009A4350; the three BlockWhileBusy helpers 72 / 72 / 75 -> 101 / 101 / 104 (NEQ both ways; retail 0x00BD9C00 / 0x00BD9C20 /
  0x00BD9CC0 are 29 / 29 / 28 and have no landed row). vland (evidence/e2_recheck.log): both RELOCATION_MATCH PASS with the flag.
- **Owner 0x00BD9B90 to pick-any (nf88):** FreeVirtual defined `inline` and its address taken by an anonymous-namespace function
  (`ReferenceFreeVirtual`, `// Fidelity: scaffolding, not retail`, the 0x00686F10 form). cmpobj SAME for all four COMDATs including the leaf; the
  five HEAD sections objsec-equal, plus one new section (the 6-byte helper, `?A0x` hash, per TU); FreeVirtual's COMDAT selection NODUPLICATES (1)
  -> ANY (2) (evidence/e2_owner_pickany.txt, tools/comdatsel.py). RELOCATION_MATCH (q.py and vland). Full Leaf line added (HEAD had none).
- **Link orders** (evidence/e2_link_orders_owner_pickany.txt, every order of the three objects): new owner + S3: 0 duplicates in all three orders,
  the kept FreeVirtual is always a 97-byte EQ copy (owner's or S3's). New owner + landed S1: 0 duplicates, but with S1 first the kept copy is S1's
  68-byte NEQ body: so the owner change must not land without S3. HEAD owner + S3: 1 or 2 LNK4006, kept copy EQ either way. HEAD set (owner + S1):
  1 or 2 LNK4006, NEQ 68-byte copy kept when S1 links first.
- Census: FreeVirtual has three body texts (owner + the two TU-mates; source grep), all EQ under S3; no new symbol name except the per-TU
  anonymous helper (one definer by construction). No binding changes (leaf relocations unchanged).
- CompilerFlags: S3 needs `/d2inlT114` on 0x00BDA150 and 0x00BD9CE0 in build_candidates.ps1 (TABLE_CHANGES.tsv; GOTCHAS 449 / 499: the Flags line
  alone does not reach the real build). The owner keeps its `/d2inlT114`.

## Entry 3: landscape host-model tests -- open (not started: context)
- Not started. Still host-model debt: the landed tests of 0x00BFDE20 (entry 1 did not rewrite it), 0x00BFDDB0 (joins the list), and the brief's
  0x00BE6090 / 0x00BFD480 / 0x00BFAAE0 / 0x00BFB590 / 0x00B684F0. Every row here keeps its landed test, which still PASSes (self-contained).

## Enum regrade (after the advisor's local-type check)
- The four layer-mesh rows (0x00BFDE20 / 0x00BFDD70 / 0x00BFDDB0 / 0x00BFDE80) were regraded after the enum moved to the shared header:
  each object ALL_EQ to its earlier NF276 object (evidence/e1_enum_regrade.txt), so every entry-1 measurement above stands; recheck_final.log is
  the run after this change.

## Census (names and addresses)
- 0x00BFDE20: spellings {self QAE, binder 0x00BFE900 UAE} -> {UAE} (2 -> 1). 0x00BFDDB0: leaf QAE_NH -> EBE_NJ; vtable-data bindings
  {0x00BFE900 table: ?CanRelocateData@CMovableResource@@UBE_NJ@Z (HEAD debt, unchanged)} + {0x00BFDE20 table: EBE_NJ = the new leaf}: no growth.
  0x00BFDD70 / 0x00BFDE80 / 0x00BD9B90 / 0x00BDA150 / 0x00BD9CE0: no binding change. 0x012A835C: {??_7CLandscapeLayerMesh@@6B@ (0x00BFDE20),
  ?CLandscapeLayerMesh_vtbl@@3PAXA (0x00BFDDD0)} unchanged (NAME_FIXES).
- New symbol NAMES and definers (source grep of src/compiled, include, tests; comments excluded): `??1CLandscapeLayerMesh@@UAE@XZ` one body
  (0x00BFDE20; 0x00BFE900's test defines its own copy in its test TU only); `?CanRelocateData@CLandscapeLayerMesh@@EBE_NJ@Z` one body (0x00BFDDB0);
  `?ReferenceFreeVirtual@?A0x..` one (0x00BD9B90). `??_GCLandscapeLayerMesh` / `??_7CLandscapeLayerMesh`: compiler-generated in 0x00BFDE20 and
  0x00BFE900 (??_G copies SAME). Names that leave: `??1CLandscapeLayerMesh@@QAE@XZ`, `?CanRelocateData@CLandscapeLayerMesh@@QAE_NH@Z`,
  `??1CMovableResource@@QAE@XZ` and `?v00@CMovableResource@@UAEXXZ` (0x00BFDE20's local-model copies).
- RENAMES.tsv: section 1 has the two self renames (catalog names and file names unchanged); section 2 empty (no binder moved).

## Decisions for the coordinator
1. **D1, 0x00BFE900's local models (carried HEAD debt made visible):** the ??_G row emits its own 3-slot ??_7CLandscapeLayerMesh and
   ??_7CMovableResource (wrong slot order) and pick-any bodies `?Verify@CMovableResource@@UBEKXZ` (3 bytes, returns 0; retail 0x009D5100 returns
   0x7B) and `?CanRelocateData@CMovableResource@@UBE_NJ@Z` from a local CMovableResource. With entry 1 landed, link order decides between its table and
   0x00BFDE20's correct one, and slot 3 of the correct table resolves to 0x00BFE900's wrong Verify body whenever 0x00BFE900 is linked (no landed
   definer of Verify otherwise). Proposed follow-up (not measured here): re-port 0x00BFE900 as the fold of 0x00BFDE20's file (the nf251 / 0x00BEB4D0
   pattern; the ??_G copy in 0x00BFDE20's TU is already pk EQ 0x00BFE900), which removes ForceNew_00bfe900, the local models and both wrong tables;
   check first whether its 20 KB host test links against the row (it defines its own ~CLandscapeLayerMesh and ForceNew_00bfe900).
2. **0x00BFDDB0 inside entry 1:** accept the leaf-only self rename as part of the unit (it keeps 0x00BFDDB0 from gaining the new table's spelling
   as a third one), or land entry 1 without it (then slot 2 of the new table is an unresolved name, as CWaterPatchMesh slot 2 is)?
3. **Entry 2:** land S3 (two rows + CompilerFlags `/d2inlT114` on both) and the owner pick-any 0x00BD9B90 as ONE set. Either half alone is
   no better than HEAD in some link order (owner alone + S1: a NEQ 68-byte copy can be kept with no warning at all, which is worse than HEAD's
   LNK4006). The BlockWhileBusy helper copies grow 72 -> 101 bytes under S3 (still NEQ, owner-less, no landed retail rows).
4. Landing sets: {0x00BFDE20, 0x00BFDDB0} together (the table and slot 2's leaf); {0x00BFDD70}, {0x00BFDE80} each independent (objects unchanged);
   {0x00BDA150, 0x00BD9CE0, 0x00BD9B90 + the two CompilerFlags} together.

## Flags / funclets / descriptive names / locks / order / reading / slips
- Flags lines: 0x00BDA150 / 0x00BD9CE0 gain `// Flags: /d2inlT114` (CompilerFlags change needed, TABLE_CHANGES.tsv). 0x00BD9B90 unchanged.
- Funclets / path includers: no file includes any of the seven changed files (grep of src / tests / include); no file rename.
- Descriptive names: `ReferenceFreeVirtual` (scaffolding helper in an anonymous namespace, never called; descriptive, marked Fidelity).
  Member names are Ego_r's (Next, IndexBuffer, ForegroundVertexBuffer, UsesSharedIndexBuffer, Map, MemoryFreeingLocked).
- Enum: 0x00BFDE80's local `enum LANDSCAPE_TEXTURE_MAPPING_DIRECTION { LANDSCAPE_TEXTURE_MAPPING_DIRECTION_DEFAULT };` (a made-up enumerator) is
  gone from all four layer-mesh rows: they define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_MAPPINGBLEND and include engine/CEngineLandscapeMeshBuilder.h,
  which owns the global enum (GOTCHAS line 14 pattern). Regraded: see "Enum regrade" below.
- Locks (rows.txt): 00bfde20 00bfdd70 00bfde80 00bfdd40 00bf38f0 00bfe900 00bfddb0 00bda150 00bd9ce0 00bd9b90 (released / new -> NF276).
  0x00BFDD40 / 0x00BF38F0 were claimed but never compiled. None of NF275's rows touched (its rows.txt: 00418dca).
- Order: every compile ran after its lock was NF276 and after a passing clang precheck of the code it graded (new and HEAD sources, all
  tests; pre.sh control with a missing include caught; two exceptions under Slips; 0x00BDA150's landed test fails clang only through VC7.1 `<list>` / `<functional>` / `<cwchar>`, the same
  three errors as a two-line `<list>` control); each grade's background launch line carried `timeout 900`; entry 1's recheck finished before
  entry 2's first compile. Link checks ran in the background and were read with kill -0 in a later call.
- Reading: this brief, NF274 / NF272 briefs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md (all), COORDINATION.md, GOTCHAS.md (all 503 lines), the
  nf267 / nf272 / nf274 landing READMEs, the NF274 and NF272 hand-backs (NOTES, LESSONS, REJECTED).
- Slips: two grades were launched in the SAME call as the precheck of the file they graded, after a comment-only edit of a source that had
  passed an earlier precheck: 0x00BFDD70 (line-1 wording fix, pre.sh, then `gr.sh e2` in one call) and 0x00BFDDB0 (comment-line reorder, pre.sh,
  then `gr.sh e5` in one call). Both prechecks passed and both rows were graded again later (enum change, final recheck) after separate prechecks.
  No git, no sleep, no rm, no __asm / naked / _emit; nothing written under /home/user/FableDecomp (find -newer BRIEF.md: 0 files). Helpers added:
  tools/leafcmp.py (leaf bytes + relocations across a rename), tools/comdatsel.py (COMDAT selection per symbol).
