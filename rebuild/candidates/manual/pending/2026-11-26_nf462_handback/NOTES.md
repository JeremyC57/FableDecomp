# NF462 (2026-11-26): r460d / r460a / r460c redo + NF460's unreached queue lines

Scratch: `$S/w22/agentNF462`. Tools: NF460's (`tools/`, paths and lock owner moved to NF462) plus `rdis.py` (capstone disassembly of
retail), `drectve.py` (.drectve text of objects), `gen2.py` (bB2 row generator from the RenderSprite templates), `cp2.sh` (compile
HEAD v new + objproof_map + drectve, header overlay on). Header overlay: `hdr/engine/` (copies of the changed headers), HEAD copies
in `orig/hdr_*.h`. Sandbox: `sandbox/` (fresh repo copies of reloc_bindings / self_symbols / descriptive_names + descriptive_check.py);
repo oracle md5 unchanged across the run (work/repo_oracles_md5.txt). No git; nothing written under the repo.

## Batches (NF460 / NF457 layout: rows/, orig/ = exact HEAD copies incl. tests and hdr_*.h, hdr/engine/, head/payload.json,
## RESULTS, SELF_SYMBOLS, UNITS, CHECKS, TABLE_CHANGES, HEADERS, renames.tsv, evidence/)
| batch | rows | what |
|---|---:|---|
| bB2_psc_alloc_headers | 14 | r460d: the 7 NParticleEngine `?Alloc@X@NParticleEngine@@SIPAVCPSCBase@2@XZ` factories + 7 DatabaseClone callers (the 4 queued + 3 found by caller census: 0x00B0F970 UpdateNormal, 0x00B17950 Orbit, 0x00B1B4B0 Spline, which spelt stand-ins `GetDrawableObject_00af7e60/_00af7f40/_00af8090` no table line binds). No reinterpret_cast anywhere. DatabaseClone selves become the PDB virtual `?DatabaseClone@X@NParticleEngine@@UBEPAVCPSCBase@2@XZ` (both PDBs; retail refs = vtable slot 2 only), body `Alloc(); clone->DatabaseCopy(*this)` (CPSCBase slot 18 from the shared header). Headers: RenderSprite / EmitterGeneric plain `_ALLOC` opt-ins; Light / UpdateNormal / SplineBase containers-free views (+ `_ALLOC` on the first two); Orbit header unchanged; Attractor / Spline (no headers) as local classes deriving from the true RTTI base's shared header (RULINGS r462a). TABLE: NF460's 7 replace + 4 delete. |
| bM_creaturemode_alloc | 2 | r460a: 0x0073EC10 / 0x00743680 onto their PDB `?Alloc@<Mode>@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z` selves calling the PDB `(CThing&)` ctors; local mode class (no header) deriving from its RTTI base CModeStandBase (engine/CModeStandBase.h, unchanged) over the new containers-free view of engine/CTCCreatureModeManager.h. TABLE: 2 replace. |

## Gates (every row, both batches)
- vland --recheck-landed HEAD v new at one LANDVERIFY_WORK path (tools/runpair_hdr.sh, overlay on the new side): RELOCATION_MATCH PASS both sides, all 16.
- WARN sets: bB2 strictly smaller (HEAD 15 warns: SHARED_HEADER_AVAILABLE x8, RELOC_NAME x7; new: none); bM identical (none).
- objproof on vland's own objects: ALL_EQ under exactly the intended renames (renames.tsv); .drectve byte-identical (LIBC + OLDNAMES, no libcp).
- link HEAD v new (+ HEAD objects of the ctor rows, RegisterPSCs 0x00AF8170 / RegisterModes 0x0073B130): 0 duplicates; unresolved 65 -> 44 (bB2), 105 -> 101 (bM), none new.
- sandbox (both batches cumulatively, fresh repo copy): apply_table --dry 0 misses, applied, multi-target 1 -> 1; SELF_SYMBOLS 16 applied; clash 0; descriptive_check 2,026 lines 0 FAIL.
- Headers: g++ -E -P HEAD v new IDENTICAL under 3 macro sets each (evidence/header_preproc_equiv.txt), #error guards fire (evidence/header_error_guards.txt); all asserts in the views pinned (_AT); typed CPSCLight layout = view (VC7.1 probe). Coordinator runs the equiv check.
- Drift: every orig/ file equals the repo working tree (bB2 33 files, bM 5 files).
- No __asm / volatile; no new /DEFAULTLIB; no new duplicate definitions (grep: no other source defines these Alloc / DatabaseClone spellings); no BEST_GUESS names (all names in Ego_r + FableWin).

## Holds / findings
- r460c (0x006F5610): HOLD, RULINGS r462c (measured: own header defines the ctor inline in its only polymorphic branch; base header needs the full CCombatActionBase model; vftable COMDAT change predicted, not built).
- CModeStunned unit (0x00740160 / 0x008ED950 / 0x007427F0): not built (budget). Now buildable on the bM views; CModeStunned : CModeOutOfActionBase (no header) : CModeStandBase. Extra finding: 0x00861E00 (CModeStandBase ctor) has 3 bound spellings (??0CBase_00861e00 @0x0073EC70, ??0CModeBase_00861e00 @0x00740160, ??0CModeStandBase@@QAE@PAVCThing@@@Z @0x00740340), PDB ??0CModeStandBase@@QAE@AAVCThing@@@Z: one family to respell.
- Same factory/clone debt outside this batch: CPSCRenderMesh_Alloc 0x00AF7D80, CPSCDecalRenderer DatabaseClone 0x00B1FF80, CPSCSingleSprite DatabaseClone 0x00B0A190 (stand-in callees, local models); next pass can reuse the bB2 opt-in pattern.

## Resume (smallest first)
1. CModeStunned unit (above), then 0x0073E410 / 0x00742E60 lines are already DONE via bM.
2. NF460 "Not reached" list unchanged: 0x0099A2D0 family, WatchFor* 0x00D17090 / 0x00E39140 / 0x00F08590, 0x00B93290, 0x006E89B0, 0x0076C830/0x00686870, 0x00686830, 0x004ABE90, then the self-change and large lines; MixFadeIn/Out (4 lines) need the CSalRenderDisable.h / CSalRenderPan.h private-static opt-ins.

## Census
bM: xref 0x0073EC10 / 0x00743680 = registry abs refs only (0x0073B2F0 / 0x0073C5E8); ctors 0x0073E410 / 0x00742E60 = one call each, from their factory. bB2: factories = RegisterPSCs abs + one DatabaseClone call each; DatabaseClone selves = vtable slot 2 only.
bM vland logs were produced while the overlay header had a stray include in the typed body (fixed after; the NO_CONTAINERS TUs skip that body): recompiling both rows with the final header gives objects ALL_EQ to vland's new objects (no renames).

## Locks (NF462)
Changed rows (16): 00af7d10 00af7e60 00af7ed0 00af7f40 00af7fb0 00af8020 00af8090 00afe580 00b0f970 00b15b00 00b17950 00b18740 00b1ae70 00b1b4b0 0073ec10 00743680.
Held, unchanged (taken for the units): 0073e410 00742e60 (bM callees). Released (written `released`): 00740160 008ed950 007427f0 006f5610 (no batch). Not taken (owner plain NF460, unchanged callee rows): 00afb7c0 00b0c9c0 00b103f0 00b16eb0 00b18020 00b1a640 00b1b350 008bfce0.
