# NF271 notes (2026-11-22, round 29): entry 1 done as far as a naming pass reaches (two units); entries 2-5 not started

Handed back after entry 1 (context). Four rows changed, one witness: one vland --recheck-landed run with the overlay on
(recheck_final.log: 0x00BD9CA0 / 0x00BFAAE0 / 0x00BFB590 / 0x00B684F0 + witness 0x00BD9B90, 5 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT).
One header opt-in (hdr/README.md, equiv_check 0 of 1,399 rows differ, control 28). Repo root (`ls -A`) before / after: identical.
No git, no sleep, no __asm / naked / _emit.

## Unit A: LocalDetailEngine (0x00BFAAE0 / 0x00BFB590) with the self row 0x00BD9CA0
- **Correction to the nf262 / nf263 plan and to the row comments:** the stand-in `LandscapeMemoryMethods::FreeAlignedVirtual` is NOT 0x00BD9B90.
  Retail 0x00BFAAE0 calls 0x00BD9CA0 at 0x00BFAB06 / 0x00BFAB11 / 0x00BFAB27 (and 0x00BD9B90 at the other two sites); the table already mapped
  the stand-in to 00bd9ca0. 0x00BD9CA0 is `?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z`: both PDB name sets (Ego_r 009c2430,
  FableWin 02cb7a40), Ego_r's run order = retail 0x00BD9C40 / 0x00BD9CA0 / 0x00BD9CC0, the body (free block - block[-1] via a FreeVirtual tail
  jump), and the freed members are the aligned arrays (evidence/unitA_freevirtualaligned.md, mnemonics only). The catalog name was already right;
  the self row defined the stand-in `?FreeVirtualAligned@CEngineLandscapeMemoryManagerMethods@@QAEXPAX@Z` (no Leaf line).
- 0x00BD9CA0: ported onto the retail branch of engine/CEngineLandscapeMemoryManager.h with one new declaration opt-in
  (`_FREEVIRTUALALIGNED`, the nf267 D3 shape); its method-projection struct and the renamed-include local model go. Full Leaf line added. objsec:
  every section byte- and relocation-equal to HEAD after the leaf rename except `.drectve` (+`/DEFAULTLIB:"libcp"`, the retail branch pulls
  stlport/fable_containers.h). No pick-any copy in the object. Test rewritten on the shared header (raw-storage manager, FreeVirtual stubbed; it
  links the row; PassPattern `00bd9ca0_TEST PASS` kept).
- 0x00BFAAE0 / 0x00BFB590: in place (nf262 D3 / nf269 ruling 1 precedent): `FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER` defined
  before the first include, `LocalDetailEngine` / `g_LocalDetailEngine` / `LandscapeMemoryMethods` deleted, the manager read as
  `NEngineInternal::GPLandscapeRenderer->MemoryManager`; 0x00BFAAE0's existing row-local `class CEngineLandscapeMemoryManager :
  CEngineLandscapeMemoryManagerLayout` gains the one declaration `FreeVirtualAligned` (in-place respell of a model the row already had; no new
  class token). The rows' other local models (CMovableResource, CIVCountedPointeeBase, CEnginePersistentPrimitiveBase, CEnginePrimitiveHandle,
  C4DVector, the manager) stay: HEAD debt (SHARED_HEADER_AVAILABLE warnings, unchanged), a port. objsec ALL_EQ with the renames (19 / 20
  sections); DEF lists identical; pk.py lists identical HEAD v new; vtcheck of `??_7CLocalDetailPrimitiveRepeatedMesh@@6B@` v 0x012A82AC and
  `??_7CLocalDetailPrimitiveMeshZSpriteBatch@@6B@` v 0x012A82D8 identical HEAD v new. The other vtables they emit (`??_7CEnginePrimitiveHandle@@6B@`, `??_7CMovableResource@@6B@`, row-local
  models, HEAD debt) are covered by objsec ALL_EQ (every data COMDAT and its relocations equal HEAD's); not vtchecked separately.
- Tests of 0x00BFAAE0 / 0x00BFB590: self-contained host models (inline their own copy of the body; they never link the row), unchanged;
  they still carry `LocalDetailEngine` / `LandscapeMemoryMethods`: add them to the entry-4 (host-model tests) debt list.

## Unit B: 0x00B684F0 (`?g_pEngineGlobal_01436EA8@@3PAUCEngineGlobal_01436EA8@@A`)
- The stand-in pointee carried only `CEngineLandscapeMemoryManager* LandscapeMemoryManager` at +0x6B0 (no callee spelling on it), so the
  global moves alone: landed `_GPLANDSCAPERENDERER` opt-in, `GPLandscapeRenderer->MemoryManager`. One binder (source grep); objsec ALL_EQ with
  the rename (13 sections), pk identical, no vtable. Added a Leaf line pinned from the HEAD object (HEAD had none):
  `?LoadNewScene@CEngineLandscapeRendererCallView@@QAE_NABVCEngineComponentLoadNewSceneParams@@@Z` (the stand-in self leaf, HEAD state).
- Left (blocked: port): the self leaf on `CEngineLandscapeRendererCallView` and its callee stand-ins `ClearTexturePool@...MemoryManagerCallView`
  (= 0x00BD9CE0 `?FreeDisposalList@CEngineLandscapeMemoryManager@@QAEXXZ`, recheck RELOC_NAME warns), the three `CEngineLandscapeMapCallView`
  members 0x00BDC180 / 0x00BDC510 / 0x00BDC020 and `LoadForegroundTextures` 0x00B68070. Each needs a header declaration (renderer / memory manager
  / quarantined CEngineLandscapeMap) and a self rename of the leaf (vtable 0x012A2B54 slot 3 binder census first).
- Test: self-contained host model, unchanged (entry-4 debt list).

## CEngineGlobal family (0x00BE61B0 / 0x00BE79D0 / 0x00BF8030 / 0x00BF84C0): blocked: port (read-only census, nothing compiled)
- One shared stand-in spelling `?g_pEngine@@3PAUCEngineGlobal@@A` (one table line, witness 0x00BE61B0), four different pointee models, so the
  spelling drops only if all four move. Three pointees carry what a global move cannot keep:
  0x00BE61B0 a by-value `CLandscapeTesselatorBase Tesselator` at +0x66C whose `TesselatePatch` (0x00BE3910, row comment) is called on it
  (the renderer header types +0x66C as `CLandscapePatchTesselator`: the callee respells with it); 0x00BF8030 `CMovableResourcePool* Pool` at +0x6B0 with `Alloc` / `Free`
  stand-ins declared for 0x00BD9AF0 / 0x00BD9B90 (the renderer's +0x6B0 is the MemoryManager: AllocVirtual / FreeVirtual move with it); 0x00BE79D0 a descriptive `OcclusionEnabled` byte at +0x63E
  (CLandscapeSettings +0x36 in the renderer header: needs the Ego_r member name). 0x00BF84C0 alone could move (`Pool->FreeVirtual` is already
  the PDB spelling) but that is count-neutral while the other three bind the stand-in. Classes that must move first: CLandscapePatchTesselator
  (callee spellings), the memory manager's AllocVirtual on 0x00BF8030, CLandscapeSettings' +0x36 member. nf260 noted two of the four fail the
  clang precheck at HEAD (not re-measured here).

## Census
- 0x00BD9CA0 landed spellings 2 -> 1 (self `...Methods` + binder `FreeAlignedVirtual@LandscapeMemoryMethods` -> the PDB name; table 1 -> 1 line,
  respelled). 0x01436EA8 table 11 -> 9 lines (`g_LocalDetailEngine`, `g_pEngineGlobal_01436EA8` lose their only binders). 0x00BD9B90 unchanged
  (its untabled `FreeAlignedVirtual` entry in nf262's plan was the 0x00BD9CA0 stand-in). No address grows; no split created.
- New NAME `?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z`: defined only by 0x00BD9CA0's object (objdump of all objects built
  here; source grep: only the 0x00BD9CA0 row defines a body; 0x00BFAAE0 declares it; 0x00BDA110 names it in a comment). Its test defines only the
  FreeVirtual stub (test-only). `GPLandscapeRenderer`: UND in every row, defined by none (as before).
- Links (LINK unset, /NODEFAULTLIB /FORCE, tools/lnk.py): the four new objects + HEAD 0x00BD9B90: 0 duplicates; the HEAD set: 0; controls (a
  second copy of 0x00BD9CA0 / of 0x00B684F0): 1 LNK4006 each.
- Table (TABLE_CHANGES.tsv): delete `FreeAlignedVirtual@LandscapeMemoryMethods`, `g_LocalDetailEngine`, `g_pEngineGlobal_01436EA8`; add
  `?FreeVirtualAligned@CEngineLandscapeMemoryManager@@QAEXPAX@Z 00bd9ca0 00bfaae0`.

## Found for entry 2 (not acted on; measure first)
- callers.py: 0x00BF4220 (CEngineLandscapePatch::UpdateForeground, July row on candidates/landscape_patch_update_foreground_compiled.h)
  is the only landed caller of 0x00BEAA00 (E8 sites 0x00BF43C3 / 0x00BF445D). Its source names no ClearBlock / ReleaseBuffers; the candidates
  header calls through a `CCountedReleaseFunction` member pointer (line 52). Which symbol its object binds at those sites was not measured:
  grade it (xref) before drawing the CWaterPatchMesh unit. (0x00BFDD40's binder 0x00BF38F0 declares a local `struct CTextureOwner { void
  ClearBlock(); }` = the table line `?ClearBlock@CTextureOwner@@QAEXXZ 00bfdd40 00bf38f0`.)
- 0x00BEAAA0 / 0x00BEB4D0 have no E8 callers; their slot binders are the rows that emit `??_7CWaterPatchMesh@@6B@` (the retail-branch header
  users, e.g. the ctor 0x00BEB490): vtcheck those first.

## Decisions for the coordinator
- Q1: unit A moves the self row 0x00BD9CA0 with its only binder (the stand-in's target was misread as 0x00BD9B90 in two plans). Accept the
  self port + header opt-in with the two in-place binders as one landing (the test of 0x00BD9CA0 links the row and needs the opt-in)?
- Q2: 0x00B684F0 moves the global only, keeping its CallView self leaf and callee stand-ins (blocked: port). Accept, and queue the port?
- Q3: the CEngineGlobal family is blocked: port (above). Queue it as a header unit (renderer Settings member, CLandscapePatchTesselator,
  AllocVirtual binder), or hold the four as they are?

## Flags / funclets / descriptive names / locks / order / reading / slips
- Flags: none added (0x00BD9B90's catalog `/d2inlT114` used as is). Funclets / path includers of the four sources: none (grep of the four file
  names in rebuild/src, tests, include). Descriptive names: none new.
- Locks (rows.txt, NF271): 0x00BD9CA0, 0x00BD9B90 (both new lock dirs), 0x00BFAAE0, 0x00BFB590, 0x00B684F0 (released -> NF271). Entry-2..5 rows
  not claimed.
- Order: every compile ran after its lock was NF271, in a later call than a passing clang precheck (new sources and test, HEAD sources and
  tests of all five rows; pre.sh control with a deliberate error caught), with `timeout 900` on each background launch line (and inside
  gr.sh / rf.sh); equiv_check under `timeout 2700`.
- Reading: this brief, NF269 / NF267 / NF265 briefs, NF214_BRIEF.md, COORDINATION.md, EXTRA_RULES.md lines 1-259 and 367-481 (the
  2026-11-05 asm rule set, library / naming-pause / include-order / GameCODA / assume sections 260-366 skimmed by heading only),
  the nf267 / nf269 landing READMEs, NF269 NOTES / LESSONS / HDR_README, NF267 NOTES / LESSONS, nf263 NOTES, the L129 plan of nf262 NOTES and
  the nf262 rulings. ASM_RULES.md not read (no asm anywhere in this pass). GOTCHAS.md: NOT read in full: grepped (nf86 / nf88 / pick-any /
  LNK4006 / overlay / pin_asserts) and its newest 24 lines (480-503) read.
- Self-check: the base brief's `$S/w22/nftools/selfcheck.py` does not exist in nftools (absrefs / apply_rows / callers / mkpay / xref only), so it
  was not run; the checks it stands for are the census / link / objsec / recheck lines above.
- Slips: two read-only Bash calls exited 1 (a `pwd` after cleaning the current link directory; a final `kill -0` on a finished PID); no compile
  was involved.
