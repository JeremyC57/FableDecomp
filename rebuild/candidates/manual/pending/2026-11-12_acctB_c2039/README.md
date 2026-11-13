# acctB hand-back, chunk 2039 (1 row matched; the optional second row was not started)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x006ACA30 `?OnCollisionWithObject@CThingPlayerCreature@@QAEXPAVCThing@@@Z` | RELOCATION_MATCH 2026v2026, test PASS (g++ and VC7.1) | 11 grades (pass 1: 10, matched at grade 9; pass 2: 1 regrade after a comment-only edit) |
| 0x006AB820 CThingPlayerCreature::ResolveFacingDirection | not attempted | optional row, no source written; pointers in `NOTES.md` (same TU, same overlay, unreferenced COMDAT copies at 0x006AE790 / 0x006AE7E0). Goes to a fresh agent |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; three `WARN RELOC_NAME`, item 3). Purity and
padded-struct greps are clean; the row has no flags, no pragma, no asm. Without the overlay the row is SRC_FAIL.

## Review items for landing

1. **`CThing::HasTC<T>()` is `FABLE_FORCEINLINE` in the CThing.h opt-in**, like the existing GetTC / PeekTC templates
   (owner decision 2026-11-01 cited in the header). Evidence in the header comment: the member template is in Ego_r.pdb
   and FableWin.pdb; its body compiled out of line grades RELOCATION_MATCH against retail's unreferenced copy at
   0x006AE680; retail expands it in the row (0x006ACAB1..0x006ACB4E); as plain inline c2 scores it 283 at this site
   (cost 175), so it could never expand at the default threshold. The row itself carries no `// Inline-budget:` line
   because the device sits in the shared header; say if you want the line on the row as well.
2. **Shared def-lookup bodies made visible** (`FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY`,
   `FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY`, both existing opt-ins): the row needs them visible so the def handles do
   not escape (each release is retail's single `dec [reg+4]`). **Header debt, agent's measurement:** those shared bodies
   do not byte-match retail's own copies (0x0042B040 116v98, 0x006AE320 171v182, 0x005BADE1 171v162); only their
   visibility matters to this row.
3. **Name fixes** (`NAME_FIXES.tsv`, 5 rows; these are the vland warnings): 0x004C79A0 is
   `?PeekGlobalDefinition@CThing@@IBEABVCGlobalDef@@XZ` (landed rows 0x006A9590, 0x00837050, 0x00837060 bind it as
   `GetMeshBank` / `GetWorldState`); 0x0055F3B0 `UpdateScoreForActivity` takes an enum nested in the class; 0x006AC200 is
   bound as `?Give@Hero..`; 0x00437980 (`CPlayerGui::AddAutoPickup`) and 0x005D9030
   (`CTCInventoryItem::GetInventoryType`) have wrong catalog labels.
4. **Headers** (`hdr/README.md`, all opt-in blocks or new files): CThing.h (`PeekGlobalDefinition`, `HasTC<T>`),
   CTCHeroStats.h (RENTMONEY branch), CTCStockItem.h, CTCContainer.h, CTCHeroReceiveItems.h, CPlayerGui.h,
   CThingPlayerCreature.h, CGoldDef.h, CInventoryItemDef.h; new CTCGold.h, CTCHeroOnlineScoreboard.h,
   CTCActionUseActivateQuest.h.
5. **Labels** (`LABELS.tsv`, 9): includes three uncatalogued PeekTC copies whose compiled bodies match retail
   (0x006AE5E0 gold, 0x006AE630 stock item, 0x006AE740 container).
6. Enumerator values without PDB names (object properties 0x400 / 0x2000000, scoreboard activity 16) are numbers with
   comments.

## How to land

`rows/006aca30.cpp` + `rows/006aca30.test.cpp` with the `hdr/engine/` overlays merged, then `payload.json` through your
usual vland. `LESSONS.md`: each early return past a local with a destructor adds a destructor reference to that local's
frame-packer weight (five `return`s put `sound` ahead of the container pointer; switch `break`s plus a flag tie them and
give retail's order); swapping then / else arms moves the cold block out of the loop; a result flag gives one shared
release where a `break` gives two; reading a getter at the call lets c2 forward a value that was just stored.
