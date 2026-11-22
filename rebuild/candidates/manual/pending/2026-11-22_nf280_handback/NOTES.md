# NF280 notes (2026-11-22, round 34b)

Base: the repo working tree at session start (landings up to nf278). NF279 runs alongside me (0x00BFD480 / 0x00BFDC90, the CMovableResource
Verify rows, the landscape tests): none of its rows touched; its hdr/ holds only engine/CEngineWaterBackgroundSubPatch.h. Every row I compiled
was `released` or had no lock directory before I claimed it with tools/lock.sh (evidence/locks.log); none is in codex_pass28 RESERVED.tsv.
Reading list: BRIEF.md (NF280), NF278 / NF275 BRIEFs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md, COORDINATION.md, GOTCHAS.md (all 503
lines, before the first compile), the nf273 / nf275 / nf278 landing READMEs, the NF278 and NF275 hand-backs (NOTES, LESSONS, NAME_FIXES).

## Entry 1: 0x00416E78 GetInputs (nf278 ruling 2) -- done, with its callee family

### Claims (two sources each)
| Address | Spelling | Source 1 | Source 2 | Was |
|---|---|---|---|---|
| 0x00416E78 (self) | ?GetInputs@CMainGameComponent@@MAEXXZ | Ego_r.pdb 0x00416188 + FableWin.pdb 0x018A6CB0 (protected virtual) | retail vtable 0x0122F180 slot 6; both emitters 0x00418DCA / 0x00418B7A bind MAE (link below) | QAE (row-local non-virtual model) |
| 0x00415FE0 | ?ProcessEvent@CMainGameComponent@@IAEXPBVCGameEvent@@@Z | Ego_r.pdb 0x00415194 + FableWin.pdb 0x018A872E | table witness 0x00416670 (IAE); body = case 2 -> SetQuit (slot 4) | GetInputs bound QAE; self row `T::ProcessEvent` |
| 0x00415FF2 | ?EventIsSystemEvent@CMainGameComponent@@IAE_NPBVCGameEvent@@@Z | Ego_r.pdb 0x004151A6 + FableWin.pdb 0x018A8762 | table witness 0x0041649C (IAE); header EVENTISSYSTEMEVENT | GetInputs bound QAE; self row `T::IsSystemEvent` |
| 0x004457F0 | ?Update@CGamePlayerInterface@@QAEXXZ | Ego_r.pdb + FableWin.pdb (public void()); Ego_r donor order Update 0x00442C50 -> GetButtonPressureForEvent 0x00442C60 -> GetMouseMovement 0x00442D40 = retail 0x004457F0 -> 0x00445800 -> 0x004458F0 | body `mov byte ptr [ecx+894h],0 ; ret` = HasCalledInputUpdateThisFrame (+0x894, Ego_r member) cleared; FableWin /Od size 21 bytes = one byte store through this; called once per frame by GetInputs before the GetGameEvent loop | GetInputs bound the made-up ?BeginEventPoll@CGamePlayerInterface@@QAEXXZ; self row the free ?CXMVPlaybackInfo@@YIXPAUCXMVPlaybackInfoObj@@@Z |
| 0x009F4A90 | ?UpdateChecksum@CInputManager@@QAEXJABVCChecksum@@@Z | Ego_r.pdb 0x00B48620 + FableWin.pdb 0x030CD6D0 | the self row 0x009F4A90 defines it (HEAD object compiled: evidence/sym_head_009f4a90.txt) | GetInputs bound PAV |

0x004457F0 exclusions: the class's other public `void()` members are ReapplyControlScheme (FableWin /Od 29 bytes, calls SetControlScheme),
SuspendAllInputProcesses / ResumeAllInputProcesses (FableWin-only inline, Suspended +0x79C). No absolute reference to 0x004457F0 (absrefs),
one E8 caller (0x00416FB3).

Other callees keep HEAD's spellings (already the PDB / table ones): PeekWorldChecksum 0x00416392, AddGameEvent 0x0041649C, GFGetSystemManager
`?GFGetSystemManager@@YIPAVCSystemManager@@XZ`, IsApplicationActive 0x009A57B0, `?GetFrame@CWorld@@SIJXZ` 0x0049D870, `?GPInputManager@@3PAVCInputManager@@A`
(both PDBs spell GP; kept as a row-local `extern`, the shared header only declares the wrong `PInputManager` under JOYSTICKQUEUE: NAME_FIXES).

### Rows
- **0x00416E78** (rows/00416e78.cpp): every local model gone (CMemoryBuffer, CGameEvent, CChecksum, CRightHandedSet, CCamera, CCameraSource,
  CCameraOwner, CInputManager, CSystemManager, CWorld, CGamePlayerInterface, CMainGameComponent). Now on engine/CMainGameComponent.h (landed
  CMAINGAMECOMPONENT, ADDGAMEEVENT, EVENTISSYSTEMEVENT, PPLAYERINTERFACE, EVENTPACKAGES, EDITOR + new PROCESSEVENT), engine/CWorld.h
  (FABLE_RETAIL_CWORLD: GetGameCamera, static GetFrame), engine/CAIGameCameraBase.h (GetRenderCamera slot 1), engine/CCamera.h (PeekWorldPos),
  engine/CGameEvent.h (DEFAULT_CONSTRUCTOR_PASS26), engine/CChecksum.h (new CTOR_THREE), engine/CInputManager.h (new UPDATECHECKSUM),
  engine/CSystemManager.h (APPACTIVE), engine/CGamePlayerInterface.h (its FABLE_RETAIL_EXPRESSION_FRAME class: GetGameEvent slot 1, Update;
  needs BASE_OBJECT / CCLASS / DEF_CLASS_BASE / CDISKFILEWIN32, the define set of landed 0x00847520). Members by their Ego_r names:
  CurrentServerFrame +0x48 (was "InputChannel"), PeekWorldSeed() (WorldSeed +0x1613C, was "Frame"), LoadingEventPackages +0x16170 (was
  "Loading"); PPlayerInterface-> replaces the made-up inline GetPlayerInterface(). Overlaid headers included first. Flags line `/GS` as HEAD.
  FABLE_RETAIL_CNETWORKCLIENT_UPDATE came with landed 0x00417747's define block (EVENTPACKAGES types the Client member); no grade ran without it.
  - Form 1 = final: RELOCATION_MATCH (393 bytes). Relocation diff v HEAD (evidence/sc_00416e78_f1.log): exactly the four callee targets
    0x00415FE0 / 0x00415FF2 (QAE -> IAE), 0x004457F0 (BeginEventPoll -> Update), 0x009F4A90 (PAV -> ABV). Defined symbols: the leaf QAE -> MAE;
    the HEAD-local ??0CChecksum@@QAE@KKJ@Z and ?GetPlayerInterface@ COMDATs gone; new header-inline COMDATs ??0CChecksum@@QAE@JJJ@Z,
    PeekWorldSeed, GetGameCamera, PeekWorldPos and the two scoped_ptr operator-> (no table line, no landed owner row: pk NO-OWNER;
    evidence/pk_00416e78.tsv); ??0CGameEvent@@QAE@XZ EQ v 0x004159A0 as at HEAD.
- **0x004457F0** (rows/004457f0.cpp): `CGamePlayerInterface::Update() { HasCalledInputUpdateThisFrame = false; }` on the EXPRESSION_FRAME class
  (plus a forward `class CEnginePrimitivePrepareInfo;`, which engine/CInputProcessManager.h needs and does not declare: header debt). MATCH;
  the object defines only the new leaf.
- **0x00415FE0** (rows/00415fe0.cpp): on CMainGameComponent.h + the new PROCESSEVENT opt-in; `SetQuit()` is the CGameComponent slot-4 virtual
  (`call [eax+10h]`). MATCH; defines only the leaf.
- **0x00415FF2** (rows/00415ff2.cpp): on CMainGameComponent.h + landed EVENTISSYSTEMEVENT (CGameEvent.h included first, the opt-in has no
  forward of its own). MATCH; defines only the leaf.
- Tests: 0x004457F0 gets a new linking test (rows/004457f0_test.cpp: plain C++ on the shared header; the row's Update called on a 0xFF-filled
  block clears exactly +0x894; PassPattern OK_0x004457f0 kept), replacing the HEAD standalone model and its `__asm` call block. The other three
  HEAD tests are kept unchanged (0x00416E78: a standalone copy of the OLD source; 0x00415FE0 / 0x00415FF2: placeholders).

### Census (spellings per address, before -> after)
- 0x00416E78: {QAE self, MAE (both emitters, vtable slot 6)} 2 -> 1.
- 0x00415FE0: {IAE 0x00416670, QAE 0x00416E78, T:: self} 3 -> 1.   0x00415FF2: {IAE 0x0041649C, QAE 0x00416E78, T:: self} 3 -> 1.
- 0x004457F0: {BeginEventPoll 0x00416E78, CXMVPlaybackInfo self} 2 -> 1 (only binder 0x00416E78, callers.py; no absolute reference).
- 0x009F4A90: {PAV 0x00416E78, ABV self} 2 -> 1 (only E8 caller 0x00416E78).
- New symbol names: the four leaves. Grep of rebuild/src/compiled: no other out-of-class or in-class body of CMainGameComponent::GetInputs /
  ProcessEvent / EventIsSystemEvent or CGamePlayerInterface::Update (only HEAD 0x00416E78's own definition, which this replaces). Old names
  dropped: ?CXMVPlaybackInfo@@YIX... (no other definer or binder; the real CXMVPlaybackInfo ctor 0x009BC7D0 is ??0CXMVPlaybackInfo@@QAE@XZ,
  untouched), ?ProcessEvent@T@@... / ?IsSystemEvent@T@@... (self-only).
- Link (evidence/lk_e1.txt; HEAD emitters compiled; LINK unset): A (HEAD emitters) 0 duplicates, ?GetInputs@CMainGameComponent@@MAEXXZ
  unresolved (2 refs); B (+ the four new objects) 0 duplicates, GetInputs / ProcessEvent / EventIsSystemEvent / Update resolved; only the
  ABV UpdateChecksum left unresolved (0x009F4A90's object, which defines it, was not in the set).
- Funclets / folds: no landed source, test or header includes any of the four rows' files.
- Pick-any names newly defined by 0x00416E78's object (??0CChecksum@@QAE@JJJ@Z, PeekWorldSeed, GetGameCamera, PeekWorldPos, the two
  scoped_ptr operator->): no catalog row is named for any of them (auto-re-candidates.tsv name column), no table line, no landed Leaf; the
  only other bodies in landed sources are three row-local `GetGameCamera() { return PCurrentGameCamera; }` (0x006946C0, 0x007FBE50,
  0x0072A090; each with PCurrentGameCamera at +0x34: the same body) and the shared header's. Header inlines with no retail copy (NO-OWNER).
- Vtables: no changed object emits a vtable (sc.sh symbol lists), so vtcheck does not apply; slot 6 of retail 0x0122F180 is 0x00416E78
  (read from the exe) and both emitters bind it MAE (link above).
- Flags: none changed (0x00416E78 keeps `/GS` = its CompilerFlags).

### Header proof
- Three add-only opt-ins (hdr/README.md): CMainGameComponent.h PROCESSEVENT, CInputManager.h UPDATECHECKSUM, CChecksum.h CTOR_THREE
  (5 asserts pinned). Whole-tree equiv_check (tools/eq.sh: repo rebuild/include v work/tree_new = repo + exactly these three files; every
  landed source and test): **57,125 rows checked, 0 differ, DONE 0** (evidence/equiv.log, evidence/equiv.tsv empty). After the run: orig/ equal
  to the repo for all three files and tree_new equal to hdr/ for all three.
- During the run the repo's engine/CEngineWaterBackgroundSubPatch.h changed (mtime 22:35:57, after my tree copy at 22:26): it gained NF279's
  `FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS` block (landing nf279 happened during my equiv_check: batches/2026-11-22_landing_nf279 exists and the repo file equals NF279's hdr copy). So for rows checked after
  that moment the old tree had that block and my new tree did not; equiv still found 0 differing rows, so my three files change no landed
  row's input, and the other block changed none either. None of my three headers changed in the repo. recheck_final ran after that
  landing, so the four rows are proven on the current tree.

## Entry 2: the `_M_erase` split at 0x004507A1 -- open (not started; context)
## Entry 3: 0x00ADCE60 -- open (not started)

## Whole-pass checks
- recheck_final.log (tools/recheck_final.sh with FABLE_HDR_OVERLAY=hdr/, the four rows, rows/004457f0_test.cpp + the three HEAD tests):
  0x00416E78 RELOCATION_MATCH PASS, 0x004457F0 / 0x00415FE0 / 0x00415FF2 MATCH PASS; 0 RELOC_CONFLICT; one WARN RELOC_NAME (Update ->
  0x004457F0 labelled `_global::SetMemberConst_004457f0`: the catalog label RENAMES fixes).
- Descriptive names: the CChecksum ctor's and UpdateChecksum's parameter names (the PDB keeps none). No descriptive function name.
- Nothing written in the repo; no git; no sleep; every grade launched in the background under `timeout 900` and checked in a later call;
  every source a grade compiled (the four new rows, their HEAD sources and tests, the controls 0x009F4A90 / 0x00418DCA / 0x00418B7A) had a
  passing clang precheck in an earlier call after its lock was mine. No VC7.1 compile ran beside the equiv_check; four clang prechecks of
  the HEAD tests, source greps and one file listing of the entry-2 rows (sizes and locks only) did.
- Scratch: work/tree_new and work/try removed with tools/clean.py; pE/ (8 KB stub dir of the equiv_check) is left because clean.py refuses a
  first-level folder.
- Locks: 0x00416E78 / 0x004457F0 / 0x00415FE0 / 0x00415FF2 stay NF280 (rows.txt); the controls 0x009F4A90 / 0x00418DCA / 0x00418B7A released.

## Rule slips
- One Bash call exited 2 (a `cat` of a test path that is under rebuild/tests, not src/compiled).
- To wait for the equiv_check without polling, I launched a background `timeout 2700 tail --pid=<equiv pid> -f /dev/null` watcher
  (run_in_background, so the harness notifies me); `tail --pid` polls internally. Listed for the coordinator to judge.
- One unquoted heredoc (a NOTES edit) let bash run a backtick fragment (`command not found`, no effect; the text was repaired).
- One scratch file was written outside my folder (`/tmp/egor_gpi.txt`, a name list for a grep filter; not removed, as the rm rule forbids it).

## Decisions for the coordinator
1. Land the four rows with the three header opt-ins (equiv result above), RENAMES (4 self renames, 4 section-2 bindings of 0x00416E78) and
   TABLE_CHANGES (4 removals, 2 additions). 0x004457F0's catalog entry moves from `_global` / CXMVPlaybackInfo::CXMVPlaybackInfo to
   CGamePlayerInterface::Update (file rename; no includer).
2. NAME_FIXES: 0x013B8388 (the header's `PInputManager` extern v the PDBs' GPInputManager), not applied.
3. Header debt: engine/CInputProcessManager.h (EXPRESSION_FRAME users) needs `class CEnginePrimitivePrepareInfo;` before use; the row forward-declares it.
4. Test debt: the 0x00416E78 HEAD test is kept, but it is a verbatim copy of the OLD source with all its local models, so after the port it
   no longer tests the landed text: a linking test (camera / input-manager / player-interface stubs) is queued. 0x004457F0's new linking test
   replaces the HEAD one (its `__asm` block goes with it).
5. Entries 2 and 3 to the next agent.
