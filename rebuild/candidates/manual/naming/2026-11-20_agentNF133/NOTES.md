# NF133 notes (round 10, PRIORITY slice 11: 33 queue lines, 0x009BE420 .. 0x00A0AA20)

Early hand-back (context). Counts from APPLIED.tsv / STATUS.tsv / recheck_final.log:
- Entries (33): 8 done (q4294, q4660, q4688, q4730, q4938, q5200, q6293, q6298), 4 done-already (q4939, q4941, q6304, q6306),
  1 rejected (q5005), 1 blocked (q4727: finished three-row family, needs a coordinator decision), 3 deferred (q4292 / q6168: 0x00499880 held by NF129; q5050: 0x009CE8D0 held by NF132),
  16 open (q4290 and q5199 / q5220 partial; q3887, q4056, q4197, q4198, q4291, q4295, q4296, q4299, q4659, q4942, q4943, q6305, q6308
  not started). Each open line in APPLIED.tsv carries the binder census, so the resume does not need to re-probe.
- Rows: 14 ported (4 MATCH + 10 RELOCATION_MATCH, every test PASS, one recheck_final.log run; no HARD / RELOC_CONFLICT), 0 blocked.
  No row was left half-done in work/.
- Header opt-ins: none (hdr/README.md).
- selfcheck.log: 0 problems.

## Probe (resume aid)
work/probe.tsv: every REFS row (512) compiled once at HEAD with its catalog flags, and the symbol it binds at each entry address
(tools/probe.py; summary script work/summ.py). Rows that graded the wrong function there (no Leaf line, multi-block July rows on
candidates/*_compiled.h: 0x00BE6F70, 0x00BEBED0, 0x00BEC000, 0x00BEC470, 0x00BEC950, 0x00BF4570) were settled by objdump of their
undefined symbols: 0x00BE6F70 / 0x00BF4570 reach the entry addresses only through helper stand-ins (bind nothing there);
0x00BEBED0 / 0x00BEC000 / 0x00BEC470 / 0x00BEC950 hand-declare wrong spellings in shared candidates headers (recorded in APPLIED /
NAME_FIXES; old candidates-header rows left alone, as NF44 says). 0x009E15E0 graded DIFFER(99v99) in the probe (no Leaf line).

## Self renames (RENAMES s1) and their binders
- 0x009FE620, 0x009E50C0, 0x00A058C0, 0x009E9080, 0x00A04090: every REFS row already bound the claim (probe), so the self rename
  closes the address.
- 0x00A0AA20: both REFS rows (0x00987FE0, 0x009880E0) are ported here and bind it.
- q4727 is a THREE-ROW FAMILY (0x00A0AA20 self + binders 0x00987FE0 / 0x009880E0); it lands whole or not at all. The binders bind the claim
  only in their ported form, and that port renames their own leaves from the View stand-ins onto Ego_r.pdb 0x00AE4640 / 0x00AE4740 +
  FableWin.pdb 0x030312E0 / 0x030321C0 ?Enable/DisableVertexShaders@CShaderRenderManager@@QAEXXZ. Most referencing rows bind that
  spelling (reloc_bindings witness 0x009D8AD0), but a source grep finds four rows on the stand-in CShaderEnableState spellings:
  0x00B5E700, 0x00B61690, 0x00B88BC0, 0x00BCB800 (~200-line rows, not in my locks, not ported). The renames neither create nor remove
  that split, but the strict "every REFS row on the new spelling" rule is not met, so q4727 is APPLIED `blocked (family, coordinator
  decision)`: land the trio if the gate accepts it, or port the four rows first.
- File names: the coordinator may rename E2_E2_00a0aa20.cpp, CInputEvent_SetAsJoystickButton1_00a04090.cpp and the View-named
  sources; no EH funclet includes any changed row (grep of rebuild/src and rebuild/tests).

## In-place respells of an existing local model (accepted form; token check: no new class token)
- 0x009FE620: local CGraphicInfoFrame: BeginLoading const, Data mutable (as the PDB class).
- 0x009E50C0: local CSymbolInfo (a global struct at HEAD) nested in the row's local CSymbolMap and class-keyed. The object defines
  only the leaf.
- 0x00A04090: local CInputEvent: SetAsJoystickButton1(bool, long, ...) -> SetAsJoystickButtonPressed(long, float, ...); m_8 a byte.
  Self parameter list from two sources (both PDBs) plus the body and the caller.
- 0x00B4F510: local CDisplayManager::ClearRenderTarget takes const CRGBColour& (was const CRGBColour*).
These rows keep their pre-existing SHARED_HEADER_AVAILABLE warnings (unchanged from HEAD).

## Header ports (moved off stand-ins onto shared headers)
- 0x00A058C0 (CRenderStateManager.h retail branch + IDirect3DDevice9.h), 0x009E9080 (CPersistTraits.h generic inline body,
  explicitly instantiated), 0x00A0AA20 (CRenderManagerCore.h, the 0x00A0AA10 SetMaterial template), 0x00987FE0 / 0x009880E0
  (CShaderRenderManager.h + CRenderManager.h), 0x00A62E20 (CMovableResourceMemoryPool.h: base CHandleBasedMemoryPool; clears the
  ??0CEnginePrimitiveTypeInfo@@QAE@XZ binding at 0x00A02D20, which reloc_bindings also has at 0x00B8FAA0), 0x009B08C0 / 0x00B26360 /
  0x00B2C8E0 / 0x00A02220 (engine/GFGetTime.h). 0x00A62E20 now also emits the compiler-generated
  ??_GCMovableResourceMemoryPool@@UAEPAXI@Z as a pick-any COMDAT; no landed row defines it strongly (0x00A62F90 is a free stand-in).
- New PDB spellings replacing stand-ins on callees, none growing an address's spelling count: 0x009E2590 in 0x009E9080 (moves onto
  the self spelling), 0x00A02D20 in 0x00A62E20, 0x009E1BC0 in four rows, 0x009BE420 in 0x00B4F510.

## Tests
Rewritten (PassPattern unchanged): 0x00A058C0, 0x00987FE0, 0x009880E0 (they linked the row's stand-in member; now standalone host
models carrying the same body) and 0x00A04090 (host model on the new signature). The others are standalone and unchanged.

## Locks claimed beyond rows.txt
0x009880E0, 0x00A62E20, 0x009B08C0, 0x00B26360, 0x00A02220, 0x00B2C8E0 (owner file read `released`; claimed). None is in
UNMATCHED_ROWS.tsv as handed off / reserved for Account B.

## Flags lines
None added. 0x00B2C8E0 grades with its build entry CompilerFlags (/d2inlS100 /d2inlT200), unchanged.

## Descriptive names
No new made-up names. Parameter names of 0x00A0AA20 (enable) and 0x00A04090 (button, pressure) are descriptive (unnamed in the PDB
declaration).

## Existing splits left as found (not in my entries or locks)
- 0x013CAA70 (allocator critical section) has four data spellings (g_flashInstance / g_fableSharedLock / g_allocatorLock /
  g_perfStatsObject); no PDB data name found (Ego_d names have none).
- 0x009BF160 SetRenderTarget is declared `(CRenderTarget*)` in 0x00B4F510's local model; the PDB takes CRenderTarget& (not changed).
- 0x00411570 is bound as ?Compare_00411570@@YIJPBD0@Z by 0x009E50C0 (unchanged) besides ?Compare@?$CBasicString@D@@KIJPBD0@Z.

## Rule slips
- One STATUS.tsv update loop matched WARN lines of a vland log and wrote them into STATUS.tsv; caught on the next read and repaired
  by hand (no data lost).
- q4290 was left partial (its last binder ported, the self port not started) before q4294 was taken: out of queue order.
- Two host-side g++ checks of rewritten tests could not run (no 32-bit libc in the container); vland's test runs are the evidence.
