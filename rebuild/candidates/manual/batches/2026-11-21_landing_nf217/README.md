# Landing nf217 (Account A, 2026-11-21): today's landing follow-ups (13 rows)

Agent NF217 (owner: "we can start a new agent when one lands / finishes"). Hand-back, state files and notes are in
`pending/2026-11-21_nf217_handback/`.

- 13 rows land, every one RELOCATION_MATCH or MATCH with its test PASS. The recheck is 44 rows (`logs/recheck.log`): the 13, the direct
  includers of engine/C3DMeshObject.h, and every landed caller of the renamed addresses; all good.
  - q12368 / q12369 / q12370: 0x00A35B30 / 0x00A35AF0 / 0x00A35B50 C3DMeshObject::GetAllDummyObjectNames / GetAllHelperPointNames /
    GetDummyObjectNamesStartingWith, on the PDB spellings (both PDBs, NF214's evidence) and engine/C3DMeshObject.h. Catalog names were
    wrong (CMainGameComponent::BeginInputLoading x2, NGameText::ConstructString); they and the file names are respelled. Callers 0x004BDA30 /
    0x004BDD70 are respelled in place (CStdVector stand-in -> std::vector<CCharString>, as NF108 did for 0x004BDB40). The tests are rewritten
    on the shared headers.
  - q12372 / q12373: 0x0066E799 SelectScreenOfType(EScreenType) and 0x0066E7BD GetCurrentScreen() returning const CBase* (both PDBs). The
    three wrong binders are 0x006116E0 (named in the queue line) plus 0x00621720 / 0x00621C80 (SetScreen(long)); they now use the shared
    headers with no local NHeroInformationScreens models. 0x006116E0 keeps /d2inlS12, and its push_back / overflow / fill_n copies still
    equal their owners (49 / 137 / 23). The self row 0x0066E7BD (a July free-function stand-in) is re-authored on the header LAYOUT opt-in
    (MATCH). Both addresses go 3 -> 1 spellings.
  - q12379: 0x00C432F0 is now CAudioObject::ReportQueueInfo (descriptive: no PDB, exe string, FSE or GameCODA 2.10 record names it; the
    three landed binders already used it). It replaces the placeholder CMarkedBufferOwner_00c43810::ReportMark. Its call to 0x00C442F0 uses
    0x00C437C0's GetInfo spelling (that address goes 2 -> 1 in reloc_bindings; the self row 0x00C442F0 is queued, NAME_FIXES).
  - q12371: 0x0047E196's fill_n<NUISystem::CComponent**> specialization is `template<> inline` with a guarded `__declspec(noinline)`, kept in
    the row's optimize region. The copy is pick-any, 23 B, equal to 0x0048514C, and no body changes vs HEAD. This removes the nf214 duplicate.
  - q12380: 0x00C576E0 (owner) and 0x00C57C90 move onto 0x00C73050's SourceUnqueueBuffers resolver text (`// Inferred:` citing
    batches/2026-11-21_landing_auth3), byte-identical to HEAD. The other six inliners spell other resolvers only.
- Header (add-only): engine/C3DMeshObject.h `FABLE_RETAIL_C3DMESHOBJECT_GETNAMES`, declarations of the three queries inside its `#ifdef`
  (plus the fable_containers.h include it needs, also inside an `#ifdef`); no other landed row defines it.
- Link (`logs/link_check.txt`, 44 objects): duplicates 8 at HEAD -> 4. The fill_n triple and the stale fn_00a35b30 stand-in are gone, and
  nothing new appears.
- reloc_bindings (`logs/tablecheck.txt`): 5 stand-in witnesses removed, 5 PDB first witnesses added, none grows. 0x00C442F0 goes 2 -> 1.
  0x00621720's `?CloseMenus@@YIXXZ` for 0x0041D165 (HEAD too) is left out because it would grow a pre-existing split (ResetMenus /
  OnCloseMenus); queued.
- The pre-existing RELOC_IDENTITY flag on 0x00C57C90 (`g_AudioContext` member offsets read as several targets) is unchanged from HEAD.
- Queued (tags land1121nf217): the 0x00C442F0 self name, the 0x0041D165 spellings, and 0x00C576E0's strong SourceUnqueueBuffers against
  pick-any copies (the nf86 / nf88 scaffolding, 1 duplicate, unchanged from HEAD). NF217 downloaded the GameCODA 2.10 SDK into the scratch
  lane ($S/sdk/gamecoda, md5 as docs/BUILDING.md) to check names; nothing from it is committed.
