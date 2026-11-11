# Chunk H40j notes (tenth pass, 23 parked rows)

Every grade used the header overlay `FABLE_HDR_OVERLAY=$A/hdr` (gr.sh). This pass adds two hdr/ blocks, CHeroLogBook.h LOGBOOKSCREEN and CTCInventoryItem.h STARTINGRIGHTHANDEDSET, for 0x006096E0. hdr/README.md describes them.
ATTEMPTS.log has one line per grade. Best variants are in differ/<addr>.cpp; lines 2 and 3 of each give the grade, the residue and the next idea.

## Matched
- **0x00E40E80** `?Main@CMagicman@CV_BordelloScript@NScript@@EAEXXZ` (NScript::CV_BordelloScript::CMagicman::Main), RELOCATION_MATCH at 15104 bytes (src/00e40e80.cpp, test/00e40e80.cpp).
  - Lever: drop FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR. With the implicit STB dtor, VC7.1 expands `~seh_me` in retail's two late epilogue copies (0x00E446EA, 0x00E447AB) and calls it at the natural exit (0x00E4452C).
  - Hygiene: the row-local enums and the GFIntToCharString declaration are replaced by the shared headers engine/ETextGroupSelectionMethod.h, engine/EScriptEntityMoveType.h (MOVE_TYPE_WALKING/RUNNING respelled ENTITY_MOVE_WALK/RUN, the header's names) and engine/fable_script_text.h. It still grades RELOCATION_MATCH.
  - The test is a behaviour model of the magician's state machine (talk answers before and while tricking, hit complaints, the beer count with the drunk cut-scene, removal once the hero owns the bordello).

## Improved (differ/, masked diff lines before -> after)
- **0x00E3BB70** CMadame::Main: 595 -> 198. In-body `#pragma inline_depth(0)` around the chit-chat movie declaration and around that arm's closing `}`.
  - Next: the exit layout (retail has 3 `ret` copies, we have 1).
- **0x00D973D0** CBanditSacrifice::Main: 1170 -> 190.
  - Changes: two bare `seh_me.Release()` calls; the Nymph loop with an in-loop `break`; the loop-bottom `continue` / return.
  - Next: d9_3 is 176 bytes long. d9_2 (9885 bytes, 342 lines) is closer in size, so find which sites the bottom change expanded.
- **0x00E5FD40** CBodyGuard::Main: 1057 -> 163. Row-local inline ~CScriptThing and the main loop `!T && true`.
  - Next: steer single ~CScriptThing sites back to calls.
- **0x00D13580** CQ_BountyHuntScript::Main: 280 -> 119. Lookout loop form.
  - Next: one movie dtor site at s74 against T74.
- **0x00DB97A0** CNOVI_Theresa::Main: 248 -> 125. Outro `break` plus a post-loop T check, and main loop `if (T || false) return`.
  - Next: the exit-pad cross-jumps.
- **0x00E4B5B0** CTollBandit::Main: 697 -> 591. `long answer = 0;` and call_timer at function scope.
  - Next: frame slots, using a packer replay.
- **0x00CF68C0** CEntranceGuard::Main: 1472 -> 864. Second loop form and MOVIEBASE_FORCEINLINE_CTOR with the dtor specialization moved first.
  - Next: the talk-movie slot.

## Touched, no gain (draft kept, H40j note appended to line 3)
- 0x0061E6D0: 1295 -> 1295. `&& true` is byte-neutral.
- 0x00E33EA0: 323 -> 324. `inline_depth(1)` around the predicate ctor. The CSpawnedFunc1 ctor specialization matches the spawn block but moves the frame.
- 0x00DF6010: 1486. An inline ~CScriptThing makes it worse (1892).
- 0x00F0FB70: 895. Dropping the unused cast map makes it worse (1647).
- 0x00E20600: 355 (no NOVTABLE: 370; split pragmas: 360).
  - Retail calls ~CIsThingAlive 0x00E241B0 for the And members but expands it for the temporaries.
  - A pragma around the template dtor definition does not separate those two cases.
- 0x00D0F640: 108. Dropping the STB OUTOFLINE_DTOR is neutral.
- 0x00B10B10: 788 -> 788. Only the forbidden `// Flags: none` line was removed; the grade is unchanged.
- 0x00B03CA0: 1022 (baseline grade only). Its `// Flags: none` line was removed.

## Identities / names
- No new identities. LABELS.tsv and NAME_FIXES.tsv carry the earlier passes' proven names, unchanged. SPLITS.tsv is unchanged (0x00D0F640 is 2101 bytes, 0x00B10B10 is 4276).
- The catalog name of 0x00C4F600 (PropertySerializer_GetPropertyTypeCode) is wrong: it is CEAXManager::SetEAXProperty, a descriptive name (NAME_FIXES.tsv).

## Header debt
- None added. 0x00E20600 still declares local ETextGroupSelectionMethod / EScriptEntityMoveType enums. The shared engine/ETextGroupSelectionMethod.h and engine/EScriptEntityMoveType.h exist, so respell the enums as was done for 0x00E40E80.

## Baseline-only rows (regraded as they sit on HEAD)
- 0x00C4F600 CEAXManager::SetEAXProperty: DIFFER(9204v9204), 85 lines. Unchanged. Next: the one rotation step (line 3).
- 0x00E50FD0 CBB_BeardyBaldyMan::Main: DIFFER(9850v9885), 99 lines. Unchanged. Next: the second entry zero store, then the ebp vtable constant.
- 0x00507C30 CWorldMap::LoadFromFile: DIFFER(7661v7661), 2 lines. Unchanged. Next: the exit reload order (prevI NOTES).
- 0x0078FD60 CTCDivineWrath::FrameUpdate: SRC_FAIL (more than 100 errors). The declaration debt is listed on line 3.
- 0x00502E90 CWorldMap::PopulateRegionWithVillagers: SRC_FAIL on HEAD (11 errors).
  - Cause: the hand879 opt-ins are missing from HEAD.
  - The renames and the hdr/ blocks it needs are listed on line 3.
- 0x00603000 CTCInventoryMap::CreateMiniMapMarkers: does not finish compiling within `timeout 900` on HEAD.
  - Tried 3 times with no output (rc=124).
  - Stopped there under the loop rule.
- 0x006096E0 CTCInventoryMap::ConstructLogbookListPC: rebased onto HEAD. It went from SRC_FAIL to DIFFER(12864v12832), 902 lines.
  - Two hdr/ blocks were added (hdr/README.md).
  - The extra 32 bytes come from HEAD's CEngineGraphic / GetEngineGraphic models compared with the hand883 overlay.
- Differ drafts still spell some inline helpers with bare `__forceinline` (0x00E3BB70, 0x00D13580, 0x00D973D0, 0x00F0FB70, 0x00E20600, 0x00E50FD0, 0x00B10B10, 0x00E5FD40, ...). Respell them FABLE_FORCEINLINE before promoting any of them to src/; src/00e40e80.cpp is already respelled.
