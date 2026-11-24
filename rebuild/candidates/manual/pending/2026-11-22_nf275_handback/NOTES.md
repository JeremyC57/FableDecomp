# NF275 notes (2026-11-22, round 31b)

Base: the repo working tree as of this session's start (orig/ copied then). NF274 runs beside me (0x00BEB490 / 0x00BEAAA0 / 0x00BEB4D0,
0x00BDA150 / 0x00BD9CE0, 0x00BFDE20 / 0x00BFDD70 / 0x00BFDE80): none of its rows touched; its hdr/ was empty when I made mine. Every row I
compiled was `released` or had no lock directory before I claimed it with tools/lock.sh (work/locks.log); none is in codex_pass28
RESERVED.tsv (grep: 0 hits for each address compiled).
Reading list: BRIEF.md (NF275), NF273 and NF270 BRIEFs, NF214_BRIEF.md, ASM_RULES.md, EXTRA_RULES.md, COORDINATION.md, GOTCHAS.md (all 503
lines, before the first compile), the nf270 / nf273 landing READMEs, the NF273 hand-back (NOTES, LESSONS, NAME_FIXES), the NF236 / NF244 /
NF247 notes on q12659 (none started it) and its NAME_FIXES_QUEUE lines, plus the NF152 "Needs header claim" and NF234 entry-2 notes those
lines point to.

## Entry 1: q12659 (0x00418DCA), the CMainGameComponent family header port -- done (needs the landing rulings below)

### The claim (two sources each)
- Vtables 0x0122F180 / 0x0122F174 are ??_7CMainGameComponent@@6BCBaseClassNonCopyable@@@ / ??_7CMainGameComponent@@6BCDeviceResetCallback@@@:
  all three PDBs' publics (NF233 / NF234 census) and retail RTTI (COL 0x0132D260 / 0x0132D20C: CMainGameComponent : CGameComponent
  (: CBaseClassNonCopyable : CBaseClass) +0, CDeviceResetCallback +4); the landed dtor 0x00418B7A already binds them. HEAD 0x00418DCA bound the
  local stand-ins 6BCBaseClass@@@ / 6BCComponentListEntry@@@ (its local CGameComponent was based on CBaseClass + a made-up CComponentListEntry).
- 0x009C0190 is ?AddDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z: Ego_r.pdb + FableWin.pdb, its self row's Leaf, and
  landed binders 0x009A6610 / 0x00B26360; the ctor passes the +4 CDeviceResetCallback subobject (retail 0x00418DE0 `lea edi,[esi+4]`,
  0x00418EDC `push edi`, ecx = GPDisplayManager 0x013B8390). HEAD bound the stand-in ?AddComponent@CComponentList@@QAEXPAUCComponentListEntry@@@Z.
- The member ctors keep the spellings HEAD already bound (each the only table spelling at its address and in Ego_r.pdb + FableWin.pdb):
  ??0CInterpolationInfo@@QAE@XZ 0x00415A60, ??0CGameEventPackageSet@@QAE@XZ 0x009F1A80, ??0CNetworkClient@@QAE@XZ 0x00419323,
  ??0CDiskFileWin32@@QAE@XZ 0x00415C30, ??0CFrameRateSmoother@@QAE@XZ 0x00419747, ??0CRoughFrameCounter@@QAE@XZ 0x009F8B10,
  ??0CMainGameComponentInit@@QAE@ABV0@@Z 0x00415E17, ??0CGameComponent@@QAE@AAVCGame@@@Z 0x0051D8E0.
- Slots (retail tools/rvt.py; PDBs): 0x0122F180 = ??_G 0x004197B3, Init 0x004184BD (Ego_r + FableWin ?Init@CMainGameComponent@@UAEXXZ),
  Run 0x004189C2, ChangeTextureColourDepth 0x0041799B, SetQuit 0x004197A0 (FableWin ?SetQuit@CMainGameComponent@@UAEXXZ; body sets Quit +8),
  Update 0x00418289 / GetInputs 0x00416E78 / Render 0x00417001 / PostInit 0x00416953 (Ego_r + FableWin MAEXXZ, protected), IsEditorActive
  0x004197B0 (FableWin UBE_NXZ); 0x0122F174 = OnPreDeviceReset 0x00415E83 / OnPostDeviceReset 0x004175DA (Ego_r + FableWin). The base table
  0x0122ECB0 has CGameComponent's own Init 0x00415920 / SetQuit 0x00415930, so the class overrides both: the shared header lacked the two
  overrides (its vtable would have bound slots 1 / 4 to the base).

### The port (rows/00418dca.cpp)
- The row drops every local model (CGameComponent : CBaseClass + CComponentListEntry, CComponentList, CMainGameComponent, CInterpolationInfo,
  CGameEventPackageSet, CNetworkClient, CDiskFileWin32, CFrameRateSmoother, CRoughFrameCounter, CMainGameComponentInit, scoped_ptr) for
  engine/CMainGameComponent.h on engine/CGameComponent.h, engine/CDisplayManager.h (FABLE_RETAIL_CDISPLAYMANAGER, which declares
  AddDeviceResetCallback; Direct3D9Types.h / Win32Types.h first, as landed 0x009A6BA0) and the member headers. Overlaid headers are included
  by the row itself, first. The body is HEAD's apart from `GPDisplayManager->AddDeviceResetCallback(this)` (the implicit derived-to-base
  conversion gives the +4 subobject).
- Existing opt-ins reused for the typed members: SMART_POINTER_ACCESSORS (PTextBank, PWorld), PPLAYERINTERFACE, EVENTPACKAGES (PDisplayEngine,
  Client; with CNETWORKCLIENT_UPDATE for the real CNetworkClient), FORCEDTICK (LastInterpolationInfo, RoughFPSCounter), UPDATE (EventPackageSet),
  MAINFONT (PCutSceneMainFont), ENGINEGRAPHICS (PMainGraphicBank), INITSTRUCTURE (InitStructure), EDITOR (slots 5..9), SCOPED_PTR,
  CDISKFILEWIN32. New (hdr/README.md): CMAINGAMECOMPONENT_CTOR, CMAINGAMECOMPONENT_INIT_SETQUIT, CNETWORKCLIENT_CTOR,
  CGAMEEVENTPACKAGESET_CTOR, CROUGHFRAMECOUNTER_CTOR, CDISKFILEWIN32_DEFAULTCTOR_OUTOFLINE, CDISKFILEWIN32_DTOR_DECL_ONLY, and the new
  engine/CFrameRateSmoother.h (size 0x30: the ctor's +0x16178 / +0x161A8 stores, 0x00419747 zeroing exactly the 12-byte Times at +8, and the
  landed RELOCATION_MATCH local models of 0x0049B9A0 / 0x0049BA70 with the same offsets; Ego_r member names).
- Forms (evidence/sc_*.log): f1 (CTOR opt-ins without the two decl-only dtors) RELOCATION_MATCH, but the object newly defined pick-any
  copies ??1CNetworkClient@@UAE@XZ (NEQ v 0x00419352, 5 v 19: the header's opaque Server), ??1CAFile@@UAE@XZ (NEQ v the folded 0x0099A300
  table line, 5 v 7), ??1CDiskFileWin32@@UAE@XZ (EQ), ??_7CDiskFileWin32@@6B@, ??_GCDiskFileWin32, GetPosition / GetLength (no owner line),
  all referenced only from .debug$F or from each other. f2 = f1 + CNETWORKCLIENT_CTOR's `virtual ~CNetworkClient();` and
  CDISKFILEWIN32_DTOR_DECL_ONLY: RELOCATION_MATCH, none of those copies. f3 = f2 with the asserts pinned: same result (final).
- Leaf relocations HEAD -> f3 (only these change): 0x009C0190 AddComponent stand-in -> AddDeviceResetCallback; 0x0122F180 6BCBaseClass@@@ ->
  6BCBaseClassNonCopyable@@@; 0x0122F174 6BCComponentListEntry@@@ -> 6BCDeviceResetCallback@@@.
- Symbols HEAD -> f3: the four row-local `scoped_ptr` ctor COMDATs become boost::scoped_ptr's header-inline ctors (no owner, inline in
  retail); ??1CGameComponent@@UAE@XZ (a 5-byte NO-OWNER copy of the local model's implicit dtor) is gone; the two vtables are renamed; the
  vtable slots now reference the PDB members (UND) instead of ?v00@CMainGameComponent@@UAEXXZ. pk (evidence/pk_e1.tsv): no new copy with an
  owner; the one NEQ copy (~CCountedPointer<CGraphicDataBank> v 0x004194B5, 44 v 44) is HEAD's, unchanged (nf203 debt). The
  ??_GCMainGameComponent copy (no table line) is byte-identical to HEAD's and to retail 0x004197B3 by hand (evidence/g_*.txt).
- Vtables (evidence/vtcheck_00418dca.txt): 10 + 2 slots, every slot on its retail target's PDB member; identical slot-for-slot to the
  0x00418B7A copy once that row's slots 5..8 are protected (below). HEAD 0x00418DCA's tables had 1 slot each.

### 0x00418B7A (the other emitter of the two vtables), in place
- At HEAD its local model declares slots 5..8 public (?Update@ / ?GetInputs@ / ?Render@ / ?PostInit@CMainGameComponent@@UAEXXZ), so the two
  rows' pick-any vtable COMDATs under one name would bind different symbols (link set C, evidence/lk_e1.txt) and my MAE spelling would grow
  0x00416E78 (self QAE + 7a's UAE + mine MAE). rows/00418b7a.cpp: `protected:` around those four existing declarations (in-place access
  respell in the row's existing local model; no class token added) plus a Leaf line. RELOCATION_MATCH, leaf relocations identical to HEAD;
  symbols differ only in those four UND slot spellings; pk unchanged (one EQ, the rest NO-OWNER).
- So the two rows move together (a family): landing 0x00418DCA alone would leave two different vtable copies under one name.

### Census (exe scan, evidence/census_e1.txt; spellings per address before -> after)
- 0x009C0190: refs 0x0040CEC0, 0x00418DCA, 0x009A6610, 0x00B26360; {AddComponent stand-in, AddDeviceResetCallback} 2 -> 1.
- 0x0122F180 / 0x0122F174: refs 0x00418B7A, 0x00418DCA only; 2 -> 1 each.
- Vtable slots (no table lines; self leaf + the two emitters): 0x00415E83 {T::Hook self, ?v00@ (dca), OnPreDeviceReset UAE (7a)} 3 -> 2;
  0x00418289 / 0x00417001 / 0x00416953 {MAE self, UAE (7a)} 2 -> 1; 0x00416E78 {QAE self, UAE (7a)} -> {QAE self, MAE} 2 -> 2;
  0x004184BD, 0x004189C2, 0x0041799B, 0x004197A0, 0x004197B0, 0x004175DA, 0x004197B3: my new references use the spelling 0x00418B7A already
  binds there (unchanged sets). No address grows.

### Link (evidence/lk_e1.txt; LINK unset, /DLL /NOENTRY /FORCE /NODEFAULTLIB; HEAD objects of 0x009C0190 and 0x00412F90 as controls)
- A (HEAD pair): 0 duplicates; ?AddComponent@CComponentList@@... and ?v00@CMainGameComponent@@UAEXXZ unresolved.
- B (my pair): 0 duplicates; AddComponent and v00 gone (AddDeviceResetCallback resolves to 0x009C0190's object; the ctor resolves for
  0x00412F90's call); the slot members unresolved as at HEAD (their self rows define stand-ins: NAME_FIXES).
- C (my 0x00418DCA + HEAD 0x00418B7A): 0 duplicates, but both UAE and MAE slot 5..8 spellings referenced: the family reason above.

### vland --recheck-landed (overlay hdr/, evidence/vl_e1.log)
- 0x00418DCA RELOCATION_MATCH + PASS (HEAD test, standalone, unchanged); one WARN RELOC_NAME (0x009F8B10's catalog label
  CBaseIntelligentPointer::CBaseIntelligentPointer against ??0CRoughFrameCounter@@QAE@XZ, which HEAD binds too: NAME_FIXES); no
  SHARED_HEADER_AVAILABLE left (the HEAD row's local models are gone).
- 0x00418B7A RELOCATION_MATCH + PASS (HEAD test unchanged); its WARNs are HEAD's local models plus one new SHARED_HEADER_AVAILABLE for
  CFrameRateSmoother (the new header). 0 RELOC_CONFLICT.
- Newly flagged by engine/CFrameRateSmoother.h (local `class|struct CFrameRateSmoother` models, rows and tests; re-port backlog under the
  Account B owner rule of 2026-11-20, not a hold): 0x00416202, 0x00416219, 0x004164E0, 0x00417001, 0x004175E5, 0x00418B7A, 0x00419747,
  0x0041977D, 0x0049B9A0, 0x0049B9E0, 0x0049BA20, 0x0049BA70.

### Header proof
- Whole-tree equiv_check (tools/eq.sh: repo rebuild/include v a copy with exactly the six overlay files: the five pinned headers and the
  new engine/CFrameRateSmoother.h; every landed source and test): 57,125 rows checked, 0 differ, DONE 0 (evidence/equiv_e1.log /
  equiv_e1.tsv, empty). orig/ re-compared with the repo before and after the run: equal (no header landed meanwhile); the tree copy equal
  to hdr/ file for file. So no landed includer's input changes; the only rechecks are the two changed rows (vland above).
- Includers (direct, grep of rebuild/src/compiled + rebuild/include): CMainGameComponent.h 497, CDiskFileWin32.h 61, CNetworkClient.h 7,
  CGameEventPackageSet.h 6, CRoughFrameCounter.h 1, CFrameRateSmoother.h 0. None defines a new macro.
- Third witness for CFrameRateSmoother's layout (found after the proof run, so the header comment does not cite it): the landed
  RELOCATION_MATCH rows 0x0049B9A0 (CalculateSmoothedTime) and 0x0049BA70 (Init) model the class locally with exactly these members and
  offsets (Times +0x08 .. SmoothedTime +0x28).

### Decisions for the coordinator (entry 1)
1. Land 0x00418DCA (header port) and 0x00418B7A (in-place `protected:` on slots 5..8) together, with the six overlay files: a family, since
   landing 0x00418DCA alone gives two vtable COMDATs of one name with different slot symbols (link set C).
2. engine/CFrameRateSmoother.h is a new header: it newly flags the 12 local-model rows listed above (re-port backlog).
3. NAME_FIXES (not applied; self rows on stand-ins): 0x00416E78 GetInputs (self QAE stand-in, needs a port to become the PDB MAE),
   0x004197A0 SetQuit / 0x004197B0 IsEditorActive / 0x00415E83 OnPreDeviceReset / 0x004197B3 ??_G (stand-in struct T / member), and the
   catalog label of 0x009F8B10 (CRoughFrameCounter ctor, labelled CBaseIntelligentPointer).
4. rebuild/include/INCLUDERS.txt lists 0x00418B7A under its HEAD headers; 0x00418DCA now includes CMainGameComponent.h & co.: regenerate if
   the index is kept by a tool.

## Entry 2: the owner-side CVertexGroupBlock dtor (nf273 ruling 2b) -- done (0x00A96DA0 keeps its bytes; needs ruling 5)

### The change (rows/00a96da0.cpp, hdr/engine/CVertexGroupBlock.h, orig/engine/CVertexGroupBlock.h)
- Opt-in `FABLE_RETAIL_CVERTEXGROUPBLOCK_DTOR_INLINE` (effective only with `_MEMBERS`) in engine/CVertexGroupBlock.h: the existing
  `~CVertexGroupBlock();  // 0x00A96DA0` line moves into the `#else` arm, the opt-in arm is `~CVertexGroupBlock() { }` (members typed, so
  ~map / ~CArray run). The class assert was already `_AT(44)` (pin_asserts: 0 pinned). The macro name had 0 hits in rebuild/.
- 0x00A96DA0 on that opt-in, with HEAD's defines / includes (same order) and the nf86 / nf88 scaffolding: an anonymous-namespace
  `ReferenceVertexGroupBlockDtor()` that builds a local block (out-of-line ctor ??0CVertexGroupBlock@@QAE@XZ 0x00AD9030) and destroys it
  (`// Fidelity: scaffolding, not retail`). First form: RELOCATION_MATCH, leaf relocations identical to HEAD (evidence/sc_e2_v1.log);
  symbols: + the helper ?ReferenceVertexGroupBlockDtor@?A0x...@@YAXXZ (per-TU hash, 23 bytes, calls the dtor out of line) and + UND
  ??0CVertexGroupBlock@@QAE@XZ (table: 0x00AD9030). COMDAT selection of ??1CVertexGroupBlock@@QAE@XZ: HEAD 1 (NODUPLICATES, strong)
  -> 2 (ANY) (evidence/comdat_e2.txt). pk (evidence/pk_e2.tsv): the same 19 copies as HEAD (the NEQ `clear` / NO-OWNER ones unchanged)
  plus the NO-OWNER helper. vland --recheck-landed (overlay): RELOCATION_MATCH + PASS (HEAD test, standalone), WARN RELOC_NAME on the
  _M_erase label as at HEAD (evidence/vl_e2.log).
- Link (evidence/lk_e2.txt): A HEAD owner + HEAD 0x00AD94F0 + HEAD 0x00ADCE60: 1 LNK4006 (0x00AD94F0's EQ copy v the strong owner);
  B new owner + the same two: 0 duplicates; E new owner + 0x00ADCE60: 0x00ADCE60's reference resolves to the owner's copy.
- Header proof: whole-tree equiv_check of that one file (tools/eq2.sh; repo rebuild/include v a copy with only this file replaced): 57,125 rows
  checked, 0 differ, DONE 0 (evidence/equiv_e2.log / equiv_e2.tsv, empty); orig/ re-compared with the repo after the run: equal.

### Census (evidence/census_e2.txt: the 33 local-model rows + the 14 includers of engine/CVertexGroupBlock.h, minus the owner = 44 rows,
### each HEAD object compiled (all RELOCATION_MATCH / MATCH), COMDAT selection + pk of ??1CVertexGroupBlock@@QAE@XZ)
- Define it pick-any (ANY), EQ to 0x00A96DA0 (84 = 84, relocations bound): 0x00AD94F0, 0x00AD9B20.
- Define it pick-any, bytes equal but NEQ:relocs (their copy's _M_erase call binds the 3-argument stand-in
  `?_M_erase@?$_Rb_tree@KU?$pair@$$CBKJ@std@@U?$less@K@2@@std@@...` (row-local `_Rb_tree<K, V, Compare>` models), a spelling the table does
  not bind at 0x004507A1): 0x00ADC190, 0x00ADC250, 0x00ADC4E0, 0x00ADCB90.
- Reference it only (UND): 0x00AA7480, 0x00AD9F10, 0x00ADA240, 0x00ADBE50, 0x00ADC790, 0x00ADCE60 (nf273's declaration). The other 32
  rows neither define nor reference it.
- Link order (evidence/lk_e2.txt set F): with the owner listed LAST, the HEAD link already keeps 0x00AD94F0's pick-any copy and ignores the
  owner's strong one (LNK4006 "already defined in head_00ad94f0.obj; second definition ignored"). So at HEAD the image already gets whichever
  definition the linker meets first, including any of the four NEQ:relocs copies: making the owner pick-any exposes nothing new; it removes
  the duplicate (sets A / F 1 -> B 0). The four NEQ:relocs copies are pre-existing debt either way (bytes equal; their _M_erase call binds a
  3-argument stand-in spelling of 0x004507A1, which the table does not bind there): NAME_FIXES, to fix by giving those rows the shared tree
  model or a declaration-only dtor (each measured on its own: do their leaves expand the dtor?).
- 0x00ADCE60 "back to the inline dtor" (ruling 2b): not done. Its local model declares ~map / ~vector out of line, so its inline copy was
  NEQ (19 v 84 at nf273's HEAD); against a pick-any owner that copy would compete like the four above. It stays on nf273's declaration
  until it is ported onto engine/CVertexGroupBlock.h (MEMBERS + DTOR_INLINE), whose inline copy would be EQ.
- Not rechecked: the 44 census rows' sources and headers are unchanged (their bytes cannot change: they do not define the new macro; the
  equiv_check above covers every landed row).


## Whole-pass checks
- recheck_final.log: one vland --recheck-landed run (FABLE_HDR_OVERLAY=hdr/) over every changed row.
- New symbol names: none defined by a body. The two objects define no new function name: 0x00418DCA's leaf is HEAD's; the two vtable names
  ??_7CMainGameComponent@@6BCBaseClassNonCopyable@@@ / 6BCDeviceResetCallback@@@ are compiler data COMDATs that 0x00418B7A already emits
  (identical slots now); the boost::scoped_ptr<T> ctor COMDATs are header-inline (no owner, inline in retail). New macro names (7) and the new
  file engine/CFrameRateSmoother.h: 0 hits in rebuild/ before (grep).
- Entry 2 names: the scaffolding helper ?ReferenceVertexGroupBlockDtor@?A0x<tu>@@YAXXZ (anonymous namespace, one body: rows/00a96da0.cpp; no
  hit in rebuild/); ??1CVertexGroupBlock@@QAE@XZ keeps its one owner body (now pick-any, byte-equal); macro
  FABLE_RETAIL_CVERTEXGROUPBLOCK_DTOR_INLINE: 0 hits in rebuild/ before.
- Funclets / folds: no landed source, test or header includes 0x00418DCA's, 0x00418B7A's or 0x00A96DA0's file (grep of each file name;
  rebuild/include/INCLUDERS.txt only indexes 0x00418B7A).
- Descriptive names added: none. Flags lines added: none.
- Nothing written in the repo; no git; no sleep and no foreground waits (jobs launched with `&`, checked with kill -0 in later calls).
- recheck_final.log (tools/recheck_final_ovl.sh, FABLE_HDR_OVERLAY=hdr/): 0x00418DCA, 0x00418B7A, 0x00A96DA0: 3 RELOCATION_MATCH + PASS, 0 RELOC_CONFLICT
  (WARNs only the HEAD-present RELOC_NAME labels and 0x00418B7A's local-model SHARED_HEADER_AVAILABLE lines, + CFrameRateSmoother).
- Locks: 0x00418DCA / 0x00418B7A stay NF275 (rows.txt). Released: the controls 0x009C0190 (no lock dir before; claimed new) / 0x00412F90,
  and the 44 census rows; 0x00A96DA0 released after the census and re-claimed (NF275) when it went back into rows/ (all were `released` or had no directory when claimed). None is in codex_pass28 RESERVED.tsv or
  among NF274's entries.
- Tools added: tools/comdat.py (copied from agentNF273), tools/eq2.sh, tools/cen2run.sh, tools/recheck_final_ovl.sh.

## Rule slips
- Three Bash calls exited 1 (a `cmp` of two objects that differ only in the COFF timestamp, a trailing `ps` / `kill -0` on finished jobs).
- tools/cen2run.sh (the 44-row census) was launched under `timeout 3600`; each of its grades carries `timeout 900` on its own sc.sh line.
- Entry 2's equiv_check ran beside entry 2's own census compiles and the 0x00A96DA0 vland run (same entry; entry 1's equiv had finished).

## Decisions for the coordinator
1. Entry 1: land rows/00418dca.cpp + rows/00418b7a.cpp together (family) with the six hdr/ files (five add-only opt-in headers, asserts
   pinned, + the new engine/CFrameRateSmoother.h); equiv 0 of 57,125 differ; HEAD tests and PassPatterns kept. TABLE_CHANGES: remove the
   three 0x00418DCA-witnessed stand-in lines.
2. engine/CFrameRateSmoother.h newly flags 12 local-model rows (list in entry 1): re-port backlog.
3. NAME_FIXES (6 lines, not applied): the stand-in self rows of the vtable slots 0x00416E78 / 0x004197A0 / 0x004197B0 / 0x00415E83 /
   0x004197B3 and the catalog label of 0x009F8B10.
4. A linking test for 0x00418DCA (nine callee stubs + the twelve slot members) is test debt; the HEAD test is a standalone model.
5. Entry 2: land rows/00a96da0.cpp with the CVertexGroupBlock.h opt-in (bytes and relocations unchanged, COMDAT NODUPLICATES -> ANY,
   LNK4006 1 -> 0). The four NEQ:relocs pick-any copies (0x00ADC190, 0x00ADC250, 0x00ADC4E0, 0x00ADCB90) can already win at HEAD (link set
   F), so the change exposes nothing new; queue those four rows (NAME_FIXES) -- or hold the owner change until they are fixed, if you prefer.
6. 0x00ADCE60 stays on nf273's declaration (REJECTED line) until it is ported onto engine/CVertexGroupBlock.h.
