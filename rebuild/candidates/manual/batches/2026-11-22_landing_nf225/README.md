# Landing nf225 (Account A, 2026-11-22): the GetFont binders, ~CManager onto its header, GPUniqueInstance (5 rows, 2 header opt-in sets)

Agent NF225. Hand-back: `pending/2026-11-22_nf225_handback/`. Entries q12562-q12564.

- **5 rows land.** Recheck (`logs/recheck.log`, HEAD control `logs/recheck_head.log`): 8 rows (the 5 plus 0x0041E641, 0x0042E98F and
  0x005954AD), all RELOCATION_MATCH + PASS on both trees; no RELOC_CONFLICT. Link: 0 duplicates on both.
- **q12562, 0x00494840 / 0x0054ED90:** onto `?GetFont@CFontManager@@...`, so 0x009E2C80 goes from 3 spellings to 1.
  - engine/CCountedPointer.h gets five add-only member-specific SIZE_BODY opt-ins (`FABLE_RETAIL_CCOUNTEDPOINTER_CONSTCFONTBANK_RELEASEDATA_SIZE_BODY`,
    `..._CONSTCFONTBANK_SHAREDATA_SIZE_BODY`, `..._CONSTCFONTBANK_CONVERTING_ASSIGN_SIZE_BODY`, `..._CFONTBANK_SHAREDATA_SIZE_BODY`,
    `..._CFONTBANK_ASSIGN_SIZE_BODY`). They follow the nf213 `_SIZE_BODY` pattern (explicit specializations under `#pragma optimize("s", on)`),
    as the coordinator authorised under the size-pragma rule.
  - Every pick-any copy with a retail owner equals it (`logs/pickany_vs_retail.txt`); 0x00494840 had 3 differing copies at HEAD.
- **q12563, 0x0042DB20 (~CManager), as a 3-row unit with 0x0042FB40 and 0x0041E5F2.**
  - The evidence gate is met as an inline copy in another compiland. In Ego_r and FableWin, CManager::Clear's only copy sits in
    new_frontend_game_component.obj, flagged "marked inline", while CManager's own methods sit in ui_manager.obj. VC7.1-era PDBs carry no
    S_INLINESITE records.
  - engine/NUISystem_CManager.h gets add-only `FABLE_RETAIL_CMANAGER_CLEAR_INLINE` and `FABLE_RETAIL_CMANAGER_GPUNIQUEINSTANCE`
    (the protected static).
  - 0x0042DB20's leaf moves from a made-up stand-in to `??_GCManager@NUISystem@@IAEPAXI@Z` (both PDBs), and it calls
    `??1CManager@NUISystem@@IAE@XZ`. 0x0041E641 goes from 2 spellings to 1.
  - 0x0042FB40 is ported onto the header, so Clear becomes a pick-any COMDAT. Both rows use the anonymous-namespace emit scaffold
    (the 0x00415B80 pattern), with no friend scaffold. Clear's copy in 0x0042DB20 equals 0x0042FB40, and the `??_G` copy in 0x0042FB40
    equals 0x0042DB20.
  - The catalog name of 0x0042DB20 stays `scalar_deleting_destructor'`, as on the other landed rows of that kind.
- **q12564, 0x013B8710:** respelled to `?GPUniqueInstance@CManager@NUISystem@@1PAV12@A` (all three PDBs), 2 spellings -> 1.
  - 0x0041E5F2 is respelled inside its existing local model: it only references the static, and its object defines no CManager symbol
    beyond its own leaf (in-place rule, both conditions).
  - A port onto the header would flip its ctor reference to protected IAE while 0x0041E3F6 still emits QAE.
- **Header proof:** pin_asserts 0 pinned; assert_names_check 0 mismatches; equiv_check in `logs/equivalence_differing_rows.tsv`.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 4 stale stand-in lines deleted (two GetFont, `Instance`, `g_pUIManagerInstance`);
  1 first witness added (GPUniqueInstance -> 0x013B8710, witness 0x0041E5F2).
- **Queued** (tag land1122nf225: q12566 q12567 q12568 q12569 q12570 ):
  - test rewrites for the five rows (they keep HEAD tests; 0x0042DB20's is a print-only stub, as at HEAD);
  - the unapplied access-specifier fixes (0x0041E641 IAE non-virtual; 0x0041E3F6 + 0x0041E5F2 protected ctor; 0x00494840's Leaf AAE);
  - the disagreeing no-owner `~CCountedPointer<CFontBank>` copies.

Equivalence (57k rows): the 4 opt-in users (0x00494840, 0x0054ED90, 0x0042FB40, 0x0042DB20) plus the 6 rows that differ on every run
(GOTCHAS, landing nf222 / nf223). Nothing else changes.
