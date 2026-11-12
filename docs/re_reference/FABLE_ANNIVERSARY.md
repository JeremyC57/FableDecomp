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

## The Xbox 360 Anniversary PDB (owner-supplied, 2026-11-04)
- `WellingtonGame-Xbox360-Shipping` (Virtuos UE3 tree), PDB signature 0x52CE7628 (2014-01-09), age 1, GUID
  {3EE45A73-50C6-41DC-9679-2E3A8CFCD2EA}, machine PowerPC (0x1F2), cl 16.00.11886. MSF with 1 KB blocks whose block map spans two
  blocks (llvm-pdbutil refuses it; a private reader parses it). Kept private in the lane scratch (`$S/anniv_pdb/`), never committed.
- **The Fable game objects were compiled without debug info**: for Fable code it has public symbols (full signatures) and the
  address -> object-file map (Lionhead `.cpp` names) only; no procedures, parameters, locals, scopes, line tables or types. So it cannot
  help any parked byte residue.
- Of 44,478 Fable code publics, 86% match FableWin.pdb by exact signature; the 5,440 in neither TLC set are all Anniversary additions.
  On the naming queue it agreed with the TLC spelling every time it had the name (31 of 34; the 3 differences are Anniversary changes).
- Small wins: a third-source spelling check; named TU boundaries (retail kept Lionhead's TU order, e.g. the particle `psc_*.cpp` files;
  quest scripts are one `register_scripts.cpp`); per-method sizes from another build (closed the 44 low rows in the catalog's favour).
- **Usage rule:** (1) private lookup only, never a source of bytes, layouts, headers or calling conventions (members mangle `__cdecl` on
  360); (2) don't consult it for byte residues; (3) a name or signature counts only as a confirming third source when it equals
  FableWin.pdb or Ego_r.pdb, and TLC wins where they differ; (4) object-file attribution is a TU hint for non-folded, non-inline
  functions only (39% of publics share a folded address); (5) record a use as "Anniv360 (third source)"; no new pipeline step.
- Applied from it (as the third source): 13 NParticleEngine globals 0x00AEAA10..0x00AEAB00 named from Ego_r donor order, anchored by
  four already-named retail rows at a constant offset (`ghidra_out/labels_naming_2026-11-04_particle_engine_globals.tsv`).
- **The `.xdb` + `.xex` (owner-supplied as "prototype", 2026-11-04): not a prototype, and nothing new.** The `.xdb` (10 MB, sha256
  fde88827…) has the same GUID {3EE45A73-…}, signature 0x52CE7628 and age 1 as the PDB above, and its 801 modules are only the XDK /
  CRT / D3D / XAudio2 libraries: zero Fable objects. The `.xex` (33 MB, sha256 f73918b9…) is `WellingtonGame-Xbox360-Shipping.pe`,
  image timestamp 0x52CE7650 (40 s after the PDB), base 0x82000000: the same shipping build. With no debug info for Fable code, it would
  only show PowerPC code with public names; not set up, kept private in the lane scratch.

## The Xbox 360 "Test" build PDB + .xdb + .xex (owner-supplied, 2026-11-04)
- `WellingtonGame-Xbox360-Test.{pdb,xdb,xex}`, PDB signature 0x525E2981 (2013-10-16, three months before the shipping PDB), GUID
  {AF1609B8-3BA8-498A-9CA5-78A8965281A0}, age 1, PowerPC; the `.xdb` and the `.xex` (`WellingtonGame-Xbox360-Test.pe`) carry the same
  GUID. Kept private in the lane scratch (`$S/anniv_dbg/`), never committed.
- **Not a debug build.** It is UE3's "Test" configuration: fully optimised (Xbox 360 SDK cl 16.00.11886, `-Ox -Os -Ob2 -Oi -Gy
  -fp:fast -Zp4`, unity files `Unity_*EtAl.cpp`), but compiled with full debug info (`-Z7`). This differs from the shipping PDB:
  - The Fable objects carry 45,188 procedures, their parameters and locals, scopes and line tables (5.2 MB).
  - It has 1.02 M type records, with class layouts, member names, method lists and enums.
  - The objects cover 95 unity modules in FableLib, FableEngine3D, FableScripting, BBBLibrary and similar directories.
- **Measured against TLC (2026-11-04, Fable code only, UE3 excluded):**
  - *Types are TLC's:* 3,032 Fable classes are in both Ego_r and this PDB. 2,936 (97%) have the identical member-name list, and
    2,512 (83%) have the identical size. The data structures were carried over almost unchanged. Ego_r already has these, so there is
    little new layout information.
  - *Functions:* 19,174 distinct 360 Fable procedure names equal a TLC catalog name, mostly for landed rows.
    - Of the 2,923 unlanded TLC functions with a real name, 301 match a 360 procedure by exact name. Another 817 match only by a
      unique bare method name. Those 817 are leads, not proof: spot checks found wrong classes, for example several catalog
      `~CTCGraphicAppearanceAnimatedMesh` rows.
    - 252 of the exact matches already have parked drafts.
  - *Source files:* the line tables give each function's original Lionhead file and line (e.g.
    `fable\fablelib\cpp\player_modes.cpp:1764 CPlayer::InitModeBetting(old_mode, processed_input)`).
    - Function order inside a file agrees with TLC retail address order for 67.8% of 8,399 adjacent pairs (chance is 50%).
    - The files descend from TLC but were edited, so this is a TU hint, not an order oracle.
  - *Not useful for:* bytes, calling conventions, or VC7.1 codegen (PowerPC, cl 16).
- **Pilot result (`batches/2026-11-04_anniv360_pilot/`): it does not get parked rows to a byte match.**
  - Of 20 parked rows rewritten along the 360 locals, 0 matched and 2 improved. Both improvements came mostly from scope placement,
    which the file does not record.
  - Register records are parameters only (checked on all 44,972 Fable procedures), only stack locals appear, and the file has
    13 scope records.
  - Use it as a lookup for local types and original source files, not as a matching aid.
- Usage rule unchanged. Parameter and local names are drafting hints. A function or class name still needs TLC confirmation.
- Reading it: llvm-pdbutil refuses the 1 KB-block MSF; `tools/decomp_pipeline/linux_lane/lane_tools/anniv/repack4k.py in.pdb out.pdb`
  rewrites it with 4 KB blocks (same streams), after which `llvm-pdbutil dump -summary/-publics/-modules/-symbols` work (2026-11-12).
