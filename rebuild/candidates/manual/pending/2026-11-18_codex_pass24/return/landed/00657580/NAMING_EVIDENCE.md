# Naming evidence

- Ego_r donor 0x006997E0 and FableWin donor 0x02113360 name the PC override. Ego_d gives its object identity and scopes, and the retail call to base Draw at 0x00652FA4 followed by the PC-specific slots corroborates it.
- Ego_d gives `info`, `phero`, `ptc_inventory`, `num_allocated_quick_access_buttons`, `num_quick_access_slots`, `i`, `gui_def`, each arm's `hot_bar_pos` and `hot_bar_sprite`, and the multi-slot arm's `end_graphic`, `graphic_bank` and `hot_bar_end_sprite`. Retail slot walks, GUI-def offset reads and the three sprite constructors corroborate their roles. The fitted `offset` reference gives the single-slot operation the retail x87 operand order.
- The two shared draw classes follow Ego_d records 0x3DF32 and 0x6113C, with Ego_r member names and the retail STLport vector width. FableWin donor 0x020F7F8C and Ego_r donor 0x00695982 identify the call to base Draw.
- `HUDRingAlpha` at 0x01375744 is the established landed descriptive name. The runtime reads its low byte before ScaleAlpha. Native 0x00643BE2 is `GFToScreen(const C2DVector&)` from both PDBs, confirmed by the return buffer and copy. Native 0x00445950 is `GetNumberOfQuickAccessItemButtons` from both PDBs, confirmed by the typed player interface and adjacent count routines. Both prior erroneous names are documented in NAME_FIXES.tsv.
- See RELOCATIONS.md for the full operand audit.

## Behavioral fixture

The fixture uses the exact selected method with the shared vector type. Fixture player, inventory, GUI resources and engine capture cover null or dead heroes, all four-slot occupancy masks, and present or absent end sprites. VC7.1 RTM /Od /W3 and GNU C++03 both passed.
