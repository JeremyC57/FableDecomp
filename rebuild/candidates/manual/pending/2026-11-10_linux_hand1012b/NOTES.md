# Chunk 1012b notes (agent1012b)

Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (hdr/README.md). gall.sh now exports it. For rows in
SPLITS.tsv it also grades with QLEN through tools/qq.py (q.py ignores QLEN). The tools/*.sh copied from chunk 1012 were
repointed at this directory. New tools:
- tools/kr.py: emulates c2's frame-slot quicksort.
- tools/slots.py: dumps the slot list from a fr.sh trace.
- tools/var34.sh, varbef0.sh, varbd80.sh: probe drivers.

## Result
- **RELOCATION_MATCH (2, in src/, tests in test/):**
  - 0x004A34F0 CWorld::AutoLoadRegionLeaving (QLEN=564).
  - 0x004A5E90 CWorld::AutoLoadRespawn.
  - Both TUs define the same LoadGameState / AutoLoadInternal bodies. That LoadGameState also grades RELOCATION_MATCH against the
    landed 0x004A3200 (probe).
- **DIFFER, parked (3, in differ/):**

  | Row | Grade | Grades used |
  |---|---|---|
  | 0x0062BEF0 EAUseProjectileWeapon | 483v483 | 1 |
  | 0x0062BD80 EAControlledCreatureStrafe | 337v354 | 2; improved |
  | 0x008A8260 GetHeroSummonedCreaturesList | 456v466 | 1 |

- **Not attempted (7):**
  - 0x0089C910, 0x0089CE70, 0x008A7A70, 0x0088EDC0, 0x004A1840, 0x004A1CB0, 0x0062B070.
  - Each needs a full rebase of a local padded model, or a fresh draft (0x0088EDC0). I ran out of session budget after the two
    matches and the parked-row work.
  - 0x0062B070's catalog extent is wrong (BOUNDS_FIX.tsv).

## The lever that matched both CWorld rows
Chunk 1012 had copied the landed LoadGameState 0x004A3200 body, which uses a separate `if (!found) return false;` in the
"load from any directory" arm. Retail's code is one `if / else if / else` chain. When the save is not found, the any-directory
case falls out to the shared `return false`.
- Both spellings compile to the same 0x004A3200.
- Inlined into the callers, the chain gives the path destructor one fewer pre-cross-jump copy. That has two effects:
  - AutoLoadRespawn's manual-save copy keeps `lea ecx,[esp+10h]` below the `je` (retail 0x004A6117 `je 4A6242h`). Before, c2
    hoisted it into the je's single-predecessor target block.
  - AutoLoadRegionLeaving's path slot drops from IL weight 11 to 10. The fullPath+heroState slot (11) now sorts first (F+0), as
    retail has it.
- Found by probing: a `do { ... break; } while (false)` around heroState also matched AutoLoadRegionLeaving, by raising heroState
  to weight 5. That showed the tie was an IL-weight question. The chain is the natural form and fixes both rows, so it is the one
  kept.
- The frame-slot sort is c2's K&R quicksort (tools/kr.py). It reproduced every traced order, and it predicted which weights flip the
  tie: hs >= 5, or qs >= 6 over name 5, or path <= 10.

## Parked rows: residue and what to try next
- **0x0062BEF0 EAUseProjectileWeapon**, DIFFER(483v483), masked diff 4 lines.
  - Residue: frame slot only.
  - Packer: ranged, this and camera each weigh 2. With FIFO order ranged, this, camera, camera joins the latest free slot (this).
    Retail needs camera to join ranged.
  - The node-order analysis (NOTES above, kr.py) shows two ways to get there:
    - `this` at weight >= 3 (with camera <= ranged);
    - camera at weight >= 3.
  - Probes that did not add the reference: a named game camera, a function-scope camera, a const view, a tested GetCameraManager
    bool, a do/while, the camera code duplicated into the AddMode/RemoveMode arms (that one gets the slots right but cross-jumps
    only part of the code: 525 bytes).
  - Next: look for a retail construct that reloads `this` (or re-reads camera) on a second, cross-jumped path.
- **0x0062BD80 EAControlledCreatureStrafe**, DIFFER(337v354).
  - The typed readers (GetUHUGE / GetFloat / GetFloat / GetSLONG; overlay opt-in) give retail's frame. uid now shares a slot with
    action.
  - Residue: the has-weapon bool is materialised (`mov al,1` / `xor al,al` / `test al,al`, 0x0062BE8D..93), with the weapon zeroed
    in ebp first.
  - Probes that all fold it into jumps: named bool, &&-expression value, nested if, the 1-arg PeekTC<T>, and a plain-inline
    (non-forceinline) PeekTC stand-in as a diagnostic.
  - FableWin's only weapon accessors are ??$GetTC / ??$PeekTC<CTCWeapon>, so no PDB helper explains it.
  - Next: trace where c2 threads the bool join (jump threading versus inline order).
- **0x008A8260 GetHeroSummonedCreaturesList**, DIFFER(456v466).
  - Residue: retail gives the null/dead-hero exit a private epilogue copy returning `bl` (edi is pushed lazily).
  - With one `return found;`, ours jumps into the main epilogue after `pop edi`.
  - Every early-return form gives the private copy, but c2 const-folds the value (`xor al,al`) and moves found to memory. c2prio:
    found goes from 8 refs to 4, and from an ebx split to memory.
  - Retail's `xor bl,bl; mov [esp+0Bh],bl` reads like an enregistered const-0 / found web.
  - Next: find what keeps found live and unfolded at an entry-block early exit.

## Never-started rows (for the next pass)
- 0x0089CE70 IsThingAwareOfOtherThingInAnyWay and 0x0089C910 CanThingBe_Sensed_ByOtherThing:
  - Need CTCPerceiveThing's range/FOV getters named from evidence (0x00744440, 0x00744450, 0x00744950, 0x007445D0, 0x00643B3B; the
    prior's names are guesses).
  - Need the physics facing call: slot 70, +0x118, returning C3DVector by hidden pointer.
  - Need one of the C3DVector Normalise opt-ins (0x013CE558 table).
- 0x008A7A70 TeleportAllFollowersToHeroPosition:
  - Needs a CTCFollowed header (TC 0xBC, GetFollowers 0x006D5850).
  - The prior relies on `/Oa`, which should be re-checked once it is rebased.
- 0x004A1840 CWorld::Load: EH with one catch (CBBBFileException&). The prior was at DIFFER(1129v1129) with three register picks.
- 0x004A1CB0 PreparePrimitivesForEngine: x87 order residue in the prior. The catalog name doubles the class.
- 0x0062B070 EAOpenHeroInformationScreen: grade with QLEN=891 (BOUNDS_FIX.tsv). The prior was at DIFFER(898v891).
- 0x0088EDC0 CameraDoConversation: no draft. Its callee 0x004C1060 is unnamed in ROWS.

## Identities, callee names, name fixes
- LABELS.tsv: AutoLoadRegionLeaving / AutoLoadRespawn / AutoLoadInternal / LoadGameState, evidence as in chunk 1012.
- Every callee in the matched rows is bound through the shared headers. The call-target check (tools/calls.py) pairs each retail
  target with the expected symbol:
  - CWideString ctor / dtor / operator= / operator!= / operator+;
  - CAFile PathExists / IsPathFile / StartFileScan;
  - the CUserProfileManager getters;
  - GetDebugSaveDir;
  - GetPathNameForFileName;
  - CDriveManager::DeleteFileW;
  - the singleton globals and g_bLoadFromAnyDirectory.
- NAME_FIXES.tsv: 0x004A5E90 (the catalog/oracle calls it AutoLoadRegionLeaving); 0x004A1CB0 (the catalog doubles the class).

## Header debt
- CTCFollowed has no shared header (0x008A7A70).
- CTCPerceiveThing lacks the range/FOV getters (0x0089C910 / 0x0089CE70).
- CGameEventDataPackage typed readers: in the overlay only (hdr/README.md).
