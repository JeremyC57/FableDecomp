# AUTH16 0x00CF4EC0 CFanMarker::Main: option (C) bounded (advisor ruling 2026-11-22, "AUTH13's 0x00CF4EC0 checks")
Base text: AUTH12 p6d (= AUTH13 `p6d.cpp`, = pending/2026-11-22_auth12/00cf4ec0/00cf4ec0.cpp), overlay AUTH13 `hdr/` (CTimer opt-in).
No candidate was graded and no new readout was run: every number below is read from existing readouts / PDBs / retail bytes.

## Step 1: the pruning check
Readout: AUTH13 `oracle/p6d.f.json` (AUTH10 inl_force.py on p6d at stock S9 / T74; the forced run is the one whose object is
RELOCATION_MATCH, so its per-site f and c1 are the values the matching decision list was taken under; forcing only overrides the result
after the score is computed). Cross-check: AUTH12 `inl/p6s9.json` (p6, unforced) gives the same f / c1 / sc at these sites.

| site (loop-2 fan hit block, after-loop return) | callee | k | sz | sav[0] | c1 | f (flo) | score | retail |
|---|---|---|---|---|---|---|---|---|
| block 1 | ~CScriptGameResourceObjectMovieBase | 9 | 25 | 6 | 19 | 347222 | 798 | expanded |
| block 1 | ~CTimer | 15 | 20 | 6 | 14 | 347222 | 588 | called 0x00CD4470 |
| block 2 | movie k11 / CTimer k17 | | | | 19 / 14 | 347222 / 347222 | 798 / 588 | expanded / called |
| block 3 | movie k13 / CTimer k19 | | | | 19 / 14 | 347222 / 347222 | 798 / 588 | expanded / called |
| in-loop returns | movie k8,k10,k12 / CTimer k14,k16,k18 | | | | 19 / 14 | 3255211, 2354602, 1679146 (pairwise equal) | 99/73, 131/96, 177/131 | movie expanded x3; ~CTimer expanded at k14 (73 < T74), called at k16 / k18 |

- **Same f.** At every one of the six hit-block returns the movie dtor and ~CTimer carry the identical frequency, and the score is
  proportional to c1 at fixed f (798/588 = 19/14 = 1.357; 99/73, 131/96, 177/131 likewise). A site expands iff score < T, so at one f the
  movie dtor (c1 19) can expand only if ~CTimer (c1 14) expands too. Retail does the reverse at five of the six returns (all three
  after-loop returns, two of the three in-loop ones; at block 1's in-loop return both expand). No S, T, frequency or loop-idiom change that
  moves f can produce the pair: the missing text must change WHICH callee, or which inline context, sits there.
- **The costs 19 / 22 (observed).** In the readout c1 = sz - sav[0] for both: 25 - 6 = 19 at every site in Main, 25 - 3 = 22 at the one
  decision inside `??_GCScriptGameResourceObjectMovieBase` (f = 1.0, score 22, inlined); ~CTimer 20 - 6 = 14. sz = 25 is the movie dtor as
  this TU sees it (25 B: call 0x006E7AB0, GROBase vtable store, jmp ~CBaseObject). **Inferred, not measured:** sav[0] is the saving for the
  `this` argument (6 when `this` is the address of a frame local, as in Main; 3 when it is the incoming pointer, as in ??_G). The formula
  has further terms elsewhere (TRetVoid::Return: sz 1, c1 -15), so this is a description of these two callees only.
- **Ego_d type of the object destroyed there.** eglocals (Ego_d S_BLOCK32 / S_BPREL32, `eglocals_fm.txt`): loop-2 block 0x1d8e5ac+2135
  holds `speech_movie` type 0xAFB1 = `CScriptGameResourceObjectMovieBase` and `PauseMyEntitiesPlease` 0xA2B2 =
  `CWideScreenMagicPauseEntities`; `ChatTimer` 0xC8F7 = `CTimer` at function scope. That is the class the 55 clean rows use (shared
  `engine/CScriptGameResourceObjectMovieBase.h`, implicit dtor, COMDAT 0x006E7B80). Ego_d names no other class there.
- **0x00EB40C0: one class.** Its only movie local is [esp+0x2c]: built at 0x00EB410F (`call 0x0099A380` CBaseObject ctor, vtable
  0x01260EF4 = CScriptGameResourceObjectMovieBase, +8/+0xC zeroed). Expanded at 0x00EB4189 / 0x00EB4443 (`lea ecx,[esp+0x34]` = +8;
  `call 0x006E7AB0`; `mov [esp+0x2c],0x0126008C`; `call 0x0099A430`) and called at 0x00EB429A / 0x00EB42C1 / 0x00EB45E2 / 0x00EB4600
  (`lea ecx,[esp+0x2c]; call 0x006E7B80`), all at the same stack depth (`sub esp,0x2c` + 4 pushes, every exit pops 4). So the expanded
  and called dtors there are one class, CScriptGameResourceObjectMovieBase (labels: 0x006E7B60 ctor stores 0x01260EF4; 0x006E7B80 = its
  dtor, Ego_r + FableWin). Disassembly: `eb40c0.dis`.

## Step 2: the 5-B instruments
### (1) PROC diff, Ego_d register_scripts.obj v our VC10 object
- Ego_d.pdb's DBI is broken (29 modules; the MSF stream directory does not list the page at raw 0x161004), so the compiland was carved by
  signature: `tools/egprocscan.py 161004 7C0806` -> `egod_rs_procs.tsv`, 9640 S_GPROC32/S_LPROC32. Second source: FableWin Mod 0011
  `Debug\register_scripts.obj` (llvm-pdbutil dump, readable) -> `fw_rs_procs.tsv`, 9642 PROCs. Name+size diff: identical except two
  FableWin entries the carve lost at page breaks (`std::_Iter_cat<...>` 27 B, `CEntityScriptBinding<CQ_HeroSoulsArenaScript>::
  CreateActiveEntityScript` 696 B), neither a candidate. So FableWin's list is the complete one and adds nothing.
- Ours: VC10 SP1 /Od (odall10 fablelib preset, lane headers + AUTH13 hdr) on p6d -> `od/p6d_od10.obj`, 28 PROCs (`od/ours_procs.tsv`).
- Compile line check: FableWin Mod 0011's own S_ENVBLOCK (FableScripting) is `-W4 -WX -Od -Oy- -DWIN32 -DNDEBUG -D_LIB -DLIB_COMP_DEBUG
  -DLIB_NOT_DLL -D_CRT_SECURE_NO_WARNINGS -D_CRT_NON_CONFORMING_SWPRINTFS -D_MBCS -Gm- -EHs -EHc -MDd -GS- -Gy -fp:precise -Zc:wchar_t
  -Zc:forScope -GR -Gr -TP`. Code switches equal the fablelib preset; only the defines differ (_LIB / LIB_COMP_DEBUG v _WINDOWS /
  GAME_COMP_DEBUG / USE_VIRTUAL_KEYBOARD), and no lane or overlay header tests them. Rerun once with exactly that line (`odfs/`): per-line
  sizes identical, total 11912, 3233 instructions equal to the fablelib build. So the 5 B is not a compile-line effect.
- The compiland's non-script PROCs (`egod_core.tsv`; kept COMDAT copies only, so the set is "functions first emitted here") hold no
  argument-less free or static function a macro line could call: the free functions are ConsolePlayCutscene / ConsoleDisplayCSTimecode
  (both `void (const std::vector<CGenericVar>&)`, console commands), Get{Number,String}FromDataMarker, LoadDataFromMarker,
  GFLimitLower/Upper<float,int>, GFDebugPDynamicCast<...> and dynamic initializers. The rest are members (a member call under /Od needs
  `lea/mov ecx` + call >= 8 B, not 5): CTimer, TRet<>, TRetVoid::Return (11 B, ours has it), CCountedPointer<CEntityScriptBase>,
  CScriptGameResourceObjectMovieBase::Release (24 B, marked inline, a member), CSpawnedFuncBase, CConversation, ...
- Typed query (advisor follow-up; no name filters, all namespaces): each carved PROC's type index resolved in Ego_d types.txt
  (`tools/noarg.py`), keeping LF_PROCEDURE with `# args = 0` and LF_MFUNCTION with no `this` type and `# args = 0` (the only shapes a
  bare 5-B `call rel32` can be under -Gr):
  - register_scripts range (`noarg_rs.txt`, 24): the five `C*Data::Alloc` / `C*Script::StatGetMasterScriptDataAlloc` factory pairs,
    eleven `CPersistContext::GetDefaultVal<enum ...>`, three dynamic initializers / atexit dtors. None is reachable from the macro (this
    row's script has no StatGetMasterScriptDataAlloc entry), and none is an empty body that VC7.1 /O2 could drop.
  - Whole PDB, code size <= 40 B (`noarg_all40.txt`, 3530 records, 780 distinct names): the empty 5-B bodies are
    CEnginePrimitiveRenderUtil::FailIfXbox, CFileInstaller / CThreadedFileManager::ProcessDirtyDiscError, DoDirtyDisc,
    CMainGameComponent::GenerateMetFilesFromLugFiles, GFMountUtilityDrive and GFFPUClear (FableWin: `void ()`, marked inline, kept in
    main_game_component.obj). These seven fit the profile exactly (an argument-less call of an empty body: 5 B under -Gr at /Od, gone at
    /O2). The rest are type getters (StatGetType / StatIsLocal ...), MicroThread / threaded-file managers and CRT / zlib / lzo entries.
    **Instrument (1) can neither confirm nor exclude the seven; none is selected.** Nothing in the PDBs ties one to the 14 macro lines
    (no code), and the repo has no script text that calls one (GFFPUClear appears in ghidra_out name tables, egod_carved_functions.tsv and
    two header comments, GFFPUSetRoundingNearest.h / GFFPUSetDoublePrecision.h, never as a call). Their FableWin addresses go into the
    instrument-(3) question.
  - A COMDAT kept in an earlier object would be missing from the compiland list, but the whole-PDB query covers that case.
  - Members of the macro's classes are out regardless: a member call under /Od needs `lea/mov ecx` + call, at least 8 B.
- **Result: does not name the 5 B** (seven unselected profile matches, listed above).

### (2) PDB parameter counts v our declarations (every function the macro line calls)
Ego_d types (LF_MFUNCTION via the class field list, `tools/tlook.py`); second source Ego_r S_GPROC32 types; ours = rebuild/include.
The `seh_*` locals are typed 0xE6CC (field list 0xE6CB); tlook read the first definition 0xCB33 (field list 0xCB32). The two field lists
are equal modulo type indices (1212 B each, diffed), and GetScriptThing / IsPerformingScriptTask / both Speak overloads re-read from
0xE6CB give the same counts and types. CGameScriptInterfaceBase's two definitions (0xCFB3 / 0xE730) are likewise equal.

| function | Ego_d | Ego_r | ours |
|---|---|---|---|
| CGameScriptInterfaceBase::GetHealth | float (const CScriptThing&) const, 1 arg | float (con...) | `virtual float GetHealth(const CScriptThing&) const` |
| CScriptGameResourceObjectScriptedThingBase::GetScriptThing | CScriptThing () const, 0 args, returns cxx udt | CScriptThing (...) | `virtual CScriptThing GetScriptThing() const` |
| CScriptGameResourceObjectScriptedThingBase::Speak (text) | void (const CScriptThing&, const char*, ETextGroupSelectionMethod, bool, bool, bool), 6 | void ... | same, 6 |
| CGameScriptInterfaceBase::GetHero | CScriptThing& (), 0 | CScriptThing& | `virtual CScriptThing& GetHero()` |
| ...::IsPerformingScriptTask | bool () const, 0 | bool | `virtual bool IsPerformingScriptTask() const` |
| CGameScriptInterfaceBase::NewScriptFrame | void () const, 0 | void () | `virtual void NewScriptFrame() const` |
| CEntityScriptBase::IsActiveThreadTerminating | bool () const, 0 | bool () | `bool IsActiveThreadTerminating() const` |
| TRetVoid::Return | void (), 0 | | `void Return() { }` |
| temporaries / scope-exit dtors (~CScriptThing, ~CWideScreenMagicPauseEntities, movie, ~CTimer, ~ScriptedThingBase) | 0 | | 0 |

- Every count and every parameter type agree; no default argument exists to supply an extra imm32 / address push.
- **Result: does not name the 5 B.**

### (3) not run (by the ruling: FableWin code for one macro expansion goes to the next Codex package). See TERMINAL.md.

## Step 3: not reached (nothing named, so no candidate is pre-registered and nothing is graded). Step 4: TERMINAL.md.

## FableWin has the same 5 B (for the instrument-(3) question)
- FableWin Mod 0011 `Debug\register_scripts.obj`: CFanMarker::Main 0002:01E8BBB0 (VA 0x0361ABB0), code size 11940; CBowerstoneFan::Main
  VA 0x03616030, 7476 (= Ego_d 11940 / 7476). FableWin's line table for CFanMarker::Main (`fw_fm_lines.txt`, from
  `llvm-pdbutil dump -l --modi=11` on FableWin.4k.pdb) is identical to Ego_d's: all 198 lines, offsets and sizes.
- Macro line 887 (loop-2 Oakvale fan hit block) = FableWin offset 0x1DD2, VA 0x0361C982, 561 B (888: 0x2003 / 0x0361CBB3; 889: 0x2234 /
  0x0361CDE4; loop 1: 740/741/742 at 0x598 / 0x7B5 / 0x9D2, 541 B). Ours (VC10 /Od, p6d lines 289..298) = 556 B: listing
  `od/vc10_line289_298.cod`.
- FableWin.exe itself is not on this lane (only Ghidra signature tables from it: ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv
  lists 0x0361ABB0 with a 7504-B Ghidra body, i.e. Ghidra split the function; BowerstoneFan 7476 B / 1841 instructions).

## Hygiene
- `eb40c0.dis` holds retail instruction bytes: scratch only, never copy it into pending/ or the repo.
- The page-walk list `egod_rs_procs.txt` was deleted (it lost records at page breaks); `egod_rs_procs.tsv` (signature scan) is the list.
