# Landing nf321 (Account A, 2026-11-24): 9 body / callee ports onto the real class model (6 add-only header opt-ins, 3 table replaces)

Hand-back: `pending/2026-11-24_nf321_handback/`; helpers under `naming_batch/agent_tools/nf321/`.
- **Entries:** 81 rows from the nf317-nf320 queues whose bodies use the local model or call stand-in callees.
- **Outcome:** 9 done, 24 skipped-held, 48 skipped-port.

## What landed
- **Rows:** 0x004FF8A0 CWorldMap::ForceUnload, 0x008FD892 CAIStateGroup_BirdGroundWander::Initialise,
  0x00891320 CGameScriptInterface::EnableDangerMusic, 0x00A39C70 CWADFile::GetAlignment, 0x00A75B50 CNavNavigableLeafNode::RemoveRegionZero,
  0x00B5EAD0 CEngineScreenEffectRadialBlurRenderer::GetConsoleEnableFunctionName, 0x00A667C0 CKeyboard::EventScanStart (const as in the
  PDB), 0x00B23B50 CEngine::SetCamera, 0x00ABFD70 C3DMeshFileMaterialExtChunk::GetChunkInfo.
- **Respelled callees** have a second source each (NF318 Q4): both PDB name sets plus a donor FableWin call, or an RTTI line / landed self
  row.
- **Headers:** 6 add-only `_DECL` opt-ins for CAIStateGroup_BirdGroundWander, CBankFile, CEngineCamera,
  CEngineScreenEffectRadialBlurRenderer, CNavNavigableLeafNode and CWorldMap, with one assert pinned.
  - NF321's seventh opt-in, on CGameScriptInterface.h, serves only the held 0x0088E520 and is NOT landed.
- **Table:** 3 replaces onto the PDB callee spellings.
  - ref_scan.py found no other landed row referencing the old spellings. For `?Run@CBankFileAsync@@QAEXXZ` that took 467 compiled rows;
    the one row that did not compile, 0x0042EC7C, does not name CBankFileAsync.
  - The owners 0x004FEDE0, 0x009CBB80 and 0x00B2FBF0 still define stand-ins, so those three lines bind a spelling their owner does not
    define yet (NF302 Q2 precedent). The owners are queued in NAME_FIXES_QUEUE (`nf321`).

## Proof
- **Per row:**
  - precheck;
  - HEAD and new under one work path;
  - objsec LIBCP_OK under the self-plus-callee rename map, with no COMDAT added or dropped and no `??_7` / `??_R`;
  - `.drectve` equal, or HEAD plus `/DEFAULTLIB:"libcp"` (5 rows);
  - a /FORCE link of HEAD and new with the callee owners' objects: 0 duplicates, nothing newly unresolved except the PDB names whose owners
    still define stand-ins.
- **Headers:** a clang `override` probe per declaration, plus a made-up-name control; equiv_check over 57,146 rows and tests, 0 differing;
  the planted control flags the default-branch includers.
- **At landing:**
  - all 17 orig/ copies equal HEAD;
  - recheck of the 9 rows plus every direct includer of the 6 headers: 403 of 403 PASS (34 MATCH, 369 RELOCATION_MATCH);
  - every row grades its PDB symbol.
  - The two RELOC_CONFLICT warnings (0x005FBEA0, 0x0121F86F) are in includer rows NF321 did not touch, and appeared before.

## Rulings on NF321's questions
Recorded in BRIEF_NF_BATCH.md:
- **Q1:** a new UND `__fltused` is allowed under conditions. 0x0088E520 can land with the CGameScriptInterface.h opt-in once those
  conditions are shown.
- **Q2:** held under NF320 Q2 unless coff_info shows no new COMDAT or symbol.
- **Q3:** held; it is a re-author row.

## Queued
- 72 T3 lines `nf321`.
- 5 NAME_FIXES_QUEUE lines `nf321`.

## Recheck
`logs/recheck.log` (landed tree, 403 rows): 403 PASS.
