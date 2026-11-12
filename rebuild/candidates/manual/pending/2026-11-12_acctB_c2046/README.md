# acctB hand-back, chunk 2046 (2 quest-script rows matched)

Authored by an Opus 5.5 agent with the method and tools of `2026-11-12_acctB_f4001`, re-verified by the acctB local
coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00D92380 `?Main@CHobbeTorrent@CQ_HobbeCaveScript@NScript@@EAEXXZ` | RELOCATION_MATCH 1468v1468, test PASS (g++ and VC7.1) | 2 grades. True length 1468: the 19,950-byte reservation estimate is wrong (`SPLITS.tsv`) |
| 0x00D77690 `?DoCutscene@CQ_HangingTreeGoodScript@NScript@@QAEXJ@Z` | RELOCATION_MATCH 3064v3064, test PASS (g++ and VC7.1) | 2 grades; all 855 instructions exact |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grades, both test
runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE, no name warnings). Purity and
padded-struct greps are clean; no flags, no pragma. The agent also paired every call target and every data relocation
(strings, jump table, vtables, constants) with retail one to one.

## Review items for landing

1. **0x00D92380, inline budget:** retail calls `vector<CScriptThing>::erase` 0x008AC9D0 for `clear()`; the inline body
   scores 66, under the threshold 74, so VC7.1 would expand it. The row uses the EXISTING shared opt-in
   `FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE` (as landed 0x00E74D70 does) with an `// Inline-budget:` line; five
   loop spellings were tried first as free probes (scores 64..72, none low enough). The `CTimer` members are written in
   the row as the 64 landed script rows that use them do.
2. **0x00D77690, inline budget:** the EXISTING shared opt-in `FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR`
   (retail expands the pause constructor at all three sites; as plain inline case 1's site scores 636 and is called
   while retail CALLS same-frequency sites scoring 600, 748 and 200, so no single `/d2inlT` separates them), and the
   `~CCountedPointer<CScriptGameResourceObjectMovieBase>` destructor declared out of line because retail calls
   0x006E7AB0 (as f4001's row and the landed PreMelee rows do). `// Inline-budget:` line with the trace numbers.
3. **Uncatalogued function:** after 0x00D92380's jump table (0x00D9291C, 5 entries) and index table (0x00D92930) comes
   0x00D92940 = `CQ_HobbeCaveScript::CGranny::Main` (vtable 0x012D59E4 slot 1, stored by the landed CGranny::Alloc
   0x00D8FC70; in both PDB name sets). `SPLITS.tsv`, `LABELS.tsv`.
4. **Header** (`hdr/README.md`): one new opt-in `FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBETORRENT` in
   `engine/CQ_HobbeCaveScript.h` (the nested CHobbeTorrent class from the Ego_r layout); default text unchanged. Row 2
   uses only repo headers and existing opt-ins.
5. **Name fixes** (`NAME_FIXES.tsv`): 0x00F35B30 is `?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ` (the
   NScript-namespaced spelling bound by 0x00EE2490 is wrong); 0x008AC970 is the STLport `~vector<CScriptThing>` (eight
   invented spellings bound); 0x008AC9D0 is STLport `erase(T*, T*)` (four invented spellings bound).
6. Local variable names and DoCutscene's parameter name are descriptive (no PDB local names exist).

## How to land

`rows/00d92380.cpp`, `rows/00d77690.cpp` and their `.test.cpp` files with `hdr/engine/CQ_HobbeCaveScript.h`, then
`payload.json` through your usual vland. `LESSONS.md`: writing a script function in the generator's own pattern (a
terminate check after every loop and at the top of every for / if body, a `bool flag` loop for "wait until any") got the
whole layout at the first compile; read the c2 inline trace before any grade.
