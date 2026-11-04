# Fable Anniversary executable as a naming source (2026-11-04)

The owner supplied `Fable Anniversary.exe` (54,301,696 bytes, sha256 2a95eea3c2cce9b47ca0f454a605b6952216f5d25158efd12ba48b70130989f2).
It was analysed statically only (never executed); no Anniversary bytes are in the repo. Unreal Engine code was ignored by owner request.
Data and scripts: `rebuild/candidates/manual/naming/2026-11-04_anniversary/` (`candidate_names.tsv`, `vt_confirmations.tsv`, `tools/`).

## What it is
- The real Anniversary game binary, not a launcher or repack. `file` says .NET because it is mixed-mode C++/CLI: UE3 builds editor glue
  as managed code into the game exe. The .NET assembly (`WellingtonGame-Win32-Shipping`, 711 types) is UnrealEd/Swarm/WPF only.
- PE32, ImageBase 0x400000, VS2010 linker, TimeDateStamp 2014-11-07. PDB path
  `E:\TC\ccfecf4029dfbc5\main\Binaries\Win32\WellingtonGame-Win32-Shipping.pdb` ("Wellington" = codename); the PDB is not available.
- No overlay or embedded payload; exports are Scaleform / Wwise; not code-signed, no SteamStub; integrity vs the Steam depot unconfirmed.

## Overlap with TLC
- RTTI: 4,010 types; 2,229 of TLC retail's 2,558 RTTI types (87%) present with identical names (anonymous-namespace hashes normalised).
  TLC-only types are mostly the old renderer/sound code (CEngineCloth*, CSoundEmitterLH, LHNetworkLib).
- 3,491 of 3,554 Fable-engine types (98.2%) are already in `ghidra_out/fablewin_pdb_names.tsv`, `egor_pdb_names.tsv` or the Ego_r layouts;
  the 63 others are Anniversary-only port/UI code.
- 73% of TLC's unique ASCII strings appear; additions are Anniversary content (new TEXT_* keys, quest variables). Only 16 `Class::Method`
  log strings (13 already in TLC). No function names, Fable source paths or assert strings.

## The useful part: class names for TLC's RTTI-less vtables
TLC retail has no RTTI for any quest/village script class (CQ_*, CV_*) and some def classes; the catalog carries 752 `CVtable_<va>`
placeholder classes (1,259 rows). Anniversary has RTTI with base-class lists for these (1,002 quest-script-related types).
- **String-literal match** (strings used by each vtable's own slot functions): validation on named TLC vtables 257/258. Matched 331
  RTTI-less TLC vtables: 316 agree with the catalog, 1 disagrees, 14 new.
- **Order alignment** between anchors (.rdata order largely kept): strict 79/79 held-out (111 agree, 0 disagree, 3 new); loose 201/204
  held-out (351 agree, 44 disagree: off-by-one where slot counts changed, CSpawnedFunc vs CSpawnedFunc1/2 arity).
- Result: **17 high-confidence new class names** (e.g. 0x012FB2F0 `CArenaWaveDef`, now `CUnprovenDef_00f25cb0`; 0x012FB5C8
  `CScriptDef`; 0x012FB778 `CFeatDef`; 0x012FB500 `CHeroSoulsCreatureDef`; 0x012FB630 `CRegionScriptDef`; 0x012F55F8
  `NScript::CExpression_WaitScript`; 0x012E22C8 `NScript::CQS_ScytheInfoScript`; 0x012DCB40 `CQ_PrisonWardenGameScript`;
  0x012E8E80 `CV_DemonDoorsScript::CDF_SummonsCreatures`), **220 medium** (mostly `CSpawnedFunc<T>` with arity to settle), **44 low**
  (disagree with the catalog), 1 rejected false positive (`NUISystem::CManager` at 0x012D6E5C), and **667 independent confirmations**
  of existing catalog class names (a second source under the naming rule).

## Lesser value
- 1,671 vtables with the same class and slot count in both builds give 9,512 TLC<->Anniversary function pairs: a second compiled
  version of much of the same C++ (VS2010, so never a byte oracle), useful for reading parked functions. The code was modified in
  places (CQ_FireHeartScript: 12 slots in TLC, 5 in Anniversary).
- Base-class chains for RTTI-less classes (e.g. `CQ_FireHeartScript : CLevelScriptBase<CQ_SunnyvaleMasterData> : CScriptBase`).

## Next steps
1. Naming wave for the 17 high-confidence classes (each `CVtable_<va>` row and its `vfunc_N` siblings take the class name).
2. Use `vt_confirmations.tsv` as the second source for landed quest-script class names.
3. Settle the CSpawnedFunc arity of the 220 medium rows from the TLC bodies, then land them; review the 44 low rows (start with
   FireHeart's CFH_Prisoner..CFH_MemorialCutsceneMarker run).
4. Optional: Ghidra BSim / Version Tracking seeded with the 1,671 vtable pairs for the ~690 remaining placeholders (class names only).
