# 2026-11-16 landing gh -- five parked matches made landable (agent w24B, hand-ported)

Owner: "work on the closest parked rows that arent reserved by other accounts / ais". A triage regrade of all 1,162 free parked rows
under the current headers found rows that already byte-match but were blocked by the landing rules (local stand-in models, same-TU
stand-in bodies) or by header drift since their lane. Agent w24B ported five of them onto the shared headers by hand; all five are
RELOCATION_MATCH with their behaviour tests passing, `LANDVERIFY_STRICT_HEADERS=1`, on the repo headers alone (`logs/vland_land.log`).

| row | function | was | what it took |
|---|---|---|---|
| 0x00542780 | NUISystem::CIconText::AddIcon | handQ6 RELOCATION_MATCH_HEADER_DEBT (local models) | shared headers with landed 0x00B64550's graphics-bank opt-ins + new `FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME`; new test |
| 0x007DDA30 | CTCCreatureFlyInCircles::QueueSwoop | handQ6 RELOCATION_MATCH_HEADER_DEBT | new shared header `engine/CTCCreatureFlyInCircles.h`; opt-ins in CCardinalSpline.h, CTCMapwho.h (`SetInMap`), C3DVector.h (`FABLE_RETAIL_C3DVECTOR_SUBTRACT_X_GROUP` with the existing `..._MAGNITUDE_NAMED_SQUARED`); new test |
| 0x00B625E0 | CEngineSkyRenderer::CEngineSkyRenderer | hand727 RELOCATION_MATCH, not landed (DECISION_00b625e0.md) | the faithful SetDynamicCast form via `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`; new `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_INLINE`; lane test |
| 0x004075C0 | CUserProfileManager::RenderUIBackground | hand733 match, SRC_FAIL under current headers | rebased; `FABLE_RETAIL_CUSERPROFILEMANAGER_RENDERUIBACKGROUND` in fable_user_profile.h; lane test |
| 0x0040BCA0 | CUserProfileManager::LoadGameState (/GS) | hand733 match, SRC_FAIL under current headers | current typed containers + an overlay `..._LOADGAMESTATE` declaration; keeps the two same-TU bodies landed 0x004093E0 carries; lane test |

Extent: 0x004075C0 is 1,194 bytes (`ret 4` at 0x00407A69, int3 padding). The manifest gap after it holds two unlisted functions,
0x00407A70 and 0x00407BA0 (`SPLITS.tsv`, both `ret 0Ch`, ending in a call to ApplyValues 0x00407370); they are not split here.

Headers (`hdr_notes/README.md`): seven files, additions only, each block behind a new opt-in; default path unchanged. CTCMapwho.h's
21 plain asserts below the new block are pinned `FABLE_STATIC_ASSERT_AT(<old line>, ...)` (pincheck 0 mismatches). Every header
balances; no new bare `__forceinline`. Whole-tree equivalence check (exact): 56,372 landed rows, all identical input except 0x00CBFB7D,
whose only difference is the scratch tree's path inside `__FILE__` (`logs/equivalence_changed_input_rows.tsv`).

Not landed: 0x00A1CC30 CNavigatorAStar::ConstructFinalRoute matches (632v632) only with a same-TU stand-in body of its callee
GetIntersectionPointsBetweenNodes 0x00A1A7E0, which is in the Codex pass 23 reservation; with a declaration it is 664v632 (the hidden
result pointer escapes). Best copy: `pending/2026-11-16_w24/differ/00a1cc30.cpp`; it lands after 0x00A1A7E0.

Names: `ghidra_out/labels_landing_2026-11-16gh.tsv`; three catalog-label fixes queued as `land1116gh` (0x009ADE10
GetPDefFromClassIndex, 0x00412DB0 / 0x00412E80 TransferVector<...>). Lander warnings: RELOC_NAME only (those labels + CCharString's
conversion operator).
