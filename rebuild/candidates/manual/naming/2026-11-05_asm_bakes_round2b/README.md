# Asm cleanup round 2b (2026-11-05): 5 of the 14 oracle bakes are now clean C++

The rules are in `docs/pipeline/ASM_RULES.md`, with the owner's attempt budget.

## Landed (the bake file is replaced in place, and its `asm_bake` grade line is removed from `build_candidates.ps1`)

| Address | Function | Grade | Purity |
|---|---|---|---|
| 0x005BD993 | NInventory::CCategory::GetHighlightedItemMaxInCategory | RELOCATION_MATCH | genuine |
| 0x007022C0 | CTCQuestCard::IsOptional | MATCH | genuine |
| 0x0066FF20 | CTCCoopSpirit::SwapToHero | RELOCATION_MATCH | genuine |
| 0x00899EF0 | CGameScriptInterface::GiveHeroMorality | RELOCATION_MATCH | `retail_asm` |
| 0x00C8C730 | libvorbis `_ov_d_seek_lap` | RELOCATION_MATCH | genuine |

- **Coordinator regrade:** vland `--recheck-landed`, every behaviour test PASS (`logs/`).
- No row source or test contains `__asm`, `naked`, `_emit` or `volatile`.
- **0x007022C0** took 51 spellings. Round 1 had parked it as compiler residue after 13.
- **0x00899EF0** goes through the shared `GFFloatToLongNear` helper. The bare `fistp dword` is at 0x00899F6A / 0x00899F6E. 30 C++
  spellings and 10 intrinsic spellings were tried first.
- **0x00C8C730** is third-party (libvorbis). The owner set third-party code aside, but this was an existing bake, and replacing it
  with C++ removes asm. It joins the landed vorbisfile slice of 0x00C8C580.

## Names
- **Applied** (vendor-code rule, owner 2026-11-04): 0x00C8C730 `_ov_d_seek_lap` and 0x00C8BF90 `_ov_splice`
  (`ghidra_out/labels_asm2b_2026-11-05.tsv`).
- **Queued** (`NAME_FIXES_QUEUE.tsv`, tag `asm2b`): 0x0057A67E `CTCHeroStats::GetMoralityMax` and 0x00449700
  `CPlayerManager::GetMainPlayerNumber` as PRIORITY; 0x00449B20 `CPlayerManager::InitialisePlayerCharacterAs` as PROPOSAL.

## Not done (`DEFERRED.tsv`)
- **Parked after the budget:**
  - 0x005DACF0: 60 spellings; the closest is 5 bytes longer (`true` held in `bl` across the call).
  - 0x004167A7: 69 attempts. Retail runs `fstp st(1)` after `fdivr`, which is x87 stack ordering, not asm evidence.
- **Under budget:**
  - 0x009BEF80 SetViewport: 8 attempts, register allocation differs.
  - Not attempted: 0x0042DF9E Draw (its `fnstcw` / `and` / `fldcw` rounding switch needs a new shared helper), 0x00551340,
    0x00597BF2, 0x0053C332.
- **Kept:** 0x009A8170 / 0x009A8840, CBankFileManager::OpenIniFile / OpenRetailBank. These are real functions (Ego_r and FableWin
  names) used as reference bytes by `rebuild/runtime/openretailbank`. Candidates to add as catalog rows.
