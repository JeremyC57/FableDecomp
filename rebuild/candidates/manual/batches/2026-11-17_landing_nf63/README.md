# 2026-11-17 landing nf63: name-fix round 7, NF63 (14 rows: first engine-side ports of blocked rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF63 took the 75 rows at or above 0x00800000 that earlier round-7 agents had left `blocked/port`: rows whose wrong binding comes
from a local model of a class that has a shared header. It had a 15-grade budget per row. It ported 14 rows and compiled one
automatically generated draft for each of the others, then ran out of time. Those 57 rows went to NF65 (with three rows the
coordinator had set aside by mistake). Brief, list and records: `naming/2026-11-17_agentNF63/`.

## Rows (STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 14 |
| not-a-binder | 2: funclets 0x01213BC8 / 0x01213BDE have no code of their own; regraded against NF52's ported parent 0x00C2DBE0, both bind the queued symbols |
| blocked | 59: 57 blocked/port, handed to NF65 with NF63's draft errors; 2 blocked/header, the coordinator's |

**Ported:**
- **CMap (5):** 0x0081FA20 / 0x0081FA90 / 0x008200B0 / 0x00820100 / 0x00820140 are now the protected PDB members
  GetThemeSizeZOffsetAt, GetThemeMinCameraSizeZOffsetAt, GetThemeSizeZAt (the C3DVector and C2DCoordI overloads) and
  GetThemeMinCameraSizeZAt. They were GetThemeHeightAt, GetThemeMinCameraHeightAt and GetThemeMaxCameraHeightAtPosition.
- **CThingTrackNode (2):** DrawGetMesh 0x00837050 (was GetDrawRecordA) and DrawGetDrawSizeX 0x00837060.
- **libpk CSoundEventManager (4):** 0x00C11F50, 0x00C120A0, 0x00C12460, 0x00C12520 were catalogued under propagated game labels
  (`CAIStateGroup_OnMapTextureChanged`, `Inventory_ProcessKeyboardBinding`, `Save_WriteInventoryState`, `Save_WriteEquipmentState`).
  They are now `CSoundEventManager::UpdatePlaybacks` and `StopEventsWithField18` / `10` / `24`. libpk has no PDB, so these are
  descriptive names (NF63 NOTES.md). The `StopEventsWithFieldNN` names say what the body does (stop every event whose field at
  that offset matches) until the fields are named.
- **Single rows:** `PKCTRList<CSegment>`'s scalar deleting destructor 0x00C2DFC0 (was `CVtable_012b4788`), 0x00C2EB00 and
  CEngineShadowRenderer::BuildDepthPackingLookupTexture 0x00B4F420.

## Entries (APPLIED.tsv): 4 done, 53 blocked
- **Done:** q10493, nf44:101, nf41:e41, nf44:111.
- **Still blocked by rows outside the slice**, though every binder of them in this slice is ported:
  - q10433: 0x0063A540;
  - nf41:e141: 0x006A9590;
  - nf44:93: 0x00C11CB0, 0x00C12140, 0x00C123B0 (seen in reloc_bindings, not in NF53's binder list);
  - nf44:105: its own row 0x00C2DDE0 still has the placeholder name `??1T@@QAE@XZ`.

## Rows and checks
- **Section 1: 13 self renames (20 file moves). Section 2: 32 respelled bindings.**
- **Funclets:** the libpk renames moved the parents of funclets 0x0121167D, 0x0121168F, 0x012116E9 and 0x012116FB. The coordinator
  repointed their includes and comment lines; funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** 18 graded from the repo after landing (the 14 rows + the 4 funclets): 14 RELOCATION_MATCH
  and 4 RELOCATION_MATCH[/EHsc], every test PASS. NF63's own final run agrees.
- **Checks:** `naming/rebind.py` removed 40 binding lines and added 21; bindings_multi_check is at its baseline (`NullCoord`).
- **Duplicate body (queued):** 0x0081FA20 and 0x0081FA90 each define the body of 0x0081F910 in their own file, under its PDB name
  `?GetStrongestEngineThemeAt@CMap@@IBEJABVC2DCoordI@@AAJ@Z` (it grades MATCH at 0x0081F910). They replaced an older made-up body in
  the same files. The row at 0x0081F910 is still catalogued `CMap::GetDominantEngineThemeAt`. Rename it, and have the two rows use one
  definition, before linking them together.

## Headers (15 opt-ins in 11 files; default path unchanged)
engine/CMap.h, CThingTrackNode.h, CSurface.h, CEngineShadowRenderer.h, and libpk/CSoundEventManager.h (2), CSoundEventSet.h (2),
CSoundEventGroup.h (2), CPooledResource.h, CPlaybackSlot.h, PKCTRList.h, CBankProcessor.h (2). Details:
`naming/2026-11-17_agentNF63/hdr/README.md`.
- libpk/CPlaybackSlot.h's opt-in sits inside a one-line struct, so the closing `};` moved to its own line. The coordinator checked
  that the default path preprocesses to the same tokens (`gcc -E -P`, whitespace-normalised).
- The CMap theme members are protected in both PDBs, so the CWorldMap callers will need `friend` access when they move onto the
  header.

## Coordinator header changes requested (blocked/header)
- 0x00EE2A80: a `__forceinline` form of `CBasicString<char>::operator==(const char*)` in the root fable_string.h (also 0x006D41F0).
- 0x00C2E2B0: rename the placeholder driver-map class `CIndexMap_00c3abd0` in libpk/CBankProcessor.h's default path (q10483).
- The proposed text is in NF63's NOTES.md.

## Name fixes queued (`land1117fu`, 12)
NF63's NAME_FIXES.tsv (11: landed callers outside its rows, and the 0x0081F910 self name) plus the coordinator's duplicate-body
line above.

## Rule slips
- To prove its funclet rechecks used the staged parent, NF63 put an `#error` probe in a row. It hung the Wine compiler inside vland;
  NF63 killed only its own processes and restored the row. Later briefs forbid `#error` in rows.
