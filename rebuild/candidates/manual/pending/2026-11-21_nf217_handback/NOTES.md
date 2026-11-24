# NF217 notes (2026-11-21, round 10: the land1121nf214 / land1121auth3 follow-ups)

Lane: grades with `S=$D/pS` q.py; vland `--recheck-landed` payloads built with tools/mkpay2.py (NF216's, repointed). REFS.tsv is built
with callers.py and absrefs.py run separately (tool column). Calibration: callers.py reproduces the 11 landed referencers of
0x00485DBC that NF213 counted (absrefs.py: none for it).

## q12368 / q12369 / q12370: C3DMeshObject name wrappers (done)
- Claims: both PDBs (ghidra_out/egor_pdb_names.tsv + fablewin_pdb_names.tsv) spell the three wrappers public const over
  `std::vector<CCharString>&` (addresses in RENAMES / the header comment); the bodies (xdis) are `mov eax,[esp+4]; mov ecx,[ecx+8]; push 0;
  push eax; call <C3DMeshStats query>; ret 4` (0x00A35B50: prefix too, `ret 8`), i.e. Mesh->Query(names[, prefix], false).
- Rows: self rows 0x00A35B30 / 0x00A35AF0 / 0x00A35B50 moved onto engine/C3DMeshObject.h (new opt-in FABLE_RETAIL_C3DMESHOBJECT_GETNAMES)
  + engine/C3DMeshStats.h (landed FABLE_RETAIL_C3DMESHSTATS_UNPACKNAMES); each RELOCATION_MATCH first grade; the callee relocation binds
  the landed callee's own self leaf (0x00A2F0F0 / 0x00A2F120 / 0x00A2F150, nf214).
- Rows claimed outside rows.txt (plain mkdir, free): 0x004BDA30 and 0x004BDD70, the landed binders. Both respelled IN PLACE in their
  existing local C3DMeshObject model (NF108's 0x004BDB40 pattern: CStdVector<CCharString> -> std::vector<CCharString> from
  stlport/fable_containers.h). No new local model; the local C3DMeshObject / CCountedPointer / CRetailVector models stay HEAD debt (the
  verifier warns SHARED_HEADER_AVAILABLE for CCountedPointer and C3DMeshObject, as at HEAD).
- Leaf-only self renames (both PDBs) that the respell forces:
  - 0x004BDA30 `...AAU?$CStdVector@VCCharString@@@@@Z` -> `...AAV?$vector@...@std@@@Z` (Ego_r 0x004C0FC0, FableWin 0x01D0B490). Census
    (REFS.tsv): its one landed referencer 0x0076EB00 already binds the new spelling (reloc_bindings), so the address goes 2 -> 1.
  - 0x004BDD70 likewise (Ego_r 0x004C1330, FableWin 0x01D0BA90). Census: callers.py one call site 0x006A03EB in no landed row; absrefs
    none. 1 -> 1 spelling.
- Census per renamed wrapper address after the hand-back: 0x00A35B30 1 spelling (0x004BDB40 already on it); 0x00A35AF0 2 -> 1 (self
  stand-in + 0x004BDA30's CStdVector); 0x00A35B50 2 -> 1 (self stand-in + 0x004BDD70's CStdVector). The HEAD stand-in
  `?fn_00a35b30@@YIXPAUBL@@HH@Z` was defined by both 0x00A35B30 and 0x00A35AF0 (one spelling, two addresses): gone.
- Callee stand-ins replaced (RENAMES section 2): ?h@N@@QAEXHH@Z, ?h_00a2f120@N@@QAEXHH@Z, ?Build@Inner@@QAE_NPAXJH@Z ->
  the C3DMeshStats PDB leaves; 0x004BDA30 / 0x004BDD70's C3DMeshObject CStdVector stand-ins -> the wrappers' PDB leaves.
- Defined-symbol diff vs HEAD (cl-only objects, work/obj/head_* v new_*): wrappers: own leaf only, plus the shared-header inline
  `??C?$CCountedPointer@$$CBVC3DMeshStats@@@@QBEPBVC3DMeshStats@@XZ` (operator->, pick-any COMDAT, as the landed 0x00A36100 HasHelperPoint
  uses); binders: own leaf only.
- Tests: the three wrapper tests are rewritten on the shared headers; each links the row object (the verifier's fallback link) and
  stubs the C3DMeshStats query, checking `this` = Mesh.Data (+8), the names / prefix references and sort = false. PassPatterns kept
  (`00a35b30_TEST PASS`, `PASS_00a35af0`, `00a35b50_TEST PASS`). The 0x004BDA30 / 0x004BDD70 tests are unchanged standalone HEAD models
  (local CStdVector / C3DMeshObject; debt, they still PASS).
- Catalog: module/name for the wrappers -> C3DMeshObject / <member> (RENAMES section 1; old labels CMainGameComponent::BeginInputLoading
  (propagated) and NGameText::ConstructString (wrong)). Minor: the oracle (auto-re-candidates) name for 0x004BDA30 is
  `CTCGraphicAppearance::GetAllHelperPointNames` without the `NGraphicAppearance::` its build entry's module has.
- Funclets: no landed row includes any of the five source files (grep of rebuild/src/compiled + rebuild/tests).
- Left alone: 0x004BDA30 / 0x004BDD70 still bind `?reserve@?$CRetailVector@VCMeshInfo@NGraphicAppearance@@@@QAEXK@Z` (0x004C46C0,
  queue q4975, unchanged spelling).

## q12372 / q12373: NHeroInformationScreens::CManager::SelectScreenOfType / GetCurrentScreen (done)
- Spellings, both PDBs: ?SelectScreenOfType@CManager@NHeroInformationScreens@@QAEXW4EScreenType@2@@Z (Ego_r 0x006A0C9B, FableWin 0x02115ADC)
  and ?GetCurrentScreen@CManager@NHeroInformationScreens@@QAEPBVCBase@2@XZ (Ego_r 0x006A0CAB, FableWin 0x0211514F): the return is const.
  Both are what engine/NHeroInformationScreens_CManager.h declares and the reloc_bindings witness 0x00577216 binds.
- Census (REFS.tsv callers; absrefs none; every landed binder compiled once, work/e2_probe.txt): 13 landed binders. 10 bound the PDB
  spellings (all on the shared header). Odd: 0x006116E0 (SetCurrentScreenType(long) + non-const GetCurrentScreen) and, not named in the
  queue line, 0x00621720 / 0x00621C80 (a third stand-in ?SetScreen@CManager@NHeroInformationScreens@@QAEXJ@Z, "name unproven").
  Claimed 0x00621720, 0x00621C80, 0x0066E7BD outside rows.txt (free locks).
- The three binders kept local NHeroInformationScreens::CBase / CManager models (and 0x006116E0 a local CCharString). An in-place respell
  needs EScreenType, which lives only in engine/NHeroInformationScreens_CBase.h beside the shared CBase, so the rows moved onto the shared
  headers instead (the local models are deleted, none added): FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER + the CManager header (pulls
  _CBase.h and fable_string.h); 0x006116E0 also needs FABLE_RETAIL_CCHARSTRING (its CreateComponent("UI_MOUSE_POINTER") converts the
  literal through the two-argument ctor; form 1 without it: C2664). Respells that come with it, two sources each:
  - slot 3 of the screen base: the header's IsInventoryScreen (PDB CBase virtuals; 0x00577216 calls IsInventoryScreen() at the same
    point) was the stand-in IsActive (a virtual call, no symbol);
  - PreviousScreenType is EScreenType (Ego_r.pdb CTCInventoryMap / CTCInventoryQuests layouts; the shared CTCInventoryExperience header);
    0x00621720 / 0x00621C80's member renamed PreviousScreen -> PreviousScreenType (PDB member name); 0x006116E0's `-1` is NULL_SCREEN_TYPE.
  - No cast anywhere: GetScreenType() returns EScreenType, and SelectScreenOfType takes it.
- 0x006116E0 keeps /d2inlS12 and its scaffolding; pick-any copies (cmpcopy, bytes + relocation targets): push_back 49 = 49 (0x00486024),
  _M_insert_overflow 137 = 137 (0x00485DBC), fill_n 23 = 23 (0x0048514C). comdiff vs the HEAD object: no function body changed; the
  defined-symbol set is identical to HEAD for all three binders.
- 0x0066E7BD self row (not named in the entry; the address's own stand-in spelling): the July free `__fastcall` stand-in re-authored on the
  header's LAYOUT opt-in exactly as NF79 did the sibling 0x0066E799 (descriptive members RegisteredScreens +0x04 / CurrentScreenIndex +0x28),
  `#pragma optimize("s", on)` kept from HEAD (`and [ecx+28h],0`). MATCH. Every landed referencer binds the PDB spelling after this
  hand-back, so 0x0066E7BD goes 3 -> 1 spelling and 0x0066E799 3 -> 1. New defined symbols: only the header-inline
  vector<const CBase*> operator[] / begin COMDATs. Test rewritten on the header (links the row; PassPattern kept).
- Tests of the three binders: unchanged standalone HEAD models (debt; they PASS). No funclet includes any of the four sources.

## q12379: 0x00C432F0 CAudioObject::ReportQueueInfo (done; DESCRIPTIVE name)
- Name sources searched: both PDB name sets (no CAudioObject), Fable.exe strings, and the GameCODA 2.10 SDK (fetched as docs/BUILDING.md
  says into `$S/sdk/gamecoda/tree`, md5 42cd21b0cb8c2cc2a0a53f02f39b4c32; CODAApiDbg / CAGEDbg / ToolkitDbg / OpenAL32Dbg libs and the
  CAGE / Toolkit / tutorial sources): none has CAudioObject, CircularQueue or a queue-info name (the tutorials' CAudioObjectName is an
  unrelated name holder). libpk is Lionhead's own layer, so the made-up name rule applies.
- The body: if the +0xD4 `__stdcall` callback is set, GetInfo (0x00C442F0) fills a 20-byte record from the +0x54 queue and the callback gets
  (this, &record, +0xD8 context). That is exactly the callback block of the landed siblings FlushQueue 0x00C437C0 / ReleaseQueue
  0x00C43810 (which first reset and clear): it reports the queue's state. `ReportQueueInfo` says that, and it is what all three landed
  referencers bind (REFS.tsv: 0x00C43A10 after FlushQueue, 0x00C5B300 / 0x00C73050 after dequeuing a buffer). So only the self row moves.
- Self row: re-authored on the landed sibling model (SQueueInfo / CircularQueue / CAudioObject, token for token as 0x00C437C0). The
  placeholder classes CMarkedBufferOwner_00c43810 / CMarkedBuffer_00c43810 / SBufferMark_00c43810 are gone. None of CAudioObject,
  CircularQueue, SQueueInfo, CVoice, CSoundBuffer has a header anywhere under rebuild/include (grep), and landed siblings use the same
  shape, so this is the allowed header-less local model (no base class). RELOCATION_MATCH; defined symbols: only the leaf changes.
- Callee: the GetInfo call now binds 0x00C437C0's `?GetInfo@CircularQueue@@QAEAAUSQueueInfo@@AAU2@@Z` (the body returns the record), so
  0x00C442F0 goes 4 -> 3 spellings; the rest (its own placeholder self leaf, 0x00C43810's void-return spelling) is in NAME_FIXES.tsv.
- Test rewritten: it links the row object, stubs GetInfo, and checks the no-callback path and the callback's arguments (PassPattern kept).
- refs/fse_api_manifest.json has no entry for 0x00C432F0. The verifier now warns `RELOC_NAME ?GetInfo@CircularQueue@@... -> 00c442f0 labelled
  CMarkedBuffer_00c44550::GetMark`: forced (once the local class is CircularQueue the callee symbol must change); it clears with the
  0x00C442F0 fix in NAME_FIXES.tsv. The STANDIN_LEAF note on the new leaf clears with the RENAMES section 1 catalog rename.

## q12371: 0x0047E196's fill_n<NUISystem::CComponent**> copy (done)
- HEAD: the row's `template<> CComponent** FABLE_FASTCALL fill_n(...)` explicit specialization is a COMDAT with selection 1
  (NODUPLICATES, a strong definition); 0x006116E0 / 0x007E4B60 emit the same symbol pick-any (selection 2) through
  FABLE_RETAIL_STLP_FILL_N_NUISYSTEM_CCOMPONENT_SIZE_BODY. Control link (/DLL /NOENTRY /FORCE /NODEFAULTLIB, LINK unset, tools/lnk.py):
  HEAD 0x0047E196 + 0x006116E0 + 0x007E4B60 -> 2 LNK4006 on fill_n.
- Fix (nf140 line: make the owner copy `inline`): `template<> inline` + `#ifdef _MSC_VER __declspec(noinline) #endif`, the header opt-in's own
  spelling of the same instance, left where it was (inside the row's optimize("y", on) / ("s", on) / auto_inline(off) region, so the copy
  keeps the frame-less size body: a non-template inline explicit specialization takes the optimize mode at its definition). The header
  opt-in itself was not usable here: it sets only optimize("s") and then `optimize("", on)`, which under this row's /O1 /Oy- command line
  would give fill_n a frame pointer.
- Result: selection 2 (pick-any); the copy is 23 = 23 against 0x0048514C (bytes and relocation targets); comdiff vs HEAD: no function
  body changed (47 functions, the leaf included); defined-symbol set identical; still out of line (noinline, called from the overflow
  copy). Landing link of new 0x0047E196 + new 0x006116E0 + 0x007E4B60: 0 duplicates. Test unchanged (PASS). No funclet includes the source.

## q12380: SourceUnqueueBuffers resolver text (done)
- The resolver lives in each row's local CAudioContext model (no shared header), so this is a row edit. grep of rebuild/src/compiled for
  `GetProcAddress("alSourceUnqueueBuffers")`: only 0x00C576E0 (the owner), 0x00C57C90 COpenALVoice::UnqueueBuffer (in-class copy) and
  0x00C73050 (already on the text). The other six rows the queue line lists (0x00C57460 0x00C57AF0 0x00C57D80 0x00C72620 0x00C72800
  0x00C72A40) inline other resolvers only (GetSourcei, SourcePlay, GetError, ...): not-a-binder, left on the ternary as the brief says.
- tools/unq.py rewrites only the alSourceUnqueueBuffers ternary into 0x00C73050's three lines and adds an `// Inferred:` line citing
  0x00C73050 / batches/2026-11-21_landing_auth3 / AUTH3's cons/. Both rows RELOCATION_MATCH + PASS; comdiff vs HEAD: no function body
  changed (the text is codegen-neutral here, as AUTH3 measured); defined symbols identical.
- Pick-any: 0x00C57C90's SourceUnqueueBuffers copy is 79 = 79 against 0x00C576E0 (bytes; GetProcAddress 0x00C4D740 and IAT 0x0143FE44
  bound; the string literal is not a reloc_bindings symbol).
- Debt seen, unchanged (HEAD and hand-back alike): 0x00C576E0's own definition is a strong COMDAT (selection 1) while 0x00C57C90 /
  0x00C73050 emit the same member pick-any (selection 2): a link of 0x00C576E0 + 0x00C57C90 gives 1 LNK4006 at HEAD and after.

## Summary for the coordinator
- Rule slips: the GameCODA 2.10 archive + tree were written outside this directory, at `$S/sdk/gamecoda` (dl/ 43 MB, tree/ 74 MB), the
  path docs/BUILDING.md names; remove it or keep it as the coordinator prefers. RENAMES leaves were first written from a format string
  (three had a stray `@`), regenerated from the objects before hand-back. rows/00c432f0.cpp was written through an unquoted heredoc
  (backticks escaped; the header was re-read and is intact).
- Checks (selfcheck.log): HEAD v hand-back WARN diff for the 13 rows adds no SHARED_HEADER / local-model / purity warning; the undefined-
  symbol diff is exactly RENAMES section 2 (10 lines).
- Rows claimed outside rows.txt (plain mkdir; all were free): 0x004BDA30, 0x004BDD70, 0x00621720, 0x00621C80, 0x0066E7BD.
- Self renames (RENAMES section 1, 7): 0x00A35B30 / 0x00A35AF0 / 0x00A35B50 (source files and catalog names move: module C3DMeshObject),
  0x004BDA30 / 0x004BDD70 (leaf only), 0x0066E7BD (catalog module CManager -> NHeroInformationScreens::CManager; file name can stay),
  0x00C432F0 (catalog CMarkedBufferOwner_00c43810::ReportMark -> CAudioObject::ReportQueueInfo; the file name moves). No funclet includes
  any changed source.
- Descriptive (made-up) names: CAudioObject::ReportQueueInfo (0x00C432F0) and the CircularQueue::GetInfo callee spelling (0x00C442F0,
  NAME_FIXES.tsv); the CManager LAYOUT member names (RegisteredScreens / CurrentScreenIndex) are the header's existing descriptive ones.
- Tests rewritten on shared headers (link the row object): 0x00A35B30, 0x00A35AF0, 0x00A35B50, 0x0066E7BD; 0x00C432F0's test links the row
  (CAudioObject has no header). Other tests unchanged.
- Flags lines: none added. 0x0066E7BD's `// Flags: none` matches its build entry (no CompilerFlags). 0x006116E0 keeps `/d2inlS12`.
- Header: one overlay, engine/C3DMeshObject.h (FABLE_RETAIL_C3DMESHOBJECT_GETNAMES), see hdr/README.md.
- Follow-ups worth queueing: 0x00C442F0 self row + 0x00C43810 onto `?GetInfo@CircularQueue@@QAEAAUSQueueInfo@@AAU2@@Z` (NAME_FIXES.tsv);
  0x00C576E0's strong SourceUnqueueBuffers definition vs the pick-any copies in 0x00C57C90 / 0x00C73050 (1 LNK4006; the leaf is the
  owner itself, so the nf86 / nf88 address-helper scaffolding applies, not a bare `inline`); the local C3DMeshObject / CCountedPointer models in 0x004BDA30 / 0x004BDB40 / 0x004BDD70 (HEAD debt).
