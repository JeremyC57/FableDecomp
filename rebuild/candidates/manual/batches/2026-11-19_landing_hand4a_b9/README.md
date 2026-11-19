# Landing hand4a b9 (2026-11-19): NPlayerGui::CDrawFishingGame::Draw (0x00649E8B, 469 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS (19 cases, 15/15 host mutants); 16 grades; no Inferred lines. The parked ndiff-2 residue (one
load on the wrong side of two stores) closed by defining the same-TU CDrawQuestInfo::UpdateMiniGame(const CRGBColour&) body (FableWin
player_gui_draw_modules.obj, 0x020EC3DC; retail still calls it out of line at 0x00644F85): with the body visible VC7.1 sees the colour is only read,
so the CRGBColour temp's may-alias edges go (same-TU callee lesson). The old local CTCFishing model replaced by engine/CTCFishing.h
(PASS22 opt-ins) + new FABLE_RETAIL_CTCFISHING_DRAW_GETTERS (FableWin inline getters, GetPFishingRodDef declaration 0x007B0880).
CFishingRodDef stays a local model (only a generated _quarantine header exists). `// Inline-budget:` line for IS_THING_ALIVE_NOINLINE.
