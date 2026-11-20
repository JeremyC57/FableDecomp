# 0x006495A8 NPlayerGui::CDrawScreenFade::Draw (p25A)

**Grade:** RELOCATION_MATCH (938/938). The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`), and so does the real-object behaviour test. Mutants: 33 of 33 killed.

## Source

`src/006495a8.cpp` replaces the handQ8 draft (DIFFER 937v938), which was a local model of every class. The new source uses shared headers only. The one exception is the drawer class, which has no shared header: it is declared in the source on the shared `NPlayerGui::CDrawBase`, the same way the TU-mate row 0x00649E8B CDrawFishingGame::Draw declares its class.

**Structure**

The structure follows Ego_d.pdb: the proc block tree plus the C13 line table in `egod_lines/006495a8.tsv`. Line sizes are VC10 /Od.

| Ego_d lines | Code | What the evidence shows |
|---|---|---|
| 1405 (15 B) | `if (g_bSaveLoadFailed)` | Comes first; its jcc opens the first scope block. |
| 1417 (21 B) | `phero = GetPlayer().PeekPControlledCreature();` | Assigned after the save/load block. Retail tests the global before it calls GetMainPlayer. |
| 1420 / 1450 (4 B / 7 B) | `ptc_quest_ui = 0` / `ptc_receive_items = 0` | Each 0-init is an `and [x],0`; disp8 vs disp32 explains the 3-byte difference. |
| 1430 (17 B) | `display_engine` | Taken after the quest-UI test. |
| 1436–1437 (17 + 11 B) | colour copy, then `colour.A = GetScreenFadeOutAlpha()` | The copy is a copy-constructor call. |
| 1442–1489 | four sprite blocks | The receive-items one is nested inside IS_THING_ALIVE / PeekTC / IsDisplayingItems. |
| 1464 + 1466 (32 + 34 B) | subtitle test over two lines | IsDisplayingText, then IsFlagSet(8) (+2 B for the `push 8`). |
| 1476 (121 B) | cutscene test, one line | IsPaused (25 B) plus three 32-byte subtitle calls. |

VC7.1 cross-jumps the three tail sprite blocks into the first one (0x00649887, 0x006498B4).

**Names**
- Locals use Ego_d's names: phero, display_engine, clip_box, background_sprite, ptc_quest_ui, colour, ptc_receive_items; the parameter is info.
- Callees use FableWin / Ego_r spellings: GetScreenFadeOutColour, GetScreenFadeOutAlpha, IsScreenFullyFadedOut, IsScreenFadeOut, IsDisplayingMiniMapOnRegionLoad, IsPerformingRegionLoad, IsPaused, IsDisplayingText, IsFlagSet, IsInCutscene, IsCutsceneSkippable, IsCutsceneSkippableWhilePaused, IsDisplayingItems, IsOverridingScreenFade, PeekPControlledCreature, PeekDisplayEngine.
- Globals:
  - VIRTUAL_SCREEN_WIDTH / VIRTUAL_SCREEN_HEIGHT are Ego_r.pdb publics.
  - g_bSaveLoadFailed (0x013B89C0) is descriptive. Three landed rows already use this name for that address: 0x00416953, 0x00498490, 0x004A5A40. A fourth landed row, 0x00499D90 CProgressDisplay::StopProgress, spells it `g_ProgressBlackScreenB`; that spelling needs a respell to keep one name per address.

**Compile mode:** `#pragma optimize("s", on)`, with retail's biased frame `lea ebp,[esp-74h]`. Flags: none.

**Inline-budget (line 2, amended after landing b43998c2a):** two existing shared-header contracts, no flag, no pragma:
- FABLE_RETAIL_IS_THING_ALIVE_NOINLINE (engine/ThingAlive.h, __declspec(noinline)): retail calls the COMDAT 0x0043DBF0 twice (0x00649666, 0x006497D3). Without it: DIFFER(977v938).
- FABLE_RETAIL_CTHING_GETTC_EXPANDED (engine/CThing.h, PeekTC<T> FABLE_FORCEINLINE, owner decision 2026-11-01). Without it: DIFFER(901v938).

The TU-mate 0x00649E8B uses both. The landed text said "none" on a later line. `amend_landed_006495a8.cpp` / `amend.diff` is the comment-only respell: line 2 now names both devices, and the old two lines are removed. It rechecks as RELOCATION_MATCH PASS (`vland_amend.log`, `--recheck-landed`).

## Header overlay (`hdr/`, `hdr.diff`)

The overlay is add-only: 45 `+` lines, no `-` lines. pin_asserts.py pinned 0, because every touched assert is already `_AT`. Each opt-in adds member functions only; no layout changes.

| Header | Opt-in | Adds |
|---|---|---|
| CDisplayEngine.h | FABLE_RETAIL_CDISPLAYENGINE_SCREENFADE_DRAW | Inline `GetScreenFadeOutColour()` (ScreenFadeOutInfo.GetColour(); bytes +0xD4.. read in place, 0x006496F9). Declaration `GetScreenFadeOutAlpha()`: called at 0x00649715, landed COMDAT 0x004364C0. |
| CFadeInFadeOutBase.h | FABLE_RETAIL_CFADEINFADEOUTBASE_GETCOLOUR | FableWin `GetColour()`; needs the typed ToColour of _FADE / _OPEN. |
| CSubtitleRenderer.h | FABLE_RETAIL_CSUBTITLERENDERER_SKIPPABLEWHILEPAUSED | FableWin `IsCutsceneSkippableWhilePaused()` (byte +0xE1 read in place, 0x006498AE). |
| CWorld.h | FABLE_RETAIL_CWORLD_SCREENFADE_CALLEES | Declarations of `IsDisplayingMiniMapOnRegionLoad()` (0x0049E9F0) and `IsPerformingRegionLoad()` (called out of line at 0x00649748; landed COMDAT 0x0048DB13). Guarded against the inline QUESTCOMPLETIONUI copy and the UPDATEREGIONLOAD_CALLEES declaration. |
| CTCQuestCompletionUI.h | FABLE_RETAIL_CTCQUESTCOMPLETIONUI_ISOVERRIDINGSCREENFADE | Declaration `IsOverridingScreenFade() const` (0x005E3E70). |
| CTCHeroReceiveItems.h | FABLE_RETAIL_CTCHERORECEIVEITEMS_ISDISPLAYINGITEMS | FableWin `IsDisplayingItems()` for the DISPLAYINGITEMS layout (byte +0x26 read in place, 0x00649819). The GLOBALUPDATE branch already has it. |

`newly_flagged_rows.tsv` lists six landed rows that use local models of the members these opt-ins add. They are re-port candidates and none of their bytes change.

## Ablation (one change at a time from the matching source)

| change | grade |
|---|---|
| sprites given `clip_box` directly instead of the shared inline `NPlayerGui::GFToScreen(clip_box)` | DIFFER(871v938), 68 hunks. Retail rebuilds the box per sprite from scalars (0x00649751..). |
| `#pragma optimize("y", off)` added (the FishingGame row's frame) | DIFFER(955v938) (plain ebp frame) |
| `FABLE_RETAIL_CRGBCOLOUR` (the Initialise copy constructor: all four reads before the stores) | DIFFER(930v938). Retail interleaves R, G, B (0x006496FF..0x00649712). |
| colour built with the 4-channel ctor and the alpha call as its argument | DIFFER(943v938) (the call would precede the byte reads) |
| phero initialised at its declaration, before the save/load test | DIFFER(908v938) |
| `= 0` on the two component pointers removed | still RELOCATION_MATCH. Kept for Ego_d's `and [x],0` lines 1420 / 1450. |
| FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY (a device) | still RELOCATION_MATCH: not needed, not used |
| FABLE_RETAIL_IS_THING_ALIVE_NOINLINE removed | DIFFER(977v938) |
| FABLE_RETAIL_CTHING_GETTC_EXPANDED removed | DIFFER(901v938) |

## Test (`test/006495a8.cpp`, links the real row object)

- **Fixtures** use the native layout:
  - drawer: PlayerGui +4, PlayerManager +8, World +0xC, Component +0x10, Drawing +0x18, PrimitiveHandles +0x1C, DrawnFullFadeOut +0x28.
  - player GUI subtitle bytes +0x94 / +0xF6 / +0xF9 / +0xFD; world Paused +0xA4; component display-engine word +0x28.
  - display engine: Active +0xBC, ToColour +0xD4.
  - hero: component bitset +0x20, (type, component) pairs +0x44, Dead bit +0x91; receive-items DisplayingItems +0x26.
  - info: Engine +0x34, vtable slot 23.
- **Stubs:** every out-of-line callee is a counting stub. The sprite ctor records all 16 arguments; SetPersistentPrimitive records the handle, the sprite and the flags.
- **Coverage:** 17 runs through Model(). They cover:
  - forced black;
  - the quest-UI override, with a live and a dead hero;
  - no fade, with the flag cleared and with it kept clear;
  - minimap region load, with and without the remembered flag;
  - receive items displaying, not displaying, and with a dead hero;
  - subtitle flag 8;
  - the paused skippable cutscene and its three failing terms;
  - M1 with alpha 255 (flag set) and below 255 (flag kept).
- **Checks:** each run compares the box, layer, colour, constants, UVs, handle, flags, Drawing, DrawnFullFadeOut, and the call counts of the short-circuited predicates.
- **Mutants:** `mutants/` holds 33 one-decision mutations of the ROW SOURCE. All are killed.

## vland warnings (labels DB, not source names)

- `RELOC_NAME`: GetScreenFadeOutAlpha 0x004364C0 is labelled `std::_Dest_val<..CMusicManager..>`. The landed row 0x004364C0 is CDisplayEngine::GetScreenFadeOutAlpha.
- `RELOC_NAME`: IsPerformingRegionLoad 0x0048DB13 is labelled `CWorld::Query_48db13`. The landed row's name is CWorld::IsPerformingRegionLoad.

## NAME_FIXES

One entry: 0x005E3E70 should be `?IsOverridingScreenFade@CTCQuestCompletionUI@@QBE_NXZ`. It is landed as the free __fastcall `?CTCQuestCompletionUI_IsOverridingScreenFade@@YI_NPAVCTCQuestCompletionUI@@@Z`; the bytes are the same.
