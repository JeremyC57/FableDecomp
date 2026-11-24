# NF263 notes (2026-11-22, round 25): L129 GB unit done (0x00BF36A0 / 0x00BF36E0 / 0x00BF9920 + binders 0x00BDD3E0 / 0x00BDFD60); the rest handed back as a plan

Handed back after one finished L129 unit (brief: "hand back after any finished unit"). 5 rows changed, all RELOCATION_MATCH + test PASS
(recheck_final.log: one `vland --recheck-landed` run over the 5 rows plus the unchanged witnesses 0x00BF36C0 / 0x00BDE600 / 0x00BF5D30:
8 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT). No header change (hdr/ holds only a README, no orig/), so no equiv_check. No new CompilerFlags.
Repo root (`ls -A`) before / after: identical. No git, no sleep; every compile ran after its lock was NF263 and its clang precheck passed
(new sources, new tests, the HEAD sources and HEAD tests of all 8 rows), in a later tool call, under an outer `timeout 900`.

## The GB unit: what the three rows are
- Retail bodies (xdis): `mov eax,[0x01436EA8] ; push ecx ; mov ecx,[eax+6B0h] ; call 0x00BD9B90 ; ret` (18 bytes each) =
  `GPLandscapeRenderer->MemoryManager->FreeVirtual(block)`.
- Callers (exe scan of E8 / E9 and absolute references): 0x00BDD3E0 (CEngineLandscapePatch vector deleting destructor: scalar arm
  0x00BDD435 -> 0x00BF36A0, array arm 0x00BDD418 -> 0x00BF36E0), 0x00BDFD60 (CLocalDetailPrimitiveMesh scalar deleting destructor,
  0x00BDFD73 -> 0x00BF9920), and two unlisted stubs `xor edx,edx ; jmp` at 0x00BDBF70 -> 0x00BF36A0 / 0x00BDBF80 -> 0x00BF36E0.
  EVERY call site zeroes edx first, so the callee is the two-argument `__fastcall` form.
- Name (two sources each): Ego_r.pdb and FableWin.pdb both name the placement forms
  `??3CEngineLandscapePatch@@SIXPAXPAVCMovableResource@@@Z`, `??_VCEngineLandscapePatch@@SIXPAXPAVCMovableResource@@@Z`,
  `??3CLocalDetailPrimitiveMesh@@SIXPAXPAVCMovableResource@@@Z`; retail's quartets 0x00BF3680 / A0 / C0 / E0 and 0x00BF9900 / 20 / 40 / 60
  (??2 / ??3 / ??_U / ??_V: alloc, free, alloc, free bodies) follow Ego_r's 0x20-spaced quartet order (00a37aa0.. / 00aaab10..); the landed
  siblings 0x00BF36C0 (`??_U...SIPAXIPAVCMovableResource@@@Z`) and 0x00BF9900 (`??2...`) already use the placement form, and 0x00BDE600
  names `??2CLocalDetailPrimitiveMesh@@SIPAXIPAVCMovableResource@@@Z`.
- **D1 (contradicts the brief's wording):** the FableWin one-parameter forms `??3CEngineLandscapePatch@@SIXPAX@Z` / `??_V...SIXPAX@Z` are NOT
  0x00BF36A0 / 0x00BF36E0: they are the stubs 0x00BDBF70 / 0x00BDBF80 (`xor edx,edx ; jmp` to the placement form; FableWin order
  GetFileHeaderSize 02cc8410, ??3 02cc84c0, ??_V 02cc84e0 = retail 0x00BDBF60 `mov eax,24h ; ret`, 0x00BDBF70, 0x00BDBF80). The bytes are
  not the same (the plain form is a 2-instruction forwarder). REJECTED.tsv has the plain-form claim; NAME_FIXES.tsv has the stubs (one PDB
  + body: not applied, not catalogued; manifest gaps). FableWin also has `??3CLocalDetailPrimitiveMesh@@SIXPAX@Z` (02e36910); its retail
  stub was not looked for (no caller of 0x00BF9920 other than 0x00BDFD60).

## Changes (STATUS.tsv)
- Self rows 0x00BF36A0 / 0x00BF36E0 / 0x00BF9920: rewritten in the shape of the landed sibling allocators (0x00BF36C0 / 0x00BF9900): a
  header-less local `class CEngineLandscapePatch` / `class CLocalDetailPrimitiveMesh` with only the static member, on the landed
  `FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER` and the retail branch of engine/CEngineLandscapeMemoryManager.h (NF262 unit 3
  recipe; unlike the siblings, no row-local CEngineLandscapeMemoryManager model). Full mangled `// Leaf:` lines (HEAD had none).
  CEngineLandscapePatch's only header is `engine/_quarantine/CEngineLandscapePatch.h` (header-less for the rule, EXTRA_RULES 2026-11-20);
  CLocalDetailPrimitiveMesh has no header anywhere under rebuild/include. Both are the landed siblings' exact shape, no base (new-local-model
  rule: all three conditions hold; listed here as required). .text identical to HEAD; the relocations differ only in the global, the callee and
  the self symbol. HEAD defined `?fn_00bf36a0@@YIPAXH@Z` in BOTH 0x00BF36A0 and 0x00BF9920 (one symbol, two addresses): gone.
- Binder 0x00BDD3E0: in-place respell of the row's existing header-less `class CEngineLandscapePatch` (four allocator / deallocator
  declarations: second parameter `long` -> `CMovableResource*`, plus `class CMovableResource;`; CMovableResource has a header, a forward
  declaration is not a model). DEF list identical, every section's bytes identical to HEAD (only the debug-info work path differs), the
  relocation lists differ only in ??3 / ??_V / ??_U. vtcheck of `??_7CEngineLandscapePatch@@6B@` v 0x012A8200: unchanged from HEAD (the model
  has 1 of retail's 3 slots: HEAD debt). pk.py: identical to HEAD (the in-class plain forwarders `??3CEngineLandscapePatch@@SAXPAX@Z` /
  `??_V...SAXPAX@Z` are 11-byte cdecl pick-any copies with no owner, HEAD debt: retail's plain forms are the fastcall stubs 0x00BDBF70 / 80;
  making them `__fastcall` would give FableWin's spelling and the stubs' bytes, but that widens the unit: queued). The `MakeSome` scaffold's
  `??_U` UND moves onto the landed 0x00BF36C0 spelling (scaffold-only: retail 0x00BDD3E0 does not call 0x00BF36C0, so this pair is
  not in RENAMES section 2 and an exe-scan gate will not see it; it is an object-name cleanup only).
- Binder 0x00BDFD60: the free stand-in `CLocalDetailPrimitiveMesh_dealloc` -> `CLocalDetailPrimitiveMesh::operator delete(mesh, 0)` on the
  row's own forward-declared struct, now given that one static member (key kept `struct`, so the row's own stand-in leaf
  `?CLocalDetailPrimitiveMesh_scalar_deleting_destructor@@YIPAXPAUCLocalDetailPrimitiveMesh@@HI@Z` is unchanged; retail's key is `class`:
  HEAD debt with the stand-in leaf). Added its full `// Leaf:` line (HEAD had none). One relocation symbol differs from HEAD, .text identical.
  The table line `?CEngineSubPrimitiveGameMesh_dealloc...` (witness 0x00BDFD60) was already stale at HEAD: the HEAD object binds
  `?CLocalDetailPrimitiveMesh_dealloc...` (untabled).
- Tests: the three self rows' tests are rewritten on the shared headers (stub FreeVirtual and the renderer global, raw storage, no
  constructors; they link the row object; catalog PassPatterns kept, incl. `PASS_00bf9920`; 0x00BF9920's old test only printed PASS).
  0x00BDD3E0 / 0x00BDFD60 tests unchanged (self-contained host models).

## Census (spelling counts; minus one each is proven by symbol, absolute counts are table lines + the object scans above)
- 0x01436EA8: `?g_00bf36a0@@3PAUGB@@A` (0x00BF36A0, 0x00BF9920) and `?g_00bf36e0@@3PAUGB@@A` (0x00BF36E0) gone: table 14 -> 12 lines
  (PDB + 11 stand-ins; + the untabled `?g_obj_01436ea8` = 12 stand-in spellings on 22 rows left).
- 0x00BD9B90: `?m@That2@@QAEPAXH@Z` gone (3 rows): table 2 -> 1 (untabled `LandscapeMemoryMethods::FreeAlignedVirtual` in 0x00BFAAE0 remains).
- 0x00BF36A0: 2 -> 1 (self `?fn_00bf36a0`, binder `...SIXPAXJ@Z`); 0x00BF36E0: 2 -> 1; 0x00BF9920: 3 -> 1 (self `?fn_00bf36a0`, binder
  `?CLocalDetailPrimitiveMesh_dealloc`, stale table `?CEngineSubPrimitiveGameMesh_dealloc`); 0x00BF36C0: the object-only `??_U...SIPAXIJ@Z`
  (MakeSome) gone. Source grep of rebuild/src, include, tests: no other file names GB / That2 / g_00bf36a0 / g_00bf36e0 / fn_00bf36a0 /
  fn_00bf36e0 / the `long` forms / the dealloc stand-ins. No file includes the five sources (no funclet or folded row moves).
- Link (5 new objects + HEAD 0x00BDE600 / 0x00BF5D30, LINK cleared, /FORCE /NODEFAULTLIB): 0 duplicates; control (5 new + a second copy of
  0x00BF36A0): 1 LNK4006 on `??3CEngineLandscapePatch@@SIXPAXPAVCMovableResource@@@Z`.

## Recheck WARN lines (recheck_final.log)
- STANDIN_LEAF on the three self rows and RELOC_NAME on the two binders: the catalog label `CEngineSubPrimitiveBase::operator_new` (fixed by
  RENAMES section 1). STANDIN_LEAF on 0x00BDD3E0's `??_E` (bare Leaf, HEAD state). SHARED_HEADER_AVAILABLE on the unchanged witnesses
  0x00BF36C0 / 0x00BF5D30 (row-local CEngineLandscapeMemoryManager: HEAD debt, queue their port onto the retail branch, as done here) and
  0x00BDE600 (CMovableResource / CLocalDetailCacheMap: HEAD debt).

## Plan for what is left of L129 (12 stand-in spellings on 22 rows; none changed here)
- **0x00BE5C30 (`?g_obj_01436ea8`, untabled) + 0x00BE6090 (`?gRenderer@Native`) with 0x00BDA110:** the second source is found:
  `?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z` is in the FableWin name set (02cb7490) as well as Ego_r (009c26a0).
  Needs an add-only declaration opt-in (e.g. `FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETETEXTURE`) in the retail branch of
  engine/CEngineLandscapeMemoryManager.h with an equiv_check proof, then both rows; 0x00BDA110 2 -> 1. Read 0x00BDA110's body first.
- LocalDetailEngine 0x00BFAAE0 / 0x00BFB590, 0x00B684F0 (ports), the CEngineGlobal rows (last), 0x00BEB4D0's `?DAT_01436ea8@@3PAEA`, and the
  rest as NF262 listed them.

## Entry 2 (0x009B0AC0): open, not attempted (NF260 plan stands).

## Decisions for the coordinator
- D1: the GB self leaves are the PDB placement forms (both PDBs + callers' `xor edx,edx` + quartet order), not the brief's FableWin
  one-parameter forms, which are the retail stubs 0x00BDBF70 / 0x00BDBF80. Accept?
- D2: 0x00BDFD60's forward-declared `struct CLocalDetailPrimitiveMesh;` now carries one static member (header-less class, landed siblings
  0x00BF9900 / 0x00BDE600 use the same member on `class`); the `struct` key is kept so the row's stand-in self leaf does not change.
  Accept, or prefer the key fix with a self rename of the stand-in leaf (a vtable binder census first)?
- D3: queue 0x00BDD3E0's cdecl in-class plain forwarders (`SAXPAX`, HEAD debt) -> `__fastcall` (FableWin `SIXPAX`, the 0x00BDBF70 / 80
  bytes), and the catalog entries / names for the five uncatalogued neighbours in NAME_FIXES.tsv?

## Flags / funclets / descriptive names / locks / slips
- Flags: none (no row has CompilerFlags; none added). Funclets: none. Descriptive names: none added; GB / That2 / fn_00bf36a0 / fn_00bf36e0 /
  CLocalDetailPrimitiveMesh_dealloc removed from code.
- Locks (rows.txt, NF263): the 5 changed rows and the 3 witnesses. Release with the landing.
- Reading: the chained briefs NF262 back to NF215, NF214's read-first list (NF92 brief / lessons / notes, NF44 brief, NF90 / NF89 / NF88
  lessons (NF89's was read late, after the unit was graded, because a glob had matched NF90 / NF91 / NF92 instead), ASM_RULES, EXTRA_RULES,
  COORDINATION), the nf262 / nf260 READMEs and hand-backs, GOTCHAS.md end to end. The two CEngineLandscapeMap tests that mention operator
  delete (0x00BDD210 / 0x00BDD2F0) were checked: they model only the global operator delete / delete[], not the class forms.
- Slips: two read-only Bash calls exited 1 (a callers.py / table-grep loop whose last grep found nothing, and a grep for tests including the
  landscape headers); no compile was involved. The final recheck's `timeout 900` sits on the vland call inside tools/recheck_final.sh, not on
  the background launch line. RENAMES section 1 `name` uses the catalog's underscore form (`operator_delete`, `operator_delete[]`).
