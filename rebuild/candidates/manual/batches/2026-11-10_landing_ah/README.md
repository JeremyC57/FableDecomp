# 2026-11-10 landing ah -- chunk 1011b (1 new row, 2 landed rows re-authored)

Source: `pending/2026-11-10_linux_hand1011b/`.

- 0x005EB590 CTCTavernGame::GivePrize (vland --land RELOCATION_MATCH): a named `const CPlayerManager& player_manager = PeekPlayerManager();`
  in the prize block ties the three candidates at priority 36 and hands the def piece edi, prize esi, as retail.
- Re-authored (vland --recheck-landed RELOCATION_MATCH, logs/vland_recheck_questreview.log): 0x005FC2D0 / 0x005FC8A0, landed as
  "_global::ConstructQuestsScreen_CurrentQuests(PC)" on an invented `NScreen005fc*` namespace, are
  ?ConstructQuestReviewList(PC)@CTCInventoryMap@@ (Ego_r 0x0064C6C0 / 0x0064CE90, FableWin 0x02088CA0 / 0x020AEC70): renamed (files,
  ps1, catalog, manifest), rewritten on shared headers; the 22 stale namespace bindings removed from reloc_bindings.tsv.
- Header: CTCInventoryMap.h opt-in QUESTREVIEW (declarations only).

Parked: 0x005FBAA0 GetRelativePosOnMiniMap DIFFER(634v634), 10 grades (2 lines: the final X*MiniMapScale load order). Unreached:
PositionMesh (needs CThing slots 17-26 named), ProcessInput (IsEventButtonPressed is protected in both PDBs), DrawPreparePrimitivesForRendering.
