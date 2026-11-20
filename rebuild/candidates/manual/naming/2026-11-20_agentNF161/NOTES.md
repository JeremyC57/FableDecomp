# NF161 notes (2026-11-20) -- early hand-back (context)

Entries finished in queue order through q4782; q4785 onward not started (APPLIED `open`; census for each in work/census.txt).

## Rows changed (19, all RELOCATION_MATCH + test PASS in recheck_final.log)
- q4196 CMemoryAllocator family (12): 009f2870 009f2b20 009f2be0 (in-place respell of the row's own local model `Allocator` ->
  `PMemoryAllocator`, token check OK), 00a67f60 00a682e0 00a68370 00a65330 00a66260 00a663d0 00415e85 00416047 004189c2 ported onto
  engine/CMemoryAllocator.h, reaching the protected static PMemoryAllocator through the public inline `CMemoryAllocator::Get()`
  (FableWin/Ego_d `?Get@CMemoryAllocator@@SIPAV1@XZ`, opt-in FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE). 00416047's `Iface` stand-in is
  gone; its own leaf `?AddToInterface@T@@QAEXXZ` (local `struct T`, pre-existing, vland STANDIN_LEAF warning) is NOT changed here.
- q6167/q4303: 00b7ad20 ported (GPRenderManager->SetVWindow(C2DBoxF(...)) on CRenderManager.h + C2DBoxF.h); 00499880 held by NF160.
- q4550: 00a5d720 self rename + port (CShaderResource.h / CShaderDataBank.h).
- q4599 / q4600: 00b50ee0 / 00b50f40 self renames onto CEngineWeatherRenderer::OverrideRain / OverrideSnow (generated header).
- q4647: 004ff560 self rename CWorldMap::PrepareForLoad.
- q4782: 00a77500 VerifyTransitionPoints + 00a77480 JoinNavigationLayers ported off candidates/nav_quad_tree_transition_points_compiled.h.

## Self renames that move a row's file (coordinator renames the file at landing)
- 0x00A5D720 -> CShaderResource::GetPixelShader(const CCharString&) const (file already CShaderResource_GetPixelShader_00a5d720.cpp).
- 0x00B50EE0 / 0x00B50F40: files CWeatherState_OverrideRain/Snow_* -> CEngineWeatherRenderer_OverrideRain/Snow_*.
- 0x004FF560 CWorldMap::PrepareForLoad (file name unchanged).
- 0x00A77500 / 0x00A77480: leaf access/const respell only (file names unchanged).
- EH funclets: none of the 19 changed rows is included by any funclet (grep of each row basename across rebuild/src/compiled: 0 hits).
- 00cb0e90 defines a different `CWorldMap::PrepareForLoad(int)`-shaped symbol at another address: not touched; flag for the catalog.

## Callee respellings (new PDB spelling replacing a stand-in / wrong spelling; spelling count does not grow)
0x013CAA90 PMemoryAllocator (protected static data), 0x009F1F40 InternalValidateArea, 0x009F2080 SaveStackMemoryTrackerLog,
0x009F26D0 NewFrame, 0x009F1F00 OnFailedAllocation, 0x00A0B560 CRenderManagerCore::SetVWindow(const C2DBoxF&),
0x009CCDC0 CBankFile::FindIndexBySymbol, 0x00A5D620 GetPixelShader(unsigned long) const, 0x004FEEC0 CWorldMap::UnloadRegion,
0x00A770B0 GetNodeAt. Per-caller pairs in RENAMES section 2.

## Casts reported to the coordinator
- 0x00A77480: `(CNavNavigableLeafNode*)GetNodeAt(...)` -- GetNodeAt returns `const CANavQuadTreeNode*` (PDB); retail pushes onto
  Neighbours at +0x14 directly (non-virtual `lea ecx,[node+14h]`), so the node is a CNavNavigableLeafNode and the const is dropped. Both
  are real header classes (no stand-in), but it is a downcast + const cast: flagged per the hard limit.

## Needs header claim
- q4662: CShaderResource::Initialise is private (AAE) in both PDBs with friend CShaderRenderManager; the header declares it otherwise.
  Unblocks 00a5d4d0, 0098acf0, 0098b860.

## Blocked (details in STATUS.tsv)
- q4549: 00bca770 (3 forms, DIFFER 263/265/269 v 271: inlined ~CShaderBase vptr restore), 00bcc5f0 (2 forms, same), 00b90270.
- q4667 / q4668 / q11907 / q11912: render-state family (00b76bc0, 00bad2e0, 00bcb800, 00bb8090) needs the NF45 CRenderManager.h port
  wave; not attempted.

## Resume pointers for open entries (census, work/census.txt)
- q4785: 00a157c0 binds LoadFromFile ...PAVCASuspendableProcess; also its own leaf AddNavigationMap PAV->AAV, whose referencer 00500230
  binds the PAV spelling (two-row move).
- q4787: self 00a76320 + 00a16050 (GetHalfCellSize stand-in) + 012242b0 / 01224430 (_CUnprovenFloatProvider) family.
- q4826: 00b51f00 binds QAE BuildBoxPositionMatrix; header opt-in BOXMATRIX declares it private.
- q4949 / q4988 AreThingsEnemies: 0064a1ae 0072b160 0082b630 009098e0 007f9e70 0093c4e0 006a7550 00807b70.
- q4967 / q4968: CTCEnemy/AI-state rows (lists in census); self 0097e380 not landed.
- q11685 / q11929: CTCWeapon_Getters stand-ins (CTCWeapon.h has FABLE_RETAIL_CTCWEAPON_AUTOLOADING).
- q11806 / q11807: self 009a4350 GFFreeFromGlobalAllocator -> Deallocate (FableWin + Ego_d confirm; no referencers).
- q11898: self 00a0aa70 + 00a63200 00a63240 00a68060 (CRenderStatistics); 00a685e0 acctB already right.
- q11906: blocked authoring (00bf4570 legacy).
- q11930: self 0097f5b0 + 0097f690.

## Descriptive names
None new. Flags lines: none. New local models: none. acctB / HELD rows: untouched.

## Coverage / slips
- STATUS.tsv has a line for every rows.txt row of a terminal entry. Rows that belong only to `open` entries (q4785 onward) have no STATUS
  line: not started.
- q11906 is left `open` in APPLIED although the census already reads it as blocked (authoring: legacy 00bf4570); record it as such.
- q11907 / q11912 were marked blocked out of queue order because they share rows with q4667 / q4668 (same render-state family).
- The first recheck_final run lacked `source $S/wine/env.sh` and died on the Windows manifest path; it was rerun and overwritten (the
  file now holds the good run: 19/19 RELOCATION_MATCH PASS).
