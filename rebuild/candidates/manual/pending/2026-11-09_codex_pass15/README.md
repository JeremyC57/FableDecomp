# Codex pass 15 reservation (2026-11-09)

20 rows packaged for an outside ChatGPT Codex pass at the owner's request (`RESERVED.tsv`): 5 never-attempted rows (1,681-3,372 bytes:
CTCInventoryClothing::CalculateArmorValues 0x005B4EBA, CMainGameComponent_InitSoundBanks 0x00417A58, CTCInventoryAbilities slot 77
0x005F519F, the CDrawQuestInfo bar / mini-game bar slot-0 draws 0x0064FDB4 / 0x00651273) and 15 parked near misses (94-585 bytes, 1-5
measured instructions from retail; best drafts from `../2026-11-09_linux_hand895/`).

Package: `fabledecomp_codex_2026-11-09.zip` (scratch only, `$S/codex0911/`, built by `$S/codex0911/build.py`), 10.6 MB, 2,298 files:
the brief (README_CODEX.md, PROMPT_CODEX.md, here), ROWS.tsv with VA / file offsets / lengths, per row ROW.md (extent, callers with the
landed function each sits in, landed neighbour sources, every earlier draft and header overlay for parked rows), `rebuild/include`, the
naming references (reloc_bindings.tsv, the three PDB name sets, struct_layouts_egor.tsv, pdb_class_keys.tsv, rtti-vtables.tsv,
ESTABLISHED_SPELLINGS.tsv, NAME_FIXES_QUEUE.tsv, catalog and manifest names without bytes), the docs (EXTERNAL_AI_HANDOFF, BUILDING,
ARCHITECTURE, EXTRA_RULES, GOTCHAS, MATCHING_PLAYBOOK, C2_ALLOCATOR_PROBE, ASM_RULES, INLINE_DEPTH_REVISIT, VC71_CODEGEN_RESEARCH,
REGISTER_SWAP_SUMMARY, SYMBOLS, NAMING_STRAGGLERS, QFE4035_COMPILER_GATE), the lessons of chunks 892-895 and the 2026-11-08/09 landing
READMEs, and two standalone tools that read the user's own Fable.exe (cut_row.py, xref.py). The owner hands the lane tools over
separately. No retail bytes (checked: 0 of 200 sampled 24-byte row windows found in any package file), no links.

In-house chunk cuts skip these 20 rows until Codex returns. Returned rows are regraded with vland.py and every bound symbol checked
against the PDB name sets before landing.

**Released 2026-11-13 (owner):** the Codex work for this pass has been handed back. `RESERVED.tsv` is renamed `RELEASED.tsv`, so the
reservation glob (`pending/*/RESERVED.tsv`) no longer holds these rows back. The rows still unlanded go to the normal parked pool with
their closest version on file: `../2026-11-13_codex_released/` (STATUS.tsv, differ/).
