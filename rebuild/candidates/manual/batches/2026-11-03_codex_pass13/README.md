# Codex pass 13 return -- verification (2026-11-03)

Outside Codex pass over the 100 rows reserved in `pending/2026-11-02_codex_pass13/RESERVED.tsv` (NA087-NA096). Return
`FableDecomp_pass13_handoff` (Google Drive, 64 MB zip): SHA256SUMS clean over 16,594 files (checked with `sha256sum -c`; the
archive's own Python tools were not run). Codex reports 51 VERIFIED_MATCH, 3 reviewed parked, 23 in progress, 2 blocked, 2 bad
extents, 19 unstarted (`STATUS_codex.tsv`, `START_HERE_codex.md`, `ROW_DETAILS_codex.md`, `LESSONS_codex.md`).

## Regrade
Each row's exact header context was rebuilt from the archive's hash store (`scripts/restore.py`, every object hash-checked; all 51
sources equal their recorded SHA-256) and graded read-only on our lane against an oracle built from our exe
(`oracle_from_exe.py`, scratch only): **51 of 51 RELOCATION_MATCH, every behaviour test PASS** (`regrade_grades.tsv`).
No row source contains `__asm`, `_emit` or `naked`.

Staged for landing without the zip: `pending/2026-11-02_codex_pass13/verified_pending/<addr>/` (source, test, ROW.json,
NAMING_EVIDENCE.md, `hdr_manifest.json`) with the headers that differ from our tree in one deduplicated store (`objects/`, 250
headers) and `restore_overlay.py`. The 49 unmatched rows' sources and PROBES.md are in `pending/2026-11-02_codex_pass13/unmatched/`.

## Headers (`hdr_pairs.tsv`, `defcheck.tsv`, `row_header_needs.tsv`)
195 of our headers are changed or new across the 51 contexts; 39 exist in more than one version (fable_string.h 6, CScriptThing.h 4,
CWorldMap.h 4, CPersistContext.h 4 ...). Default path (stubbed cpp, no FABLE_RETAIL_ macro) of every changed version against the
package base and our tree: **only fable_string.h changes our default path**, additively (`class CAFile;` and a free
`operator+(const char*, const CCharString&)` declaration), to be moved under FABLE_RETAIL_CCHARSTRING. The sources define 79
FABLE_RETAIL_ macros, many per-feature (`_DECLARED_DTOR`, `_EXTERNAL_CTOR`, `_THREE_CHANNEL_CTOR`, `_TEXT_COMPARE` ...): all 51 rows
need the class-branch consolidation of the pass 12 header wave before they land.

## Names (`AUDIT.tsv`, `second_spellings.tsv`)
- Leafs: 50 of 51 are exact symbols in the Ego_r / FableWin name lists; 0x0121AF20 is `_$E1` (a dynamic initialiser, to be named
  through the .CRT$XCU order). For 4 of them landed callers bind the address with another spelling (access or class):
  0x00989CB0 UpdateObjectSpaceLighting (AAE), 0x004FB990 / 0x004FD040 CWorldMap::Save*ToString (IAE), 0x009DA9F0
  CRenderManager2D::Render2DDrawList.
- Relocations: 462 bind a PDB name no landed row contradicts, 492 a PDB name landed rows spell differently (second spellings to
  respell or queue), 196 symbols are not in the function-name lists (vtables, string literals, statics, STLport instantiations,
  CRT). Five hit queue entries: 0x007E72A0 `??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ` against mig5's `?Construct@...`,
  and 0x00415D70 `??BCDefString@@QBE...` (mig9's duplicate-COMDAT question).
- Data names Codex cites from PDB DATA / PUBLIC32 records that our committed name lists (functions only) cannot confirm: NPCAttitudes
  0x0143E928, the seven CRenderStateManager D3D*Table statics (0x01396DC8 ...), CRenderManager2D::LibraryStateBlockFunctions,
  NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748, CShaderRenderManager::SingletonInstance. Accepted over
  descriptive names; to be checked with `cvdump` against the owner's PDBs.

Landing: `../2026-11-03_codex_pass13_landing/` (header consolidation, includer recheck, land, labels, queue).
