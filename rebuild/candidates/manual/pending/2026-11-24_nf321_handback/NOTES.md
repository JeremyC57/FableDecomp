# NF321 (2026-11-23/24): body and callee ports onto the real class model (81 entries)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Entries: the nf317 / nf318 / nf319 / nf320
skipped-port / held rows whose member exists on the right class but whose body could not move onto engine/X.h as it stood.

## Result (STATUS.tsv, one line per entry, 81)
- **9 done**, ready to land (rows.txt, rows/<addr>.cpp; no test changed: every landed test is a standalone model and passed unchanged
  in vland on both sides).
- **24 skipped-held**: including 0x0088E520 (drafted and proven except one new UND `__fltused`: question Q1; draft in
  evidence/drafts/0088e520.cpp, lock kept).: the move would touch a stay-off target or needs a ruling (questions below).
- **48 skipped-port**: needs more than a body / callee port (mutable on an existing header line, a stand-in container's const
  overload, an emitted-vftable / throw-info change, class headers with no virtual model, or authoring on a byte difference).
- Order (NF319 Q1): every entry triaged by script first (tools/tri4.py -> work/tri4.txt / work/tri4.tsv: PDB spelling, how engine/X.h
  declares the member, local models / stand-in markers, every direct callee with its landed row, Leaf, RTTI label and table spellings,
  every virtual slot), then the callee targets screened against stay-off (work/offlimits.txt, T3_PORT_QUEUE columns 2 and 3, other
  agents, locks), then the feasible rows worked one batch at a time (b1..b6).

## Done rows (9) and the Q1 draft (0x0088E520, held)
| row | member | how | callee respells (second source) |
|---|---|---|---|
| 0x004FF8A0 | CWorldMap::ForceUnload | opt-in FABLE_RETAIL_CWORLDMAP_UNLOADLEVEL_DECL | Impl -> ?UnloadLevel@CWorldMap@@IAEXJ@Z (both PDB sets + donor call) |
| 0x0088E520 | CGameScriptInterface::EndCutFade | opt-in FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES | helperA -> ?EndFadeScreen@..@@IAEXXZ (both PDB sets + donor call); helperB(int 0x3F000000) -> ?Rest@..@@IBEXM@Z(0.5f) (landed self row + PDB + donor) |
| 0x008FD892 | CAIStateGroup_BirdGroundWander::Initialise | opt-in ..._BIRDGROUNDWANDER_INITIALISE_DECL (override) | CAIStateGroupBase_Retail::Initialise -> ?Initialise@CAIStateGroupBase@@UAE..@Z (RTTI line + landed self row) |
| 0x00891320 | CGameScriptInterface::EnableDangerMusic | no header change (CWorld PMusicManager_Data, CMusicManager.h) | CMusicTarget::CacheMusicSet_006bde50 -> ?EnableDangerMusic@CMusicManager@@QAEX_N@Z (landed self row + PDB) |
| 0x00A39C70 | CWADFile::GetAlignment | opt-in FABLE_RETAIL_CBANKFILE_GETALIGNMENT_DECL | CBankFileAsync::Run -> ?GetAlignment@CBankFile@@QBEKXZ (both PDB sets + donor call) |
| 0x00A75B50 | CNavNavigableLeafNode::RemoveRegionZero | opt-in ..._REMOVEREGIONZERO_DECL (override) | none (slot 5 = GetRegionID, virtual) |
| 0x00B5EAD0 | CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName | opt-in ..._GETCONSOLEENABLEFUNCTIONNAME_DECL (override) | none (local CCharString -> fable_string.h) |
| 0x00A667C0 | CKeyboard::EventScanStart (const) | no header change (FABLE_RETAIL_CKEYBOARD: PDB const + mutable) | none |
| 0x00B23B50 | CEngine::SetCamera(const CCamera*) | opt-in FABLE_RETAIL_CENGINECAMERA_SETGAMECAMERA_PROCESS_DECL | AddExclusiveObserver(CCamera*) -> ?SetGameCamera@CEngineCamera@@QAEXPBVCCamera@@@Z (both PDB sets + donor call) |
| 0x00ABFD70 | C3DMeshFileMaterialExtChunk::GetChunkInfo | no header change (fable_string.h CONCAT_STRING) | none (local CCharString / operator+ model -> fable_string.h) |

## Proof (per changed row)
1. Precheck: clang -fsyntax-only of the landed test (before the lock), the HEAD source and the new row (NF_TREE = the full new include
   tree work/tree_new): evidence/pre.log. The four add-only override declarations each have a clang probe: `struct P : X { <PDB decl>
   override; };` on the tree WITHOUT the opt-in passes, a made-up name fails (evidence/probes/).
2. Final regrade: all 9 ready rows once more on the final tree (batch bf, evidence/recheck_bf{h,n}.log = recheck_final.log):
   MATCH / RELOCATION_MATCH + test PASS on both sides, objsec LIBCP_OK for all 9 under their maps (evidence/libcp_bf.txt). The
   per-batch runs below came first (b1..b4 were graded before the later opt-ins entered tree_new; recheck_b1n.log also holds the
   superseded 0x00891320 draft).
   HEAD and new compiled through vland --recheck-landed under ONE LANDVERIFY_WORK path (tools/runpair.sh renames wd_cur after each side;
   no ?A0x drift); the new side with FABLE_HDR_OVERLAY = work/tree_new: evidence/recheck_b{1..6}{h,n}.log, recheck_final.log. Every row
   MATCH / RELOCATION_MATCH + test PASS on both sides; the graded symbol of the new side is the PDB symbol (the vland result lines).
3. objsec (tools/libcpN.py with the explicit map: the self rename plus each callee respell; evidence/libcp_b{1..6}.txt,
   evidence/symdiff_all.txt): every non-debug section equal in bytes, flags and relocations under the map; UND / DEF sets equal under the
   map; no ??_7 / ??_R; .drectve equal (4 rows) or HEAD + exactly /DEFAULTLIB:"libcp" (6 rows, NF318 Q1). No COMDAT added or dropped.
   One exception, question Q1: 0x0088E520 gains the UND `__fltused` (the float argument 0.5f; no section or relocation changes).
4. Link (evidence/link_all10.txt, link_all10_summary.txt): /FORCE link of the 10 HEAD objects v the 10 new objects (the 9 rows + the Q1 draft), each with the HEAD
   objects of the 8 respelled-callee owners: 0 duplicates in both (that link included the 0x0088E520 draft); the stand-ins HEAD left unresolved are gone; the new set leaves
   unresolved only the PDB names whose owner rows still define stand-ins (NF302 Q2, queued in NAME_FIXES.tsv); Rest,
   CAIStateGroupBase::Initialise and CMusicManager::EnableDangerMusic now resolve; nothing HEAD resolved goes unresolved.
5. Binders: exe census of the 10 addresses (the 9 rows + the Q1 draft) (evidence/census_done.tsv): no landed row calls or references any of them (they are
   vtable-only), so no binder moves. The table lines these rows witness for the respelled callees are replaced (TABLE_CHANGES.tsv, 5
   lines); the two respells with no table line (0x008FCEC0, 0x006BDE50) move onto spellings the table already holds.
6. New-name census: no other landed source defines any of the 10 new self names or a different body under the respelled callee names
   (grep of rebuild/src/compiled, the only hits are the HEAD rows themselves and the owner rows).

## Headers (7 add-only opt-ins; hdr/)
- hdr/engine/<X>.h: full copies of TODAY's repo headers plus one new `#ifdef FABLE_RETAIL_<CLASS>_<MEMBER>_DECL` block each
  (declarations only, both PDB name sets); hdr/<X>.diff the exact change; orig/hdr_<X>.h the HEAD copies (equal to the repo at hand-back).
  - CWorldMap.h: FABLE_RETAIL_CWORLDMAP_UNLOADLEVEL_DECL (protected UnloadLevel(long), inside the existing protected run).
  - CGameScriptInterface.h: FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES (protected EndFadeScreen(), Rest(float) const).
  - CAIStateGroup_BirdGroundWander.h: FABLE_RETAIL_CAISTATEGROUP_BIRDGROUNDWANDER_INITIALISE_DECL (the slot-1 override); one assert
    pinned (FABLE_STATIC_ASSERT_AT(23, ...) via pin_asserts.py).
  - CBankFile.h: FABLE_RETAIL_CBANKFILE_GETALIGNMENT_DECL (unsigned long GetAlignment() const; STATICBLOCKS branch).
  - CNavNavigableLeafNode.h: FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REMOVEREGIONZERO_DECL (protected slot-14 override).
  - CEngineScreenEffectRadialBlurRenderer.h: ..._GETCONSOLEENABLEFUNCTIONNAME_DECL (the override, in both class branches).
  - CEngineCamera.h: FABLE_RETAIL_CENGINECAMERA_SETGAMECAMERA_PROCESS_DECL (SetGameCamera(const CCamera*), Process(bool); the
    FABLE_RETAIL_CENGINECAMERA branch).
- pin_asserts.py run on every copy (1 pin). equiv_check (exact) of the repo tree v work/tree_new over every landed row and test:
  hdr/equiv_check.tsv: 57,146 rows and tests checked, 0 differing (work/equiv.out last line). Planted control: work/tree_ctl = tree_new plus a token in
  CNavNavigableLeafNode.h's flattened default branch, run over its 11 direct includers (work/ctl_addrs.txt): it flags exactly the four
  default-branch includers (0x00A75B50 HEAD, 0x00A76C30, 0x00A76C70, 0x00A77420; sources and tests) and none of the 7 definers
  (hdr/equiv_planted_control.tsv).
- CGameScriptInterface.h's FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES serves only the held 0x0088E520 draft: land it only with
  that row (if Q1 allows); the other six opt-ins serve the 9 ready rows.
- Landing: apply hdr/<X>.diff onto the current header and re-run pin_asserts.py, then the 9 rows (10 with 0x0088E520 if Q1 allows). NF322 / NF323 had no hdr/ copies of
  these headers at hand-back (re-checked at the end: their hdr/ and rows/ hold none of my headers or rows; diff -rq of the repo
  include tree v work/tree_new lists only the 7 headers; orig/ equals HEAD for every header and row).
- New switches used by the rows are only add-only `_DECL` opt-ins plus class-model switches that landed rows already define
  (FABLE_RETAIL_CWORLDMAP, _NSCRIPT, _CAISTATEGROUPBASE(_MEMBERS), _CAISTATEGROUP_BIRDGROUNDWANDER, _CWADFILE,
  _STATICBLOCKS_TEXTURE_HELPERS (188 definers), _CNAVNAVIGABLELEAFNODE, _CENGINESCREENEFFECTRADIALBLURRENDERER, _CKEYBOARD,
  _CENGINE + _OVERRIDES (+ the OnActivate preamble), _CENGINECAMERA, _C3DMESHFILEMATERIALEXTCHUNK, _CCHUNKEDFILECHUNK,
  _CCHARSTRING_CONCAT_STRING, _CWORLD, _DEF_CLASS_BASE, _CDEFSTRINGTABLE, _CCHARSTRING).

## Notes per row
- 0x00891320: the first draft (b1) went through FableWin.pdb's inline CWorld::GetMusicManager() (FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI):
  same bytes, but VC7.1 then emits ?GetMusicManager@CWorld@@QAEAAVCMusicManager@@XZ as an unreferenced pick-any COMDAT (NF320 Q2). The
  final row (b2) reads the header member PMusicManager_Data directly. evidence/recheck_b1n.log still shows the superseded b1 grade.
- 0x00B23B50: Ego_r.pdb types CEngine::CurrentCamera `CCamera*` while SetCamera takes `const CCamera*`: the store is a const_cast
  between two PDB types (nf105: the conversion C++ requires). The global stays the established `NEngineInternal::GPCamera`.
- 0x00A39C70: the HEAD row was a void stand-in; the PDB member returns long, the callee unsigned long; `return Bank->GetAlignment();`
  through engine/CBankFileAsync.h's CBankFileAsync : CBankFile (Bank is the header's CBankFileAsync*). Same bytes.
- 0x00A75B50: `return GetRegionID() == 0;` (slot 5 of vtable 0x0129DDA4 is ?GetRegionID@CNavNavigableLeafNode@@UBEJXZ by RTTI); MATCH.
- vland's non-blocking WARN RELOC_NAME on the new side names only targets whose catalog labels are descriptive placeholders
  (0x006E7270, 0x009CBB80, 0x00B2FBF0, the GPCamera data label): all queued in NAME_FIXES.tsv. The HEAD sides' SHARED_HEADER_AVAILABLE
  warnings (local CWorld / CBankFileAsync / CKeyboard / CInputEvent / CEngineCamera models) are gone on the new side.

## Held / port groups (see STATUS.tsv for each line)
- Stay-off callee targets (17 entries): a callee these bodies must respell is a T3_PORT_QUEUE column-2 target (0x006F9720 / 0x006F9790,
  0x00686DB0, 0x00993EE0 WriteFloat, 0x006FEFB0, 0x005FA740 CDefString::operator!=, 0x006925A0, 0x008ABED0, 0x00773C00, 0x00A24A80 /
  0x009E9F40, 0x009BF5E0, 0x009B8350) or this row sits in a unit column (0x00B662F0). Callee targets whose binding the port leaves
  unchanged (CCharString ctor / dtor, GetPItem ...) were not counted as stay-off.
- In-class inline (0x00405FA0, 0x00453E16): question Q2.
- Hard-coded literal address + same-TU strong stand-in ctor (0x00AA7230, 0x00ABEC70): question Q3.
- NF317 Q4 width rows (0x008913C0, 0x00934100); NF318 Q2 (0x00904380); NF320 Q2 (0x004FC130).
- `mutable` / const on stand-ins (0x004C72D0, 0x0090CB80, 0x009C32C0, 0x005C30D3, 0x005C99BE, 0x006A11B0, 0x0070E680, 0x00772720,
  0x00772790, 0x00828590, 0x00A84E40): an existing header line or a stand-in container would change.
- ScriptedThing family (15): engine/CScriptGameResourceObjectScriptedThing.h declares none of the 14 members; every body runs on local
  CThing_Retail / intelligent-pointer / scripted-control / action models; one unit (not piloted: the census showed 14 header declarations
  plus the CThing / CTCScriptedControl callee identities are needed before the first grade).
- Emitted tables: 0x0099ABF0 (throw info of a local CBBBFileException), 0x00AA7850 / 0x00BBEA70 / 0x00BC3410 (local models with their own
  vftables).
- Flat / absent headers and local engine models: 0x009643D0, 0x00965520, 0x00A26170, 0x00B4F510, 0x00BACC50, 0x00BC5FF0, 0x00BCFDE0,
  0x00BE65A0, 0x00A3ABE0, 0x00AA8460.
- Feasible but not attempted in this pass (port notes in STATUS): 0x00896E60 (CTCInventory callees with PDB bool returns + the inline
  GetTC on CThing), 0x00A0D2B0 (AddInputProcess is protected in both PDBs: an NF309 Q2 access opt-in + friend, plus the slot-2
  declaration), 0x00692480 (binder unit 0x008579D0, slot 62 = GetFrameUpdateTime).

## Questions
- **Q1 (0x0088E520):** retail's `push 3F000000h` is the PDB float argument `Rest(0.5f)`; HEAD faked it with an int. The new object is
  byte-, relocation- and .drectve-equal under the 3-name map, but its symbol table gains an UND `__fltused` (the compiler's float-use
  marker, resolved by the CRT; no relocation). Does that count as "objsec ALL_EQ"? Held under the NF318 Q5 / NF320 Q2 precedent
  (symbol set changed); it lands with the CGameScriptInterface.h opt-in, its 2 TABLE_CHANGES replaces (helperA -> EndFadeScreen at
  0x006E7270, helperB -> Rest at 0x006E71F0, witness 0x0088E520) and RENAMES lines if you allow it.
- **Q2 (0x00405FA0, 0x00453E16):** the retail address is the out-of-line copy of a header in-class inline. The only spelling that gives
  one definition is the nf86 / nf88 pick-any scaffolding, which adds an anonymous-namespace helper (the DEF set grows). Does that
  scaffolding fall under NF320 Q2's hold, or is it its own accepted route for in-class-inline selves?
- **Flagged done row 0x00B23B50:** the store `CurrentCamera = const_cast<CCamera*>(camera)` (Ego_r.pdb types the member `CCamera*`,
  the PDB parameter is `const CCamera*`); both sides are PDB types (nf105). Flag it if you read that rule more narrowly.
- **Q3 (0x00AA7230, 0x00ABEC70):** HEAD fakes CCharString(const char*, long) with a same-TU strong stand-in body and passes the string as
  a hard-coded address. The faithful port (fable_string.h + the literal) gives the same code bytes but drops that strong DEF and adds a
  string-literal relocation and COMDAT, so it cannot be ALL_EQ. Is such a port graded by vland alone (MATCH + the literal read from the
  exe), or held?

## Locks
Kept on the 9 done rows and on the held 0x0088E520 draft. The 8 link controls (owners of the respelled callees) were locked only to compile them and are released.

## Slips
- None of the hard rules was crossed. Two background jobs (equiv_check, its control) ran with `timeout 3000` / `timeout 900` on their own
  launch lines (they are gcc cpp passes, not grades).
