# 2026-11-22 landing p28: Codex pass 28 hand-back (21 of 25 rows landed)

Owner: "here is what codex was able to do out of the 25". Package `FableDecomp_pass28_21_matched_Claude_handoff.zip` (SHA256SUMS verified):
21 rows match their complete retail extents (42,651 of the 53,875 assigned bytes), 4 stay parked (0x00850A80, 0x00879900, 0x00A2F740,
0x00D07490, DIFFER; still reserved in `pending/2026-11-22_codex_pass28/RESERVED.tsv`, sources kept in `codex/rows/<addr>/`).

## Gates
- **Header patches:** Codex's `hdr/PATCHES.json` (137 replacements, 76 paths, 14 new files) replayed in order on the current include tree.
  135 applied as given; patches 56 / 57 (`engine/CLandscapeBackgroundPatch.h`, the two STATIC_ASSERT blocks pinned to `_AT`) found no
  `old_text` because nf267 had already made exactly that change: the repo text equals the patch's `new_text`, so nothing to do.
- **assert_names_check** (HEAD include tree v the patched one): 0 mismatches.
- **Strict-header dry run** on the patched tree (`LANDVERIFY_STRICT_HEADERS=1`, `logs/vland_dry.log`): 21 / 21 RELOCATION_MATCH, every test PASS.
- **Whole-tree equivalence** (`lane_merge/equiv_check.py`, HEAD headers v patched, `logs/equivalence.log`): 57,125 rows checked, 0 differ,
  so no landed row's input changed and no recheck of existing rows is needed.
- **Landing** (`vland.py --land`, strict headers, `logs/vland_land.log`): 21 / 21 RELOCATION_MATCH + PASS. Flags as Codex recorded them:
  0x00560D00 `/Gr`, 0x006C6040 `/d2inlT80`, 0x007EBEA0 `/GS`, 0x00BE6880 `/GS`, 0x00DFD600 `/d2inlT70`.
- **Purity:** no `__asm` / `naked` / `_emit` / `volatile` / `register` / `__forceinline` in any row, test or patch.
- **Names:** catalog names from the demangled leaf (Codex's rows.tsv had `-` for eight script / GUI rows): e.g. 0x00DFD600
  `NScript::CQ_TraderConflictGoodScript::AttackPeople`, 0x00647AB4 `NPlayerGui::CDrawHealth::Draw`.
- **reloc_bindings:** the lander recorded 39 new lines; 7 that would add a second spelling at an address already bound under another name are
  removed and queued (`logs/split_growth.tsv`; e.g. 0x01436E20 `?GPEngineDef@NEngineInternal@@3PBVCEngineDef@@B` against three stand-ins, the
  ABV / AAV signature forms at 0x00682810 / 0x008E8BB0 / 0x00BCA950), as landing p27 did; 32 stay.
- **Codex audits carried as evidence** (`codex/rows/<addr>/`): per-row operand audits, pick-any / strong-copy checks (0 leaf-referenced NEQ copies;
  0x00647AB4's one NEQ copy is the pre-existing folded destructor alias, BINDING_EVIDENCE.json), native behaviour tests.

## Names queued
Codex's NAME_FIXES (34 lines) and the 7 held bindings are in `NAME_FIXES_QUEUE.tsv` under `land1122p28`.
